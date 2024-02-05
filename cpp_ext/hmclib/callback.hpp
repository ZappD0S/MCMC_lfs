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

    void operator()(const std::vector<double> &Phi, bool last) override;
};

class DLL_EXPORT X2Callback : public HMCCallback
{
  private:
    std::vector<double> m_samples;
    std::vector<double> m_x2_buffer;

  public:
    explicit X2Callback(std::size_t reserve_size = 0)
    {
        m_samples.reserve(reserve_size);
    }

    const std::vector<double> &get_data() { return m_samples; }

    void operator()(const std::vector<double> &Phi, bool last) override;
};

class DLL_EXPORT XXCallback : public HMCCallback
{
  private:
    std::vector<std::vector<double>> m_samples;
    int m_max_shift;
    std::vector<double> m_xx_buffer;

  public:
    const std::vector<std::vector<double>> &get_data() { return m_samples; }

    explicit XXCallback(int max_shift, int reserve_size = 0)
        : m_samples(std::vector<std::vector<double>>(max_shift)),
          m_max_shift(max_shift)
    {
        for (auto i = 0; i < max_shift; i++)
        {
            m_samples[i].reserve(reserve_size);
        }
    }

    void operator()(const std::vector<double> &Phi, bool last) override;
};