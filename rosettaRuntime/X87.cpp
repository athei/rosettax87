#include "X87.h"

#include <arm_neon.h>

#include "Export.h"
#include "Log.h"
#include "Neon.h"
#include "X87Register.h"
#include "X87State.h"
#include "mmath.h"

void* init_library(SymbolList const* a1, unsigned long long a2, ThreadContextOffsets const* a3) {
  exports_init();

  simple_printf(
      "RosettaRuntimex87 built %s"
      "\n",
      __DATE__ " " __TIME__);

  return orig_init_library(a1, a2, a3);
}
void* register_runtime_routine_offsets(unsigned long long const* a1, char const** a2, unsigned long a3) {
  return orig_register_runtime_routine_offsets(a1, a2, a3);
}
void* translator_use_t8027_codegen(bool a1) {
  // LOG(1, "translator_use_t8027_codegen\n", 28);
  return orig_translator_use_t8027_codegen(a1);
}
void* translator_reset(void) {
  // LOG(1, "translator_reset\n", 17);
  return orig_translator_reset();
}
void* ir_create_bad_access(unsigned long long a1, BadAccessKind a2) {
  // LOG(1, "ir_create_bad_access\n", 22);
  return orig_ir_create_bad_access(a1, a2);
}
void* ir_create(unsigned long long a1, unsigned int a2, unsigned int a3, TranslationMode a4, ExecutionMode a5) {
  // LOG(1, "ir_create\n", 10);
  return orig_ir_create(a1, a2, a3, a4, a5);
}
void* module_free(ModuleResult const* a1) {
  // LOG(1, "module_free\n", 12);
  return orig_module_free(a1);
}
void* module_get_size(ModuleResult const* a1) {
  // LOG(1, "module_get_size\n", 16);
  return orig_module_get_size(a1);
}
void* module_is_bad_access(ModuleResult const* a1) {
  // LOG(1, "module_is_bad_access\n", 21);
  return orig_module_is_bad_access(a1);
}
void* module_print(ModuleResult const* a1, int a2) {
  // LOG(1, "module_print\n", 13);
  return orig_module_print(a1, a2);
}
void* translator_translate(ModuleResult const* a1, TranslationMode a2) {
  // LOG(1, "translator_translate\n", 21);
  return orig_translator_translate(a1, a2);
}
void* translator_free(TranslationResult const* a1) {
  // LOG(1, "translator_free\n", 17);
  return orig_translator_free(a1);
}
void* translator_get_data(TranslationResult const* a1) {
  // LOG(1, "translator_get_data\n", 20);
  return orig_translator_get_data(a1);
}
void* translator_get_size(TranslationResult const* a1) {
  // LOG(1, "translator_get_size\n", 20);
  return orig_translator_get_size(a1);
}
void* translator_get_branch_slots_offset(TranslationResult const* a1) {
  // LOG(1, "translator_get_branch_slots_offset\n", 35);
  return orig_translator_get_branch_slots_offset(a1);
}
void* translator_get_branch_slots_count(TranslationResult const* a1) {
  // LOG(1, "translator_get_branch_slots_count\n", 34);
  return orig_translator_get_branch_slots_count(a1);
}
translator_get_branch_entries_result translator_get_branch_entries(TranslationResult const* a1) {
  // LOG(1, "translator_get_branch_entries\n", 29);
  return orig_translator_get_branch_entries(a1);
}
translator_get_instruction_offsets_result translator_get_instruction_offsets(TranslationResult const* a1) {
  // LOG(1, "translator_get_instruction_offsets\n", 35);
  return orig_translator_get_instruction_offsets(a1);
}
void* translator_apply_fixups(TranslationResult* a1, unsigned char* a2, unsigned long long a3) {
  // LOG(1, "translator_apply_fixups\n", 24);
  return orig_translator_apply_fixups(a1, a2, a3);
}

void x87_init(X87State* a1) {
  LOG(1, "x87_init\n", 9);

  *a1 = X87State();
}
void x87_state_from_x86_float_state(X87State* a1, X86FloatState64 const* a2) {
  MISSING(1, "x87_state_from_x86_float_state\n", 31);
  orig_x87_state_from_x86_float_state(a1, a2);
}
void x87_state_to_x86_float_state(X87State const* a1, X86FloatState64* a2) {
  MISSING(1, "x87_state_to_x86_float_state\n", 29);
  orig_x87_state_to_x86_float_state(a1, a2);
}
void x87_pop_register_stack(X87State* a1) {
  MISSING(1, "x87_pop_register_stack\n", 23);
  orig_x87_pop_register_stack(a1);
}
// Computes the exponential value of 2 to the power of the source operand minus 1. The source operand is located in
// register ST(0) and the result is also stored in ST(0). The value of the source operand must lie in the range –1.0 to
// +1.0. If the source value is outside this range, the result is undefined.
void x87_f2xm1(X87State* state) {
  // Get value from ST(0)
  auto x = state->get_st(0);

  // // Check range [-1.0, +1.0]
  if (x < -1.0f || x > 1.0f) {
    // Set to NaN for undefined result
    state->set_st(0, 0);
    return;
  }

  // Calculate 2^x - 1 using mmath::exp2
  auto result = mmath::exp2(x) - 1.0f;

  // Store result back in ST(0)
  state->set_st(0, result);
}

// Clears the sign bit of ST(0) to create the absolute value of the operand. The following table shows the results
// obtained when creating the absolute value of various classes of numbers. C1 	Set to 0.
void x87_fabs(X87State* a1) {
  LOG(1, "x87_fabs\n", 10);

  // Clear condition code 1 and exception flags
  a1->status_word &= ~X87StatusWordFlag::kConditionCode1;

  // Get value in ST(0)
  auto value = a1->get_st(0);

  // Set value in ST(0) to its absolute value
  a1->set_st(0, value < 0 ? -value : value);
}

void x87_fadd_ST(X87State* a1, unsigned int st_offset_1, unsigned int st_offset_2, bool pop_stack) {
  LOG(1, "x87_fadd_ST\n", 13);

  // Clear condition code 1 and exception flags
  a1->status_word &= ~X87StatusWordFlag::kConditionCode1;

  // Get register indices and values
  const auto val1 = a1->get_st(st_offset_1);
  const auto val2 = a1->get_st(st_offset_2);

  // Perform addition and store result in ST(idx1)
  a1->set_st(st_offset_1, val1 + val2);

  if (pop_stack) {
    a1->pop();
  }
}

void x87_fadd_f32(X87State* a1, unsigned int fp32) {
  LOG(1, "x87_fadd_f32\n", 14);

  a1->status_word &= ~X87StatusWordFlag::kConditionCode1;

  auto value = *reinterpret_cast<float*>(&fp32);
  auto st0 = a1->get_st(0);

  a1->set_st(0, st0 + value);
}
void x87_fadd_f64(X87State* a1, unsigned long long a2) {
  LOG(1, "x87_fadd_f64\n", 14);

  a1->status_word &= ~X87StatusWordFlag::kConditionCode1;

  auto value = *reinterpret_cast<double*>(&a2);
  auto st0 = a1->get_st(0);

  a1->set_st(0, st0 + value);
}

void x87_fbld(X87State* a1, unsigned long long a2, unsigned long long a3) {
  MISSING(1, "x87_fbld\n", 10);
  orig_x87_fbld(a1, a2, a3);
}
void x87_fbstp(X87State const* a1) {
  MISSING(1, "x87_fbstp\n", 11);
  orig_x87_fbstp(a1);
}
void x87_fchs(X87State* a1) {
  LOG(1, "x87_fchs\n", 10);

  // set C1 to 0
  a1->status_word &= ~X87StatusWordFlag::kConditionCode1;

  // Negate value in ST(0)
  a1->set_st(0, -a1->get_st(0));
}

// void __fastcall rosetta::runtime::library::x87_fcmov(X87State *a1, int a2, char st_offset)
// {
//   // a2 is the condition flag - determines if move should occur
//   // if a2 == 0, condition is false and no move happens
//   // if a2 == 1, condition is true and move executes

//   unsigned __int64 status_word = (unsigned __int16)a1->status_word;
//   a1->status_word = status_word & ~0x200;  // Clear precision flag

//   // Check if source operand is valid via tag word
//   if ( (~(a1->tag_word >> (2 * (((status_word >> 11) + st_offset) & 7))) & 3) != 0 )
//   {
//     if ( !a2 )  // If condition is false
//       return;    // Skip the move
//     // Calculate source register address
//     v5 = &a1->st[((unsigned __int8)(status_word >> 11) + st_offset) & 7];
//     top_index = (status_word >> 11) & 7;
//   }
//   else
//   {
//     // Handle invalid source operand
//     a1->status_word = status_word & ~0x241 | 0x41;  // Set invalid operation
//     top_index = (status_word >> 11) & 7;
//     v5 = (struct X87Register *)&qword_54820;  // Load default value
//   }

//   // Perform the actual register move
//   exponent = v5->exponent;
//   v7 = (char *)a1 + 10 * (unsigned int)top_index;  // Destination address
//   *(_QWORD *)(v7 + 6) = v5->mantissa;  // Copy mantissa
//   *((_WORD *)v7 + 7) = exponent;       // Copy exponent
//   update_x87_register_tag(a1, 0);      // Update tags
// }

void x87_fcmov(X87State* state, unsigned int condition, unsigned int st_offset) {
  LOG(1, "x87_fcmov\n", 11);

  // clear precision flag
  state->status_word &= ~X87StatusWordFlag::kConditionCode1;

  double value;

  auto st_tag_word = state->get_st_tag(st_offset);
  if (st_tag_word != X87TagState::kEmpty) {
    if (condition == 0) {
      return;
    }

    value = state->get_st(st_offset);
  } else {
    state->status_word |= 0x41;  // Set invalid operation
    value = 0.0f;
  }

  state->set_st(0, value);  // Perform the actual register move
}

void x87_fcom_ST(X87State* a1, unsigned int st_offset, unsigned int number_of_pops) {
  LOG(1, "x87_fcom_ST\n", 13);

  // Get values to compare
  auto st0 = a1->get_st(0);
  auto sti = a1->get_st(st_offset);

  // Clear condition code bits C0, C2, C3 (bits 8, 9, 14)
  a1->status_word &= ~(kConditionCode0 | kConditionCode1 | kConditionCode2 | kConditionCode3);

  // Set condition codes based on comparison
  if (std::isnan(st0) || std::isnan(sti)) {
    a1->status_word |= kConditionCode0 | kConditionCode2 | kConditionCode3;  // Set C0=C2=C3=1
  } else if (st0 > sti) {
    // Leave C0=C2=C3=0
  } else if (st0 < sti) {
    a1->status_word |= kConditionCode0;  // Set C0=1
  } else {                               // st0 == sti
    a1->status_word |= kConditionCode3;  // Set C3=1
  }

  // Handle pops if requested
  for (unsigned int i = 0; i < number_of_pops; i++) {
    a1->pop();
  }
}

void x87_fcom_f32(X87State* a1, unsigned int fp32, bool pop) {
  LOG(1, "x87_fcom_f32\n", 14);

  auto st0 = a1->get_st(0);
  auto value = *reinterpret_cast<float*>(&fp32);

  a1->status_word &= ~(kConditionCode0 | kConditionCode1 | kConditionCode2 | kConditionCode3);

  if (std::isnan(st0) || std::isnan(value)) {
    a1->status_word |= kConditionCode0 | kConditionCode2 | kConditionCode3;  // Set C0=C2=C3=1
  } else if (st0 > value) {
    // Leave C0=C2=C3=0
  } else if (st0 < value) {
    a1->status_word |= kConditionCode0;  // Set C0=1
  } else {                               // st0 == value
    a1->status_word |= kConditionCode3;  // Set C3=1
  }

  if (pop) {
    a1->pop();
  }
}
void x87_fcom_f64(X87State* a1, unsigned long long fp64, bool pop) {
  LOG(1, "x87_fcom_f64\n", 14);

  auto st0 = a1->get_st(0);
  auto value = *reinterpret_cast<double*>(&fp64);

  a1->status_word &= ~(kConditionCode0 | kConditionCode1 | kConditionCode2 | kConditionCode3);

  if (std::isnan(st0) || std::isnan(value)) {
    a1->status_word |= kConditionCode0 | kConditionCode2 | kConditionCode3;  // Set C0=C2=C3=1
  } else if (st0 > value) {
    // Leave C0=C2=C3=0
  } else if (st0 < value) {
    a1->status_word |= kConditionCode0;  // Set C0=1
  } else {                               // st0 == value
    a1->status_word |= kConditionCode3;  // Set C3=1
  }

  if (pop) {
    a1->pop();
  }
}

uint32_t x87_fcomi(X87State* state, unsigned int st_offset, bool pop) {
  LOG(1, "x87_fcomi\n", 11);

  state->status_word &= ~(kConditionCode0);

  auto st0_val = state->get_st(0);
  auto sti_val = state->get_st(st_offset);

  uint32_t flags = 0;
  /*
  Filters: fcomi
  Randomness seeded to: 3528984885
  x87_fcomi_less
  x87_fcomi result: 0x000000000000000
  x87_fcomi_greater
  x87_fcomi result: 0x000000020000000
  x87_fcomi_equal
  x87_fcomi result: 0x000000060000000
  */

  if (st0_val < sti_val) {
    flags = 0x000000000000000;
  } else if (st0_val > sti_val) {
    flags = 0x000000020000000;
  } else {
    flags = 0x000000060000000;
  }

  if (pop) {
    state->pop();
  }

  return flags;
}

void x87_fcos(X87State* a1) {
  LOG(1, "x87_fcos\n", 10);

  a1->status_word &= ~(kConditionCode1 | kConditionCode2);
  // Get ST(0)
  auto value = a1->get_st(0);

  // Calculate cosine
  auto result = mmath::cos(value);

  // Store result back in ST(0)
  a1->set_st(0, result);
}
void x87_fdecstp(X87State* a1) {
  MISSING(1, "x87_fdecstp\n", 13);
  orig_x87_fdecstp(a1);
}

void x87_fdiv_ST(X87State* a1, unsigned int st_offset_1, unsigned int st_offset_2, bool pop_stack) {
  LOG(1, "x87_fdiv_ST\n", 13);

  // Clear condition code 1 and exception flags
  a1->status_word &= ~X87StatusWordFlag::kConditionCode1;

  // Get register indices and values
  const auto val1 = a1->get_st(st_offset_1);
  const auto val2 = a1->get_st(st_offset_2);

  // Perform reversed division and store result
  a1->set_st(st_offset_1, val1 / val2);

  if (pop_stack) {
    a1->pop();
  }
}

void x87_fdiv_f32(X87State* a1, unsigned int a2) {
  LOG(1, "x87_fdiv_f32\n", 14);

  a1->status_word &= ~X87StatusWordFlag::kConditionCode1;

  auto value = *reinterpret_cast<float*>(&a2);
  auto st0 = a1->get_st(0);

  a1->set_st(0, st0 / value);
}

void x87_fdiv_f64(X87State* a1, unsigned long long a2) {
  LOG(1, "x87_fdiv_f64\n", 14);

  a1->status_word &= ~X87StatusWordFlag::kConditionCode1;

  auto value = *reinterpret_cast<double*>(&a2);
  auto st0 = a1->get_st(0);

  a1->set_st(0, st0 / value);
}

void x87_fdivr_ST(X87State* a1, unsigned int st_offset_1, unsigned int st_offset_2, bool pop_stack) {
  LOG(1, "x87_fdivr_ST\n", 14);

  // Clear condition code 1 and exception flags
  a1->status_word &= ~X87StatusWordFlag::kConditionCode1;

  // Get register indices and values
  const auto val1 = a1->get_st(st_offset_1);
  const auto val2 = a1->get_st(st_offset_2);

  // Perform reversed division and store result
  a1->set_st(st_offset_1, val2 / val1);

  if (pop_stack) {
    a1->pop();
  }
}

void x87_fdivr_f32(X87State* a1, unsigned int a2) {
  LOG(1, "x87_fdivr_f32\n", 15);

  a1->status_word &= ~X87StatusWordFlag::kConditionCode1;

  auto value = *reinterpret_cast<float*>(&a2);
  auto st0 = a1->get_st(0);

  a1->set_st(0, value / st0);
}

void x87_fdivr_f64(X87State* a1, unsigned long long a2) {
  LOG(1, "x87_fdivr_f64\n", 15);

  a1->status_word &= ~X87StatusWordFlag::kConditionCode1;

  auto value = *reinterpret_cast<double*>(&a2);
  auto st0 = a1->get_st(0);

  a1->set_st(0, value / st0);
}

void x87_ffree(X87State* a1, unsigned int a2) {
  LOG(1, "x87_ffree\n", 11);
  orig_x87_ffree(a1, a2);
}
void x87_fiadd(X87State* a1, int m32int) {
  LOG(1, "x87_fiadd\n", 11);

  // simple_printf("m32int: %d\n", m32int);

  // Clear condition code 1 and exception flags
  a1->status_word &= ~X87StatusWordFlag::kConditionCode1;

  // Get value in ST(0)
  auto st0 = a1->get_st(0);

  // Add integer value
  st0 += m32int;

  // Store result back in ST(0)
  a1->set_st(0, st0);
}

void x87_ficom(X87State* a1, int a2, bool a3) {
  MISSING(1, "x87_ficom\n", 11);
  orig_x87_ficom(a1, a2, a3);
}

void x87_fidiv(X87State* a1, int a2) {
  LOG(1, "x87_fidiv\n", 11);

  // Clear condition code 1 and exception flags
  a1->status_word &= ~X87StatusWordFlag::kConditionCode1;

  // Get value in ST(0)
  auto value = a1->get_st(0);

  // Divide by integer value
  value /= a2;

  // Store result back in ST(0)
  a1->set_st(0, value);
}

void x87_fidivr(X87State* a1, int a2) {
  LOG(1, "x87_fidivr\n", 12);

  // Clear condition code 1 and exception flags
  a1->status_word &= ~X87StatusWordFlag::kConditionCode1;

  // Get value in ST(0)
  auto value = a1->get_st(0);

  // Divide integer value by value in ST(0)
  value = a2 / value;

  // Store result back in ST(0)
  a1->set_st(0, value);
}

// Converts the signed-integer source operand into double extended-precision floating-point format and pushes the value
// onto the FPU register stack. The source operand can be a word, doubleword, or quadword integer. It is loaded without
// rounding errors. The sign of the source operand is preserved.
void x87_fild(X87State* a1, long long a2) {
  LOG(1, "x87_fild\n", 10);

  a1->push();
  a1->set_st(0, a2);
}

void x87_fimul(X87State* a1, int a2) {
  LOG(1, "x87_fimul\n", 11);

  // Clear condition code 1 and exception flags
  a1->status_word &= ~X87StatusWordFlag::kConditionCode1;

  // Get value in ST(0)
  auto value = a1->get_st(0);

  // Multiply by integer value
  value *= a2;

  // Store result back in ST(0)
  a1->set_st(0, value);
}
void x87_fincstp(X87State* a1) {
  MISSING(1, "x87_fincstp\n", 13);
  orig_x87_fincstp(a1);
}

X87ResultStatusWord x87_fist_i16(X87State const* a1) {
  LOG(1, "x87_fist_i16\n", 14);

  auto [value, status_word] = a1->get_st_const(0);
  X87ResultStatusWord result{0, status_word};

  // Special case: value > INT16_MAX or infinity (changed from >=)
  if (value > static_cast<double>(INT16_MAX)) {
    result.signed_result = INT16_MIN;  // 0x8000
    result.status_word |= X87StatusWordFlag::kConditionCode1;
    return result;
  }

  // Special case: value <= INT16_MIN
  if (value <= static_cast<double>(INT16_MIN)) {
    result.signed_result = INT16_MIN;
    result.status_word |= X87StatusWordFlag::kConditionCode1;
    return result;
  }

  // Normal case
  auto round_bits = a1->control_word & X87ControlWord::kRoundingControlMask;

  switch (round_bits) {
    case X87ControlWord::kRoundToNearest: {
      result.signed_result = static_cast<int16_t>(std::nearbyint(value));
    } break;

    case X87ControlWord::kRoundDown: {
      result.signed_result = static_cast<int16_t>(std::floor(value));
      return result;
    } break;
    case X87ControlWord::kRoundUp: {
      result.signed_result = static_cast<int16_t>(std::ceil(value));
      return result;
    } break;

    case X87ControlWord::kRoundToZero: {
      result.signed_result = static_cast<int16_t>(value);
      return result;
    } break;
  }

  return result;
}

X87ResultStatusWord x87_fist_i32(X87State const* a1) {
  LOG(1, "x87_fist_i32\n", 14);

  auto [value, status_word] = a1->get_st_const(0);
  X87ResultStatusWord result{0, status_word};

  // Special case: value >= INT32_MAX or infinity
  if (value >= static_cast<double>(INT32_MAX)) {
    result.signed_result = INT32_MIN;  // 0x80000000
    result.status_word |= X87StatusWordFlag::kConditionCode1;
    return result;
  }

  // Special case: value <= INT32_MIN
  if (value <= static_cast<double>(INT32_MIN)) {
    result.signed_result = INT32_MIN;
    result.status_word |= X87StatusWordFlag::kConditionCode1;
    return result;
  }

  auto round_bits = a1->control_word & X87ControlWord::kRoundingControlMask;

  switch (round_bits) {
    case X87ControlWord::kRoundToNearest: {
      result.signed_result = static_cast<int32_t>(std::nearbyint(value));
    } break;

    case X87ControlWord::kRoundDown: {
      result.signed_result = static_cast<int32_t>(std::floor(value));
      return result;
    } break;
    case X87ControlWord::kRoundUp: {
      result.signed_result = static_cast<int32_t>(std::ceil(value));
      return result;
    } break;

    case X87ControlWord::kRoundToZero: {
      result.signed_result = static_cast<int32_t>(value);
      return result;
    } break;
  }

  return result;
}
X87ResultStatusWord x87_fist_i64(X87State const* a1) {
  LOG(1, "x87_fist_i64\n", 14);

  // Get value in ST(0)
  auto [value, status_word] = a1->get_st_const(0);

  X87ResultStatusWord result{0, status_word};

  // Special case: value >= INT64_MAX or infinity
  if (value >= static_cast<double>(INT64_MAX)) {
    result.signed_result = INT64_MIN;  // 0x8000000000000000
    result.status_word |= X87StatusWordFlag::kConditionCode1;
    return result;
  }

  // Special case: value <= INT64_MIN
  if (value <= static_cast<double>(INT64_MIN)) {
    result.signed_result = INT64_MIN;
    result.status_word |= X87StatusWordFlag::kConditionCode1;
    return result;
  }

  // Normal case

  auto round_bits = a1->control_word & X87ControlWord::kRoundingControlMask;

  switch (round_bits) {
    case X87ControlWord::kRoundToNearest: {
      result.signed_result = static_cast<int64_t>(std::nearbyint(value));
    } break;

    case X87ControlWord::kRoundDown: {
      result.signed_result = static_cast<int64_t>(std::floor(value));
      return result;
    } break;
    case X87ControlWord::kRoundUp: {
      result.signed_result = static_cast<int64_t>(std::ceil(value));
      return result;
    } break;

    case X87ControlWord::kRoundToZero: {
      result.signed_result = static_cast<int64_t>(value);
      return result;
    } break;
  }

  return result;
}

X87ResultStatusWord x87_fistt_i16(X87State const* a1) {
  LOG(1, "x87_fistt_i16\n", 15);

  // Get value in ST(0)
  auto [value, status_word] = a1->get_st_const(0);

  return {.signed_result = static_cast<int16_t>(value), status_word};
}

X87ResultStatusWord x87_fistt_i32(X87State const* a1) {
  LOG(1, "x87_fistt_i32\n", 15);

  // Get value in ST(0)
  auto [value, status_word] = a1->get_st_const(0);

  return {.signed_result = static_cast<int32_t>(value), status_word};
}

X87ResultStatusWord x87_fistt_i64(X87State const* a1) {
  LOG(1, "x87_fistt_i64\n", 15);

  // Get value in ST(0)
  auto [value, status_word] = a1->get_st_const(0);

  return {.signed_result = static_cast<int64_t>(value), status_word};
}
void x87_fisub(X87State* a1, int a2) {
  LOG(1, "x87_fisub\n", 11);

  // Clear condition code 1
  a1->status_word &= ~(X87StatusWordFlag::kConditionCode1);

  // Get value in ST(0)
  auto value = a1->get_st(0);

  // Subtract integer value
  value -= a2;

  // Store result back in ST(0)
  a1->set_st(0, value);
}

void x87_fisubr(X87State* a1, int a2) {
  LOG(1, "x87_fisubr\n", 12);

  // Clear condition code 1
  a1->status_word &= ~(X87StatusWordFlag::kConditionCode1);

  // Get value in ST(0)
  auto value = a1->get_st(0);

  // Subtract integer value
  value = a2 - value;

  // Store result back in ST(0)
  a1->set_st(0, value);
}

// Push ST(i) onto the FPU register stack.
void x87_fld_STi(X87State* a1, unsigned int st_offset) {
  LOG(1, "x87_fld_STi\n", 13);
  a1->status_word &= ~0x200u;

  // Get index of ST(i) register
  const auto value = a1->get_st(st_offset);

  // make room for new value
  a1->push();

  // Copy value from ST(i) to ST(0)
  a1->set_st(0, value);
}
void x87_fld_constant(X87State* a1, X87Constant a2) {
  LOG(1, "x87_fld_constant\n", 18);
  // simple_printf("x87_fld_constant %d\n", (int)a2);

  switch (a2) {
    case X87Constant::kOne: {  // fld1
      a1->push();
      a1->set_st(0, 1.0);
    } break;

    case X87Constant::kZero: {  // fldz
      a1->push();
      a1->set_st(0, 0.0);
    } break;

    case X87Constant::kPi: {  // fldpi
      // store_x87_extended_value(a1, {.ieee754 = 3.141592741f});
      a1->push();
      a1->set_st(0, 3.141592741f);
    } break;

    case X87Constant::kLog2e: {  // fldl2e
      // store_x87_extended_value(a1, {.ieee754 = 1.44269502f});
      a1->push();
      a1->set_st(0, 1.44269502f);
    } break;

    case X87Constant::kLoge2: {  // fldln2
      // store_x87_extended_value(a1, {.ieee754 = 0.693147182f});
      a1->push();
      a1->set_st(0, 0.693147182f);
    } break;

    case X87Constant::kLog2t: {  // fldl2t
      // store_x87_extended_value(a1, {.ieee754 = 3.321928f});
      a1->push();
      a1->set_st(0, 3.321928f);
    } break;

    case X87Constant::kLog102: {  // fldl2e
      // store_x87_extended_value(a1, {.ieee754 = 0.301029987f});
      a1->push();
      a1->set_st(0, 0.301029987f);
    } break;

    default: {
      simple_printf("x87_fld_constant ERROR %d\n", (int)a2);
    } break;
  }

  // orig_x87_fld_constant(a1, a2);
}
void x87_fld_fp32(X87State* a1, unsigned int a2) {
  LOG(1, "x87_fld_fp32\n", 14);

  // Push new value onto stack, get reference to new top
  a1->push();

  a1->set_st(0, *reinterpret_cast<float*>(&a2));
}

void x87_fld_fp64(X87State* a1, unsigned long long a2) {
  LOG(1, "x87_fld_fp64\n", 14);

  // Push new value onto stack, get reference to new top
  a1->push();

  a1->set_st(0, *reinterpret_cast<double*>(&a2));
}

uint32_t ConvertX87RegisterToFloat32(X87Float80 x87, uint16_t* status_flags) {
  // Extract components
  uint64_t mantissa = x87.mantissa;
  uint16_t biased_exp = x87.exponent & 0x7FFF;
  uint32_t sign = (x87.exponent & 0x8000) ? 0x80000000 : 0;

  // Handle zero
  if (mantissa == 0) {
    return sign;
  }

  // Handle infinity and NaN
  if (biased_exp == 0x7FFF) {
    if (mantissa != 0x8000000000000000ULL) {
      // NaN
      *status_flags |= 0x1;
      return sign | 0x7F800000 | 0x400000;  // Quiet NaN
    }
    // Infinity
    return sign | 0x7F800000;
  }

  // Convert biased exponent from x87 (16383) to IEEE 754 (127)
  int32_t exp = biased_exp - 16383 + 127;

  // Handle denormals and underflow
  if (exp <= 0) {
    *status_flags |= 0x2;  // Set underflow flag
    if (exp < -23) {
      return sign;  // Too small, return signed zero
    }
    // Denormalize
    mantissa = (mantissa >> (-exp + 1));
    exp = 0;
  }

  // Handle overflow
  if (exp >= 255) {
    *status_flags |= 0x4;      // Set overflow flag
    return sign | 0x7F800000;  // Return infinity
  }

  // Extract 23 bits for mantissa, with rounding
  uint32_t significant = (mantissa >> 40) & 0x7FFFFF;

  // Round to nearest even
  uint64_t round_bit = (mantissa >> 39) & 1;
  uint64_t sticky_bits = (mantissa & ((1ULL << 39) - 1)) != 0;

  if (round_bit && (sticky_bits || (significant & 1))) {
    significant++;
    // Handle carry from rounding
    if (significant == 0x800000) {
      significant = 0;
      exp++;
    }
  }

  return sign | (exp << 23) | significant;
}

void x87_fld_fp80(X87State* a1, X87Float80 a2) {
  LOG(1, "x87_fld_fp80\n", 14);

  auto ieee754 = ConvertX87RegisterToFloat32(a2, &a1->status_word);

  // convert to ieee754 float
  a1->push();
  a1->set_st(0, *reinterpret_cast<float*>(&ieee754));
}

void x87_fmul_ST(X87State* a1, unsigned int st_offset_1, unsigned int st_offset_2, bool pop_stack) {
  LOG(1, "x87_fmul_ST\n", 13);

  // Clear condition code 1 and exception flags
  a1->status_word &= ~X87StatusWordFlag::kConditionCode1;

  // Get register indices and values
  const auto val1 = a1->get_st(st_offset_1);
  const auto val2 = a1->get_st(st_offset_2);

  // Perform reversed division and store result
  a1->set_st(st_offset_1, val1 * val2);

  if (pop_stack) {
    a1->pop();
  }
}

void x87_fmul_f32(X87State* a1, unsigned int fp32) {
  LOG(1, "x87_fmul_f32\n", 14);

  a1->status_word &= ~X87StatusWordFlag::kConditionCode1;

  auto value = *reinterpret_cast<float*>(&fp32);
  auto st0 = a1->get_st(0);

  a1->set_st(0, st0 * value);
}

void x87_fmul_f64(X87State* a1, unsigned long long a2) {
  LOG(1, "x87_fmul_f64\n", 14);

  a1->status_word &= ~X87StatusWordFlag::kConditionCode1;

  auto value = *reinterpret_cast<double*>(&a2);
  auto st0 = a1->get_st(0);

  a1->set_st(0, st0 * value);
}

// Replace ST(1) with arctan(ST(1)/ST(0)) and pop the register stack.
void x87_fpatan(X87State* a1) {
  LOG(1, "x87_fpatan\n", 12);

  a1->status_word &= ~(X87StatusWordFlag::kConditionCode1);

  // Get values from ST(0) and ST(1)
  auto st0 = a1->get_st(0);
  auto st1 = a1->get_st(1);

  // Calculate arctan(ST(1)/ST(0))
  auto result = mmath::atan2(st1, st0);

  // Store result in ST(1) and pop the register stack
  a1->set_st(1, result);

  a1->pop();
}

// Replace ST(0) with the remainder obtained from dividing ST(0) by ST(1).
// Computes the remainder obtained from dividing the value in the ST(0) register (the dividend) by the value in the
// ST(1) register (the divisor or modulus), and stores the result in ST(0). The remainder represents the following
// value: Remainder := ST(0) − (Q ∗ ST(1)) Here, Q is an integer value that is obtained by truncating the floating-point
// number quotient of [ST(0) / ST(1)] toward zero. The sign of the remainder is the same as the sign of the dividend.
// The magnitude of the remainder is less than that of the modulus, unless a partial remainder was computed (as
// described below). This instruction produces an exact result; the inexact-result exception does not occur and the
// rounding control has no effect. The following table shows the results obtained when computing the remainder of
// various classes of numbers, assuming that underflow does not occur.
void x87_fprem(X87State* a1) {
  LOG(1, "x87_fprem\n", 11);

  // Clear condition code bits initially
  a1->status_word &= ~(X87StatusWordFlag::kConditionCode0 | X87StatusWordFlag::kConditionCode1 |
                       X87StatusWordFlag::kConditionCode2 | X87StatusWordFlag::kConditionCode3);

  auto st0 = a1->get_st(0);
  auto st1 = a1->get_st(1);

  // simple_printf("ST0=%f ST1=%f\n", st0, st1);

  // Handle special cases
  if (std::isnan(st0) || std::isnan(st1) || std::isinf(st0) || st1 == 0.0) {
    a1->set_st(0, std::numeric_limits<double>::quiet_NaN());
    a1->status_word |= X87StatusWordFlag::kInvalidOperation;
    return;
  }

  if (std::isinf(st1)) {
    a1->set_st(0, st0);
    return;
  }

  // Calculate raw division
  auto raw_div = st0 / st1;
  // simple_printf("raw division=%f\n", raw_div);

  // Calculate quotient by truncating toward zero
  auto truncated = std::trunc(raw_div);
  int64_t quotient = static_cast<int64_t>(truncated);
  // simple_printf("truncated=%f quotient=%d\n", truncated, quotient);

  // Calculate remainder
  auto result = st0 - (static_cast<double>(quotient) * st1);
  // simple_printf("final result=%f\n", result);

  // Set condition code bits based on quotient least significant bits
  if (quotient & 1) a1->status_word |= X87StatusWordFlag::kConditionCode1;
  if (quotient & 2) a1->status_word |= X87StatusWordFlag::kConditionCode3;
  if (quotient & 4) a1->status_word |= X87StatusWordFlag::kConditionCode0;

  // C2=0 indicates complete remainder
  // Convert to unsigned for comparison
  uint64_t abs_quotient = (quotient >= 0) ? quotient : -quotient;
  if (abs_quotient < (1ULL << 63)) {  // Use 63 bits to avoid overflow
    a1->status_word &= ~X87StatusWordFlag::kConditionCode2;
  } else {
    a1->status_word |= X87StatusWordFlag::kConditionCode2;
  }
  // simple_printf("final result=%f\n", result);

  a1->set_st(0, result);
}
void x87_fprem1(X87State* a1) {
  MISSING(1, "x87_fprem1\n", 12);
  orig_x87_fprem1(a1);
}
// Computes the approximate tangent of the source operand in register ST(0), stores the result in ST(0), and pushes
// a 1.0 onto the FPU register stack. The source operand must be given in radians and must be less than ±263. The
// following table shows the unmasked results obtained when computing the partial tangent of various classes of numbers,
// assuming that underflow does not occur.
void x87_fptan(X87State* a1) {
  LOG(1, "x87_fptan\n", 11);

  a1->status_word &= ~(X87StatusWordFlag::kConditionCode1 | X87StatusWordFlag::kConditionCode2);

  // Get value from ST(0)
  const auto value = a1->get_st(0);

  // Calculate tangent
  auto tan_value = mmath::tan(value);

  // Store result in ST(0)
  a1->set_st(0, tan_value);

  // Push 1.0 onto the FPU register stack
  a1->push();
  a1->set_st(0, 1.0);
}

// Rounds the source value in the ST(0) register to the nearest integral value, depending on the current rounding mode
// (setting of the RC field of the FPU control word), and stores the result in ST(0).
void x87_frndint(X87State* a1) {
  LOG(1, "x87_frndint\n", 13);

  a1->status_word &= ~(X87StatusWordFlag::kConditionCode1);

  // Get current value and round it
  double value = a1->get_st(0);
  double rounded = nearbyint(value);

  // Store rounded value and update tag
  a1->set_st(0, rounded);
}

// Truncates the value in the source operand (toward 0) to an integral value and adds that value to the exponent of the
// destination operand. The destination and source operands are floating-point values located in registers ST(0) and
// ST(1), respectively. This instruction provides rapid multiplication or division by integral powers of 2. The
// following table shows the results obtained when scaling various classes of numbers, assuming that neither overflow
// nor underflow occurs.
void x87_fscale(X87State* state) {
  LOG(1, "x87_fscale\n", 12);

  state->status_word &= ~(X87StatusWordFlag::kConditionCode1);

  // Get values from ST(0) and ST(1)
  double st0 = state->get_st(0);
  double st1 = state->get_st(1);

  // Round ST(1) to nearest integer
  int scale = static_cast<int>(st1);

  // Scale ST(0) by 2^scale using bit manipulation for integer powers
  int32_t exponent = scale + 1023;  // IEEE-754 bias
  uint64_t scaleFactor = static_cast<uint64_t>(exponent) << 52;
  double factor = *reinterpret_cast<double*>(&scaleFactor);

  // Multiply ST(0) by scale factor
  double result = st0 * factor;

  // Store result back in ST(0)
  state->set_st(0, result);
}

void x87_fsin(X87State* a1) {
  LOG(1, "x87_fsin\n", 10);

  a1->status_word &= ~(X87StatusWordFlag::kConditionCode1 | X87StatusWordFlag::kConditionCode2);

  // Get current value from top register
  const double value = a1->get_st(0);

  // Convert to NEON vector and calculate sin
  float32x4_t vec = vdupq_n_f32(value);
  float32x4_t result = vsinq_neon_f32(vec);

  // Store result and update tag
  a1->set_st(0, vgetq_lane_f32(result, 0));
}

// Compute the sine and cosine of ST(0); replace ST(0) with the approximate sine, and push the approximate cosine onto
// the register stack.
/*
IF ST(0) < 2^63
    THEN
        C2 := 0;
        TEMP := fcos(ST(0)); // approximation of cosine
        ST(0) := fsin(ST(0)); // approximation of sine
        TOP := TOP − 1;
        ST(0) := TEMP;
    ELSE (* Source operand out of range *)
        C2 := 1;
FI;
*/
void x87_fsincos(X87State* a1) {
  LOG(1, "x87_fsincos\n", 13);

  a1->status_word &= ~(X87StatusWordFlag::kConditionCode1 | X87StatusWordFlag::kConditionCode2);

  // Get value from ST(0)
  const auto value = a1->get_st(0);

  // Calculate sine and cosine
  auto sin_value = mmath::sin(value);
  auto cos_value = mmath::cos(value);

  // Store sine in ST(0)
  a1->set_st(0, sin_value);

  // Push cosine onto the FPU register stack
  a1->push();
  a1->set_st(0, cos_value);

  // Clear C2 condition code bit
  a1->status_word &= ~X87StatusWordFlag::kConditionCode2;
}
// Computes square root of ST(0) and stores the result in ST(0).
void x87_fsqrt(X87State* a1) {
  LOG(1, "x87_fsqrt\n", 11);

  a1->status_word &= ~(X87StatusWordFlag::kConditionCode1);

  // Get current value and calculate sqrt
  const double value = a1->get_st(0);

  a1->status_word |= X87StatusWordFlag::kPrecision;

  // Store result and update tag
  a1->set_st(0, sqrt(value));
}

void x87_fst_STi(X87State* a1, unsigned int st_offset, bool pop) {
  LOG(1, "x87_fst_STi\n", 13);

  // Clear C1 condition code (bit 9)
  a1->status_word &= ~X87StatusWordFlag::kConditionCode1;

  // Copy ST(0) to ST(i)
  a1->set_st(st_offset, a1->get_st(0));

  // Pop if requested
  if (pop) {
    a1->pop();
  }
}

X87ResultStatusWord x87_fst_fp32(X87State const* a1) {
  LOG(1, "x87_fst_fp32\n", 14);

  auto [value, status_word] = a1->get_st_const(0);
  float tmp = value;
  return {*reinterpret_cast<uint32_t*>(&tmp), status_word};
}

X87ResultStatusWord x87_fst_fp64(X87State const* a1) {
  LOG(1, "x87_fst_fp64\n", 14);

  // Create temporary double to ensure proper value representation
  auto [value, status_word] = a1->get_st_const(0);
  double tmp = value;
  return {*reinterpret_cast<uint64_t*>(&tmp), status_word};
}

X87Float80 x87_fst_fp80(X87State const* a1) {
  LOG(1, "x87_fst_fp80\n", 14);

  // Get value from ST(0)
  auto [value, status_word] = a1->get_st_const(0);

  float tmp = value;
  uint32_t float32 = *reinterpret_cast<uint32_t*>(&tmp);
  ;

  // Extract components from float32
  uint32_t mantissa = float32 & 0x7FFFFF;  // 23 bits
  uint8_t exp = (float32 >> 23) & 0xFF;    // 8 bits
  uint16_t sign = (float32 >> 31) << 15;   // Move sign to bit 15

  X87Float80 result;

  // Handle zero
  if (exp == 0 && mantissa == 0) {
    result.mantissa = 0;
    result.exponent = sign;
    return result;
  }

  // Handle subnormal numbers
  if (exp == 0) {
    // Set denormal flag

    // Count leading zeros to normalize
    int leading_zeros = __builtin_clz(mantissa) - 8;  // -8 because mantissa is in upper 23 bits
    mantissa <<= leading_zeros;

    // Adjust exponent for normalization
    exp = 1 - leading_zeros;
  }
  // Handle infinity or NaN
  else if (exp == 255) {
    // Set invalid operation flag if NaN

    result.mantissa = (uint64_t)mantissa << 40 | 0x8000000000000000ULL;
    result.exponent = sign | 0x7FFF;  // Maximum exponent
    return result;
  }

  // Normal numbers: Convert to x87 format
  // Shift 23-bit mantissa to 64 bits and set explicit integer bit
  result.mantissa = ((uint64_t)mantissa << 40) | 0x8000000000000000ULL;

  // Bias adjustment: IEEE 754 bias(127) to x87 bias(16383)
  result.exponent = sign | (exp + 16383 - 127);

  return result;
}
void x87_fsub_ST(X87State* a1, unsigned int st_offset1, unsigned int st_offset2, bool pop) {
  LOG(1, "x87_fsub_ST\n", 13);

  // Clear condition code 1 and exception flags
  a1->status_word &= ~X87StatusWordFlag::kConditionCode1;

  // Get register indices and values
  const auto val1 = a1->get_st(st_offset1);
  const auto val2 = a1->get_st(st_offset2);

  // Perform subtraction and store result in ST(st_offset1)
  a1->set_st(st_offset1, val1 - val2);

  if (pop) {
    a1->pop();
  }
}

void x87_fsub_f32(X87State* a1, unsigned int a2) {
  LOG(1, "x87_fsub_f32\n", 14);

  a1->status_word &= ~X87StatusWordFlag::kConditionCode1;

  auto value = *reinterpret_cast<float*>(&a2);
  auto st0 = a1->get_st(0);

  a1->set_st(0, st0 - value);
}

void x87_fsub_f64(X87State* a1, unsigned long long a2) {
  LOG(1, "x87_fsub_f64\n", 14);

  a1->status_word &= ~X87StatusWordFlag::kConditionCode1;

  auto value = *reinterpret_cast<double*>(&a2);
  auto st0 = a1->get_st(0);

  a1->set_st(0, st0 - value);
}

void x87_fsubr_ST(X87State* a1, unsigned int st_offset1, unsigned int st_offset2, bool pop) {
  LOG(1, "x87_fsubr_ST\n", 14);

  // Clear condition code 1 and exception flags
  a1->status_word &= ~X87StatusWordFlag::kConditionCode1;

  // Get register indices and values

  const auto val1 = a1->get_st(st_offset1);
  const auto val2 = a1->get_st(st_offset2);

  // Perform reversed subtraction and store result in ST(st_offset1)
  a1->set_st(st_offset1, val2 - val1);

  if (pop) {
    a1->pop();
  }
}

void x87_fsubr_f32(X87State* a1, unsigned int a2) {
  LOG(1, "x87_fsubr_f32\n", 15);

  a1->status_word &= ~X87StatusWordFlag::kConditionCode1;

  auto value = *reinterpret_cast<float*>(&a2);
  auto st0 = a1->get_st(0);

  a1->set_st(0, value - st0);
}

void x87_fsubr_f64(X87State* a1, unsigned long long a2) {
  LOG(1, "x87_fsubr_f64\n", 15);

  a1->status_word &= ~X87StatusWordFlag::kConditionCode1;

  auto value = *reinterpret_cast<double*>(&a2);
  auto st0 = a1->get_st(0);

  a1->set_st(0, value - st0);
}

void x87_fucom(X87State* a1, unsigned int a2, unsigned int a3) {
  MISSING(1, "x87_fucom\n", 11);
  orig_x87_fucom(a1, a2, a3);
}
// Compare ST(0) with ST(i), check for ordered values, set status flags accordingly, and pop register stack.
uint32_t x87_fucomi(X87State* state, unsigned int st_offset, bool pop_stack) {
  LOG(1, "x87_fucomi\n", 12);

  state->status_word &= ~X87StatusWordFlag::kConditionCode1;

  auto st0_val = state->get_st(0);
  auto sti_val = state->get_st(st_offset);

  uint32_t flags = 0;
  /*
  Filters: fcomi
  Randomness seeded to: 3528984885
  x87_fcomi_less
  x87_fcomi result: 0x000000000000000
  x87_fcomi_greater
  x87_fcomi result: 0x000000020000000
  x87_fcomi_equal
  x87_fcomi result: 0x000000060000000
  */

  if (st0_val < sti_val) {
    flags = 0x000000000000000;
  } else if (st0_val > sti_val) {
    flags = 0x000000020000000;
  } else {
    flags = 0x000000060000000;
  }

  if (pop_stack) {
    state->pop();
  }

  return flags;
}

/*
C1 := sign bit of ST; (* 0 for positive, 1 for negative *)
CASE (class of value or number in ST(0)) OF
    Unsupported:C3, C2, C0 := 000;
    NaN:
        C3, C2, C0 := 001;
    Normal:
        C3, C2, C0 := 010;
    Infinity:
        C3, C2, C0 := 011;
    Zero:
        C3, C2, C0 := 100;
    Empty:
        C3, C2, C0 := 101;
    Denormal:
        C3, C2, C0 := 110;
ESAC;

*/
void x87_fxam(X87State* a1) {
  LOG(1, "x87_fxam\n", 10);

  // Get tag state for ST(0)
  X87TagState tag = a1->get_st_tag(0);

  // simple_printf("tag: %d\n", tag);

  static_assert((X87StatusWordFlag::kConditionCode0 | X87StatusWordFlag::kConditionCode1 |
                 X87StatusWordFlag::kConditionCode2 | X87StatusWordFlag::kConditionCode3) == 0x4700);

  // Clear C3,C2,C1,C0 bits
  a1->status_word &= ~(X87StatusWordFlag::kConditionCode0 | X87StatusWordFlag::kConditionCode1 |
                       X87StatusWordFlag::kConditionCode2 | X87StatusWordFlag::kConditionCode3);

  // Handle empty and zero based on tag word
  if (tag == X87TagState::kEmpty) {
    a1->status_word |= X87StatusWordFlag::kConditionCode3 | X87StatusWordFlag::kConditionCode0;  // C3=1, C0=1 (101)
    return;
  }
  if (tag == X87TagState::kZero) {
    a1->status_word |= X87StatusWordFlag::kConditionCode3;  // C3=1 (100)
    return;
  }

  // Get actual value for other cases
  auto value = a1->get_st(0);

  // Set C1 based on sign
  if (std::signbit(value)) {
    a1->status_word |= X87StatusWordFlag::kConditionCode1;
  }

  // Set C3,C2,C0 based on value type
  if (std::isnan(value)) {
    a1->status_word |= X87StatusWordFlag::kConditionCode0;  // 001
  } else if (std::isinf(value)) {
    a1->status_word |= X87StatusWordFlag::kConditionCode2 | X87StatusWordFlag::kConditionCode0;  // 011
  } else if (std::fpclassify(value) == FP_SUBNORMAL) {
    a1->status_word |= X87StatusWordFlag::kConditionCode3 | X87StatusWordFlag::kConditionCode2;  // 110
  } else {
    a1->status_word |= X87StatusWordFlag::kConditionCode2;  // 010 (normal)
  }
}

void x87_fxch(X87State* a1, unsigned int st_offset) {
  LOG(1, "x87_fxch\n", 10);

  // Clear condition code 1
  a1->status_word &= ~X87StatusWordFlag::kConditionCode1;

  auto st0 = a1->get_st(0);
  auto sti = a1->get_st(st_offset);

  a1->set_st(0, sti);
  a1->set_st(st_offset, st0);
}
void x87_fxtract(X87State* a1) {
  MISSING(1, "x87_fxtract\n", 13);
  orig_x87_fxtract(a1);
}

// Replace ST(1) with (ST(1) ∗ log2ST(0)) and pop the register stack.
void x87_fyl2x(X87State* state) {
  LOG(1, "x87_fyl2x\n", 12);

  state->status_word &= ~X87StatusWordFlag::kConditionCode1;

  // Get x from ST(0) and y from ST(1)
  auto st0 = state->get_st(0);
  auto st1 = state->get_st(1);

  // Calculate y * log2(x)
  auto result = st1 * (mmath::log2(st0));

  // Pop ST(0)
  state->pop();

  // Store result in new ST(0)
  state->set_st(0, result);
}

void x87_fyl2xp1(X87State* a1) {
  MISSING(1, "x87_fyl2xp1\n", 13);
  orig_x87_fyl2xp1(a1);
}
void* sse_pcmpestri(unsigned long long a1, unsigned long long a2, unsigned long long a3, unsigned long long a4,
                    unsigned char a5, long long a6, long long a7) {
  // LOG(1, "sse_pcmpestri\n", 15);
  return orig_sse_pcmpestri(a1, a2, a3, a4, a5, a6, a7);
}
void* sse_pcmpestrm(unsigned long long a1, unsigned long long a2, unsigned long long a3, unsigned long long a4,
                    unsigned char a5, long long a6, long long a7) {
  // LOG(1, "sse_pcmpestrm\n", 15);
  return orig_sse_pcmpestrm(a1, a2, a3, a4, a5, a6, a7);
}
void* sse_pcmpistri(unsigned long long a1, unsigned long long a2, unsigned long long a3, unsigned long long a4,
                    unsigned char a5) {
  // LOG(1, "sse_pcmpistri\n", 15);
  return orig_sse_pcmpistri(a1, a2, a3, a4, a5);
}
void* sse_pcmpistrm(unsigned long long a1, unsigned long long a2, unsigned long long a3, unsigned long long a4,
                    unsigned char a5) {
  // LOG(1, "sse_pcmpistrm\n", 15);
  return orig_sse_pcmpistrm(a1, a2, a3, a4, a5);
}
void* is_ldt_initialized(void) {
  // LOG(1, "is_ldt_initialized\n", 19);
  return orig_is_ldt_initialized();
}
void* get_ldt(unsigned int a1, unsigned int a2, void* a3, unsigned int a4) {
  // LOG(1, "get_ldt\n", 9);
  return orig_get_ldt(a1, a2, a3, a4);
}
void* set_ldt(unsigned int a1, unsigned int a2, void* const* a3, unsigned int a4) {
  // LOG(1, "set_ldt\n", 9);
  return orig_set_ldt(a1, a2, a3, a4);
}
void* execution_mode_for_code_segment_selector(unsigned int a1, unsigned short a2) {
  // LOG(1, "execution_mode_for_code_segment_selector\n", 41);
  return orig_execution_mode_for_code_segment_selector(a1, a2);
}
void* mov_segment(unsigned int a1, SegmentRegisters* a2, SegmentRegister a3, unsigned short a4) {
  // LOG(1, "mov_segment\n", 13);
  return orig_mov_segment(a1, a2, a3, a4);
}
void* abi_for_address(unsigned long long a1) {
  // LOG(1, "abi_for_address\n", 17);
  return orig_abi_for_address(a1);
}
void* determine_state_recovery_action(unsigned int const* a1, unsigned int a2, unsigned int a3) {
  // LOG(1, "determine_state_recovery_action\n", 32);
  return orig_determine_state_recovery_action(a1, a2, a3);
}
void* get_segment_limit(unsigned int a1, unsigned short a2) {
  // LOG(1, "get_segment_limit\n", 18);
  return orig_get_segment_limit(a1, a2);
}
void* translator_set_variant(bool a1) {
  // LOG(1, "translator_set_variant\n", 23);
  return orig_translator_set_variant(a1);
}

void __attribute__((naked, used)) runtime_cpuid(uint16_t a1, uint16_t a2) {
  asm volatile(
    "adrp x22, _orig_runtime_cpuid@PAGE\n"
    "ldr x22, [x22, _orig_runtime_cpuid@PAGEOFF]\n"
    "br x22"
  );
}

void runtime_wide_udiv_64(uint64_t a1, uint64_t a2, uint64_t a3) {
  LOG(1, "runtime_wide_udiv_64\n", 21);
  orig_runtime_wide_udiv_64(a1, a2, a3);
}
void runtime_wide_sdiv_64(int64_t a1, int64_t a2, int64_t* a3, int64_t* a4) {
  LOG(1, "runtime_wide_sdiv_64\n", 21);
  orig_runtime_wide_sdiv_64(a1, a2, a3, a4);
}