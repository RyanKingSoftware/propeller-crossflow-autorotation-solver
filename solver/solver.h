#ifndef _SOLVER_H_
#define _SOLVER_H_

#include <atomic>
#include <cmath>
#include <vector>
#include <iostream>

#include "configuration.h"
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
        float phi, tangentialLocalVelocity, dynamicPressure, sectionDrag, reynolds, hubReynolds;
        float k1, k2, k3, k4;
        Vec3 omega, phiHat, rHat, radius, circularVeclocity, localVelocity;

        for (int t = 0; t < solution.time.size(); ++t)
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
                    reynolds = (tangentialLocalVelocity * configuration.bladeChordAt(r)) / configuration.kinematicViscosity;

                    if(tangentialLocalVelocity > 0)
                    {
                        sectionDrag = dynamicPressure * configuration.bladeChordAt(r) * configuration.radialStep * configuration.dragCoefficientAt(r, reynolds);
                        solution.lift[t] += dynamicPressure * configuration.bladeChordAt(r) * configuration.radialStep * configuration.liftCoefficientAt(r, reynolds);
                        solution.torque[t] -= sectionDrag * r;
                        solution.drag[t] -= std::sin(phi) * sectionDrag;
                        solution.sideForce[t] -= std::cos(phi) * sectionDrag;
                    }
                    else // Reversed flow
                    {
                        sectionDrag = dynamicPressure * configuration.bladeChordAt(r) * configuration.radialStep * configuration.reverseDragCoefficientAt(r, reynolds);
                        solution.lift[t] += dynamicPressure * configuration.bladeChordAt(r) * configuration.radialStep * configuration.reverseLiftCoefficientAt(r, reynolds);
                        solution.torque[t] += sectionDrag * r;
                        solution.drag[t] += std::sin(phi) * sectionDrag;
                        solution.sideForce[t] += std::cos(phi) * sectionDrag;
                    }
                }
            }
            
            // Hub Drag
            hubReynolds = (2 * configuration.freestreamVelocity[0] * configuration.hubRadius) / configuration.kinematicViscosity;
            if (hubReynolds <= 10)
            {
                // Cd = 24 / Re
                solution.drag[t] += (24 / hubReynolds) *  configuration.hubRadius * configuration.airDensity * std::pow(configuration.freestreamVelocity[0], 2) * configuration.hubHieght;
            } 
            else if (hubReynolds <= 1000)
            {
                // Cd = -0.002Re + 2.42
                solution.drag[t] += ((-0.002 * hubReynolds) + 2.42) *  configuration.hubRadius * configuration.airDensity * std::pow(configuration.freestreamVelocity[0], 2) * configuration.hubHieght;
            }
            else if (hubReynolds <= 300000)
            {
                // Cd = 0.5
                solution.drag[t] += 0.5 * configuration.hubRadius * configuration.airDensity * std::pow(configuration.freestreamVelocity[0], 2) * configuration.hubHieght;
            }
            else
            {
                // Cd = 0.15
                solution.drag[t] += 0.15 * configuration.hubRadius * configuration.airDensity * std::pow(configuration.freestreamVelocity[0], 2) * configuration.hubHieght;
            }

            solution.torque[t] -= solution.angularVelocity[t] / (configuration.motorVelocityConstant * configuration.motorVelocityConstant * configuration.motorResistance);

            solution.angularAcceleration[t] = solution.torque[t] / (configuration.propellerMomentOfInertia + configuration.motorRotorMomentOfInertia);
            
            if(t+1 == solution.time.size()) break;

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
        solution.clean();
        return solution;
    }
}

#endif // _SOLVER_H_
