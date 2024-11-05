#include "hmc.hpp"
#include "pairwise.hpp"
#include <cassert>
#include <memory>

HMC::HMC(std::shared_ptr<HMCSystem> system,
         int Nhmc,
         double epsilon,
         bool rev_check,
         std::optional<int> seed)
    : m_system(system),
      m_leapfrog(std::make_unique<VanillaLeapfrog>(system, Nhmc, epsilon, rev_check)),
      m_T_buffer(system->size())
{

    if (seed)
    {
        m_rng = std::make_unique<std::mt19937>(*seed);
    }
    else
    {
        std::random_device r;
        std::seed_seq sseq{
            r(),
            r(),
            r(),
            r(),
            r(),
            r(),
            r(),
            r(),
        };
        m_rng = std::make_unique<std::mt19937>(sseq);
    }
}

double HMC::T(const std::vector<double> &Pi)
{
    const auto N = m_system->size();

    assert(static_cast<int>(Pi.size()) == N &&
           static_cast<int>(m_T_buffer.size()) == N);

    for (auto i = 0; i < N; i++)
    {
        m_T_buffer[i] = std::pow(Pi[i], 2);
    }

    return 0.5 * pairwise_sum(m_T_buffer);
    // return 0.5 * std::reduce(m_T_buffer.begin(), m_T_buffer.end(), 0.0);
}

int HMC::run(const std::vector<double> &Phi0,
             int Niter,
             std::vector<std::shared_ptr<HMCCallback>> &callbacks)
{
    const auto N = m_system->size();
    assert(static_cast<int>(Phi0.size()) == N);

    std::normal_distribution<> randn(0.0, 1.0);
    std::uniform_real_distribution<> rand(0.0, 1.0);

    PhaseSpaceCoords cur{Phi0, std::vector<double>(N)};
    PhaseSpaceCoords proposal{std::vector<double>(N), std::vector<double>(N)};

    auto &rng = *m_rng;

    for (auto i = 0; i < Niter; i++)
    {
        for (auto j = 0; j < N; j++)
        {
            cur.Pi[j] = randn(rng);
        }

        proposal = cur;

        auto success = m_leapfrog->step(proposal);

        if (!success)
        {
            return -1;
        }

        auto dT = T(proposal.Pi) - T(cur.Pi);
        auto dS = m_system->S(proposal.Phi) - m_system->S(cur.Phi);
        auto dH = dT + dS;

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
