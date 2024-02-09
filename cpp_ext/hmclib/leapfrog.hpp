#include "system.hpp"
#include <memory>
#include <vector>

bool allclose(
    const std::vector<double> &a,
    const std::vector<double> &b,
    double rtol = 1e-5,
    double atol = 1e-8);

struct PhaseSpaceCoords
{
    std::vector<double> Phi;
    std::vector<double> Pi;
};

class Leapfrog
{
  private:
    bool m_rev_check;
    PhaseSpaceCoords m_state0;
    PhaseSpaceCoords m_state0_rev;
    virtual void step_impl(PhaseSpaceCoords &state, bool backward = false) = 0;

  public:
    bool step(PhaseSpaceCoords &state);

    Leapfrog(bool rev_check) : m_rev_check(rev_check) {}
};

class VanillaLeapfrog : public Leapfrog
{
  private:
    std::shared_ptr<HMCSystem> m_system;
    int m_Nhmc;
    double m_epsilon;
    std::vector<double> m_dS;

    void step_impl(PhaseSpaceCoords &state, bool backward = false) override;

  public:
    VanillaLeapfrog(std::shared_ptr<HMCSystem> system, int NHmc, double epsilon, bool rev_check)
        : Leapfrog(rev_check),
          m_system(system),
          m_Nhmc(NHmc),
          m_epsilon(epsilon),
          m_dS(std::vector<double>(system->size()))
    {
    }
};