#pragma once
#include "cppshrhelp.hpp"
#include <vector>

class DLL_EXPORT HMCSystem
{
  public:
    virtual double S(const std::vector<double> &Phi) = 0;

    virtual int size() const = 0;

    virtual const std::vector<double> &dSdPhi(const std::vector<double> &Phi) = 0;
};

class DLL_EXPORT QhoSystem : public HMCSystem
{
    int m_N;
    double m_m0;
    double m_omg0;
    std::vector<double> m_dS;

  public:
    QhoSystem(int N, double m0, double omg0);

    int size() const override { return m_N; }

    double S(const std::vector<double> &Phi) override;

    const std::vector<double> &dSdPhi(const std::vector<double> &Phi) override;
};
