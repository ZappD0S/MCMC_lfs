#include "cppshrhelp.hpp"
#include "system.hpp"
#include <memory>
#include <vector>

struct PhaseSpaceCoords
{
    std::vector<double> Phi;
    std::vector<double> Pi;
};

class DLL_EXPORT Leapfrog
{
  private:
    PhaseSpaceCoords m_state0;
    PhaseSpaceCoords m_state0_rev;
    virtual void step_impl(PhaseSpaceCoords &state, bool backward = false) const = 0;

  public:
    bool step(PhaseSpaceCoords &state);
};

class DLL_EXPORT VanillaLeapfrog : public Leapfrog
{
  private:
    std::shared_ptr<HMCSystem> m_system;
    int m_Nhmc;
    double m_epsilon;

    inline void update_Phi(std::vector<double> &Phi, const std::vector<double> &Pi, double epsilon) const;
    inline void update_Pi(std::vector<double> &Pi, const std::vector<double> &Phi, double epsilon) const;

    void step_impl(PhaseSpaceCoords &state, bool backward = false) const override;

  public:
    VanillaLeapfrog(std::shared_ptr<HMCSystem> system, int NHmc, double epsilon)
        : m_system(system),
          m_Nhmc(NHmc),
          m_epsilon(epsilon)
    {
    }
};