#include "callback.hpp"
#include "pairwise.hpp"
#include <cassert>
#include <cmath>

void LastPhiCallback::operator()(const std::vector<double> &Phi, bool last)
{
    if (last)
    {
        m_last_Phi = Phi;
    }
}

void E0Callback::operator()(const std::vector<double> &Phi, bool last)
{
    m_samples.push_back(m_sys->E0(Phi));

    if (last)
    {
        m_samples.shrink_to_fit();
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

    for (auto k = 0; k <= m_max_shift; k++)
    {
        for (auto i = 0; i < N - k; i++)
        {
            m_xx_buffer[i] = Phi[i] * Phi[i + k];
        }

        for (auto i = N - k; i < N; i++)
        {
            m_xx_buffer[i] = Phi[i] * Phi[i - (N - k)];
        }

        m_samples[k].push_back(pairwise_sum(m_xx_buffer) / N);
    }

    if (!last)
        return;

    for (auto i = 0; i <= m_max_shift; i++)
    {
        m_samples[i].shrink_to_fit();
    }
}

const std::vector<std::vector<double>> &ConnectedCorrCallback::get_data()
{
    auto N = m_N,
         max_shift = m_max_shift;

    const auto &x = m_x;
    const auto &xx = m_xx;

    for (auto i = 0; i < N; i++)
    {
        for (auto j = i + 1; j - i <= max_shift && j < N; j++)
        {
            auto shift = j - i;
            m_concorr[i][shift - 1] = xx[i][shift - 1] - x[i] * x[j];
        }

        for (auto j = 0; j - i + N <= max_shift; j++)
        {
            auto shift = j - i + N;
            m_concorr[i][shift - 1] = xx[i][shift - 1] - x[i] * x[j];
        }
    }

    return m_concorr;
}

void ConnectedCorrCallback::operator()(const std::vector<double> &Phi, bool last)
{
    auto N = m_N,
         max_shift = m_max_shift;

    assert(static_cast<int>(Phi.size()) == N);

    m_count += 1;

    for (auto i = 0; i < N; i++)
    {
        auto mean = m_x[i];
        m_x[i] += (Phi[i] - mean) / m_count;

        for (auto j = i + 1; j - i <= max_shift && j < N; j++)
        {
            auto shift = j - i;
            auto mean = m_xx[i][shift - 1];
            m_xx[i][shift - 1] += (Phi[j] * Phi[i] - mean) / m_count;
        }

        for (auto j = 0; j - i + N <= max_shift; j++)
        {
            auto shift = j - i + N;
            auto mean = m_xx[i][shift - 1];
            m_xx[i][shift - 1] += (Phi[j] * Phi[i] - mean) / m_count;
        }
    }

    if (last)
    {
        m_count = 0;
    }
}

void AllPhiCallback::operator()(const std::vector<double> &Phi, bool last)
{
    m_samples.push_back(Phi);

    if (last)
    {
        m_samples.shrink_to_fit();
    }
}
