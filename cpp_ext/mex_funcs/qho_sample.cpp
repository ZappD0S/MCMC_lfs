#include "hmc.hpp"
#include "mex.hpp"
#include "mexAdapter.hpp"
#include <memory>
#include <optional>
#include <string>

// TODO: rename this file

class MexFunction : public matlab::mex::Function
{
    std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr = getEngine();
    matlab::data::ArrayFactory factory;

    void displayOnMATLAB(const std::string &str)
    {
        matlabPtr->feval(u"fprintf", 0,
                         std::vector<matlab::data::Array>({factory.createScalar(str + "\n")}));
    }

  public:
    void operator()(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs)
    {
        // TODO: argument checks?

        // obj is MATLAB "this"
        // matlab::data::Array obj = std::move(inputs[0]);
        matlab::data::Array obj(inputs[0]);

        matlab::data::TypedArray<matlab::data::MATLABString> sys_type_matlab = matlabPtr->getProperty(obj, u"sys_type");
        auto sys_type = std::string(sys_type_matlab[0]);

        int N_warmup = matlabPtr->getProperty(obj, u"N_warmup")[0];
        int N_sample = matlabPtr->getProperty(obj, u"N_sample")[0];

        int NHmc = matlabPtr->getProperty(obj, u"NHmc")[0];
        double epsilon = matlabPtr->getProperty(obj, u"epsilon")[0];

        int max_shift = matlabPtr->getProperty(obj, u"max_shift")[0];

        bool rev_check = matlabPtr->getProperty(obj, u"rev_check")[0];

        std::optional<int> seed;
        auto maybe_seed = matlabPtr->getProperty(obj, u"seed");
        if (!maybe_seed.isEmpty())
        {
            seed = maybe_seed[0];
        }

        int N = inputs[1][0];
        double m0 = inputs[2][0];
        double omg0 = inputs[3][0];

        std::vector<double> Phi0(N, 0.0);

        std::shared_ptr<HMCSystem> sys;

        if (sys_type == "qho")
        {
            sys = std::make_unique<QhoSystem>(N, m0, omg0);
        }
        else if (sys_type == "qao")
        {
            double lambda0 = inputs[4][0];
            sys = std::make_unique<QaoSystem>(N, m0, omg0, lambda0);
        }
        else
        {
            matlabPtr->feval(u"error",
                             0, std::vector<matlab::data::Array>({factory.createScalar("Unknown system type")}));
            return;
        }

        assert(sys);
        HMC hmc(sys, NHmc, epsilon, rev_check, seed);

        std::vector<std::shared_ptr<HMCCallback>> callbacks;
        callbacks.reserve(2);

        auto lastPhi_callback = std::make_shared<LastPhiCallback>();
        callbacks.push_back(lastPhi_callback);

        if (hmc.run(Phi0, N_warmup, callbacks) != 0)
        {
            matlabPtr->feval(u"error",
                             0,
                             std::vector<matlab::data::Array>(
                                 {factory.createScalar("leapfrog not reversible!")}));
        }

        Phi0 = lastPhi_callback->get_data();

        callbacks.clear();
        auto e0_callback = std::make_shared<E0Callback>(sys, N_sample);
        auto xx_callback = std::make_shared<XXCallback>(max_shift, N_sample);
        callbacks.push_back(e0_callback);
        callbacks.push_back(xx_callback);

        if (hmc.run(Phi0, N_sample, callbacks) != 0)
        {
            matlabPtr->feval(u"error",
                             0,
                             std::vector<matlab::data::Array>(
                                 {factory.createScalar("leapfrog not reversible!")}));
        }

        const auto &e0_samples = e0_callback->get_data();
        const auto &xx_samples = xx_callback->get_data();

        auto e0_matlab = factory.createArray(
            {1, e0_samples.size()},
            e0_samples.begin(),
            e0_samples.end());

        auto xx_matlab = factory.createArray<double>(
            {xx_samples[0].size(), xx_samples.size()});

        auto xx_matlab_it = xx_matlab.begin();
        for (auto column = xx_samples.begin(); column != xx_samples.end(); column++)
        {
            xx_matlab_it = std::copy(column->begin(), column->end(), xx_matlab_it);
        }

        outputs[0] = obj;
        outputs[1] = std::move(e0_matlab);
        outputs[2] = std::move(xx_matlab);
    }
};