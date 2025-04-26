#include "X87.h"

#include <arm_neon.h>

#include "Export.h"
#include "Log.h"
#include "X87Register.h"
#include "X87State.h"
#include "Neon.h"

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

  // *a1 = X87State();
  orig_x87_init(a1);
}
void x87_state_from_x86_float_state(X87State* a1, X86FloatState64 const* a2) {
  LOG(1, "x87_state_from_x86_float_state\n", 31);
  orig_x87_state_from_x86_float_state(a1, a2);
}
void x87_state_to_x86_float_state(X87State const* a1, X86FloatState64* a2) {
  LOG(1, "x87_state_to_x86_float_state\n", 29);
  orig_x87_state_to_x86_float_state(a1, a2);
}
void x87_pop_register_stack(X87State* a1) {
  LOG(1, "x87_pop_register_stack\n", 23);
  orig_x87_pop_register_stack(a1);
}
void x87_f2xm1(X87State* a1) {
  LOG(1, "x87_f2xm1\n", 11);
  orig_x87_f2xm1(a1);
}
void x87_fabs(X87State* a1) {
  LOG(1, "x87_fabs\n", 10);
  orig_x87_fabs(a1);
}

void x87_fadd_ST(X87State* a1, unsigned int st_offset_1, unsigned int st_offset_2, bool pop_stack) {
  LOG(1, "x87_fadd_ST\n", 13);
  orig_x87_fadd_ST(a1, st_offset_1, st_offset_2, pop_stack);
}

void x87_fadd_f32(X87State* a1, unsigned int a2) {
  LOG(1, "x87_fadd_f32\n", 14);
  orig_x87_fadd_f32(a1, a2);
}
void x87_fadd_f64(X87State* a1, unsigned long long a2) {
  LOG(1, "x87_fadd_f64\n", 14);
  orig_x87_fadd_f64(a1, a2);
}
void x87_fbld(X87State* a1, unsigned long long a2, unsigned long long a3) {
  LOG(1, "x87_fbld\n", 10);
  orig_x87_fbld(a1, a2, a3);
}
void x87_fbstp(X87State const* a1) {
  LOG(1, "x87_fbstp\n", 11);
  orig_x87_fbstp(a1);
}
void x87_fchs(X87State* a1) {
  LOG(1, "x87_fchs\n", 10);
  orig_x87_fchs(a1);
}
void x87_fcmov(X87State* a1, unsigned int a2, unsigned int a3) {
  LOG(1, "x87_fcmov\n", 11);
  orig_x87_fcmov(a1, a2, a3);
}
void x87_fcom_ST(X87State* a1, unsigned int a2, unsigned int a3) {
  LOG(1, "x87_fcom_ST\n", 13);
  orig_x87_fcom_ST(a1, a2, a3);
}
void x87_fcom_f32(X87State* a1, unsigned int a2, bool a3) {
  LOG(1, "x87_fcom_f32\n", 14);
  orig_x87_fcom_f32(a1, a2, a3);
}
void x87_fcom_f64(X87State* a1, unsigned long long a2, bool a3) {
  LOG(1, "x87_fcom_f64\n", 14);
  orig_x87_fcom_f64(a1, a2, a3);
}
uint32_t x87_fcomi(X87State* a1, unsigned int a2, bool a3) {
  LOG(1, "x87_fcomi\n", 11);
  auto result = orig_x87_fcomi(a1, a2, a3);
  return result;
}
void x87_fcos(X87State* a1) {
  LOG(1, "x87_fcos\n", 10);
  orig_x87_fcos(a1);
}
void x87_fdecstp(X87State* a1) {
  LOG(1, "x87_fdecstp\n", 13);
  orig_x87_fdecstp(a1);
}

void x87_fdiv_ST(X87State* a1, unsigned int st_offset_1, unsigned int st_offset_2, bool pop_stack) {
  LOG(1, "x87_fdiv_ST\n", 13);

  orig_x87_fdiv_ST(a1, st_offset_1, st_offset_2, pop_stack);
}

void x87_fdiv_f32(X87State* a1, unsigned int a2) {
  LOG(1, "x87_fdiv_f32\n", 14);
  orig_x87_fdiv_f32(a1, a2);
}
void x87_fdiv_f64(X87State* a1, unsigned long long a2) {
  LOG(1, "x87_fdiv_f64\n", 14);
  orig_x87_fdiv_f64(a1, a2);
}

void x87_fdivr_ST(X87State* a1, unsigned int st_offset_1, unsigned int st_offset_2, bool pop_stack) {
  LOG(1, "x87_fdivr_ST\n", 14);
  orig_x87_fdivr_ST(a1, st_offset_1, st_offset_2, pop_stack);
}

void x87_fdivr_f32(X87State* a1, unsigned int a2) {
  LOG(1, "x87_fdivr_f32\n", 15);
  orig_x87_fdivr_f32(a1, a2);
}
void x87_fdivr_f64(X87State* a1, unsigned long long a2) {
  LOG(1, "x87_fdivr_f64\n", 15);
  orig_x87_fdivr_f64(a1, a2);
}
void x87_ffree(X87State* a1, unsigned int a2) {
  LOG(1, "x87_ffree\n", 11);
  orig_x87_ffree(a1, a2);
}
void x87_fiadd(X87State* a1, int a2) {
  LOG(1, "x87_fiadd\n", 11);
  orig_x87_fiadd(a1, a2);
}
void x87_ficom(X87State* a1, int a2, bool a3) {
  LOG(1, "x87_ficom\n", 11);
  orig_x87_ficom(a1, a2, a3);
}
void x87_fidiv(X87State* a1, int a2) {
  LOG(1, "x87_fidiv\n", 11);
  orig_x87_fidiv(a1, a2);
}
void x87_fidivr(X87State* a1, int a2) {
  LOG(1, "x87_fidivr\n", 12);
  orig_x87_fidivr(a1, a2);
}
void x87_fild(X87State* a1, long long a2) {
  LOG(1, "x87_fild\n", 10);
  orig_x87_fild(a1, a2);
}
void x87_fimul(X87State* a1, int a2) {
  LOG(1, "x87_fimul\n", 11);
  orig_x87_fimul(a1, a2);
}
void x87_fincstp(X87State* a1) {
  LOG(1, "x87_fincstp\n", 13);
  orig_x87_fincstp(a1);
}
X87ResultStatusWord x87_fist_i16(X87State const* a1) {
  LOG(1, "x87_fist_i16\n", 14);
  return orig_x87_fist_i16(a1);
}
X87ResultStatusWord x87_fist_i32(X87State const* a1) {
  LOG(1, "x87_fist_i32\n", 14);
  return orig_x87_fist_i32(a1);
}
X87ResultStatusWord x87_fist_i64(X87State const* a1) {
  LOG(1, "x87_fist_i64\n", 14);
  return orig_x87_fist_i64(a1);
}
X87ResultStatusWord x87_fistt_i16(X87State const* a1) {
  LOG(1, "x87_fistt_i16\n", 15);
  return orig_x87_fistt_i16(a1);
}
X87ResultStatusWord x87_fistt_i32(X87State const* a1) {
  LOG(1, "x87_fistt_i32\n", 15);
  return orig_x87_fistt_i32(a1);
}
X87ResultStatusWord x87_fistt_i64(X87State const* a1) {
  LOG(1, "x87_fistt_i64\n", 15);
  return orig_x87_fistt_i64(a1);
}
void x87_fisub(X87State* a1, int a2) {
  LOG(1, "x87_fisub\n", 11);
  orig_x87_fisub(a1, a2);
}
void x87_fisubr(X87State* a1, int a2) {
  LOG(1, "x87_fisubr\n", 12);
  orig_x87_fisubr(a1, a2);
}
void x87_fld_STi(X87State* a1, unsigned int a2) {
  LOG(1, "x87_fld_STi\n", 13);
  orig_x87_fld_STi(a1, a2);
}
void x87_fld_constant(X87State* a1, X87Constant a2) {
  LOG(1, "x87_fld_constant\n", 18);
  // simple_printf("x87_fld_constant %d\n", (int)a2);
  orig_x87_fld_constant(a1, a2);
  // a1->print();
}
void x87_fld_fp32(X87State* a1, unsigned int a2) {
  LOG(1, "x87_fld_fp32\n", 14);
  orig_x87_fld_fp32(a1, a2);
}

void x87_fld_fp64(X87State* a1, unsigned long long a2) {
  LOG(1, "x87_fld_fp64\n", 14);
  orig_x87_fld_fp64(a1, a2);
}
void x87_fld_fp80(X87State* a1, X87Float80 a2) {
  LOG(1, "x87_fld_fp80\n", 14);
  orig_x87_fld_fp80(a1, a2);
}

void x87_fmul_ST(X87State* a1, unsigned int st_offset_1, unsigned int st_offset_2, bool pop_stack) {
  LOG(1, "x87_fmul_ST\n", 13);
  orig_x87_fmul_ST(a1, st_offset_1, st_offset_2, pop_stack);
}

void x87_fmul_f32(X87State* a1, unsigned int a2) {
  LOG(1, "x87_fmul_f32\n", 14);
  orig_x87_fmul_f32(a1, a2);
}
void x87_fmul_f64(X87State* a1, unsigned long long a2) {
  LOG(1, "x87_fmul_f64\n", 14);
  orig_x87_fmul_f64(a1, a2);
}
void x87_fpatan(X87State* a1) {
  LOG(1, "x87_fpatan\n", 12);
  orig_x87_fpatan(a1);
}
void x87_fprem(X87State* a1) {
  LOG(1, "x87_fprem\n", 11);
  orig_x87_fprem(a1);
}
void x87_fprem1(X87State* a1) {
  LOG(1, "x87_fprem1\n", 12);
  orig_x87_fprem1(a1);
}
void x87_fptan(X87State* a1) {
  LOG(1, "x87_fptan\n", 11);
  orig_x87_fptan(a1);
}

// Rounds the source value in the ST(0) register to the nearest integral value, depending on the current rounding mode
// (setting of the RC field of the FPU control word), and stores the result in ST(0).
void x87_frndint(X87State* a1) {
  LOG(1, "x87_frndint\n", 13);
  orig_x87_frndint(a1);
}

void x87_fscale(X87State* a1) {
  LOG(1, "x87_fscale\n", 12);
  orig_x87_fscale(a1);
}


void x87_fsin(X87State* a1) {
  LOG(1, "x87_fsin\n", 10);
  orig_x87_fsin(a1);
}

void x87_fsincos(X87State* a1) {
  LOG(1, "x87_fsincos\n", 13);
  orig_x87_fsincos(a1);
}
// Computes square root of ST(0) and stores the result in ST(0).
void x87_fsqrt(X87State* a1) {
  LOG(1, "x87_fsqrt\n", 11);
  orig_x87_fsqrt(a1);
}

void x87_fst_STi(X87State* a1, unsigned int a2, bool a3) {
  LOG(1, "x87_fst_STi\n", 13);
  orig_x87_fst_STi(a1, a2, a3);
}

X87ResultStatusWord x87_fst_fp32(X87State const* a1) {
  LOG(1, "x87_fst_fp32\n", 13);
  auto result = orig_x87_fst_fp32(a1);
  return result;
}

X87ResultStatusWord x87_fst_fp64(X87State const* a1) {
  LOG(1, "x87_fst_fp64\n", 14);
  return orig_x87_fst_fp64(a1);
}

X87Float80 x87_fst_fp80(X87State const* a1) {
  LOG(1, "x87_fst_fp80\n", 14);
  return orig_x87_fst_fp80(a1);
}
void x87_fsub_ST(X87State* a1, unsigned int a2, unsigned int a3, bool a4) {
  LOG(1, "x87_fsub_ST\n", 13);
  orig_x87_fsub_ST(a1, a2, a3, a4);
}
void x87_fsub_f32(X87State* a1, unsigned int a2) {
  LOG(1, "x87_fsub_f32\n", 14);
  orig_x87_fsub_f32(a1, a2);
}
void x87_fsub_f64(X87State* a1, unsigned long long a2) {
  LOG(1, "x87_fsub_f64\n", 14);
  orig_x87_fsub_f64(a1, a2);
}
void x87_fsubr_ST(X87State* a1, unsigned int a2, unsigned int a3, bool a4) {
  LOG(1, "x87_fsubr_ST\n", 14);
  orig_x87_fsubr_ST(a1, a2, a3, a4);
}
void x87_fsubr_f32(X87State* a1, unsigned int a2) {
  LOG(1, "x87_fsubr_f32\n", 15);
  orig_x87_fsubr_f32(a1, a2);
}
void x87_fsubr_f64(X87State* a1, unsigned long long a2) {
  LOG(1, "x87_fsubr_f64\n", 15);
  orig_x87_fsubr_f64(a1, a2);
}
void x87_fucom(X87State* a1, unsigned int a2, unsigned int a3) {
  LOG(1, "x87_fucom\n", 11);
  orig_x87_fucom(a1, a2, a3);
}
uint32_t x87_fucomi(X87State* a1, unsigned int a2, bool a3) {
  LOG(1, "x87_fucomi\n", 12);
  auto result = orig_x87_fucomi(a1, a2, a3);
  simple_printf("x87_fucomi %p\n", result);
  return result;
}
void x87_fxam(X87State* a1) {
  LOG(1, "x87_fxam\n", 10);
  orig_x87_fxam(a1);
}

void x87_fxch(X87State* a1, unsigned int st_offset) {
  LOG(1, "x87_fxch\n", 10);

  orig_x87_fxch(a1, st_offset);
}
void x87_fxtract(X87State* a1) {
  LOG(1, "x87_fxtract\n", 13);
  orig_x87_fxtract(a1);
}
void x87_fyl2x(X87State* a1) {
  LOG(1, "x87_fyl2x\n", 11);
  orig_x87_fyl2x(a1);
}
void x87_fyl2xp1(X87State* a1) {
  LOG(1, "x87_fyl2xp1\n", 13);
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

void runtime_cpuid(uint16_t a1, uint16_t a2) {
  LOG(1, "runtime_cpuid\n", 14);
  orig_runtime_cpuid(a1, a2);
}
void runtime_wide_udiv_64(uint64_t a1, uint64_t a2, uint64_t a3) {
  LOG(1, "runtime_wide_udiv_64\n", 21);
  orig_runtime_wide_udiv_64(a1, a2, a3);
}
void runtime_wide_sdiv_64(int64_t a1, int64_t a2, int64_t* a3, int64_t* a4) {
  LOG(1, "runtime_wide_sdiv_64\n", 21);
  orig_runtime_wide_sdiv_64(a1, a2, a3, a4);
}