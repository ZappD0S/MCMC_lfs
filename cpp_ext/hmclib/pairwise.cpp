#include "pairwise.hpp"

double pairwise_sum(const std::span<double> x)
{
    auto N = x.size();

    if (N <= BLOCKSIZE)
    {
        double tot = 0.0;

        for (auto i = 0; i < static_cast<int>(N); i++)
        {
            tot += x[i];
        }

        return tot;
    }

    auto N2 = N / 2;
    return pairwise_sum(x.subspan(0, N2)) +
           pairwise_sum(x.subspan(N2));
}
