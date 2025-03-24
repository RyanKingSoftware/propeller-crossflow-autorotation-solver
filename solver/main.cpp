// Propeller Autorotation Solver for the Perpendicular Crossflow Condition
// Author: Ryan King
// Date: Febuary 8th, 2025

#include "app.h"
#include "solver.h"
#include "vec3.h"

#include <array>
#include <iostream>
int main()
{
    Solver::solve();

    App app;
    app.run();

    return 0;
}