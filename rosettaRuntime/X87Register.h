#pragma once

#include <cstdint>
#include <cstddef>


#if 1
#pragma pack(push, 1)
struct X87Register {
  union {
    struct {
      uint64_t mantissa;
      uint16_t exponent;
    };
    uint8_t bytes[10];
    double ieee754;      // 32 bits
  };
};
#pragma pack(pop)
static_assert(sizeof(X87Register) == 0x0A, "Invalid size for X87Register");
static_assert(offsetof(X87Register, mantissa) == 0, "Invalid offset for X87Register::mantissa");
static_assert(offsetof(X87Register, exponent) == 8, "Invalid offset for X87Register::exponent");
static_assert(offsetof(X87Register, ieee754) == 0, "Invalid offset for X87Register::ieee754");
#else
#include "softfloat.h"
using X87Register = floatx80;

static_assert(sizeof(X87Register) == 0x0A, "Invalid size for X87Register");

#endif