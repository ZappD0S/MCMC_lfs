#include <algorithm>
#include <cmath>
#include <execution>
#include <iostream>
#include <numeric>
#include <oneapi/tbb/tbb_allocator.h>
#include <string>
#include <vector>

// taken  from https://stackoverflow.com/a/868894

class InputParser
{
  public:
    InputParser(int &argc, char **argv)
    {
        for (int i = 1; i < argc; ++i)
            this->tokens.push_back(std::string(argv[i]));
    }
    /// @author iain
    const std::string &getCmdOption(const std::string &option) const
    {
        std::vector<std::string>::const_iterator itr;
        itr = std::find(this->tokens.begin(), this->tokens.end(), option);
        if (itr != this->tokens.end() && ++itr != this->tokens.end())
        {
            return *itr;
        }
        static const std::string empty_string("");
        return empty_string;
    }
    /// @author iain
    bool cmdOptionExists(const std::string &option) const
    {
        return std::find(this->tokens.begin(), this->tokens.end(), option) != this->tokens.end();
    }

  private:
    std::vector<std::string> tokens;
};

int main(int argc, char **argv)
{
    InputParser input(argc, argv);
    const std::string &n_str = input.getCmdOption("-n");
    int n = std::stoi(n_str);

    std::vector<double> v(100);
    // std::iota(v.begin(), v.end(), 0);

    std::generate(v.begin(), v.end(), []()
                  { return ((double)std::rand() / (double)RAND_MAX); });

    // auto sad = tbb::detail::d1::tbb_allocator::scalable<double>();

    double sum = 0.0;

    for (int i = 0; i < n; i++)
    {
        auto x = std::transform_reduce(
                                       v.begin(), v.end(),
                                       0.0, std::plus{}, [](auto x)
                                       { return std::pow(x, 2); });
        sum += x / v.size();
    }

    std::cout << std::to_string(sum) << std::endl;

    return 0;
}