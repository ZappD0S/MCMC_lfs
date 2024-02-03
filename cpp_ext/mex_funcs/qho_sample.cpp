#include "hmc.hpp"
#include "mex.hpp"
#include "mexAdapter.hpp"

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
        // TODO: checkArguments(outputs, inputs);

        // obj is MATLAB "this"
        // matlab::data::Array obj = std::move(inputs[0]);
        matlab::data::Array obj(inputs[0]);

        int N_warmup = matlabPtr->getProperty(obj, u"N_warmup")[0];
        int N_sample = matlabPtr->getProperty(obj, u"N_sample")[0];

        int NHmc = matlabPtr->getProperty(obj, u"NHmc")[0];
        double epsilon = matlabPtr->getProperty(obj, u"epsilon")[0];

        int seed = matlabPtr->getProperty(obj, u"seed")[0];

        int N = inputs[1][0];
        double m0 = inputs[2][0];
        double omg0 = inputs[3][0];

        std::vector<double> Phi0(N, 0.0);

        auto sys = std::make_unique<QhoSystem>(N, m0, omg0);
        HMC hmc(std::move(sys), NHmc, epsilon, seed);

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
        auto x2_callback = std::make_shared<X2Callback>(N_sample);
        auto xx_callback = std::make_shared<XXCallback>(10, N_sample);
        callbacks.push_back(x2_callback);
        callbacks.push_back(xx_callback);

        if (hmc.run(Phi0, N_sample, callbacks) != 0)
        {
            matlabPtr->feval(u"error",
                             0,
                             std::vector<matlab::data::Array>(
                                 {factory.createScalar("leapfrog not reversible!")}));
        }

        const auto &x2_samples = x2_callback->get_data();
        const auto &xx_samples = xx_callback->get_data();

        auto x2_matlab = factory.createArray(
            {1, x2_samples.size()},
            x2_samples.begin(),
            x2_samples.end());

        auto xx_matlab = factory.createArray<double>(
            {xx_samples[0].size(), xx_samples.size()});

        auto xx_matlab_it = xx_matlab.begin();
        for (auto column = xx_samples.begin(); column != xx_samples.end(); column++)
        {
            xx_matlab_it = std::copy(column->begin(), column->end(), xx_matlab_it);
        }

        outputs[0] = obj;
        outputs[1] = std::move(x2_matlab);
        outputs[2] = std::move(xx_matlab);
    }

    // void checkArguments([[maybe_unused]] matlab::mex::ArgumentList outputs, [[maybe_unused]] matlab::mex::ArgumentList inputs)
    // {
    //     std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr = getEngine();
    //     matlab::data::ArrayFactory factory;

    //     if (inputs.size() != 2)
    //     {
    //         matlabPtr->feval(u"error",
    //                          0, std::vector<matlab::data::Array>({factory.createScalar("Two inputs required")}));
    //     }

    //     if (inputs[0].getNumberOfElements() != 1)
    //     {
    //         matlabPtr->feval(u"error",
    //                          0, std::vector<matlab::data::Array>({factory.createScalar("Input multiplier must be a scalar")}));
    //     }

    //     if (inputs[0].getType() != matlab::data::ArrayType::DOUBLE ||
    //         inputs[0].getType() == matlab::data::ArrayType::COMPLEX_DOUBLE)
    //     {
    //         matlabPtr->feval(u"error",
    //                          0, std::vector<matlab::data::Array>({factory.createScalar("Input multiplier must be a noncomplex scalar double")}));
    //     }

    //     if (inputs[1].getType() != matlab::data::ArrayType::DOUBLE ||
    //         inputs[1].getType() == matlab::data::ArrayType::COMPLEX_DOUBLE)
    //     {
    //         matlabPtr->feval(u"error",
    //                          0, std::vector<matlab::data::Array>({factory.createScalar("Input matrix must be type double")}));
    //     }

    //     if (inputs[1].getDimensions().size() != 2)
    //     {
    //         matlabPtr->feval(u"error",
    //                          0, std::vector<matlab::data::Array>({factory.createScalar("Input must be m-by-n dimension")}));
    //     }
    // }
};