#ifndef _SYSTEM_PARAMETERS_H_
#define _SYSTEM_PARAMETERS_H_

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
    const Vec3 FreestreamVelocity = {100, 0, 0};
    
    // Motor Parameters
    constexpr float MotorResistance = 0.013;
    constexpr float MotorVelcoityConstant = 105 * (2 * PI / 60);
    constexpr float MotorTorqueConstant = 0.0909f;
    constexpr float MotorRotorMomentOfInertia = 0.0021865f;

    // Propeller / Hub Geometry
    constexpr float HubRadius = 0.05;
    constexpr float PropellerRadius = 0.5;
    constexpr size_t NumBlades = 4;
    constexpr float PropellerMomentOfInertia = 100;

    // Define all blade airfoils here.
    constexpr enum class Airfoil
    {
        NACA2412 = 0
    };

    // Returns the chord length at a given radial position.
    constexpr float bladeChordAt(const float& r)
    {
        assert(r >= HubRadius && r <= PropellerRadius);
        return 0.1f;
    }

    // Returns the AoA at a given radial position.
    constexpr float bladeAlphaAt(const float& r)
    {
        assert(r >= HubRadius && r <= PropellerRadius);
        return PI / 60;
    }

    // Returns the airfoil at a given radial position.
    constexpr Airfoil bladeAirfoilAt(const float& r)
    {
        assert(r >= HubRadius && r <= PropellerRadius);
        return Airfoil::NACA2412;
    }

    // Airflow from leading edge to trailing edge.
    constexpr float dragCoefficientAt(const float& r)
    {
        return 0.05;
    }

    // Airflow from trailing edge to leading edge.
    constexpr float reverseDragCoefficientAt(const float& r)
    {
        return 0.03;
    }
}

#endif // _SYSTEM_PARAMETERS_H_