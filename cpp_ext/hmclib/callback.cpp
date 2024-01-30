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

void E0Callback::operator()(const std::vector<double> &Phi, bool last)
{
    double tot = 0.0;

    for (size_t i = 0; i < Phi.size(); i++)
    {
        tot += std::pow(Phi[i], 2);
    }

    m_samples.push_back(tot / Phi.size());

    if (last)
    {
        m_samples.shrink_to_fit();
    }
}

void DeltaECallback::operator()(const std::vector<double> &Phi, bool last)
{
    for (int k = 0; k < m_max_shift; k++)
    {
        double tot = 0.0;

        for (std::size_t i = 0; i < Phi.size() - k; i++)
        {
            tot += Phi[i] * Phi[i + k];
        }

        for (std::size_t i = Phi.size() - k; i < Phi.size(); i++)
        {
            tot += Phi[i] * Phi[i - Phi.size() + k];
        }

        m_samples[k].push_back(tot / Phi.size());

        if (last)
        {
            m_samples[k].shrink_to_fit();
        }
    }
}
