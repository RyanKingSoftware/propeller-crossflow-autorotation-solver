#ifndef _UTIL_H_
#define _UTIL_H_

#include <algorithm>
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

    template <typename T>
    std::vector<T> downsampleMinmax(const std::vector<T>& data, size_t windowSize) {
        if (windowSize == 0 || data.size() <= 2) return data;

        std::vector<T> downsampled;
        for (size_t i = 0; i < data.size(); i += windowSize) {
            size_t end = std::min(i + windowSize, data.size());
            auto [min_it, max_it] = std::minmax_element(data.begin() + i, data.begin() + end);
            downsampled.push_back(*min_it);
            downsampled.push_back(*max_it);
        }   
        return downsampled;
    }

    void writeSolutionToCsv(const Solution& solution);
}

#endif // _UTIL_H_