#include "solution.h"
#include "util.h"

#include <iomanip>

void Util::writeSolutionToCsv(const Solution& solution)
{
    // Write CSV file
    std::string solutionFilepath = solution.name + ".csv";
    std::ofstream solutionFile(solutionFilepath);
    if (!solutionFile.is_open())
    {
        std::cerr << "Error opening file for writing: " << solutionFilepath << std::endl;
        return;
    }

    // Write the header
    solutionFile << "Time,Angular Position,Angular Velocity,Angular Acceleration,Torque,Lift,Drag,Side Force\n";

    // Write the data (transpose the data from solution)
    for (size_t t = 0; t < solution.time.size(); ++t)
    {
        solutionFile << std::fixed << std::setprecision(6)
                     << solution.time[t] << ","
                     << solution.angularPosition[t] << ","
                     << solution.angularVelocity[t] << ","
                     << solution.angularAcceleration[t] << ","
                     << solution.torque[t] << ","
                     << solution.lift[t] << ","
                     << solution.drag[t] << ","
                     << solution.sideForce[t] << "\n";
    }

    solutionFile.close();

    // Write configuration file
    std::string configFilename = solution.name + "_config.txt";
    std::ofstream configFile(configFilename);
    if (!configFile.is_open())
    {
        std::cerr << "Error opening file for writing: " << configFilename << std::endl;
        return;
    }

    configFile << std::fixed << std::setprecision(6);

    configFile << "Simulation Parameters\n";
    configFile << "Sim Time: " << solution.configuration.simTime << "\n";
    configFile << "Time Step: " << solution.configuration.timeStep << "\n";
    configFile << "Radial Step: " << solution.configuration.radialStep << "\n";

    configFile << "\nFlight Conditions\n";
    configFile << "Freestream Velocity: " << solution.configuration.freestreamVelocity[0] << ", "
               << solution.configuration.freestreamVelocity[1] << ", "
               << solution.configuration.freestreamVelocity[2] << "\n";
    configFile << "Air Density: " << solution.configuration.airDensity << "\n";
    configFile << "Kinematic Viscosity: " << solution.configuration.kinematicViscosity << "\n";

    configFile << "\nInitial Conditions\n";
    configFile << "Initial Angular Velocity: " << solution.configuration.initialAngularVelocity << "\n";

    configFile << "\nMotor Parameters\n";
    configFile << "Motor Resistance: " << solution.configuration.motorResistance << "\n";
    configFile << "Motor Velocity Constant: " << solution.configuration.motorVelocityConstant << "\n";
    configFile << "Motor Rotor Moment of Inertia: " << solution.configuration.motorRotorMomentOfInertia << "\n";

    configFile << "\nPropeller and Hub Geometry\n";
    configFile << "Propeller Radius: " << solution.configuration.propellerRadius << "\n";
    configFile << "Number of Blades: " << solution.configuration.numBlades << "\n";
    configFile << "Propeller Moment of Inertia: " << solution.configuration.propellerMomentOfInertia << "\n";
    configFile << "Hub Radius: " << solution.configuration.hubRadius << "\n";
    configFile << "Hub Height: " << solution.configuration.hubHieght << "\n"; // Added hubHeight

    configFile << "\nBlade Geometry\n";
    configFile << "Blade Airfoil: " << (solution.configuration.bladeAirfoil == Airfoil::DAE_51 ? "DAE_51" : "Unknown") << "\n";
    configFile << "Blade Chord: ";
    for (size_t i = 0; i < solution.configuration.bladeChord.size(); ++i)
    {
        configFile << solution.configuration.bladeChord[i];
        if (i < solution.configuration.bladeChord.size() - 1) configFile << ", ";
    }
    configFile << "\n";
    configFile << "Blade Pitch: ";
    for (size_t i = 0; i < solution.configuration.bladePitch.size(); ++i)
    {
        configFile << solution.configuration.bladePitch[i];
        if (i < solution.configuration.bladePitch.size() - 1) configFile << ", ";
    }
    configFile << "\n";

    configFile.close();
}