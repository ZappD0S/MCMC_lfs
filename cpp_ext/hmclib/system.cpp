#include "system.hpp"
#include <cassert>
#include <cmath>
#include <vector>

QhoSystem::QhoSystem(int N, double m0, double omg0)
    : m_N(N),
      m_m0(m0),
      m_omg0(omg0)
{
}

double QhoSystem::S(const std::vector<double> &Phi) const
{
    const auto N = m_N;

    assert(static_cast<int>(Phi.size()) == N);

    auto op = [&, m0 = m_m0, omg0 = m_omg0](auto i, auto i_p)
    {
        return 0.5 * m0 * (std::pow(Phi[i_p] - Phi[i], 2) + std::pow(omg0 * Phi[i], 2));
    };

    double S = 0.0;

    for (int i = 0; i < N - 1; i++)
    {
        S += op(i, i + 1);
    }

    S += op(N - 1, 0);

    return S;
}

void QhoSystem::dSdPhi(std::vector<double> &dS, const std::vector<double> &Phi) const
{
    const auto N = m_N;

    assert(static_cast<int>(dS.size()) == N &&
           static_cast<int>(Phi.size()) == N);

    auto op = [&, m0 = m_omg0, omg0 = m_omg0](auto i_m, auto i, auto i_p)
    {
        return m0 * (-(Phi[i_p] + Phi[i_m]) + (2 + std::pow(omg0, 2)) * Phi[i]);
    };

    dS[0] = op(N - 1, 0, 1);

    for (int i = 1; i < N - 1; i++)
    {
        dS[i] = op(i - 1, i, i + 1);
    }

    dS[N - 1] = op(N - 2, N - 1, 0);
}
