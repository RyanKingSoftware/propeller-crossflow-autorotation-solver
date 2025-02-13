#ifndef UTIL_H_
#define UTIL_H_

#include <array>
#include <vector>

namespace Util
{
    template<typename T, size_t N>
    constexpr std::array<T, N> linspace(T start, T end)
    {
        std::array<T, N> result;
        T step = (end - start) / (N);

        for (size_t i = 0; i < N; ++i)
        {
            result[i] = start + i * step;
        }

        return result;
    }

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
}

#endif // UTIL_H_
