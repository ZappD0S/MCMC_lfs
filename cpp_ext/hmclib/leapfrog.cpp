#include "leapfrog.hpp"
#include <cassert>
#include <cmath>
#include <cstddef>

bool allclose(
    const std::vector<double> &a,
    const std::vector<double> &b,
    double rtol = 1e-5,
    double atol = 1e-8)
{
    assert(a.size() == b.size());

    auto pred = [=](auto x, auto y) -> bool
    {
        return (std::abs(x - y) <= (atol + rtol * std::abs(y)) && std::isfinite(y)) || (x == y);
    };

    // return std::transform_reduce(
    //     a.begin(), a.end(), b.begin(),
    //     true, std::logical_and{}, pred);

    bool res = true;

    for (std::size_t i = 0; i < a.size(); i++)
    {
        res &= pred(a[i], b[i]);
    }

    return res;
}

bool Leapfrog::step(PhaseSpaceCoords &state)
{
    m_state0 = state;

    step_impl(state);

    m_state0_rev = state;

    step_impl(m_state0_rev, true);

    return allclose(m_state0.Phi, m_state0_rev.Phi) &&
           allclose(m_state0.Pi, m_state0_rev.Pi);
}

inline void VanillaLeapfrog::update_Phi(std::vector<double> &Phi, const std::vector<double> &Pi, double epsilon) const
{
    for (int i = 0; i < static_cast<int>(Phi.size()); i++)
    {
        Phi[i] += epsilon * Pi[i];
    }
}

inline void VanillaLeapfrog::update_Pi(std::vector<double> &Pi, const std::vector<double> &Phi, double epsilon) const
{
    const auto &dS = m_system->dSdPhi(Phi);

    for (int i = 0; i < static_cast<int>(Pi.size()); i++)
    {
        Pi[i] -= epsilon * dS[i];
    }
}

void VanillaLeapfrog::step_impl(PhaseSpaceCoords &state, bool backward) const
{
    auto &Phi = state.Phi;
    auto &Pi = state.Pi;

    auto sign = backward ? -1 : 1;
    auto epsilon = sign * m_epsilon;

    // first Pi half step
    update_Pi(Pi, Phi, 0.5 * epsilon);

    for (int step = 0; step < m_Nhmc; step++)
    {
        update_Phi(Phi, Pi, epsilon);
        update_Pi(Pi, Phi, epsilon);
    }

    update_Phi(Phi, Pi, epsilon);
    // last Pi half step
    update_Pi(Pi, Phi, 0.5 * epsilon);
}
