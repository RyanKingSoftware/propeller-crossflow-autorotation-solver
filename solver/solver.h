#ifndef _SOLVER_H_
#define _SOLVER_H_

#include <vector>
#include <iostream>

#include "system_parameters.h"
#include "vec3.h"
#include "util.h"

namespace Solver
{
    void solve()
    {
        // Blade Angles
        constexpr std::array<float, SystemParameters::NumBlades> BladeAngles = Util::linspace<float, SystemParameters::NumBlades>(0, 2 * SystemParameters::PI);

        // Radius Discretization
        constexpr float BladeLength = SystemParameters::PropellerRadius - SystemParameters::HubRadius;
        constexpr size_t RadialSteps = BladeLength / SystemParameters::RadialStep;
        constexpr std::array<float, RadialSteps> RadialDiscretization = Util::linspace<float, RadialSteps>(SystemParameters::HubRadius, SystemParameters::PropellerRadius);

        // Solution State and Time Discretization
        constexpr size_t TimeSteps = (SystemParameters::SimTime / SystemParameters::TimeStep);

        // Time, angular position, angular velocity, angular acceleration, lift, drag(fx), side force(fy), torque
        std::vector<std::vector<float>> solution(8, std::vector<float>(TimeSteps, 0.0f));        
        solution[0] = Util::linspace<float>(0,  SystemParameters::SimTime, TimeSteps);

        // Initial Conditions
        solution[2][0] = SystemParameters::InitialAngularVelocity;

        // Solve
        float torque, lift, phi, tangentialLocalVelocity, dynamicPressure, sectionDrag, reynolds, fx, fy;
        float k1, k2, k3, k4;
        Vec3 omega, phiHat, rHat, radius, circularVeclocity, localVelocity;

        for (int t = 0; t < solution[0].size() - 1; ++t)
        {
            torque = 0.0f;
            lift = 0.0f;
            fx = 0.0f; // drag
            fy = 0.0f;
            omega = {0,0,solution[2][t]};

            for(const float& bladeAngle : BladeAngles)
            {
                phi = bladeAngle + solution[1][t];

                rHat = {std::cos(phi), std::sin(phi), 0};
                phiHat = {-rHat[1], rHat[0], 0};                

                for(const float r : RadialDiscretization)
                {
                    radius = r * rHat;   
                    circularVeclocity = cross(omega, radius);
                    localVelocity = circularVeclocity + SystemParameters::FreestreamVelocity;
                    
                    tangentialLocalVelocity = dot(localVelocity, phiHat);
                    dynamicPressure = 0.5f * SystemParameters::AirDensity * std::pow(tangentialLocalVelocity, 2);
                    reynolds = (tangentialLocalVelocity * SystemParameters::bladeChordAt(r)) / SystemParameters::KinematicViscosity;

                    if(tangentialLocalVelocity > 0)
                    {
                        sectionDrag = dynamicPressure * SystemParameters::bladeChordAt(r) * SystemParameters::RadialStep * SystemParameters::dragCoefficientAt(r, reynolds);
                        lift += dynamicPressure * SystemParameters::bladeChordAt(r) * SystemParameters::RadialStep * SystemParameters::liftCoefficientAt(r, reynolds);
                        torque -= sectionDrag * r;
                        fx += std::sin(phi) * sectionDrag;
                        fy -= std::cos(phi) * sectionDrag;
                    }
                    else // Reversed flow
                    {
                        sectionDrag = dynamicPressure * SystemParameters::bladeChordAt(r) * SystemParameters::RadialStep * SystemParameters::reverseDragCoefficientAt(r, reynolds);
                        lift += dynamicPressure * SystemParameters::bladeChordAt(r) * SystemParameters::RadialStep * SystemParameters::reverseLiftCoefficientAt(r, reynolds);
                        torque += sectionDrag * r;
                        fx -= std::sin(phi) * sectionDrag;
                        fy += std::cos(phi) * sectionDrag;
                    }
                }
            }

            torque -= (solution[2][t] * SystemParameters::MotorTorqueConstant) / (SystemParameters::MotorResistance * SystemParameters::MotorVelcoityConstant);

            solution[3][t] = torque / (SystemParameters::PropellerMomentOfInertia + SystemParameters::MotorRotorMomentOfInertia); // angular acceleration
            solution[4][t] = lift; //even when i explicityly set it?
            solution[5][t] = fx;
            solution[6][t] = fy;
            solution[7][t] = torque;

            // RK4 Integration
            #define dt SystemParameters::TimeStep

            // RK4 for angular position (solution[1])
            k1 = solution[2][t];
            k2 = solution[2][t] + (0.5f * dt * k1);
            k3 = solution[2][t] + (0.5f * dt * k2);
            k4 = solution[2][t] + (dt * k3);
            solution[1][t+1] = solution[1][t] + ((dt / 6) * (k1 + 2*k2 + 2*k3 + k4));

            // RK4 for angular velocity (solution[2])
            k1 = solution[3][t];
            k2 = solution[3][t] + (0.5f * dt * k1);
            k3 = solution[3][t] + (0.5f * dt * k2);
            k4 = solution[3][t] + (dt * k3);
            solution[2][t+1] = solution[2][t] + ((dt / 6) * (k1 + 2*k2 + 2*k3 + k4));
        }
        std::cout << "Complete" << std::endl;
        std::cout << "Steady State Omega: " << solution[2][solution[0].size()-1] << std::endl;
        std::cout << "Steady State Lift: " << solution[4][solution[0].size()-2] << std::endl;

        writeSolutionToCsv(solution, "solution.csv");
    }
}

#endif // _SOLVER_H_
