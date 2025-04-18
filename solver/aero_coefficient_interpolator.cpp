#include "aero_coefficient_interpolator.h"
#include <algorithm>
#include <limits>

AeroCoefficientInterpolator::AeroCoefficientInterpolator(const CoefficientData& coefData)
    : data(coefData) {
    // Sort data by alpha for each Reynolds number
    for (auto& reEntry : data) {
        std::sort(reEntry.second.begin(), reEntry.second.end());
    }
}

float AeroCoefficientInterpolator::lerp(float x1, float x2, float y1, float y2, float x) const {
    if (x2 == x1) return y1;
    return y1 + (y2 - y1) * (x - x1) / (x2 - x1);
}

float AeroCoefficientInterpolator::interpolateAtReynolds(float alpha,
                                                       const std::vector<std::pair<float, float>>& dataset) const {
    if (dataset.empty()) {
        // No data: handled by caller (findClosestPoint or fallback to 0.0)
        return 0.0f;
    }

    if (dataset.size() == 1) {
        // Only one point: return its coefficient
        return dataset[0].second;
    }

    // Find the closest points for alpha
    auto it = std::lower_bound(dataset.begin(), dataset.end(),
                               std::make_pair(alpha, 0.0f),
                               [](const auto& a, const auto& b) { return a.first < b.first; });

    if (it == dataset.begin()) {
        // Alpha is below the lowest: return coefficient at lowest alpha
        return dataset[0].second;
    }
    if (it == dataset.end()) {
        // Alpha is above the highest: return coefficient at highest alpha
        return dataset.back().second;
    }

    // Interpolate between bracketing alphas
    float alpha1 = (it-1)->first;
    float alpha2 = it->first;
    float coef1 = (it-1)->second;
    float coef2 = it->second;
    return lerp(alpha1, alpha2, coef1, coef2, alpha);
}

float AeroCoefficientInterpolator::findClosestPoint(float alpha, float reynolds) const {
    float minDistance = std::numeric_limits<float>::max();
    float closestCoef = 0.0f;

    for (const auto& reEntry : data) {
        float re = reEntry.first;
        const auto& points = reEntry.second;
        for (const auto& point : points) {
            float a = point.first;
            float coef = point.second;
            // Compute normalized distance (assuming Reynolds is on a logarithmic scale)
            float reDistance = std::log10(reynolds / re);
            float alphaDistance = alpha - a;
            float distance = std::sqrt(alphaDistance * alphaDistance + reDistance * reDistance);
            if (distance < minDistance) {
                minDistance = distance;
                closestCoef = coef;
            }
        }
    }

    return closestCoef;
}

float AeroCoefficientInterpolator::coefficientAt(float alpha, float reynolds) const {
    if (data.empty()) {
        // No data at all: return 0.0 as last resort
        return 0.0f;
    }

    // Find bracketing Reynolds numbers
    auto it2 = data.lower_bound(reynolds);
    if (it2 == data.end()) {
        // Reynolds is above highest: use highest Reynolds data
        return interpolateAtReynolds(alpha, data.rbegin()->second);
    }
    if (it2 == data.begin()) {
        // Reynolds is below lowest: use lowest Reynolds data
        return interpolateAtReynolds(alpha, data.begin()->second);
    }

    // Try to interpolate between bracketing Reynolds numbers
    float re1 = std::prev(it2)->first;
    float re2 = it2->first;
    const auto& data1 = std::prev(it2)->second;
    const auto& data2 = it2->second;

    // If either dataset is empty or has insufficient points, find closest point
    if (data1.empty() || data1.size() < 2 || data2.empty() || data2.size() < 2) {
        return findClosestPoint(alpha, reynolds);
    }

    float coef1 = interpolateAtReynolds(alpha, data1);
    float coef2 = interpolateAtReynolds(alpha, data2);
    return lerp(re1, re2, coef1, coef2, reynolds);
}