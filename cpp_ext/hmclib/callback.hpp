#pragma once
#include "cppshrhelp.hpp"
#include "system.hpp"
#include <memory>
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

class DLL_EXPORT E0Callback : public HMCCallback
{
  private:
    std::shared_ptr<HMCSystem> m_sys;
    std::vector<double> m_samples;

  public:
    E0Callback(std::shared_ptr<HMCSystem> sys, std::size_t reserve_size = 0)
        : m_sys(sys)
    {
        m_samples.reserve(reserve_size);
    }

    const std::vector<double> &get_data() { return m_samples; }

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
        : m_samples(std::vector<std::vector<double>>(max_shift + 1)),
          m_max_shift(max_shift)
    {
        for (auto i = 0; i <= max_shift; i++)
        {
            m_samples[i].reserve(reserve_size);
        }
    }

    void operator()(const std::vector<double> &Phi, bool last) override;
};

class DLL_EXPORT ConnectedCorrCallback : public HMCCallback
{
  private:
    std::vector<double> m_x;
    std::vector<std::vector<double>> m_xx;
    std::vector<std::vector<double>> m_concorr;
    int m_N;
    int m_max_shift;
    int m_count;

  public:
    const std::vector<std::vector<double>> &get_data();

    explicit ConnectedCorrCallback(int N, int max_shift)
        : m_x(std::vector<double>(N)),
          m_xx(std::vector<std::vector<double>>(N)),
          m_concorr(std::vector<std::vector<double>>(N)),
          m_N(N),
          m_max_shift(max_shift),
          m_count(0)
    {
        for (auto i = 0; i < N; i++)
        {

            m_concorr[i] = std::vector<double>(max_shift, 0.0);
            m_xx[i] = std::vector<double>(max_shift, 0.0);
        }
    }

    void operator()(const std::vector<double> &Phi, bool last) override;
};

class DLL_EXPORT AllPhiCallback : public HMCCallback
{
  private:
    std::vector<std::vector<double>> m_samples;

  public:
    const std::vector<std::vector<double>> &get_data() { return m_samples; }

    explicit AllPhiCallback(int reserve_size = 0)
        : m_samples(std::vector<std::vector<double>>())
    {
        m_samples.reserve(reserve_size);
    }

    void operator()(const std::vector<double> &Phi, bool last) override;
};