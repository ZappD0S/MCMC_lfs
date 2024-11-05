#pragma once
#include <span>

#define BLOCKSIZE 16

double pairwise_sum(const std::span<double> x);

// template <typename F>
// inline double pairwise_reduce(std::size_t offset, std::size_t count, F func)
// {

//     if (count <= BLOCKSIZE)
//     {
//         double tot = 0.0;

//         for (auto i = 0; i < count; i++)
//         {
//             tot += func(offset + i);
//         }

//         return tot;
//     }

//     auto half = count / 2;

//     return pairwise_reduce(offset, half, func) +
//            pairwise_reduce(offset + half, count - half, func);
// }

// template <typename F>
// inline double pairwise_reduce(std::size_t count, F func)
// {
//     return pairwise_reduce(0, count, func);
// }
