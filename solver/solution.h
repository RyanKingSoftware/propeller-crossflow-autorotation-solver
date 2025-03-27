#ifndef _SOLUTION_H_
#define _SOLUTION_H_

#include <vector>
#include <mutex>

#include "configuration.h"
#include "imgui.h"

struct Solution {
    public:

        Solution(){}

        explicit Solution(size_t size);

        // Getters for function pointers in PlotConfiguration
        const std::vector<float>& getTime() const { return time; }
        const std::vector<float>& getAngularPosition() const { return angularPosition; }
        const std::vector<float>& getAngularVelocity() const { return angularVelocity; }
        const std::vector<float>& getAngularAcceleration() const { return angularAcceleration; }
        const std::vector<float>& getTorque() const { return torque; }
        const std::vector<float>& getLift() const { return lift; }
        const std::vector<float>& getDrag() const { return drag; }
        const std::vector<float>& getSideForce() const { return sideForce; }

    public:
        std::vector<float> time, angularPosition, angularVelocity, angularAcceleration, torque, lift, drag, sideForce;
        Configuration configuration;
        std::string name;
        ImVec4 color;

    private:
        static size_t solutionNumber;
};
#endif //_SOLUTION_H_