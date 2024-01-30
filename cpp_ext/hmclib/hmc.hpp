#include <memory>
#include <random>
#include <vector>

#include "callback.hpp"
#include "cppshrhelp.hpp"
#include "leapfrog.hpp"
#include "system.hpp"

class DLL_EXPORT HMC
{
  private:
    std::shared_ptr<HMCSystem> m_system;
    std::unique_ptr<Leapfrog> m_leapfrog;
    std::unique_ptr<std::mt19937> m_rng;

    double T(const std::vector<double> &Phi) const;

  public:
    const HMCSystem &get_system() { return *m_system; }

    HMC(std::shared_ptr<HMCSystem> system, int Nhmc, double epsilon, int seed);

    int run(const std::vector<double> &Phi0, int Niter, std::vector<std::shared_ptr<HMCCallback>> &callbacks) const;
};
