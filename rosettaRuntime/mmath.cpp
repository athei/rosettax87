#include "mmath.h"

auto mmath::cos(double x) -> double {
  constexpr double PI = std::numbers::pi_v<double>;
  constexpr double TWO_PI = 2.0 * PI;
  constexpr double INV_FACTORIAL_2 = 1.0 / 2.0;
  constexpr double INV_FACTORIAL_4 = 1.0 / 24.0;
  constexpr double INV_FACTORIAL_6 = 1.0 / 720.0;

  x = x - TWO_PI * std::floor((x + PI) / TWO_PI);
  double x2 = x * x;
  double x4 = x2 * x2;
  double x6 = x4 * x2;

  return 1.0 - (x2 * INV_FACTORIAL_2) + (x4 * INV_FACTORIAL_4) - (x6 * INV_FACTORIAL_6);
}


// NEON vectorized version processing 4 floats at once
auto mmath::log2_neon(float32x4_t x) -> float32x4_t {
    const float32x4_t one = vdupq_n_f32(1.0f);
    const float32x4_t ln2_inv = vdupq_n_f32(1.442695040888963f);
    
    // Extract exponent and mantissa
    int32x4_t exp;
    float32x4_t mant = vrecpeq_f32(x);  // Approximate 1/x
    mant = vmulq_f32(mant, x);          // Normalize to [1,2)
    
    // Calculate z = x-1
    float32x4_t z = vsubq_f32(mant, one);
    
    // Polynomial approximation
    float32x4_t z2 = vmulq_f32(z, z);
    float32x4_t z3 = vmulq_f32(z2, z);
    
    // c1 + c2*z + c3*z^2 + c4*z^3
    const float32x4_t c1 = vdupq_n_f32(0.0f);
    const float32x4_t c2 = vdupq_n_f32(1.0f);
    const float32x4_t c3 = vdupq_n_f32(-0.5f);
    const float32x4_t c4 = vdupq_n_f32(0.333333333f);
    
    float32x4_t result = vaddq_f32(
        vaddq_f32(
            vmulq_f32(c4, z3),
            vmulq_f32(c3, z2)
        ),
        vaddq_f32(
            vmulq_f32(c2, z),
            c1
        )
    );
    
    return vmulq_f32(result, ln2_inv);
}

auto mmath::log2(double x) -> double {
    // Scalar fallback version 
    constexpr double LN2 = 0.693147180559945f;
    double power_adj = 0.0f;
    
    while (x >= 2.0f) {
        x *= 0.5f;
        power_adj += 1.0f;
    }
    while (x < 1.0f) {
        x *= 2.0f;
        power_adj -= 1.0f;
    }
    
    double z = x - 1.0f;
    double z2 = z * z;
    double z3 = z2 * z;
    return (z - (z2 * 0.5f) + (z3 * 0.333333333f)) / LN2 + power_adj;
}

#include "fastermath.h"

auto mmath::exp2(double x) -> double {
    return fm_exp2(x);
}

double mmath::fmod(double a, double b) {
    // Handle special cases
    if (b == 0.0f || std::isnan(a) || std::isnan(b)) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    if (std::isinf(a)) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    if (std::isinf(b)) {
        return a;
    }
    
    // Handle signs
    double abs_a = std::abs(a);
    double abs_b = std::abs(b);
    
    // Compute remainder using built-in fmod for proper handling
    double result = std::fabs(abs_a - (std::floor(abs_a / abs_b) * abs_b));
    
    // Restore sign of dividend
    return std::copysign(result, a);
}

auto mmath::sin(double x) -> double {
    constexpr double PI = std::numbers::pi_v<double>;

    // Normalize angle to [-PI, PI]
    x = mmath::fmod(x, 2.0f * PI);
    if (x > PI) x -= 2.0f * PI;
    if (x < -PI) x += 2.0f * PI;
    
    // Taylor series for sin(x)
    double x2 = x * x;
    double term = x;
    double sum = term;
    
    // First 4 terms of Taylor series
    term = -term * x2 / (2.0f * 3.0f);
    sum += term;
    term = -term * x2 / (4.0f * 5.0f);
    sum += term;
    term = -term * x2 / (6.0f * 7.0f);
    sum += term;
    
    return sum;
}

auto mmath::tan(double x) -> double {
    double c = cos(x);
    if (c == 0.0f) return INFINITY;
    return sin(x) / c;
}

auto mmath::atan(double x) -> double {
    constexpr double PI = 3.14159265358979323846;
    
    // Handle special cases
    if (x == 0.0) return 0.0;
    
    // For large values, use atan(x) = π/2 - atan(1/x)
    if (abs(x) > 1.0) {
        return (x > 0 ? PI/2 : -PI/2) - atan(1.0/x);
    }
    
    // Taylor series for |x| <= 1
    double result = 0.0;
    double term = x;
    double x2 = x * x;
    int sign = 1;
    
    for (int n = 1; n <= 11; n += 2) {
        result += sign * term / n;
        term *= x2;
        sign = -sign;
    }
    
    return result;
}

auto mmath::atan2(double y, double x) -> double {
    constexpr double PI = 3.14159265358979323846;
    
    // Handle special cases
    if (x == 0.0) {
        if (y == 0.0) return 0.0;
        return y > 0.0 ? PI/2 : -PI/2;
    }
    
    double angle = atan(y/x);
    
    // Adjust angle based on quadrant
    if (x < 0.0) {
        angle += (y >= 0.0) ? PI : -PI;
    }
    
    return angle;
}