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
    std::shared_ptr<HMCSystem> m_system;
    int m_Nhmc;
    double m_epsilon;
    std::vector<std::size_t> m_idx;
    PhaseSpaceCoords m_state0;
    PhaseSpaceCoords m_state0_rev;

    void step_impl(PhaseSpaceCoords &state, bool backward = false) const;

  public:
    Leapfrog(std::shared_ptr<HMCSystem> system, int NHmc, double epsilon)
        : m_system(system),
          m_Nhmc(NHmc),
          m_epsilon(epsilon),
          m_idx(std::vector<std::size_t>(m_system->size()))
    {
        std::iota(m_idx.begin(), m_idx.end(), 0);
    }

    bool step(PhaseSpaceCoords &state);
};