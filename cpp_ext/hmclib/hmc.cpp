#include "hmc.hpp"
#include <cassert>
#include <cstddef>
#include <memory>

HMC::HMC(std::shared_ptr<HMCSystem> system, int Nhmc, double epsilon, int seed)
    : m_system(system),
      m_leapfrog(std::make_unique<VanillaLeapfrog>(system, Nhmc, epsilon))
{
    std::random_device rd;
    m_rng = std::make_unique<std::mt19937>(rd());
    m_rng->seed(seed);
}

double HMC::T(const std::vector<double> &Phi) const
{
    double T = 0.0;

    for (std::size_t i = 0; i < Phi.size(); i++)
    {
        T += std::pow(Phi[i], 2);
    }

    return 0.5 * T;
}

int HMC::run(const std::vector<double> &Phi0, int Niter, std::vector<std::shared_ptr<HMCCallback>> &callbacks) const
{
    assert(static_cast<int>(Phi0.size()) == m_system->size());

    std::normal_distribution<> randn(0.0, 1.0);
    std::uniform_real_distribution<> rand(0.0, 1.0);

    PhaseSpaceCoords cur{Phi0, std::vector<double>(Phi0.size())};
    PhaseSpaceCoords proposal;

    auto &rng = *m_rng;

    for (int i = 0; i < Niter; i++)
    {
        proposal = cur;

        for (int j = 0; j < m_system->size(); j++)
        {
            proposal.Pi[j] = randn(rng);
        }

        auto success = m_leapfrog->step(proposal);

        if (!success)
        {
            return -1;
        }

        auto H = T(cur.Phi) + m_system->S(cur.Phi);
        auto H_proposal = T(proposal.Phi) + m_system->S(proposal.Phi);
        auto dH = H_proposal - H;

        if (dH <= 0 || rand(rng) < std::exp(-dH))
        {
            cur = proposal;
        }

        for (auto &callback : callbacks)
        {
            (*callback)(cur.Phi, i == Niter - 1);
        }
    }

    return 0;
}
