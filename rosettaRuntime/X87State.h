#pragma once

#include <cstddef>
#include <cstdint>
#include <cmath>
#include <utility>

#include "Log.h"
#include "X87Register.h"

enum X87StatusWordFlag : uint16_t {
  // Exception flags
  kInvalidOperation    = 0x0001,  // Invalid Operation Exception
  kDenormalizedOperand = 0x0002,  // Denormalized Operand Exception
  kZeroDivide         = 0x0004,  // Zero Divide Exception
  kOverflow           = 0x0008,  // Overflow Exception–
  kUnderflow          = 0x0010,  // Underflow Exception
  kPrecision          = 0x0020,  // Precision Exception

  // Status flags
  kStackFault         = 0x0040,  // Stack Fault
  kErrorSummary       = 0x0080,  // Error Summary Status
  
  // Condition codes
  kConditionCode0     = 0x0100,  // Condition Code 0
  kConditionCode1     = 0x0200,  // Condition Code 1
  kConditionCode2     = 0x0400,  // Condition Code 2
  kConditionCode3     = 0x4000,  // Condition Code 3
  
  // Special flags
  kTopOfStack         = 0x3800,  // Top of Stack Pointer (bits 11-13)
  kBusy               = 0x8000,  // FPU Busy
};

enum class X87TagState {
  kValid = 0,    // 00: Valid non-zero value
  kZero = 1,     // 01: Valid zero value
  kSpecial = 2,  // 10: Special value (NaN, Infinity, Denormal)
  kEmpty = 3     // 11: Empty register
};

enum X87ControlWord : uint16_t {
    // Exception Masks (1=masked)
    kInvalidOpMask      = 0x0001,
    kDenormalMask      = 0x0002,
    kZeroDivideMask    = 0x0004,
    kOverflowMask      = 0x0008,
    kUnderflowMask     = 0x0010,
    kPrecisionMask     = 0x0020,

    // Precision Control
    kPrecisionControl  = 0x0300,
    kPrecision24Bit    = 0x0000,  // Single precision
    kPrecision53Bit    = 0x0200,  // Double precision
    kPrecision64Bit    = 0x0300,  // Extended precision

    // Rounding Control
    kRoundingControlMask = 0x0C00,
    kRoundToNearest     = 0x0000,
    kRoundDown         = 0x0400,
    kRoundUp           = 0x0800,
    kRoundToZero       = 0x0C00,

    // Infinity Control (only on 287)
    kInfinityControl   = 0x1000
};

#pragma pack(push, 1)
struct X87State {
  uint16_t control_word;
  uint16_t status_word;
  int16_t tag_word;
  X87Register st[8];

  X87State()
      : control_word(0x037F),
        status_word(0x0000),
        tag_word(0xFFFF)  // All registers marked empty (11)
  {
    // Initialize all registers to zero
    for (int i = 0; i < 8; i++) {
      st[i].ieee754 = 0.0;
    }
  }

  // Get index of top register
  auto top_index() const -> unsigned int { return (status_word >> 11) & 7; }  // Get reference to top register

  // auto top_register() const -> const X87Register& { return st[top_index()]; }

  // Get index of ST(i) register
  auto get_st_index(unsigned int st_offset) const -> unsigned int { return (st_offset + top_index()) & 7; }

  // Get value from register at ST(i). Checks tag bits for validity, returns 0.0 if empty. Updates status word.
  auto get_st(unsigned int st_offset) -> double { 
    const unsigned int reg_idx = get_st_index(st_offset);
    const int tag = (tag_word >> (reg_idx * 2)) & 3;
    if (tag == 3) {
      //FP_X_STK | FP_X_INV
      status_word |= 0x41;
      return std::numeric_limits<double>::quiet_NaN();
    }

    return st[reg_idx].ieee754;
  }

  auto get_st_const(unsigned int st_offset) const -> std::pair<double, uint16_t> {
    const unsigned int reg_idx = get_st_index(st_offset);
    const X87TagState tag = static_cast<X87TagState>((tag_word >> (reg_idx * 2)) & 3);

    uint16_t new_status_word = status_word & ~(X87StatusWordFlag::kConditionCode1);
    if (tag == X87TagState::kEmpty) {
      //FP_X_STK | FP_X_INV
      // return nan
      return { std::numeric_limits<double>::quiet_NaN(), new_status_word | 0x41};
    }

    return {st[reg_idx].ieee754, new_status_word};
  }

  auto get_st_tag(unsigned int st_offset) const -> X87TagState {
    const unsigned int reg_idx = get_st_index(st_offset);
    return static_cast<X87TagState>((tag_word >> (reg_idx * 2)) & 3);
  }

  // Get reference to register at ST(i)
  // auto get_st(unsigned int st_offset) -> X87Register& { return st[get_st_index(st_offset)]; }

  // Push value to FPU stack
  auto push() -> void {
    const int current_top = top_index();
    const int new_top = (current_top - 1) & 7;
    status_word = (status_word & ~0x3800) | (new_top << 11);
    // Clear tag bits (set to valid 00) for new register
    tag_word &= ~(3 << (new_top * 2));
  }

  // Pop value from FPU stack
  auto pop() -> void {
    const int current_top = top_index();
    // Set tag bits to empty (11) for popped register
    tag_word |= (3 << (current_top * 2));
    st[current_top].ieee754 = 0.0;
    status_word = (status_word & ~0x3800) | (((current_top + 1) & 7) << 11);
  }

  // Set value in register at absolute index
  auto set_st(unsigned int st_offset, double value) -> void {
    auto st_idx = get_st_index(st_offset);
    st[st_idx].ieee754 = value;
    X87TagState tag;
    if (value == 0.0) {
      tag = X87TagState::kZero;
    }
    else if (std::isnan(value) || std::isinf(value) || std::fpclassify(value) == FP_SUBNORMAL) {
      tag = X87TagState::kSpecial;
    }
    else
    { tag = X87TagState::kValid; }

    // simple_printf("set_st tag: %d\n", tag);

    // Clear existing tag bits and set new state
    tag_word &= ~(3 << (st_idx * 2));
    tag_word |= (static_cast<int>(tag) << (st_idx * 2));
  }

  auto swap_registers(unsigned int reg_offset1, unsigned int reg_offset2) -> void {
    // Swap register contents
    auto reg_idx1 = get_st_index(reg_offset1);
    auto reg_idx2 = get_st_index(reg_offset2);

    auto temp = st[reg_idx1].ieee754;
    st[reg_idx1].ieee754 = st[reg_idx2].ieee754;
    st[reg_idx2].ieee754 = temp;
  
    // Get current tags
    const int tag1 = (tag_word >> (reg_idx1 * 2)) & 3;
    const int tag2 = (tag_word >> (reg_idx2 * 2)) & 3;

    // Clear both tags
    tag_word &= ~((3 << (reg_idx1 * 2)) | (3 << (reg_idx2 * 2)));

    // Set swapped tags
    tag_word |= (tag2 << (reg_idx1 * 2)) | (tag1 << (reg_idx2 * 2));
  }

  auto print() const -> void {
    simple_printf("FPU state:\n");
    simple_printf("Control word: %d\n", control_word);
    simple_printf("Status word: %d\n", status_word);
    simple_printf("Tag word: %d\n", tag_word);
    simple_printf("Top index: %d\n", top_index());
    simple_printf("\n");
  }
};
#pragma pack(pop)
static_assert(sizeof(X87State) == 0x56, "Invalid size for X87State");
static_assert(offsetof(X87State, control_word) == 0, "Invalid offset for X87State::control_word");
static_assert(offsetof(X87State, status_word) == 2, "Invalid offset for X87State::status_word");
static_assert(offsetof(X87State, tag_word) == 4, "Invalid offset for X87State::tag_word");
static_assert(offsetof(X87State, st) == 6, "Invalid offset for X87State::st0");