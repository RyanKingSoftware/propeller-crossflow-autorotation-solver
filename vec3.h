#ifndef VEC3_H_
#define VEC3_H_

#include <array>
#include <cassert>
#include <cmath>
#include <xmmintrin.h>  // For SSE intrinsics

struct Vec3 {
    union {
        __m128 data;
        float array[4]; // 4th element is unused (used for SIMD alignment)
    };

    // Default constructor
    Vec3() : data(_mm_setzero_ps()) {}

    // Constructor from three floats (x, y, z)
    Vec3(float x, float y, float z) {
        data = _mm_set_ps(0.0f, z, y, x);  // Set x, y, z, and 0.0f (unused)
    }

    // Constructor from __m128 (to allow SIMD operations directly)
    explicit Vec3(__m128 simdData) : data(simdData) {}

    // Access elements using array-style indexing
    float& operator[](size_t index) {
        return array[index];
    }

    const float& operator[](size_t index) const {
        return array[index];
    }
};

// Scalar multiplication
inline Vec3 operator*(const Vec3& vec, const float& scalar) noexcept {
    __m128 scalarVec = _mm_set1_ps(scalar);
    return Vec3(_mm_mul_ps(vec.data, scalarVec));
}

// Scalar multiplication (reverse order)
inline Vec3 operator*(const float& scalar, const Vec3& vec) noexcept {
    return vec * scalar;
}

// Scalar division
inline Vec3 operator/(const Vec3& vec, const float& scalar) noexcept {
    assert(scalar != 0.0f);
    __m128 scalarVec = _mm_set1_ps(scalar);
    return Vec3(_mm_div_ps(vec.data, scalarVec));
}

// Vector addition
inline Vec3 operator+(const Vec3& a, const Vec3& b) noexcept {
    return Vec3(_mm_add_ps(a.data, b.data));
}

// Vector subtraction
inline Vec3 operator-(const Vec3& a, const Vec3& b) noexcept {
    return Vec3(_mm_sub_ps(a.data, b.data));
}

// Cross product
inline Vec3 cross(const Vec3& a, const Vec3& b) noexcept {
    // Cross product is computed by using the formula:
    // (a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x)
    __m128 a1 = _mm_shuffle_ps(a.data, a.data, _MM_SHUFFLE(3, 0, 2, 1));  // y, z, x, unused
    __m128 b1 = _mm_shuffle_ps(b.data, b.data, _MM_SHUFFLE(3, 0, 2, 1));  // y, z, x, unused
    
    __m128 c1 = _mm_mul_ps(a1, b.data);  // (a.y*b.z, a.z*b.x, a.x*b.y, unused)
    __m128 c2 = _mm_mul_ps(a.data, b1);  // (a.z*b.y, a.x*b.z, a.y*b.x, unused)
    
    return Vec3(_mm_sub_ps(c1, c2));  // (a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x)
}

inline float dot(const Vec3& a, const Vec3& b) noexcept {
    __m128 result = _mm_mul_ps(a.data, b.data);  // Multiply corresponding elements
    result = _mm_add_ps(result, _mm_shuffle_ps(result, result, _MM_SHUFFLE(2, 3, 0, 1)));  // Sum x and y
    result = _mm_add_ps(result, _mm_shuffle_ps(result, result, _MM_SHUFFLE(1, 0, 3, 2)));  // Sum z
    return _mm_cvtss_f32(result);  // Extract the result (scalar value)
}

// Magnitude of the vector (length)
inline float magnitude(const Vec3& vec) noexcept {
    return std::sqrt(dot(vec, vec));  // Length = sqrt(x^2 + y^2 + z^2)
}

#endif // VEC3_H_