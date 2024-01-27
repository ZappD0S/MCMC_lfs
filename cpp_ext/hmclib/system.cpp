#include <vector>
#include <cmath>
#include <execution>
#include "system.hpp"

QhoSystem::QhoSystem(int N, double m0, double omg0)
    : m_N(N), m_m0(m0), m_omg0(omg0)
{
    m_idx = std::make_unique<std::vector<std::size_t>>(N);
    std::iota(m_idx->begin(), m_idx->end(), 0);
}

double QhoSystem::S(const std::vector<double> &Phi) const
{
    auto Phi_p(Phi);
    std::rotate(Phi_p.begin(), Phi_p.begin() + 1, Phi_p.end());

    auto m0 = m_m0, omg0 = m_omg0;

    auto op = [&, m0, omg0](double sum, std::size_t i) -> double
    {
        auto term = 0.5 * m0 * (std::pow(Phi_p[i] - Phi[i], 2) + std::pow(omg0 * Phi[i], 2));
        return sum + term;
    };

    const auto &idx = *m_idx;
    return std::reduce(std::execution::par, idx.begin(), idx.end(), 0.0, op);
}

std::vector<double> QhoSystem::dSdPhi(const std::vector<double> &Phi) const
{
    auto Phi_p(Phi), Phi_m(Phi);
    std::rotate(Phi_p.begin(), Phi_p.begin() + 1, Phi_p.end());
    std::rotate(Phi_m.rbegin(), Phi_m.rbegin() + 1, Phi_m.rend());

    auto m0 = m_m0, omg0 = m_omg0;
    std::vector<double> dS(Phi.size());

    auto op = [&, m0, omg0](std::size_t i) -> double
    {
        return m0 * (-(Phi_p[i] + Phi_m[i]) + (2 + std::pow(omg0, 2)) * Phi[i]);
    };

    const auto &idx = *m_idx;
    std::transform(std::execution::par, idx.begin(), idx.end(), dS.begin(), op);

    return dS;
}
