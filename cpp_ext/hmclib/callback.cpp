#include "callback.hpp"
#include <execution>
#include <cmath>

void LastPhiCallback::operator()(const std::vector<double> &Phi, bool last)
{
    if (last)
    {
        last_Phi = std::make_unique<std::vector<double>>(Phi);
    }
}

void E0Callback::operator()(const std::vector<double> &Phi, bool last)
{
    auto op = [&](double sum, double phi)
    {
        return sum + std::pow(phi, 2);
    };

    // TODO: divide by size
    auto sample = std::reduce(std::execution::par, Phi.begin(), Phi.end(), 0.0, op);
    m_samples->push_back(sample);

    if (last)
    {
        m_samples->shrink_to_fit();
    }
}

void DeltaECallback::operator()(const std::vector<double> &Phi, bool last)
{
    auto &samples = *m_samples;

    // TODO: this loop can be parallelized
    auto Phi_shift(Phi);
    for (int k = 0; k < m_max_shift; k++)
    {
        // shift by 1 at every iteration
        std::rotate(Phi_shift.begin(), Phi_shift.begin() + 1, Phi_shift.end());

        // TODO: divide by size
        samples[k].push_back(std::transform_reduce(std::execution::par, Phi.begin(), Phi.end(),
                                                   Phi_shift.begin(), 0.0, std::plus{}, std::multiplies{}));
        if (last)
        {
            samples[k].shrink_to_fit();
        }
    }
}
