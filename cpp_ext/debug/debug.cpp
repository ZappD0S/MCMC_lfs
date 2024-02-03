#include "hmc.hpp"
#include <iostream>

int main()
{
    std::cout << "start" << std::endl;

    auto sys = std::make_unique<QhoSystem>(100, 1.0, 1.0);
    HMC hmc(std::move(sys), 100, 0.1, 1234);

    std::vector<std::shared_ptr<HMCCallback>> callbacks;
    callbacks.reserve(2);

    auto lastPhi_callback = std::make_shared<LastPhiCallback>();
    callbacks.push_back(lastPhi_callback);

    std::vector<double> Phi0(100, 0.0);

    if (hmc.run(Phi0, 1e5, callbacks) != 0)
    {
        std::cout << "error!" << std::endl;
        return -1;
    }

    Phi0 = lastPhi_callback->get_data();

    callbacks.clear();
    auto e0_callback = std::make_shared<X2Callback>(1e6);
    auto de_callback = std::make_shared<XXCallback>(10, 1e6);
    callbacks.push_back(e0_callback);
    callbacks.push_back(de_callback);

    if (hmc.run(Phi0, 1e6, callbacks) != 0)
    {
        std::cout << "error!" << std::endl;
        return -1;
    }

    const auto &arr = e0_callback->get_data();

    // std::cout << std::to_string(arr[0]) << std::endl;
    std::cout << "end" << std::endl;

    return 0;
}