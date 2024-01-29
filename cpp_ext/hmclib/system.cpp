#include "system.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <numeric>
#include <vector>

QhoSystem::QhoSystem(int N, double m0, double omg0)
    : m_N(N), m_m0(m0), m_omg0(omg0),
      m_idx(std::vector<std::size_t>(N)),
      m_Phi_m(std::vector<double>(N)),
      m_Phi_p(std::vector<double>(N)),
      m_dS(std::vector<double>(N))
{
    std::iota(m_idx.begin(), m_idx.end(), 0);
}

double QhoSystem::S(const std::vector<double> &Phi)
{
    assert(static_cast<int>(Phi.size()) == m_N);
    auto &Phi_p = m_Phi_p;
    Phi_p = Phi;

    std::rotate(Phi_p.begin(), Phi_p.begin() + 1, Phi_p.end());

    auto m0 = m_m0, omg0 = m_omg0;

    auto op = [&, m0, omg0](std::size_t i) -> double
    {
        return 0.5 * m0 * (std::pow(Phi_p[i] - Phi[i], 2) + std::pow(omg0 * Phi[i], 2));
    };

    return std::transform_reduce(
        m_idx.begin(), m_idx.end(),
        0.0, std::plus{}, op);
}

const std::vector<double> &QhoSystem::dSdPhi(const std::vector<double> &Phi)
{
    assert(static_cast<int>(Phi.size()) == m_N);

    auto &Phi_m = m_Phi_m,
         &Phi_p = m_Phi_p;

    Phi_m = Phi;
    Phi_p = Phi;

    std::rotate(Phi_p.begin(), Phi_p.begin() + 1, Phi_p.end());
    std::rotate(Phi_m.rbegin(), Phi_m.rbegin() + 1, Phi_m.rend());

    auto m0 = m_m0, omg0 = m_omg0;

    auto op = [&, m0, omg0](std::size_t i) -> double
    {
        return m0 * (-(Phi_p[i] + Phi_m[i]) + (2 + std::pow(omg0, 2)) * Phi[i]);
    };

    std::transform(m_idx.begin(), m_idx.end(), m_dS.begin(), op);

    return m_dS;
}
