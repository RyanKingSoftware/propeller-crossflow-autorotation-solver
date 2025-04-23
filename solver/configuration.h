#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include "aero_coefficient_interpolator.h"

#include <array>

#include "util.h"
#include "vec3.h"

enum class Airfoil
{
    DAE_51 = 0
};

struct Configuration
{
    // Simulation Parameters
    float simTime = 10;
    float timeStep = 0.001;
    float radialStep = 0.01;

    // Flight Conditions
    Vec3 freestreamVelocity = {87, 0, 0};
    float airDensity = 1.225;
    float kinematicViscosity = 0.00001461;

    // Inital Conditions
    float initialAngularVelocity = 0;
    
    // Motor Parameters
    float motorResistance = 0.125;
    float motorVelocityConstant = 1.75;
    float motorRotorMomentOfInertia = 0.0021865;

    // Propeller and Hub Geometry
    float propellerRadius = 1.5;
    int numBlades = 4;
    float propellerMomentOfInertia = 10;
    float hubRadius = 0.18;
    float hubHieght = 0.25;

    // Blade Geometry
    std::array<float, 11> bladeChord = { 0.0667f, 0.1333f, 0.2205f, 0.2499f, 0.2646f, 0.2499f, 0.2352f, 0.2205f, 0.1764f, 0.137f, 0.00f };
    std::array<float, 11> bladePitch = { 0.9756390519f, 0.9756390519f, 0.9756390519f, 0.8534660042f, 0.7382742736f, 0.6492624817f, 0.5724679947f, 0.5235987756f, 0.4607669225f, 0.4241150082f, 0.3926990817f };

    Airfoil bladeAirfoil = Airfoil::DAE_51;

    float bladeChordAt(const float& r) const
    {
        float span = propellerRadius - hubRadius;
        float t = (r - hubRadius) / span;
        t = std::clamp(t, 0.0f, 1.0f);

        float index = t * (bladeChord.size() - 1);
        int i = static_cast<int>(std::floor(index));
        float frac = index - i;

        if (i >= bladeChord.size() - 1) return bladeChord.back();  // edge case
        return bladeChord[i] * (1.0f - frac) + bladeChord[i + 1] * frac;
    }

    float bladePitchAt(const float& r) const
    {
        float span = propellerRadius - hubRadius;
        float t = (r - hubRadius) / span;
        t = std::clamp(t, 0.0f, 1.0f);

        float index = t * (bladePitch.size() - 1);
        int i = static_cast<int>(std::floor(index));
        float frac = index - i;

        if (i >= bladePitch.size() - 1) return bladePitch.back();  // edge case
        return bladePitch[i] * (1.0f - frac) + bladePitch[i + 1] * frac;
    }

    // Should differentiate between airfoils (for now assume DAE-51)
    float dragCoefficientAt(const float& r, const float& reynolds) const
    {
        //return 0.005; // ~ Re = 300,000 + 5deg AOA 0.005
        return AeroCoefficientInterpolator::Dae51Drag.coefficientAt(bladePitchAt(r), reynolds);
    }

    float liftCoefficientAt(const float& r, const float& reynolds) const
    {
        //return 0.75; // Usually at 5deg AOA
        return AeroCoefficientInterpolator::Dae51Lift.coefficientAt(bladePitchAt(r), reynolds);
    }

    float reverseDragCoefficientAt(const float& r, const float& reynolds) const
    {
        //return 0.03;
        return AeroCoefficientInterpolator::Dae51DragReversed.coefficientAt(bladePitchAt(r), reynolds);
    }

    float reverseLiftCoefficientAt(const float& r, const float& reynolds) const
    {
        return AeroCoefficientInterpolator::Dae51LiftReversed.coefficientAt(bladePitchAt(r), reynolds);
        //return 0.3;
    }
};

#endif // _CONFIGURATION_H_