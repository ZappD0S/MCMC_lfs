#include "callback.hpp"
#include <cmath>
#include <cstddef>

void LastPhiCallback::operator()(const std::vector<double> &Phi, bool last)
{
    if (last)
    {
        m_last_Phi = Phi;
    }
}

void X2Callback::operator()(const std::vector<double> &Phi, bool last)
{
    auto N = Phi.size();
    double tot = 0.0;

    for (size_t i = 0; i < N; i++)
    {
        tot += std::pow(Phi[i], 2);
    }

    m_samples.push_back(tot / N);

    if (last)
    {
        m_samples.shrink_to_fit();
    }
}

void XXCallback::operator()(const std::vector<double> &Phi, bool last)
{
    auto N = Phi.size();

    for (int k = 0; k < m_max_shift; k++)
    {
        double tot = 0.0;

        for (std::size_t i = 0; i < N - (k + 1); i++)
        {
            tot += Phi[i] * Phi[i + k + 1];
        }

        for (std::size_t i = N - (k + 1); i < N; i++)
        {
            tot += Phi[i] * Phi[i - N + k + 1];
        }

        m_samples[k].push_back(tot / N);
    }

    if (!last)
        return;

    for (int k = 0; k < m_max_shift; k++)
    {
        m_samples[k].shrink_to_fit();
    }
}
