#ifndef _SYSTEM_PARAMETERS_H_
#define _SYSTEM_PARAMETERS_H_

#include <algorithm> // For std::min and std::max
#include <cassert>

#include "vec3.h"

namespace SystemParameters
{
    enum class Airfoil
    {
        NACA2412 = 0
    };

    constexpr float bladeChordAt(const float& r)
    {
        //assert(r >= HubRadius && r <= PropellerRadius);
        return 0.05f;
    }

    constexpr float bladeAlphaAt(const float& r)
    {
        //assert(r >= HubRadius && r <= PropellerRadius);
        return 0.0872665; // 5 deg
    }

    constexpr Airfoil bladeAirfoilAt(const float& r)
    {
        //assert(r >= HubRadius && r <= PropellerRadius);
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