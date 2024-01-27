#pragma once
#include <vector>
#include <memory>
#include "cppshrhelp.hpp"

class DLL_EXPORT HMCCallback
{
public:
    virtual void operator()(const std::vector<double> &Phi, bool last) = 0;

    // virtual ~HMCCallback() = default;
};

class DLL_EXPORT LastPhiCallback : public HMCCallback
{
private:
    std::unique_ptr<std::vector<double>> last_Phi;

public:
    const std::vector<double> &get_data() { return *last_Phi; }

    void operator()(const std::vector<double> &Phi, bool last);
};

class DLL_EXPORT E0Callback : public HMCCallback
{
private:
    std::unique_ptr<std::vector<double>> m_samples =
        std::make_unique<std::vector<double>>();

public:
    const std::vector<double> &get_data() { return *m_samples; }

    void operator()(const std::vector<double> &Phi, bool last);
};

class DLL_EXPORT DeltaECallback : public HMCCallback
{
private:
    std::unique_ptr<std::vector<std::vector<double>>> m_samples;
    int m_max_shift;

public:
    const std::vector<std::vector<double>> &get_data() { return *m_samples; }

    explicit DeltaECallback(int max_shift)
        : m_samples(std::make_unique<std::vector<std::vector<double>>>(max_shift)),
          m_max_shift(max_shift)
    {
    }

    void operator()(const std::vector<double> &Phi, bool last);
};