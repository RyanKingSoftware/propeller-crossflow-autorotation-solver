#ifndef _AERO_COEFFICIENT_INTERPOLATOR_H_
#define _AERO_COEFFICIENT_INTERPOLATOR_H_

#include <vector>
#include <map>
#include <cmath>

class AeroCoefficientInterpolator {
public:
    // Data structure for input: Reynolds -> vector of {alpha, coefficient}
    using CoefficientData = std::map<float, std::vector<std::pair<float, float>>>;

    // Constructor with data for a single coefficient
    AeroCoefficientInterpolator(const CoefficientData& coefData);

    // Get coefficient at given alpha and Reynolds number
    float coefficientAt(float alpha, float reynolds) const;

    // Static instances for DAE-51 airfoil
    static const AeroCoefficientInterpolator Dae51Lift;
    static const AeroCoefficientInterpolator Dae51Drag;
    static const AeroCoefficientInterpolator Dae51LiftReversed;
    static const AeroCoefficientInterpolator Dae51DragReversed;

private:
    // Data: Reynolds -> vector of {alpha, coefficient}
    CoefficientData data;

    // Linear interpolation
    float lerp(float x1, float x2, float y1, float y2, float x) const;

    // Interpolate for a specific Reynolds number or find closest point
    float interpolateAtReynolds(float alpha,
                               const std::vector<std::pair<float, float>>& dataset) const;

    // Find the closest data point across all Reynolds numbers
    float findClosestPoint(float alpha, float reynolds) const;
};

// Inline static instance definitions
inline const AeroCoefficientInterpolator AeroCoefficientInterpolator::Dae51Lift(
    CoefficientData{
        {1e6, {
            {-0.436332313f, -0.62286894f},  // alpha = -25°, Cl = -0.62286894
            {-0.3490658504f, -0.5375499f},  // alpha = -20°, Cl = -0.5375499
            {-0.2617993878f, -0.40355853f}, // alpha = -15°, Cl = -0.40355853
            {-0.1745329252f, -0.31516635f}, // alpha = -10°, Cl = -0.31516635
            {-0.1396263402f, -0.26242659f}, // alpha = -8°, Cl = -0.26242659
            {-0.1047197551f, -0.16900409f}, // alpha = -6°, Cl = -0.16900409
            {-0.06981317008f, -0.021360144f}, // alpha = -4°, Cl = -0.021360144
            {-0.03490658504f, 0.19118146f},  // alpha = -2°, Cl = 0.19118146
            {0.0f, 0.39933459f},             // alpha = 0°, Cl = 0.39933459
            {0.03490658504f, 0.62167793f},   // alpha = 2°, Cl = 0.62167793
            {0.06981317008f, 0.82048786f},   // alpha = 4°, Cl = 0.82048786
            {0.1047197551f, 0.93943706f},    // alpha = 6°, Cl = 0.93943706
            {0.1396263402f, 0.91766234f},    // alpha = 8°, Cl = 0.91766234
            {0.1745329252f, 1.0738337f},     // alpha = 10°, Cl = 1.0738337
            {0.2617993878f, 0.95249894f},    // alpha = 15°, Cl = 0.95249894
            {0.3490658504f, 0.95610401f},    // alpha = 20°, Cl = 0.95610401
            {0.436332313f, 1.0044682f},      // alpha = 25°, Cl = 1.0044682
            {0.5235987756f, 1.0191244f},     // alpha = 30°, Cl = 1.0191244
            {0.6108652382f, 1.0709564f},     // alpha = 35°, Cl = 1.0709564
            {0.6981317008f, 1.1024401f},     // alpha = 40°, Cl = 1.1024401
            {0.7853981634f, 1.1041184f},     // alpha = 45°, Cl = 1.1041184
            {0.872664626f, 1.0783589f},      // alpha = 50°, Cl = 1.0783589
            {0.9599310886f, 1.013605f}       // alpha = 55°, Cl = 1.013605
        }}
    }
);

inline const AeroCoefficientInterpolator AeroCoefficientInterpolator::Dae51Drag(
    CoefficientData{
        {1e6, {
            {-0.436332313f, 0.37781166f},   // alpha = -25°, Cd = 0.37781166
            {-0.3490658504f, 0.26392221f},  // alpha = -20°, Cd = 0.26392221
            {-0.2617993878f, 0.17215489f},  // alpha = -15°, Cd = 0.17215489
            {-0.1745329252f, 0.10409953f},  // alpha = -10°, Cd = 0.10409953
            {-0.1396263402f, 0.074132374f}, // alpha = -8°, Cd = 0.074132374
            {-0.1047197551f, 0.055134778f}, // alpha = -6°, Cd = 0.055134778
            {-0.06981317008f, 0.036927284f}, // alpha = -4°, Cd = 0.036927284
            {-0.03490658504f, 0.025270896f}, // alpha = -2°, Cd = 0.025270896
            {0.0f, 0.018184236f},            // alpha = 0°, Cd = 0.018184236
            {0.03490658504f, 0.021953756f},  // alpha = 2°, Cd = 0.021953756
            {0.06981317008f, 0.029440516f},  // alpha = 4°, Cd = 0.029440516
            {0.1047197551f, 0.051744512f},   // alpha = 6°, Cd = 0.051744512
            {0.1396263402f, 0.08820373f},    // alpha = 8°, Cd = 0.08820373
            {0.1745329252f, 0.11223418f},    // alpha = 10°, Cd = 0.11223418
            {0.2617993878f, 0.18383662f},    // alpha = 15°, Cd = 0.18383662
            {0.3490658504f, 0.29796849f},    // alpha = 20°, Cd = 0.29796849
            {0.436332313f, 0.42042049f},     // alpha = 25°, Cd = 0.42042049
            {0.5235987756f, 0.5754442f},     // alpha = 30°, Cd = 0.5754442
            {0.6108652382f, 0.7356429f},     // alpha = 35°, Cd = 0.7356429
            {0.6981317008f, 0.92286753f},    // alpha = 40°, Cd = 0.92286753
            {0.7853981634f, 1.0778461f},     // alpha = 45°, Cd = 1.0778461
            {0.872664626f, 1.2629665f},      // alpha = 50°, Cd = 1.2629665
            {0.9599310886f, 1.4016037f}      // alpha = 55°, Cd = 1.4016037
        }}
    }
);

// Inline static instance definitions for reversed flow
inline const AeroCoefficientInterpolator AeroCoefficientInterpolator::Dae51LiftReversed(
    CoefficientData{
        {1e6, {
            {-0.436332313f, 0.8625198f},    // alpha = -25°, Cl = 0.8625198
            {-0.3490658504f, 0.78064613f},  // alpha = -20°, Cl = 0.78064613
            {-0.2617993878f, 0.77163174f},  // alpha = -15°, Cl = 0.77163174
            {-0.1745329252f, 0.92676684f},  // alpha = -10°, Cl = 0.92676684
            {-0.1396263402f, 0.88272408f},  // alpha = -8°, Cl = 0.88272408
            {-0.1047197551f, 0.8123486f},   // alpha = -6°, Cl = 0.8123486
            {-0.06981317008f, 0.71720479f}, // alpha = -4°, Cl = 0.71720479
            {-0.03490658504f, 0.65349604f}, // alpha = -2°, Cl = 0.65349604
            {0.0f, 0.40962666f},            // alpha = 0°, Cl = 0.40962666
            {0.03490658504f, 0.22224746f},  // alpha = 2°, Cl = 0.22224746
            {0.06981317008f, 0.11600063f},  // alpha = 4°, Cl = 0.11600063
            {0.1047197551f, 0.0081612337f}, // alpha = 6°, Cl = 0.0081612337
            {0.1396263402f, -0.24529097f},  // alpha = 8°, Cl = -0.24529097
            {0.1745329252f, -0.20547747f},  // alpha = 10°, Cl = -0.20547747
            {0.2617993878f, -0.18605256f},  // alpha = 15°, Cl = -0.18605256
            {0.3490658504f, -0.31289578f},  // alpha = 20°, Cl = -0.31289578
            {0.436332313f, -0.41758209f},   // alpha = 25°, Cl = -0.41758209
            {0.5235987756f, -0.47010328f},  // alpha = 30°, Cl = -0.47010328
            {0.6108652382f, -0.49180354f},  // alpha = 35°, Cl = -0.49180354
            {0.6981317008f, -0.56641723f},  // alpha = 40°, Cl = -0.56641723
            {0.7853981634f, -0.64230682f},  // alpha = 45°, Cl = -0.64230682
            {0.872664626f, -0.67469166f},   // alpha = 50°, Cl = -0.67469166
            {0.9599310886f, -0.64981738f}   // alpha = 55°, Cl = -0.64981738
        }}
    }
);

inline const AeroCoefficientInterpolator AeroCoefficientInterpolator::Dae51DragReversed(
    CoefficientData{
        {1e6, {
            {-0.436332313f, 0.4032045f},    // alpha = -25°, Cd = 0.4032045
            {-0.3490658504f, 0.27544315f},  // alpha = -20°, Cd = 0.27544315
            {-0.2617993878f, 0.17710651f},  // alpha = -15°, Cd = 0.17710651
            {-0.1745329252f, 0.10634668f},  // alpha = -10°, Cd = 0.10634668
            {-0.1396263402f, 0.075091242f}, // alpha = -8°, Cd = 0.075091242
            {-0.1047197551f, 0.052473904f}, // alpha = -6°, Cd = 0.052473904
            {-0.06981317008f, 0.034827825f}, // alpha = -4°, Cd = 0.034827825
            {-0.03490658504f, 0.023995526f}, // alpha = -2°, Cd = 0.023995526
            {0.0f, 0.018284585f},           // alpha = 0°, Cd = 0.018284585
            {0.03490658504f, 0.023042524f}, // alpha = 2°, Cd = 0.023042524
            {0.06981317008f, 0.033051952f}, // alpha = 4°, Cd = 0.033051952
            {0.1047197551f, 0.053107724f},  // alpha = 6°, Cd = 0.053107724
            {0.1396263402f, 0.087665586f},  // alpha = 8°, Cd = 0.087665586
            {0.1745329252f, 0.10693677f},   // alpha = 10°, Cd = 0.10693677
            {0.2617993878f, 0.15154737f},   // alpha = 15°, Cd = 0.15154737
            {0.3490658504f, 0.23386554f},   // alpha = 20°, Cd = 0.23386554
            {0.436332313f, 0.33334358f},    // alpha = 25°, Cd = 0.33334358
            {0.5235987756f, 0.43478519f},   // alpha = 30°, Cd = 0.43478519
            {0.6108652382f, 0.53529064f},   // alpha = 35°, Cd = 0.53529064
            {0.6981317008f, 0.68185475f},   // alpha = 40°, Cd = 0.68185475
            {0.7853981634f, 0.86292088f},   // alpha = 45°, Cd = 0.86292088
            {0.872664626f, 1.0343788f},     // alpha = 50°, Cd = 1.0343788
            {0.9599310886f, 1.1862034f}     // alpha = 55°, Cd = 1.1862034
        }}
    }
);



#endif // _AERO_COEFFICIENT_INTERPOLATOR_H_