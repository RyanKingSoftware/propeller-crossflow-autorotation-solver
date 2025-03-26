#ifndef _SOLUTION_H_
#define _SOLUTION_H_

#include <vector>
#include <mutex>

struct Solution {
    std::vector<float> time, angularPosition, angularVelocity, angularAcceleration, torque, lift, drag, sideForce;
    std::mutex mutex;
};

#endif //_SOLUTION_H_