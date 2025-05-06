#pragma once

#include <cstdint>

struct SIMDGuard {
  using SIMDRegister_t = uint8_t[16];

  SIMDGuard() {
    // Save the current SIMD register values to member variables
    asm volatile("str q0, [%0]\n\t"
                 "str q1, [%1]\n\t"
                 "str q2, [%2]\n\t"
                 "str q3, [%3]\n\t"
                 "str q4, [%4]\n\t"
                 "str q5, [%5]\n\t"
                 "str q6, [%6]\n\t"
                 "str q7, [%7]\n\t"
                 :
                 : "r"(v0), "r"(v1), "r"(v2), "r"(v3), "r"(v4), "r"(v5),
                   "r"(v6), "r"(v7)
                 : "memory");
  }

  ~SIMDGuard() {
    // Restore the saved SIMD register values
    asm volatile("ldr q0, [%0]\n\t"
                 "ldr q1, [%1]\n\t"
                 "ldr q2, [%2]\n\t"
                 "ldr q3, [%3]\n\t"
                 "ldr q4, [%4]\n\t"
                 "ldr q5, [%5]\n\t"
                 "ldr q6, [%6]\n\t"
                 "ldr q7, [%7]\n\t"
                 :
                 : "r"(v0), "r"(v1), "r"(v2), "r"(v3), "r"(v4), "r"(v5),
                   "r"(v6), "r"(v7)
                 : "v0", "v1", "v2", "v3", "v4", "v5", "v6", "v7", "memory");
  }

  SIMDRegister_t v0;
  SIMDRegister_t v1;
  SIMDRegister_t v2;
  SIMDRegister_t v3;
  SIMDRegister_t v4;
  SIMDRegister_t v5;
  SIMDRegister_t v6;
  SIMDRegister_t v7;
};