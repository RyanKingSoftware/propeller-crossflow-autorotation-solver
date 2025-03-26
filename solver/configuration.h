#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include <cmath>

#include "vec3.h"

struct Configuration
{
    // Simulation Parameters
    float simTime = 3000;
    float timeStep = 0.01;
    float radialStep = 0.01;

    // Flight Conditions
    Vec3 freestreamVelocity = {50, 0, 0};
    float airDensity = 1.225;
    float kinematicViscosity = 0.00001461;

    // Inital Conditions
    float initialAngularVelocity = 0;
    
    // Motor Parameters
    float motorResistance = 0.013;
    float motorVelcoityConstant = 105 * (2 * M_PI / 60);
    float motorTorqueConstant = 0.0909f;
    float motorRotorMomentOfInertia = 0.0021865;

    // Propeller and Hub Geometry
    float propellerRadius = 0.5;
    int numBlades = 4;
    float propellerMomentOfInertia = 10;
    float hubRadius = 0.05;
};

#endif // _CONFIGURATION_H_