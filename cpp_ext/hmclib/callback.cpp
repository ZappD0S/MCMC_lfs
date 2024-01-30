#include "callback.hpp"
#include <algorithm>
#include <cmath>
#include <numeric>

void LastPhiCallback::operator()(const std::vector<double> &Phi, bool last)
{
    if (last)
    {
        m_last_Phi = Phi;
    }
}

void E0Callback::operator()(const std::vector<double> &Phi, bool last)
{
    auto op = [](double phi)
    {
        return std::pow(phi, 2);
    };

    // TODO: divide by size
    auto sample = std::transform_reduce(
        Phi.begin(), Phi.end(),
        0.0, std::plus{}, op);
    m_samples.push_back(sample);

    if (last)
    {
        m_samples.shrink_to_fit();
    }
}

void DeltaECallback::operator()(const std::vector<double> &Phi, bool last)
{
    // if both vectors are the same size this should't allocate anything
    m_Phi_shift = Phi;

    // TODO: this loop can be parallelized
    for (int k = 0; k < m_max_shift; k++)
    {
        // shift by 1 at every iteration
        std::rotate(m_Phi_shift.begin(), m_Phi_shift.begin() + 1, m_Phi_shift.end());

        // TODO: divide by size
        m_samples[k].push_back(std::transform_reduce(
            Phi.begin(), Phi.end(), m_Phi_shift.begin(),
            0.0, std::plus{}, std::multiplies{}));

        if (last)
        {
            m_samples[k].shrink_to_fit();
        }
    }
}
