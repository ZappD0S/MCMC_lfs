#pragma once
#include <vector>
#include <memory>
#include "cppshrhelp.hpp"

class DLL_EXPORT HMCSystem
{
public:
    virtual double S(const std::vector<double> &Phi) const = 0;

    virtual int size() const = 0;

    virtual std::vector<double> dSdPhi(const std::vector<double> &Phi) const = 0;
};

class DLL_EXPORT QhoSystem : public HMCSystem
{
    int m_N;
    double m_m0;
    double m_omg0;
    std::unique_ptr<std::vector<std::size_t>> m_idx;

public:
    QhoSystem(int N, double m0, double omg0);

    int size() const { return m_N; }

    double S(const std::vector<double> &Phi) const;

    std::vector<double> dSdPhi(const std::vector<double> &Phi) const;
};
