#include "cppshrhelp.hpp"
#include "system.hpp"
#include <memory>
#include <numeric>
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

class DLL_EXPORT LeapfrogSTL : public Leapfrog
{
  private:
    std::shared_ptr<HMCSystem> m_system;
    int m_Nhmc;
    double m_epsilon;
    std::vector<std::size_t> m_idx;

    void step_impl(PhaseSpaceCoords &state, bool backward = false) const override;

  public:
    LeapfrogSTL(std::shared_ptr<HMCSystem> system, int NHmc, double epsilon)
        : m_system(system),
          m_Nhmc(NHmc),
          m_epsilon(epsilon),
          m_idx(std::vector<std::size_t>(m_system->size()))
    {
        std::iota(m_idx.begin(), m_idx.end(), 0);
    }
};

class DLL_EXPORT LeapfrogVanilla : public Leapfrog
{
  private:
    std::shared_ptr<HMCSystem> m_system;
    int m_Nhmc;
    double m_epsilon;
    PhaseSpaceCoords m_state0;
    PhaseSpaceCoords m_state0_rev;

    static inline void update_Phi(std::vector<double> &Phi, const std::vector<double> &Pi, double epsilon);
    static inline void update_Pi(std::vector<double> &Pi, const std::vector<double> &dS, double epsilon);

    void step_impl(PhaseSpaceCoords &state, bool backward = false) const override;

  public:
    LeapfrogVanilla(std::shared_ptr<HMCSystem> system, int NHmc, double epsilon)
        : m_system(system),
          m_Nhmc(NHmc),
          m_epsilon(epsilon)
    {
    }
};