#include "hmc.hpp"
#include <algorithm>
#include <execution>
#include <stdexcept>

bool allclose(
    const std::vector<double> &a,
    const std::vector<double> &b,
    double rtol = 1e-5,
    double atol = 1e-8)
{
    auto pred = [=](double x, double y) -> bool
    {
        return (std::abs(x - y) <= (atol + rtol * std::abs(y)) && std::isfinite(y)) || (x == y);
    };

    return std::transform_reduce(std::execution::par,
                                 a.begin(), a.end(), b.begin(), true, std::logical_and{}, pred);
}

HMC::HMC(std::unique_ptr<HMCSystem> system, int Nhmc, double epsilon, int seed)
    : m_system(std::move(system)),
      m_Nhmc(Nhmc),
      m_epsilon(epsilon)
{
    m_idx = std::make_unique<std::vector<std::size_t>>(m_system->size());
    std::iota(m_idx->begin(), m_idx->end(), 0);

    std::random_device rd;
    m_rng = std::make_unique<std::mt19937>(rd());
    m_rng->seed(seed);
}

std::tuple<bool, HMC::PhaseSpaceCoords> HMC::leapfrog_rev_check(const HMC::PhaseSpaceCoords &ps_coords) const
{
    auto forward = HMC::leapfrog(ps_coords);
    auto backward = HMC::leapfrog(forward, true);

    const auto &Phi0 = *ps_coords.Phi;
    const auto &Pi0 = *ps_coords.Pi;
    const auto &Phi0_rev = *backward.Phi;
    const auto &Pi0_rev = *backward.Pi;

    auto success = allclose(Phi0, Phi0_rev) && allclose(Pi0, Pi0_rev);
    return std::make_tuple(success, std::move(forward));
}

HMC::PhaseSpaceCoords HMC::leapfrog(const HMC::PhaseSpaceCoords &ps_coords, bool backward /*= false */) const
{
    // copy the input into new vectors
    auto Phi_ptr = std::make_unique<std::vector<double>>(*ps_coords.Phi);
    auto Pi_ptr = std::make_unique<std::vector<double>>(*ps_coords.Pi);

    auto &Phi = *Phi_ptr;
    auto &Pi = *Pi_ptr;

    auto update_Phi = [&](std::size_t i, double epsilon)
    {
        Phi[i] += epsilon * Pi[i];
    };

    auto update_Pi = [&](std::size_t i, const std::vector<double> &dS, double epsilon)
    {
        Pi[i] -= epsilon * dS[i];
    };

    auto epsilon = m_epsilon;
    if (backward)
    {
        epsilon = -epsilon;
    }

    const auto &idx = *m_idx;
    auto exec_policy = std::execution::par;

    // first Pi half step
    {
        auto dS = m_system->dSdPhi(Phi);
        std::for_each(exec_policy,
                      idx.begin(), idx.end(),
                      [&, epsilon](std::size_t i)
                      { update_Pi(i, dS, 0.5 * epsilon); });
    }

    for (int step = 0; step < m_Nhmc; step++)
    {
        std::for_each(exec_policy,
                      idx.begin(), idx.end(),
                      [&, epsilon](std::size_t i)
                      { update_Phi(i, epsilon); });

        auto dS = m_system->dSdPhi(Phi);
        std::for_each(exec_policy,
                      idx.begin(), idx.end(),
                      [&, epsilon](std::size_t i)
                      { update_Pi(i, dS, epsilon); });
    }

    std::for_each(exec_policy,
                  idx.begin(), idx.end(),
                  [&, epsilon](std::size_t i)
                  { update_Phi(i, epsilon); });

    // last Pi half step
    {
        auto dS = m_system->dSdPhi(Phi);
        std::for_each(exec_policy,
                      idx.begin(), idx.end(),
                      [&, epsilon](std::size_t i)
                      { update_Pi(i, dS, 0.5 * epsilon); });
    }

    return {std::move(Phi_ptr), std::move(Pi_ptr)};
}

double HMC::T(const std::vector<double> &Phi) const
{
    std::vector<double> T_terms(Phi.size());

    auto op = [&](double sum, double phi)
    {
        return sum + 0.5 * std::pow(phi, 2);
    };

    return std::reduce(std::execution::par, Phi.begin(), Phi.end(), 0.0, op);
}

int HMC::run(const std::vector<double> &Phi0, int Niter, std::vector<std::shared_ptr<HMCCallback>> &callbacks) const
{
    std::normal_distribution<> randn(0.0, 1.0);
    std::uniform_real_distribution<> rand(0.0, 1.0);

    PhaseSpaceCoords cur{
        std::make_unique<std::vector<double>>(Phi0),
        std::make_unique<std::vector<double>>(Phi0.size())};

    auto &rng = *m_rng;

    for (int i = 0; i < Niter; i++)
    {
        std::generate(cur.Pi->begin(), cur.Pi->end(), [&]()
                      { return randn(rng); });

        auto [success, proposal] = leapfrog_rev_check(cur);

        if (!success)
        {
            return -1;
        }

        auto H = T(*cur.Phi) + m_system->S(*cur.Phi);
        auto H_proposal = T(*proposal.Phi) + m_system->S(*proposal.Phi);
        auto dH = H_proposal - H;

        if (dH <= 0 || rand(rng) < std::exp(-dH))
        {
            cur.Phi = std::move(proposal.Phi);
        }

        for (auto &callback : callbacks)
        {
            (*callback)(*cur.Phi, i == Niter - 1);
        }
    }

    return 0;
}
