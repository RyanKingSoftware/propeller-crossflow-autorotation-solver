#ifndef _SOLVER_H_
#define _SOLVER_H_

#include <atomic>
#include <cmath>
#include <vector>
#include <iostream>

#include "configuration.h"
#include "system_parameters.h"
#include "solution.h"
#include "vec3.h"
#include "util.h"

namespace Solver
{
    Solution solve(const Configuration configuration, std::atomic<float>& progress) // Configuration Copy
    {
        const std::vector<float> BladeAngles = Util::linspace<float>(0, 2 * Util::PI, (size_t)configuration.numBlades);

        // Radius Discretization
        const float BladeLength = configuration.propellerRadius - configuration.hubRadius;
        const size_t RadialSteps = BladeLength / configuration.radialStep;
        const std::vector<float> RadialDiscretization = Util::linspace<float>(configuration.hubRadius, configuration.propellerRadius, RadialSteps);
        
        // Solution State and Time Discretization
        const size_t TimeSteps = (configuration.simTime / configuration.timeStep);

        // Time, angular position, angular velocity, angular acceleration, lift, drag(fx), side force(fy), torque 
        Solution solution(TimeSteps);

        // Time Discretization
        solution.time = Util::linspace<float>(0,  configuration.simTime, TimeSteps);

        // Initial Conditions
        solution.angularVelocity[0] = configuration.initialAngularVelocity;

        // Solve
        float phi, tangentialLocalVelocity, dynamicPressure, sectionDrag, reynolds;
        float k1, k2, k3, k4;
        Vec3 omega, phiHat, rHat, radius, circularVeclocity, localVelocity;

        for (int t = 0; t < solution.time.size() - 1; ++t)
        {
            progress = ((float)t / ((float)solution.time.size() - 1.0f));
            omega = {0,0,solution.angularVelocity[t]};

            for(const float& bladeAngle : BladeAngles)
            {
                phi = bladeAngle + solution.angularPosition[t];
                rHat = {std::cos(phi), std::sin(phi), 0};
                phiHat = {-rHat[1], rHat[0], 0};                

                for(const float r : RadialDiscretization)
                {
                    radius = r * rHat;   
                    circularVeclocity = cross(omega, radius);
                    localVelocity = circularVeclocity + configuration.freestreamVelocity;
                    
                    tangentialLocalVelocity = dot(localVelocity, phiHat);
                    dynamicPressure = 0.5f * configuration.airDensity * std::pow(tangentialLocalVelocity, 2);
                    reynolds = (tangentialLocalVelocity * SystemParameters::bladeChordAt(r)) / configuration.kinematicViscosity;

                    if(tangentialLocalVelocity > 0)
                    {
                        sectionDrag = dynamicPressure * SystemParameters::bladeChordAt(r) * configuration.radialStep * SystemParameters::dragCoefficientAt(r, reynolds);
                        solution.lift[t] += dynamicPressure * SystemParameters::bladeChordAt(r) * configuration.radialStep * SystemParameters::liftCoefficientAt(r, reynolds);
                        solution.torque[t] -= sectionDrag * r;
                        solution.drag[t] -= std::sin(phi) * sectionDrag;
                        solution.sideForce[t] -= std::cos(phi) * sectionDrag;
                    }
                    else // Reversed flow
                    {
                        sectionDrag = dynamicPressure * SystemParameters::bladeChordAt(r) * configuration.radialStep * SystemParameters::reverseDragCoefficientAt(r, reynolds);
                        solution.lift[t] += dynamicPressure * SystemParameters::bladeChordAt(r) * configuration.radialStep * SystemParameters::reverseLiftCoefficientAt(r, reynolds);
                        solution.torque[t] += sectionDrag * r;
                        solution.drag[t] += std::sin(phi) * sectionDrag;
                        solution.sideForce[t] += std::cos(phi) * sectionDrag;
                    }
                }
            }

            solution.torque[t] -= (solution.angularVelocity[t] * configuration.motorTorqueConstant) / (configuration.motorResistance * configuration.motorVelcoityConstant);
            solution.angularAcceleration[t] = solution.torque[t] / (configuration.propellerMomentOfInertia + configuration.motorRotorMomentOfInertia);

            // RK4 Integration
            #define dt configuration.timeStep

            // RK4 for angular position (solution[1])
            k1 = solution.angularVelocity[t];
            k2 = solution.angularVelocity[t] + (0.5f * dt * k1);
            k3 = solution.angularVelocity[t] + (0.5f * dt * k2);
            k4 = solution.angularVelocity[t] + (dt * k3);
            solution.angularPosition[t+1] = solution.angularPosition[t] + ((dt / 6) * (k1 + 2*k2 + 2*k3 + k4));

            // RK4 for angular velocity (solution[2])
            k1 = solution.angularAcceleration[t];
            k2 = solution.angularAcceleration[t] + (0.5f * dt * k1);
            k3 = solution.angularAcceleration[t] + (0.5f * dt * k2);
            k4 = solution.angularAcceleration[t] + (dt * k3);
            solution.angularVelocity[t+1] = solution.angularVelocity[t] + ((dt / 6) * (k1 + 2*k2 + 2*k3 + k4));
        }
        solution.configuration = std::move(configuration);
        return solution;
    }
}

#endif // _SOLVER_H_
