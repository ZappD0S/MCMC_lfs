#include "leapfrog.hpp"
#include <algorithm>
#include <cmath>

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

    return std::transform_reduce(
        a.begin(), a.end(), b.begin(),
        true, std::logical_and{}, pred);
}

// void Leapfrog::update_Phi(std::vector<double> &Phi, std::size_t i, double mult)
// {
//     Phi[i] += mult * m_epsilon * Pi[i];
// }
// void Leapfrog::update_Pi(std::vector<double> &Pi, const std::vector<double> &dS, std::size_t i, double mult)
// {
// }

bool Leapfrog::step(PhaseSpaceCoords &state)
{
    m_state0 = state;

    step_impl(state);

    m_state0_rev = state;

    step_impl(m_state0_rev, true);

    return allclose(m_state0.Phi, m_state0_rev.Phi) &&
           allclose(m_state0.Pi, m_state0_rev.Pi);
}

void Leapfrog::step_impl(PhaseSpaceCoords &state, bool backward) const
{
    auto &Phi = state.Phi;
    auto &Pi = state.Pi;

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

    const auto &idx = m_idx;

    // first Pi half step
    {
        const auto &dS = m_system->dSdPhi(Phi);
        std::for_each(
            idx.begin(), idx.end(),
            [&, epsilon](std::size_t i)
            { update_Pi(i, dS, 0.5 * epsilon); });
    }

    for (int step = 0; step < m_Nhmc; step++)
    {
        std::for_each(
            idx.begin(), idx.end(),
            [&, epsilon](std::size_t i)
            { update_Phi(i, epsilon); });

        const auto &dS = m_system->dSdPhi(Phi);
        std::for_each(
            idx.begin(), idx.end(),
            [&, epsilon](std::size_t i)
            { update_Pi(i, dS, epsilon); });
    }

    std::for_each(
        idx.begin(), idx.end(),
        [&, epsilon](std::size_t i)
        { update_Phi(i, epsilon); });

    // last Pi half step
    {
        const auto &dS = m_system->dSdPhi(Phi);
        std::for_each(
            idx.begin(), idx.end(),
            [&, epsilon](std::size_t i)
            { update_Pi(i, dS, 0.5 * epsilon); });
    }
}
