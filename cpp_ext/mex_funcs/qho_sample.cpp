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

        displayOnMATLAB(std::to_string(N));
        displayOnMATLAB(std::to_string(N_warmup));
        displayOnMATLAB(std::to_string(N_sample));
        displayOnMATLAB(std::to_string(NHmc));
        displayOnMATLAB(std::to_string(seed));

        auto sys = std::make_unique<QhoSystem>(N, m0, omg0);
        HMC hmc(std::move(sys), NHmc, epsilon, seed);

        std::vector<std::shared_ptr<HMCCallback>> callbacks;
        callbacks.reserve(2);

        auto lastPhi_callback = std::make_shared<LastPhiCallback>();
        callbacks.push_back(lastPhi_callback);

        if (hmc.run(Phi0, N_warmup, callbacks) != 0)
        {
            matlabPtr->feval(u"error",
                             0, std::vector<matlab::data::Array>({factory.createScalar("leapfrog not reversible!")}));
        }

        Phi0 = lastPhi_callback->get_data();

        callbacks.clear();
        auto e0_callback = std::make_shared<E0Callback>();
        auto de_callback = std::make_shared<DeltaECallback>(10);
        callbacks.push_back(e0_callback);
        callbacks.push_back(de_callback);

        if (hmc.run(Phi0, N_sample, callbacks) != 0)
        {
            matlabPtr->feval(u"error",
                             0, std::vector<matlab::data::Array>({factory.createScalar("leapfrog not reversible!")}));
        }

        displayOnMATLAB("we get here!");

        // const auto &e0_samples = e0_callback->get_data();
        // const auto &de_samples = de_callback->get_data();

        auto e0_matlab = factory.createArray<double>({1, 10});

        auto de_matlab = factory.createArray<double>({100, 10});

        // auto e0_matlab = factory.createArray({1, e0_samples.size()}, e0_samples.begin(), e0_samples.end());

        // auto de_matlab = factory.createArray<double>({de_samples[0].size(), de_samples.size()});

        // auto de_matlab_it = de_matlab.begin();
        // for (auto column = de_samples.begin(); column != de_samples.end(); column++)
        // {
        //     de_matlab_it = std::copy(column->begin(), column->end(), de_matlab_it);
        // }

        outputs[0] = obj;
        outputs[1] = std::move(e0_matlab);
        outputs[2] = std::move(de_matlab);
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