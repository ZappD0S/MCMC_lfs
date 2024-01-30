#include "system.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <iterator>
#include <numeric>
#include <vector>

QhoSystem::QhoSystem(int N, double m0, double omg0)
    : m_N(N), m_m0(m0), m_omg0(omg0),
      m_dS(std::vector<double>(N))
{
}

double QhoSystem::S(const std::vector<double> &Phi)
{
    assert(static_cast<int>(Phi.size()) == m_N);

    auto m0 = m_m0, omg0 = m_omg0;

    auto op = [&, m0, omg0](auto i, auto i_p)
    {
        return 0.5 * m0 * (std::pow(Phi[i_p] - Phi[i], 2) + std::pow(omg0 * Phi[i], 2));
    };

    double S = 0.0;

    for (std::size_t i = 0; i < Phi.size() - 1; i++)
    {
        S += op(i, i + 1);
    }

    S += op(m_N - 1, 0);

    return S;
}

const std::vector<double> &QhoSystem::dSdPhi(const std::vector<double> &Phi)
{
    assert(static_cast<int>(Phi.size()) == m_N);

    auto m0 = m_m0, omg0 = m_omg0;

    auto op = [&, m0, omg0](auto i_m, auto i, auto i_p)
    {
        return m0 * (-(Phi[i_p] + Phi[i_m]) + (2 + std::pow(omg0, 2)) * Phi[i]);
    };

    m_dS[0] = op(m_N - 1, 0, 1);

    for (std::size_t i = 1; i < Phi.size() - 1; i++)
    {
        m_dS[i] = op(i - 1, i, i + 1);
    }

    m_dS[m_N - 1] = op(m_N - 2, m_N - 1, 0);

    return m_dS;
}
