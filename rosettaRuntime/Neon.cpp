#include "Neon.h"
#include <cmath>

// https://github.com/ARM-software/EndpointAI/blob/master/Kernels/Migrating_to_Helium_from_Neon_Companion_SW/vmath.c

/** Sin polynomial coefficients */
float te_sin_coeff2 = 0.166666666666f;  // 1/(2*3)
float te_sin_coeff3 = 0.05f;            // 1/(4*5)
float te_sin_coeff4 = 0.023809523810f;  // 1/(6*7)
float te_sin_coeff5 = 0.013888888889f;  // 1/(8*9)

// https://developer.arm.com/documentation/102107a/0100/Single-precision-vector-sine
float32x4_t vsinq_neon_f32(float32x4_t val) {
  const float32x4_t pi_v = vdupq_n_f32(M_PI);
  const float32x4_t pio2_v = vdupq_n_f32(M_PI / 2);
  const float32x4_t ipi_v = vdupq_n_f32(1 / M_PI);

  // Find positive or negative
  const int32x4_t c_v = vabsq_s32(vcvtq_s32_f32(vmulq_f32(val, ipi_v)));
  const uint32x4_t sign_v = vcleq_f32(val, vdupq_n_f32(0));
  const uint32x4_t odd_v = vandq_u32(vreinterpretq_u32_s32(c_v), vdupq_n_u32(1));

  uint32x4_t neg_v = veorq_u32(odd_v, sign_v);

  // Modulus a - (n * int(a*(1/n)))
  float32x4_t ma = vsubq_f32(vabsq_f32(val), vmulq_f32(pi_v, vcvtq_f32_s32(c_v)));

  const uint32x4_t reb_v = vcgeq_f32(ma, pio2_v);

  // Rebase a between 0 and pi/2
  ma = vbslq_f32(reb_v, vsubq_f32(pi_v, ma), ma);

  // Taylor series
  const float32x4_t ma2 = vmulq_f32(ma, ma);

  // 2nd elem: x^3 / 3!
  float32x4_t elem = vmulq_f32(vmulq_f32(ma, ma2), vdupq_n_f32(te_sin_coeff2));
  float32x4_t res = vsubq_f32(ma, elem);

  // 3rd elem: x^5 / 5!
  elem = vmulq_f32(vmulq_f32(elem, ma2), vdupq_n_f32(te_sin_coeff3));
  res = vaddq_f32(res, elem);

  // 4th elem: x^7 / 7!float32x2_t vsin_f32(float32x2_t val)
  elem = vmulq_f32(vmulq_f32(elem, ma2), vdupq_n_f32(te_sin_coeff4));
  res = vsubq_f32(res, elem);

  // 5th elem: x^9 / 9!
  elem = vmulq_f32(vmulq_f32(elem, ma2), vdupq_n_f32(te_sin_coeff5));
  res = vaddq_f32(res, elem);

  // Change of sign
  neg_v = vshlq_n_u32(neg_v, 31);
  res = vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(res), neg_v));
  return res;
}

// https://developer.arm.com/documentation/102107a/0100/Single-precision-vector-logarithm

/** Logarithm polynomial coefficients */
const float32_t log_tab[4 * 8] = {
    -2.29561495781f, -2.29561495781f, -2.29561495781f, -2.29561495781f,
    -2.47071170807f, -2.47071170807f, -2.47071170807f, -2.47071170807f,
    -5.68692588806f, -5.68692588806f, -5.68692588806f, -5.68692588806f,
    -0.165253549814f, -0.165253549814f, -0.165253549814f, -0.165253549814f,
    5.17591238022f, 5.17591238022f, 5.17591238022f, 5.17591238022f,
    0.844007015228f, 0.844007015228f, 0.844007015228f, 0.844007015228f,
    4.58445882797f, 4.58445882797f, 4.58445882797f, 4.58445882797f,
    0.0141278216615f, 0.0141278216615f, 0.0141278216615f, 0.0141278216615f
};

const float32_t exp_tab[4 * 8] = {
    1.f, 1.f, 1.f, 1.f,
    0.0416598916054f, 0.0416598916054f, 0.0416598916054f, 0.0416598916054f,
    0.500000596046f, 0.500000596046f, 0.500000596046f, 0.500000596046f,
    0.0014122662833f, 0.0014122662833f, 0.0014122662833f, 0.0014122662833f,
    1.00000011921f, 1.00000011921f, 1.00000011921f, 1.00000011921f,
    0.00833693705499f, 0.00833693705499f, 0.00833693705499f, 0.00833693705499f,
    0.166665703058f, 0.166665703058f, 0.166665703058f, 0.166665703058f,
    0.000195780929062f, 0.000195780929062f, 0.000195780929062f, 0.000195780929062f
};


static inline float32x4_t vtaylor_polyq_f32(float32x4_t x, const float32_t * coeffs)
{
  float32x4_t A   = vmlaq_f32(vld1q_f32(&coeffs[4 * 0]), vld1q_f32(&coeffs[4 * 4]), x);
  float32x4_t B   = vmlaq_f32(vld1q_f32(&coeffs[4 * 2]), vld1q_f32(&coeffs[4 * 6]), x);
  float32x4_t C   = vmlaq_f32(vld1q_f32(&coeffs[4 * 1]), vld1q_f32(&coeffs[4 * 5]), x);
  float32x4_t D   = vmlaq_f32(vld1q_f32(&coeffs[4 * 3]), vld1q_f32(&coeffs[4 * 7]), x);
  float32x4_t x2  = vmulq_f32(x, x);
  float32x4_t x4  = vmulq_f32(x2, x2);
  float32x4_t res = vmlaq_f32(vmlaq_f32(A, B, x2), vmlaq_f32(C, D, x2), x4);
  return res;
}

#define LOG2    0.6931471805f
#define INVLOG2 1.4426950408f

float32x4_t vlogq_neon_f32(float32x4_t x)
{
  // Extract exponent
  int32x4_t m = vsubq_s32(vreinterpretq_s32_u32(vshrq_n_u32(
     vreinterpretq_u32_f32(x), 23)), vdupq_n_s32(127));

  float32x4_t val = vreinterpretq_f32_s32(vsubq_s32(vreinterpretq_s32_f32(x), 
     vshlq_n_s32(m, 23)));

  // Polynomial Approximation
  float32x4_t poly = vtaylor_polyq_f32(val, log_tab);
  // Reconstruct
  poly = vmlaq_f32(poly, vcvtq_f32_s32(m), vdupq_n_f32(LOG2));

  return poly;
}