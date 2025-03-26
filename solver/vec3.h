#ifndef _VEC3_H_
#define _VEC3_H_

#include <cassert>
#include <cmath>
#include <xmmintrin.h>  // SSE intrinsics

// Forward declaration to keep header lean
struct Vec3;

// Operator declarations (for better organization)
inline Vec3 operator*(const Vec3& vec, float scalar) noexcept;
inline Vec3 operator*(float scalar, const Vec3& vec) noexcept;
inline Vec3 operator/(const Vec3& vec, float scalar) noexcept;
inline Vec3 operator+(const Vec3& a, const Vec3& b) noexcept;
inline Vec3 operator-(const Vec3& a, const Vec3& b) noexcept;
inline Vec3 cross(const Vec3& a, const Vec3& b) noexcept;
inline float dot(const Vec3& a, const Vec3& b) noexcept;
inline float magnitude(const Vec3& vec) noexcept;

struct Vec3 {
    union {
        __m128 data;      // SIMD register
        float array[4];   // x, y, z, w (w unused, for alignment)
    };

    // Constructors
    Vec3() noexcept : data(_mm_setzero_ps()) {}
    Vec3(float x, float y, float z) noexcept : data(_mm_set_ps(0.0f, z, y, x)) {}
    explicit Vec3(__m128 simdData) noexcept : data(simdData) {}

    // Element access
    float& operator[](size_t i) noexcept { return array[i]; }
    const float& operator[](size_t i) const noexcept { return array[i]; }
};

// Operator implementations
inline Vec3 operator*(const Vec3& vec, float scalar) noexcept {
    return Vec3(_mm_mul_ps(vec.data, _mm_set1_ps(scalar)));
}

inline Vec3 operator*(float scalar, const Vec3& vec) noexcept {
    return vec * scalar;  // Reuse commutative property
}

inline Vec3 operator/(const Vec3& vec, float scalar) noexcept {
    assert(scalar != 0.0f && "Division by zero");
    return Vec3(_mm_div_ps(vec.data, _mm_set1_ps(scalar)));
}

inline Vec3 operator+(const Vec3& a, const Vec3& b) noexcept {
    return Vec3(_mm_add_ps(a.data, b.data));
}

inline Vec3 operator-(const Vec3& a, const Vec3& b) noexcept {
    return Vec3(_mm_sub_ps(a.data, b.data));
}

inline Vec3 cross(const Vec3& a, const Vec3& b) noexcept {
    const auto a_yzx = _mm_shuffle_ps(a.data, a.data, _MM_SHUFFLE(3, 0, 2, 1));
    const auto b_yzx = _mm_shuffle_ps(b.data, b.data, _MM_SHUFFLE(3, 0, 2, 1));
    const auto m0 = _mm_mul_ps(a.data, b_yzx);
    const auto m1 = _mm_mul_ps(a_yzx, b.data);
    const auto res = _mm_sub_ps(m0, m1);
    return Vec3(_mm_shuffle_ps(res, res, _MM_SHUFFLE(3, 0, 2, 1)));
}

inline float dot(const Vec3& a, const Vec3& b) noexcept {
    const auto mul = _mm_mul_ps(a.data, b.data);
    const auto sum1 = _mm_add_ps(mul, _mm_shuffle_ps(mul, mul, _MM_SHUFFLE(2, 3, 0, 1)));
    const auto sum2 = _mm_add_ps(sum1, _mm_shuffle_ps(sum1, sum1, _MM_SHUFFLE(1, 0, 3, 2)));
    return _mm_cvtss_f32(sum2);
}

inline float magnitude(const Vec3& vec) noexcept {
    return std::sqrt(dot(vec, vec));
}

#endif // _VEC3_H_