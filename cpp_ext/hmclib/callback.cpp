#include "callback.hpp"
#include "pairwise.hpp"
#include <cmath>

void LastPhiCallback::operator()(const std::vector<double> &Phi, bool last)
{
    if (last)
    {
        m_last_Phi = Phi;
    }
}

void X2Callback::operator()(const std::vector<double> &Phi, bool last)
{
    m_x2_buffer.resize(Phi.size());
    auto N = static_cast<int>(Phi.size());

    for (auto i = 0; i < N; i++)
    {
        m_x2_buffer[i] = std::pow(Phi[i], 2);
    }

    m_samples.push_back(pairwise_sum(m_x2_buffer) / N);

    if (last)
    {
        m_samples.shrink_to_fit();
    }
}

void XXCallback::operator()(const std::vector<double> &Phi, bool last)
{
    auto N = static_cast<int>(Phi.size());
    m_xx_buffer.resize(N);

    for (auto k = 1; k <= m_max_shift; k++)
    {
        for (auto i = 0; i < N - k; i++)
        {
            m_xx_buffer[i] = Phi[i] * Phi[i + k];
        }

        for (auto i = N - k; i < N; i++)
        {
            m_xx_buffer[i] = Phi[i] * Phi[i - (N - k)];
        }

        m_samples[k - 1].push_back(pairwise_sum(m_xx_buffer) / N);
    }

    if (!last)
        return;

    for (auto i = 0; i < m_max_shift; i++)
    {
        m_samples[i].shrink_to_fit();
    }
}
