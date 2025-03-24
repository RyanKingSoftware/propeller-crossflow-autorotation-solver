#ifndef _SYSTEM_PARAMETERS_H_
#define _SYSTEM_PARAMETERS_H_

#include <algorithm> // For std::min and std::max
#include <cassert>

#include "vec3.h"

namespace SystemParameters
{
    constexpr float PI = 3.1415927f;

    // Simulation Parameters
    constexpr float SimTime = 3000;
    constexpr float TimeStep = 0.01;
    constexpr float RadialStep = 0.01;

    // Flight Conditions
    constexpr float AirDensity = 1.225;
    constexpr float KinematicViscosity = 0.00001461;
    const Vec3 FreestreamVelocity = {50, 0, 0};
    
    // Inital Conditions
    constexpr float InitialAngularVelocity = 0;
    
    // Motor Parameters
    constexpr float MotorResistance = 0.013;
    constexpr float MotorVelcoityConstant = 105 * (2 * PI / 60);
    constexpr float MotorTorqueConstant = 0.0909f;
    constexpr float MotorRotorMomentOfInertia = 0.0021865;

    // Propeller / Hub Geometry
    constexpr float HubRadius = 0.05;
    constexpr float PropellerRadius = 0.5;
    constexpr size_t NumBlades = 4;
    constexpr float PropellerMomentOfInertia = 10;

    enum class Airfoil
    {
        NACA2412 = 0
    };

    constexpr float bladeChordAt(const float& r)
    {
        assert(r >= HubRadius && r <= PropellerRadius);
        return 0.05f;
    }

    constexpr float bladeAlphaAt(const float& r)
    {
        assert(r >= HubRadius && r <= PropellerRadius);
        return 0.0872665; // 5 deg
    }

    constexpr Airfoil bladeAirfoilAt(const float& r)
    {
        assert(r >= HubRadius && r <= PropellerRadius);
        return Airfoil::NACA2412;
    }

    constexpr float dragCoefficientAt(const float& r, const float& reynolds)
    {
        return 0.005; // ~ Re = 300,000 + 5deg AOA 0.005
    }

    constexpr float liftCoefficientAt(const float& r, const float& reynolds)
    {
        return 0.75; // Usually at 5deg AOA
    }

    constexpr float reverseDragCoefficientAt(const float& r, const float& reynolds)
    {
        return 0.03;
    }

    constexpr float reverseLiftCoefficientAt(const float& r, const float& reynolds)
    {
        
        return 0.3;
    }
}

#endif // _SYSTEM_PARAMETERS_H_