// This file is part of AsmJit project <https://asmjit.com>
//
// See asmjit.h or LICENSE.md for license and copyright information
// SPDX-License-Identifier: Zlib

#ifndef ASMJIT_RISCV_RISCV64EMITTER_H_INCLUDED
#define ASMJIT_RISCV_RISCV64EMITTER_H_INCLUDED

#include "../core/emitter.h"
#include "../core/support.h"
#include "../riscv/riscv64globals.h"
#include "../riscv/riscv64operand.h"

ASMJIT_BEGIN_SUB_NAMESPACE(riscv64)

// TODO: [RISC-V] `#define ASMJIT_INST_` macros

#define ASMJIT_INST_0x(NAME, INST_ID) \
  inline Error NAME() { return _emitter()->_emitI(INST_ID); }

#define ASMJIT_INST_1x(NAME, INST_ID, T0) \
  inline Error NAME(const T0& o0) { return _emitter()->_emitI(INST_ID, o0); }

#define ASMJIT_INST_2x(NAME, INST_ID, T0, T1) \
  inline Error NAME(const T0& o0, const T1& o1) { return _emitter()->_emitI(INST_ID, o0, o1); }

#define ASMJIT_INST_3x(NAME, INST_ID, T0, T1, T2) \
  inline Error NAME(const T0& o0, const T1& o1, const T2& o2) { return _emitter()->_emitI(INST_ID, o0, o1, o2); }

#define ASMJIT_INST_4x(NAME, INST_ID, T0, T1, T2, T3) \
  inline Error NAME(const T0& o0, const T1& o1, const T2& o2, const T3& o3) { return _emitter()->_emitI(INST_ID, o0, o1, o2, o3); }

#define ASMJIT_INST_5x(NAME, INST_ID, T0, T1, T2, T3, T4) \
  inline Error NAME(const T0& o0, const T1& o1, const T2& o2, const T3& o3, const T4& o4) { return _emitter()->_emitI(INST_ID, o0, o1, o2, o3, o4); }

//! \addtogroup asmjit_riscv64
//! \{

//! RISC-V 64-bit emitter.
//!
//! NOTE: This class cannot be instantiated, you can only cast to it and use it as emitter that emits to either
//! \ref Assembler, \ref Builder, or \ref Compiler (use withcaution with \ref Compiler as it expects virtual
//! registers to be used).
template<typename This>
struct EmitterExplicitT {
  //! \cond

  // These two are unfortunately reported by the sanitizer. We know what we do, however, the sanitizer doesn't.
  // I have tried to use reinterpret_cast instead, but that would generate bad code when compiled by MSC.
  ASMJIT_ATTRIBUTE_NO_SANITIZE_UNDEF inline This* _emitter() noexcept { return static_cast<This*>(this); }
  ASMJIT_ATTRIBUTE_NO_SANITIZE_UNDEF inline const This* _emitter() const noexcept { return static_cast<const This*>(this); }

  //! \endcond

  // ${riscv64::EmitterExplicit:Begin}
  // ------------------- Automatically generated, do not edit -------------------
  ASMJIT_INST_3x(add, Inst::kIdAdd, Gp, Gp, Gp)
  ASMJIT_INST_3x(addi, Inst::kIdAddi, Gp, Gp, Imm)
  ASMJIT_INST_3x(and_, Inst::kIdAnd, Gp, Gp, Gp)
  ASMJIT_INST_3x(andi, Inst::kIdAndi, Gp, Gp, Imm)
  ASMJIT_INST_2x(auipc, Inst::kIdAuipc, Gp, Imm)
  ASMJIT_INST_4x(beq, Inst::kIdBeq, Imm, Gp, Gp, Imm)
  ASMJIT_INST_4x(bge, Inst::kIdBge, Imm, Gp, Gp, Imm)
  ASMJIT_INST_4x(bgeu, Inst::kIdBgeu, Imm, Gp, Gp, Imm)
  ASMJIT_INST_4x(blt, Inst::kIdBlt, Imm, Gp, Gp, Imm)
  ASMJIT_INST_4x(bltu, Inst::kIdBltu, Imm, Gp, Gp, Imm)
  ASMJIT_INST_4x(bne, Inst::kIdBne, Imm, Gp, Gp, Imm)
  ASMJIT_INST_0x(ebreak, Inst::kIdEbreak)
  ASMJIT_INST_0x(ecall, Inst::kIdEcall)
  ASMJIT_INST_5x(fence, Inst::kIdFence, Imm, Imm, Imm, Gp, Gp)
  ASMJIT_INST_2x(jal, Inst::kIdJal, Gp, Imm)
  ASMJIT_INST_3x(jalr, Inst::kIdJalr, Gp, Gp, Imm)
  ASMJIT_INST_3x(lb, Inst::kIdLb, Gp, Gp, Imm)
  ASMJIT_INST_3x(lbu, Inst::kIdLbu, Gp, Gp, Imm)
  ASMJIT_INST_3x(lh, Inst::kIdLh, Gp, Gp, Imm)
  ASMJIT_INST_3x(lhu, Inst::kIdLhu, Gp, Gp, Imm)
  ASMJIT_INST_2x(lui, Inst::kIdLui, Gp, Imm)
  ASMJIT_INST_3x(lw, Inst::kIdLw, Gp, Gp, Imm)
  ASMJIT_INST_3x(or_, Inst::kIdOr, Gp, Gp, Gp)
  ASMJIT_INST_3x(ori, Inst::kIdOri, Gp, Gp, Imm)
  ASMJIT_INST_4x(sb, Inst::kIdSb, Imm, Gp, Gp, Imm)
  ASMJIT_INST_4x(sh, Inst::kIdSh, Imm, Gp, Gp, Imm)
  ASMJIT_INST_3x(sll, Inst::kIdSll, Gp, Gp, Gp)
  ASMJIT_INST_3x(slt, Inst::kIdSlt, Gp, Gp, Gp)
  ASMJIT_INST_3x(slti, Inst::kIdSlti, Gp, Gp, Imm)
  ASMJIT_INST_3x(sltiu, Inst::kIdSltiu, Gp, Gp, Imm)
  ASMJIT_INST_3x(sltu, Inst::kIdSltu, Gp, Gp, Gp)
  ASMJIT_INST_3x(sra, Inst::kIdSra, Gp, Gp, Gp)
  ASMJIT_INST_3x(srl, Inst::kIdSrl, Gp, Gp, Gp)
  ASMJIT_INST_3x(sub, Inst::kIdSub, Gp, Gp, Gp)
  ASMJIT_INST_4x(sw, Inst::kIdSw, Imm, Gp, Gp, Imm)
  ASMJIT_INST_3x(xor_, Inst::kIdXor, Gp, Gp, Gp)
  ASMJIT_INST_3x(xori, Inst::kIdXori, Gp, Gp, Imm)
  // ----------------------------------------------------------------------------
  // ${riscv64::EmitterExplicit:End}
};

//! Emitter (RISC-V 64-bit).
//!
//! \note This class cannot be instantiated, you can only cast to it and use it as emitter that emits to either
//! `riscv64::Assembler`, `riscv64::Builder`, or `riscv64::Compiler` (use with caution with `riscv64::Compiler` as it requires
//! virtual registers).
class Emitter : public BaseEmitter, public EmitterExplicitT<Emitter> {
  ASMJIT_NONCONSTRUCTIBLE(Emitter)
};

ASMJIT_END_SUB_NAMESPACE

#endif // ASMJIT_RISCV_RISCV64EMITTER_H_INCLUDED
