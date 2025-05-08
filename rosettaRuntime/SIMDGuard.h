#pragma once

#include <cstdint>
#define ENABLE_SIMD_GUARD

struct SIMDGuard {
  using SIMDRegister_t = uint8_t[8];

  SIMDGuard() {
#if defined(ENABLE_SIMD_GUARD)
    // Save the current SIMD register values to member variables
    asm volatile("stp  d0,  d1, [%0, # 0]\n\t" // stores 2×8 B = 16 B
                 "stp  d2,  d3, [%0, #16]\n\t"
                 "stp  d4,  d5, [%0, #32]\n\t"
                 "stp  d6,  d7, [%0, #48]\n\t"
                 : /* no outputs */
                 : "r"(buf)
                 : "memory");
#endif
  }

  ~SIMDGuard() {
#if defined(ENABLE_SIMD_GUARD)
    // Restore the saved SIMD register values
    asm volatile("ldp  d6,  d7, [%0, #48]\n\t"
                 "ldp  d4,  d5, [%0, #32]\n\t"
                 "ldp  d2,  d3, [%0, #16]\n\t"
                 "ldp  d0,  d1, [%0, # 0]\n\t"
                 :
                 : "r"(buf)
                 : "memory");
#endif
  }

  alignas(16) SIMDRegister_t buf[8];
};