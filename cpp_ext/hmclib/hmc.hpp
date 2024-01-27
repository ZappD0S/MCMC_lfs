#include <vector>
#include <random>
#include <memory>

#include "callback.hpp"
#include "system.hpp"
#include "cppshrhelp.hpp"

class DLL_EXPORT HMC
{
private:
    std::unique_ptr<HMCSystem> m_system;
    int m_Nhmc;
    double m_epsilon;
    std::unique_ptr<std::mt19937> m_rng;
    std::unique_ptr<std::vector<std::size_t>> m_idx;

    struct PhaseSpaceCoords
    {
        std::unique_ptr<std::vector<double>> Phi;
        std::unique_ptr<std::vector<double>> Pi;
    };
    std::tuple<bool, HMC::PhaseSpaceCoords> leapfrog_rev_check(const PhaseSpaceCoords &ps_coords) const;

    PhaseSpaceCoords leapfrog(const PhaseSpaceCoords &ps_coords, bool backward = false) const;

    double T(const std::vector<double> &Phi) const;

public:
    const HMCSystem &get_system() { return *m_system; }

    HMC(std::unique_ptr<HMCSystem> system, int Nhmc, double epsilon, int seed);

    // TODO: instead return std::tuple<bool, std::vector<double>> ?
    int run(const std::vector<double> &Phi0, int Niter, std::vector<std::shared_ptr<HMCCallback>> &callbacks) const;
};
