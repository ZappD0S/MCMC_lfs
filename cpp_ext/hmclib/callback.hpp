#pragma once
#include "cppshrhelp.hpp"
#include <vector>

class DLL_EXPORT HMCCallback
{
  public:
    virtual void operator()(const std::vector<double> &Phi, bool last) = 0;
};

class DLL_EXPORT LastPhiCallback : public HMCCallback
{
  private:
    std::vector<double> m_last_Phi;

  public:
    const std::vector<double> &get_data() { return m_last_Phi; }

    void operator()(const std::vector<double> &Phi, bool last);
};

class DLL_EXPORT E0Callback : public HMCCallback
{
  private:
    std::vector<double> m_samples;

  public:
    explicit E0Callback(std::size_t reserve_size = 0)
    {
        m_samples.reserve(reserve_size);
    }

    const std::vector<double> &get_data() { return m_samples; }

    void operator()(const std::vector<double> &Phi, bool last);
};

class DLL_EXPORT DeltaECallback : public HMCCallback
{
  private:
    std::vector<std::vector<double>> m_samples;
    int m_max_shift;

  public:
    const std::vector<std::vector<double>> &get_data() { return m_samples; }

    explicit DeltaECallback(int max_shift, int reserve_size = 0)
        : m_samples(std::vector<std::vector<double>>(max_shift)),
          m_max_shift(max_shift)
    {
        for (int k = 0; k < max_shift; k++)
        {
            m_samples[k].reserve(reserve_size);
        }
    }

    void operator()(const std::vector<double> &Phi, bool last);
};