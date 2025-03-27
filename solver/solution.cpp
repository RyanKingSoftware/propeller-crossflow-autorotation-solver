#include "solution.h"
#include "util.h"

size_t Solution::solutionNumber = 0;

Solution::Solution(size_t size) : time(size, 0.0f), 
    angularPosition(size, 0.0f), 
    angularVelocity(size, 0.0f), 
    angularAcceleration(size, 0.0f), 
    torque(size, 0.0f), 
    lift(size, 0.0f), 
    drag(size, 0.0f), 
    sideForce(size, 0.0f) 
{
    name = "solution_" + std::to_string(solutionNumber);
    solutionNumber++;

    // Ensure random seed is initialized
    static bool seedInitialized = false;
    if (!seedInitialized) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        seedInitialized = true;
    }

    // Generate vibrant random color
    float r = (std::rand() % 156 + 100) / 255.0f; // Between 100-255
    float g = (std::rand() % 156 + 100) / 255.0f; // Between 100-255
    float b = (std::rand() % 156 + 100) / 255.0f; // Between 100-255

    color = ImVec4(r, g, b, 1.0f);
}

void Solution::clean() {
    if (time.size() > 1000) {
        size_t windowSize = time.size() / 1000;
        downsample(windowSize);
    }
}

void Solution::downsample(const size_t& windowSize) {
    time                = Util::downsampleMinmax(time, windowSize);
    angularPosition     = Util::downsampleMinmax(angularPosition, windowSize);
    angularVelocity     = Util::downsampleMinmax(angularVelocity, windowSize);
    angularAcceleration = Util::downsampleMinmax(angularAcceleration, windowSize);
    torque              = Util::downsampleMinmax(torque, windowSize);
    lift                = Util::downsampleMinmax(lift, windowSize);
    drag                = Util::downsampleMinmax(drag, windowSize);
    sideForce           = Util::downsampleMinmax(sideForce, windowSize);
}