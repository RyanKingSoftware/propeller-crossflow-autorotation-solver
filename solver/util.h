#ifndef _UTIL_H_
#define _UTIL_H_

#include <array>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class Solution;

namespace Util
{
    constexpr float PI = 3.14159265f;

    template<typename T>
    std::vector<T> linspace(T start, T end, size_t N)
    {
        std::vector<T> result(N);
        T step = (end - start) / (N);

        for (size_t i = 0; i < N; ++i)
        {
            result[i] = start + i * step;
        }

        return result;
    }

    void writeSolutionToCsv(const Solution& solution);
}

#endif // _UTIL_H_