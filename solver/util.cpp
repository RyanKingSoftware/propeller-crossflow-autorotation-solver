#include "solution.h"
#include "util.h"

void Util::writeSolutionToCsv(const Solution& solution)
{
    std::string solutionFilepath = solution.name + ".csv";
    std::ofstream solutionFile(solutionFilepath);
    if (!solutionFile.is_open())
    {
        std::cerr << "Error opening file for writing!" << std::endl;
        return;
    }

    // Write the header
    solutionFile << "Time,Angular Position,Angular Velocity,Angular Acceleration,Torque,Lift,Drag,Side Force\n";

    // Write the data (transpose the data from solution)
    for (size_t t = 0; t < solution.time.size(); ++t)
    {
        solutionFile << solution.time[t]        << ","
             << solution.angularPosition[t]     << ","
             << solution.angularVelocity[t]     << ","
             << solution.angularAcceleration[t] << ","
             << solution.torque[t]              << ","
             << solution.lift[t]                << ","
             << solution.drag[t]                << ","
             << solution.sideForce[t]           << "\n";
    }

    solutionFile.close();

    std::string configFilename = solution.name + "_config.txt";
    std::ofstream configFile(configFilename);
    if (!configFile.is_open())
    {
        std::cerr << "Error opening file for writing!" << std::endl;
        return;
    }

    configFile << "Simulation Parameters\n";
    configFile << "Sim Time: " << solution.configuration.simTime << "\n";
    configFile << "Time Step: " << solution.configuration.timeStep << "\n";
    configFile << "Radial Step: " << solution.configuration.radialStep << "\n";

    configFile << "\nFlight Conditions\n";
    configFile << "Freestream Velocity: " << solution.configuration.freestreamVelocity[0] << "\n";
    configFile << "Air Density: " << solution.configuration.airDensity << "\n";
    configFile << "Kinematic Viscosity: " << solution.configuration.kinematicViscosity << "\n";

    configFile << "\nInitial Conditions\n";
    configFile << "Initial Angular Velocity: " << solution.configuration.initialAngularVelocity << "\n";

    configFile << "\nMotor Parameters\n";
    configFile << "Motor Resistance: " << solution.configuration.motorResistance << "\n";
    configFile << "Motor Velocity Constant: " << solution.configuration.motorVelcoityConstant << "\n";
    configFile << "Motor Torque Constant: " << solution.configuration.motorTorqueConstant << "\n";
    configFile << "Motor Rotor Moment of Inertia: " << solution.configuration.motorRotorMomentOfInertia << "\n";

    configFile << "\nPropeller and Hub Geometry\n";
    configFile << "Propeller Radius: " << solution.configuration.propellerRadius << "\n";
    configFile << "Number of Blades: " << solution.configuration.numBlades << "\n";
    configFile << "Propeller Moment of Inertia: " << solution.configuration.propellerMomentOfInertia << "\n";
    configFile << "Hub Radius: " << solution.configuration.hubRadius << "\n";

    configFile.close();
}