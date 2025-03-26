#ifndef _UTIL_H_
#define _UTIL_H_

#include <array>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

namespace Util
{
    template<typename T, size_t N>
    std::array<T, N> linspace(T start, T end)
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

// Function to write the results to a CSV file
void writeSolutionToCsv(const std::vector<std::vector<float>>& solution, const std::string& filename)
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error opening file for writing!" << std::endl;
        return;
    }

    // Write the header
    file << "Time,Angular Position,Angular Velocity,Angular Acceleration,Lift,Drag,Torque\n";

    // Write the data (transpose the data from solution)
    for (size_t t = 0; t < solution[0].size(); ++t)
    {
        file << solution[0][t] << "," // Time
             << solution[1][t] << "," // Angular Position
             << solution[2][t] << "," // Angular Velocity
             << solution[3][t] << "," // Angular Acceleration
             << solution[4][t] << "," // Lift
             << solution[5][t] << "," // Fx
             << solution[6][t] << "," // Fy
             << solution[7][t] << "\n"; // Torque
    }

    file.close();
    std::cout << "Data written to " << filename << std::endl;
}

#endif // _UTIL_H_