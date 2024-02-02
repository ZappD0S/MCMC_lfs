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

void VanillaLeapfrog::step_impl(PhaseSpaceCoords &state, bool backward)
{
    auto &Phi = state.Phi;
    auto &Pi = state.Pi;
    auto &dS = m_dS;

    const auto N = m_system->size();
    assert(static_cast<int>(Phi.size()) == N &&
           static_cast<int>(Pi.size()) == N);

    auto sign = backward ? -1 : 1;
    auto epsilon = sign * m_epsilon;

    auto update_Phi = [&](double epsilon)
    {
        for (int i = 0; i < N; i++)
        {
            Phi[i] += epsilon * Pi[i];
        }
    };

    auto update_Pi = [&](double epsilon)
    {
        m_system->dSdPhi(dS, Phi);

        for (int i = 0; i < N; i++)
        {
            Pi[i] -= epsilon * dS[i];
        }
    };

    update_Pi(0.5 * epsilon);

    for (int step = 0; step < m_Nhmc; step++)
    {
        update_Phi(epsilon);
        update_Pi(epsilon);
    }

    update_Phi(epsilon);
    update_Pi(0.5 * epsilon);
}
