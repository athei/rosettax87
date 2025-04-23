#pragma once
#include <numbers>
#include <cmath>
#include <arm_neon.h>

namespace mmath {
    auto cos(double value) -> double;

    auto log2_neon(float32x4_t x) -> float32x4_t;

    auto log2(double x) -> double;

    auto exp2(double x) -> double;

    auto sin(double x) -> double;

    auto tan(double x) -> double;

    auto fmod(double a, double b) -> double;

    auto atan(double x) -> double;

    auto atan2(double y, double x) -> double;
}; // namespace math