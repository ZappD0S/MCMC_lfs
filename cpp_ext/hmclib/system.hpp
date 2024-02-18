#pragma once
#include "cppshrhelp.hpp"
#include <vector>

class DLL_EXPORT HMCSystem
{
  public:
    virtual double S(const std::vector<double> &Phi) = 0;

    virtual double E0(const std::vector<double> &Phi) = 0;

    virtual int size() const = 0;

    virtual void dSdPhi(std::vector<double> &dS, const std::vector<double> &Phi) const = 0;
};

class DLL_EXPORT QhoSystem : public HMCSystem
{
  private:
    int m_N;
    double m_m0;
    double m_omg0;
    std::vector<double> m_S_buffer;
    std::vector<double> m_E0_buffer;

  public:
    QhoSystem(int N, double m0, double omg0);

    int size() const override { return m_N; }

    double S(const std::vector<double> &Phi) override;

    double E0(const std::vector<double> &Phi) override;

    void dSdPhi(std::vector<double> &dS, const std::vector<double> &Phi) const override;
};

class DLL_EXPORT QaoSystem : public HMCSystem
{
  private:
    int m_N;
    double m_m0;
    double m_omg0;
    double m_lambda0;
    std::vector<double> m_S_buffer;
    std::vector<double> m_E0_buffer;

  public:
    QaoSystem(int N, double m0, double omg0, double lambda0);

    int size() const override { return m_N; }

    double S(const std::vector<double> &Phi) override;

    double E0(const std::vector<double> &Phi) override;

    void dSdPhi(std::vector<double> &dS, const std::vector<double> &Phi) const override;
};
