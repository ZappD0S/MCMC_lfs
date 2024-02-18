#include "system.hpp"
#include "pairwise.hpp"
#include <cassert>
#include <cmath>
#include <vector>

QhoSystem::QhoSystem(int N, double m0, double omg0)
    : m_N(N),
      m_m0(m0),
      m_omg0(omg0),
      m_S_buffer(N),
      m_E0_buffer(N)
{
}

double QhoSystem::E0(const std::vector<double> &Phi)
{
    const auto N = m_N;

    assert(static_cast<int>(Phi.size()) == N &&
           static_cast<int>(m_E0_buffer.size()) == N);

    auto m0 = m_m0,
         omg2 = std::pow(m_omg0, 2);

    for (auto i = 0; i < N; i++)
    {
        m_E0_buffer[i] = m0 * omg2 * std::pow(Phi[i], 2);
    }

    return pairwise_sum(m_E0_buffer) / N;
}

double QhoSystem::S(const std::vector<double> &Phi)
{
    const auto N = m_N;

    assert(static_cast<int>(Phi.size()) == N &&
           static_cast<int>(m_S_buffer.size()) == N);

    auto m0 = m_m0,
         omg2 = std::pow(m_omg0, 2);

    auto op = [=, &Phi](auto i, auto i_p)
    {
        return 0.5 * m0 * (std::pow(Phi[i_p] - Phi[i], 2) + omg2 * std::pow(Phi[i], 2));
    };

    for (auto i = 0; i < N - 1; i++)
    {
        m_S_buffer[i] = op(i, i + 1);
    }

    m_S_buffer[N - 1] = op(N - 1, 0);

    return pairwise_sum(m_S_buffer);
}

void QhoSystem::dSdPhi(std::vector<double> &dS, const std::vector<double> &Phi) const
{
    const auto N = m_N;

    assert(static_cast<int>(dS.size()) == N &&
           static_cast<int>(Phi.size()) == N);

    auto m0 = m_m0,
         omg2 = std::pow(m_omg0, 2);

    auto op = [=, &Phi](auto i_m, auto i, auto i_p)
    {
        return m0 * (-(Phi[i_p] - 2 * Phi[i] + Phi[i_m]) + omg2 * Phi[i]);
    };

    dS[0] = op(N - 1, 0, 1);

    for (auto i = 1; i < N - 1; i++)
    {
        dS[i] = op(i - 1, i, i + 1);
    }

    dS[N - 1] = op(N - 2, N - 1, 0);
}

QaoSystem::QaoSystem(int N, double m0, double omg0, double lambda0)
    : m_N(N),
      m_m0(m0),
      m_omg0(omg0),
      m_lambda0(lambda0),
      m_S_buffer(N),
      m_E0_buffer(N)
{
}

double QaoSystem::E0(const std::vector<double> &Phi)
{
    const auto N = m_N;

    assert(static_cast<int>(Phi.size()) == N &&
           static_cast<int>(m_E0_buffer.size()) == N);

    auto m0 = m_m0,
         omg2 = std::pow(m_omg0, 2),
         lambda0 = m_lambda0;

    for (auto i = 0; i < N; i++)
    {
        m_E0_buffer[i] = m0 * omg2 * std::pow(Phi[i], 2) +
                        3 * lambda0 * std::pow(Phi[i], 4);
    }

    return pairwise_sum(m_E0_buffer) / N;
}

double QaoSystem::S(const std::vector<double> &Phi)
{
    const auto N = m_N;

    assert(static_cast<int>(Phi.size()) == N &&
           static_cast<int>(m_S_buffer.size()) == N);

    auto m0 = m_m0,
         omg2 = std::pow(m_omg0, 2),
         lambda0 = m_lambda0;

    auto op = [=, &Phi](auto i, auto i_p)
    {
        return 0.5 * m0 * (std::pow(Phi[i_p] - Phi[i], 2) + omg2 * std::pow(Phi[i], 2)) + lambda0 * std::pow(Phi[i], 4);
    };

    for (auto i = 0; i < N - 1; i++)
    {
        m_S_buffer[i] = op(i, i + 1);
    }

    m_S_buffer[N - 1] = op(N - 1, 0);

    return pairwise_sum(m_S_buffer);
    // return std::reduce(m_S_buffer.begin(), m_S_buffer.end(), 0.0);
}

void QaoSystem::dSdPhi(std::vector<double> &dS, const std::vector<double> &Phi) const
{
    const auto N = m_N;

    assert(static_cast<int>(dS.size()) == N &&
           static_cast<int>(Phi.size()) == N);

    auto m0 = m_m0,
         omg2 = std::pow(m_omg0, 2),
         lambda0 = m_lambda0;

    auto op = [=, &Phi](auto i_m, auto i, auto i_p)
    {
        return m0 * (-(Phi[i_p] - 2 * Phi[i] + Phi[i_m]) + omg2 * Phi[i]) + 4 * lambda0 * std::pow(Phi[i], 3);
    };

    dS[0] = op(N - 1, 0, 1);

    for (auto i = 1; i < N - 1; i++)
    {
        dS[i] = op(i - 1, i, i + 1);
    }

    dS[N - 1] = op(N - 2, N - 1, 0);
}
