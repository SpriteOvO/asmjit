// This file is part of AsmJit project <https://asmjit.com>
//
// See asmjit.h or LICENSE.md for license and copyright information
// SPDX-License-Identifier: Zlib

#include "../core/api-build_p.h"
#if !defined(ASMJIT_NO_RISCV64)

#include "../core/codewriter_p.h"
#include "../core/cpuinfo.h"
#include "../core/emitterutils_p.h"
#include "../core/formatter.h"
#include "../core/logger.h"
#include "../core/misc_p.h"
#include "../core/support.h"
#include "../riscv/riscv64assembler.h"

ASMJIT_BEGIN_SUB_NAMESPACE(riscv64)

// TODO: [RISC-V] consider use the public one if any
enum EncodingType {
  R, I, U, B, J, S,
  // special cases
  I_ebreak, I_ecall, _fence
};

struct InstDispatchRecord {
  EncodingType encodingType;
  uint16_t index;
};

static Error ASMJIT_CDECL Emitter_emitProlog(BaseEmitter* emitter, const FuncFrame& frame) {
  return DebugUtils::errored(kErrorInvalidState);
}

static Error ASMJIT_CDECL Emitter_emitEpilog(BaseEmitter* emitter, const FuncFrame& frame) {
  return DebugUtils::errored(kErrorInvalidState);
}

static Error ASMJIT_CDECL Emitter_emitArgsAssignment(BaseEmitter* emitter, const FuncFrame& frame, const FuncArgsAssignment& args) {
  return DebugUtils::errored(kErrorInvalidState);
}

// riscv64::Assembler - Signature Checker
// ======================================

struct SignatureChecker {
  uint64_t _0to5;

  ASMJIT_FORCE_INLINE void init(const Operand_& o0, const Operand_& o1, const Operand_& o2, const Operand_& o3, const Operand_& o4, const Operand_& o5) noexcept {
    _0to5 = (uint64_t(o0._signature._bits & 0xFFu)      ) |
            (uint64_t(o1._signature._bits & 0xFFu) <<  8) |
            (uint64_t(o2._signature._bits & 0xFFu) << 16) |
            (uint64_t(o3._signature._bits & 0xFFu) << 24) |
            (uint64_t(o4._signature._bits & 0xFFu) << 32) |
            (uint64_t(o5._signature._bits & 0xFFu) << 40) ;
  }

  ASMJIT_FORCE_INLINE bool empty() const noexcept { return _0to5 == 0u; }

  template<uint32_t o0>
  ASMJIT_FORCE_INLINE bool test() const noexcept { return _0to5 == o0; }

  template<uint32_t o0, uint32_t o1>
  ASMJIT_FORCE_INLINE bool test() const noexcept { return _0to5 == (o0 | (o1 << 8u)); }

  template<uint32_t o0, uint32_t o1, uint32_t o2>
  ASMJIT_FORCE_INLINE bool test() const noexcept { return _0to5 == (o0 | (o1 << 8u) | (o2 << 16u)); }

  template<uint32_t o0, uint32_t o1, uint32_t o2, uint32_t o3>
  ASMJIT_FORCE_INLINE bool test() const noexcept { return _0to5 == (o0 | (o1 << 8u) | (o2 << 16u) | (o3 << 24u)); }

  template<uint32_t o0, uint32_t o1, uint32_t o2, uint32_t o3, uint32_t o4>
  ASMJIT_FORCE_INLINE bool test() const noexcept { return _0to5 == (o0 | (o1 << 8u) | (o2 << 16u) | (o3 << 24u) | (uint64_t(o4) << 32u)); }

  template<uint32_t o0, uint32_t o1, uint32_t o2, uint32_t o3, uint32_t o4, uint32_t o5>
  ASMJIT_FORCE_INLINE bool test() const noexcept { return _0to5 == (o0 | (o1 << 8u) | (o2 << 16u) | (o3 << 24u) | (uint64_t(o4) << 32u) | (uint64_t(o5) << 40u)); }
};

// ${riscv64::Assembler::Dispatch:Begin}
// ------------------- Automatically generated, do not edit -------------------
static const InstDispatchRecord instDispatchTable[] = {
  {EncodingType::R, 0},
  {EncodingType::I, 0},
  {EncodingType::R, 1},
  {EncodingType::I, 1},
  {EncodingType::U, 0},
  {EncodingType::B, 0},
  {EncodingType::B, 1},
  {EncodingType::B, 2},
  {EncodingType::B, 3},
  {EncodingType::B, 4},
  {EncodingType::B, 5},
  {EncodingType::I_ebreak, 0},
  {EncodingType::I_ecall, 0},
  {EncodingType::_fence, 0},
  {EncodingType::J, 0},
  {EncodingType::I, 2},
  {EncodingType::I, 3},
  {EncodingType::I, 4},
  {EncodingType::I, 5},
  {EncodingType::I, 6},
  {EncodingType::U, 1},
  {EncodingType::I, 7},
  {EncodingType::R, 2},
  {EncodingType::I, 8},
  {EncodingType::S, 0},
  {EncodingType::S, 1},
  {EncodingType::R, 3},
  {EncodingType::R, 4},
  {EncodingType::I, 9},
  {EncodingType::I, 10},
  {EncodingType::R, 5},
  {EncodingType::R, 6},
  {EncodingType::R, 7},
  {EncodingType::R, 8},
  {EncodingType::S, 2},
  {EncodingType::R, 9},
  {EncodingType::I, 11}
};
// ----------------------------------------------------------------------------
// ${riscv64::Assembler::Dispatch:End}

// TODO: [RISC-V]

// riscv64::Assembler - Construction & Destruction
// ===========================================

Assembler::Assembler(CodeHolder* code) noexcept : BaseAssembler() {
  _archMask = (uint64_t(1) << uint32_t(Arch::kRISCV64));
  _funcs.emitProlog = Emitter_emitProlog;
  _funcs.emitEpilog = Emitter_emitEpilog;
  _funcs.emitArgsAssignment = Emitter_emitArgsAssignment;

#ifndef ASMJIT_NO_LOGGING
  // TODO: [RISC-V]
  // _funcs.formatInstruction = FormatterInternal::formatInstruction;
#endif

#ifndef ASMJIT_NO_VALIDATION
  // TODO: [RISC-V]
  // _funcs.validate = InstInternal::validate;
#endif

  if (code)
    code->attach(this);
};

Assembler::~Assembler() noexcept {}

// riscv64::Assembler - Emit
// =====================

Error Assembler::_emit(InstId instId, const Operand_& o0, const Operand_& o1, const Operand_& o2, const Operand_* opExt) {
  // Logging/Validation/Error.
  constexpr InstOptions kRequiresSpecialHandling = InstOptions::kReserved;

  constexpr uint32_t kOpRegR = uint32_t(OperandType::kReg) | (uint32_t(RegType::kGp64) << 3);
  constexpr uint32_t kOpImmI = uint32_t(OperandType::kImm) | (uint32_t(ImmType::kInt ) << 3);

  // TODO: [RISC-V]

  Error err;
  CodeWriter writer(this);

  // TODO: [RISC-V]

  if (instId >= Inst::_kIdCount)
    instId = 0;

  uint32_t encoded;

  // TODO: [RISC-V]

  const Operand_& o3 = opExt[EmitterUtils::kOp3];
  const Operand_& o4 = opExt[EmitterUtils::kOp4];
  const Operand_& o5 = opExt[EmitterUtils::kOp5];

  // TODO: [RISC-V]

  SignatureChecker sgn;

  // TODO: [RISC-V]

  // Combine all instruction options and also check whether the instruction
  // is valid. All options that require special handling (including invalid
  // instruction) are handled by the next branch.
  InstOptions options = InstOptions(instId == 0) | InstOptions((size_t)(_bufferEnd - writer.cursor()) < 4) | instOptions() | forcedInstOptions();
  InstDispatchRecord idr;

  // Combined signatures of input operands for quick checks.
  sgn.init(o0, o1, o2, o3, o4, o5);

  if (ASMJIT_UNLIKELY(Support::test(options, kRequiresSpecialHandling))) {
    if (ASMJIT_UNLIKELY(!_code))
      return reportError(DebugUtils::errored(kErrorNotInitialized));

    // Unknown instruction.
    if (ASMJIT_UNLIKELY(instId == 0))
      goto InvalidInstruction;

    // Grow request, happens rarely.
    err = writer.ensureSpace(this, 4);
    if (ASMJIT_UNLIKELY(err))
      goto Emit_Failed;
  }

  // ${riscv64::Assembler::Impl:Begin}
  // ------------------- Automatically generated, do not edit -------------------
  idr = instDispatchTable[instId];
  switch (idr.encodingType) {
    case EncodingType::R: {
      // Group of 'add|and|or|sll|slt|sltu|sra|srl|sub|xor'.
      static const uint32_t matchTable[] = {
        0x00000033u, // Instruction 'add'.
        0x00007033u, // Instruction 'and'.
        0x00006033u, // Instruction 'or'.
        0x00001033u, // Instruction 'sll'.
        0x00002033u, // Instruction 'slt'.
        0x00003033u, // Instruction 'sltu'.
        0x40005033u, // Instruction 'sra'.
        0x00005033u, // Instruction 'srl'.
        0x40000033u, // Instruction 'sub'.
        0x00004033u  // Instruction 'xor'.
      };
      const uint32_t mask = 0xFE00707Fu;
      const uint32_t match = matchTable[idr.index];

      if (sgn.test<kOpRegR, kOpRegR, kOpRegR>()) {
        encoded = match;
        goto Emit_R;
      }

      break;
    }

    case EncodingType::I: {
      // Group of 'addi|andi|jalr|lb|lbu|lh|lhu|lw|ori|slti|sltiu|xori'.
      static const uint32_t matchTable[] = {
        0x00000013u, // Instruction 'addi'.
        0x00007013u, // Instruction 'andi'.
        0x00000067u, // Instruction 'jalr'.
        0x00000003u, // Instruction 'lb'.
        0x00004003u, // Instruction 'lbu'.
        0x00001003u, // Instruction 'lh'.
        0x00005003u, // Instruction 'lhu'.
        0x00002003u, // Instruction 'lw'.
        0x00006013u, // Instruction 'ori'.
        0x00002013u, // Instruction 'slti'.
        0x00003013u, // Instruction 'sltiu'.
        0x00004013u  // Instruction 'xori'.
      };
      const uint32_t mask = 0x0000707Fu;
      const uint32_t match = matchTable[idr.index];

      if (sgn.test<kOpRegR, kOpRegR, kOpImmI>()) {
        encoded = match;
        goto Emit_I;
      }

      break;
    }

    case EncodingType::U: {
      // Group of 'auipc|lui'.
      static const uint32_t matchTable[] = {
        0x00000017u, // Instruction 'auipc'.
        0x00000037u  // Instruction 'lui'.
      };
      const uint32_t mask = 0x0000007Fu;
      const uint32_t match = matchTable[idr.index];

      if (sgn.test<kOpRegR, kOpImmI>()) {
        encoded = match;
        goto Emit_U;
      }

      break;
    }

    case EncodingType::B: {
      // Group of 'beq|bge|bgeu|blt|bltu|bne'.
      static const uint32_t matchTable[] = {
        0x00000063u, // Instruction 'beq'.
        0x00005063u, // Instruction 'bge'.
        0x00007063u, // Instruction 'bgeu'.
        0x00004063u, // Instruction 'blt'.
        0x00006063u, // Instruction 'bltu'.
        0x00001063u  // Instruction 'bne'.
      };
      const uint32_t mask = 0x0000707Fu;
      const uint32_t match = matchTable[idr.index];

      if (sgn.test<kOpImmI, kOpRegR, kOpRegR, kOpImmI>()) {
        encoded = match;
        goto Emit_B;
      }

      break;
    }

    case EncodingType::I_ebreak: {
      // Group of 'ebreak'.
      const uint32_t mask = 0xFFFFFFFFu;
      const uint32_t match = 0x00100073u;

      if (sgn.empty()) {
        encoded = match;
        goto Emit_I_ebreak;
      }

      break;
    }

    case EncodingType::I_ecall: {
      // Group of 'ecall'.
      const uint32_t mask = 0xFFFFFFFFu;
      const uint32_t match = 0x00000073u;

      if (sgn.empty()) {
        encoded = match;
        goto Emit_I_ecall;
      }

      break;
    }

    case EncodingType::_fence: {
      // Group of 'fence'.
      const uint32_t mask = 0x0000707Fu;
      const uint32_t match = 0x0000000Fu;

      if (sgn.test<kOpImmI, kOpImmI, kOpImmI, kOpRegR, kOpRegR>()) {
        encoded = match;
        goto Emit__fence;
      }

      break;
    }

    case EncodingType::J: {
      // Group of 'jal'.
      const uint32_t mask = 0x0000007Fu;
      const uint32_t match = 0x0000006Fu;

      if (sgn.test<kOpRegR, kOpImmI>()) {
        encoded = match;
        goto Emit_J;
      }

      break;
    }

    case EncodingType::S: {
      // Group of 'sb|sh|sw'.
      static const uint32_t matchTable[] = {
        0x00000023u, // Instruction 'sb'.
        0x00001023u, // Instruction 'sh'.
        0x00002023u  // Instruction 'sw'.
      };
      const uint32_t mask = 0x0000707Fu;
      const uint32_t match = matchTable[idr.index];

      if (sgn.test<kOpImmI, kOpRegR, kOpRegR, kOpImmI>()) {
        encoded = match;
        goto Emit_S;
      }

      break;
    }

    default: {
      break;
    }
  }

  goto InvalidInstruction;

  Emit_R: {
    uint32_t rd = o0.as<Reg>().id();
    uint32_t rs1 = o1.as<Reg>().id();
    uint32_t rs2 = o2.as<Reg>().id();
    encoded |= (rd & 0x1Fu) << 7u;
    encoded |= (rs1 & 0x1Fu) << 15u;
    encoded |= (rs2 & 0x1Fu) << 20u;
    goto Emit_Op32;
  }

  Emit_I: {
    uint32_t rd = o0.as<Reg>().id();
    uint32_t rs1 = o1.as<Reg>().id();
    uint32_t imm = o2.as<Imm>().valueAs<uint32_t>();
    encoded |= (rd & 0x1Fu) << 7u;
    encoded |= (rs1 & 0x1Fu) << 15u;
    encoded |= (imm & 0xFFFu) << 20u;
    goto Emit_Op32;
  }

  Emit_U: {
    uint32_t rd = o0.as<Reg>().id();
    uint32_t imm = o1.as<Imm>().valueAs<uint32_t>();
    encoded |= (rd & 0x1Fu) << 7u;
    encoded |= imm & 0xFFFFF000u;
    goto Emit_Op32;
  }

  Emit_B: {
    uint32_t rs1 = o0.as<Reg>().id();
    uint32_t rs2 = o1.as<Reg>().id();
    uint32_t imm = o2.as<Imm>().valueAs<uint32_t>();
    encoded |= ((imm >> 11u) & 1u) << 7u;
    encoded |= ((imm >> 1u) & 0xFu) << 8u;
    encoded |= (rs1 & 0x1Fu) << 15u;
    encoded |= (rs2 & 0x1Fu) << 20u;
    encoded |= ((imm >> 5u) & 0x3Fu) << 25u;
    encoded |= ((imm >> 12u) & 1u) << 31u;
    goto Emit_Op32;
  }

  Emit_I_ebreak: {
    goto Emit_Op32;
  }

  Emit_I_ecall: {
    goto Emit_Op32;
  }

  Emit__fence: {
    uint32_t rd = o0.as<Reg>().id();
    uint32_t rs1 = o1.as<Reg>().id();
    goto Emit_Op32;
  }

  Emit_J: {
    uint32_t rd = o0.as<Reg>().id();
    uint32_t imm = o1.as<Imm>().valueAs<uint32_t>();
    encoded |= (rd & 0x1Fu) << 7u;
    encoded |= ((imm >> 12u) & 0xFFu) << 12u;
    encoded |= ((imm >> 11u) & 1u) << 20u;
    encoded |= ((imm >> 1u) & 0x3FFu) << 21u;
    encoded |= ((imm >> 20u) & 1u) << 31u;
    goto Emit_Op32;
  }

  Emit_S: {
    uint32_t rs1 = o0.as<Reg>().id();
    uint32_t imm = o1.as<Imm>().valueAs<uint32_t>();
    uint32_t rs2 = o2.as<Reg>().id();
    encoded |= (imm & 0x1Fu) << 7u;
    encoded |= (rs1 & 0x1Fu) << 15u;
    encoded |= (rs2 & 0x1Fu) << 20u;
    encoded |= ((imm >> 5u) & 0x7Fu) << 7u;
    goto Emit_Op32;
  }
  // ----------------------------------------------------------------------------
  // ${riscv64::Assembler::Impl:End}

  // Emit - Opcode
  // -------------

  Emit_Op32: {
    writer.emit32uLE(encoded);
    goto EmitDone;
  }

  // Emit - Success
  // --------------

  EmitDone: {
    if (Support::test(options, InstOptions::kReserved)) {
#ifndef ASMJIT_NO_LOGGING
      if (_logger)
        EmitterUtils::logInstructionEmitted(this, instId, options, o0, o1, o2, opExt, 0, 0, writer.cursor());
#endif
    }

    resetExtraReg();
    resetInstOptions();
    resetInlineComment();

    writer.done(this);
    return kErrorOk;
  }

  // Emit - Failure
  // --------------

  #define ERROR_HANDLER(ERR) ERR: err = DebugUtils::errored(kError##ERR); goto Emit_Failed;

  ERROR_HANDLER(OutOfMemory)
  ERROR_HANDLER(InvalidAddress)
  ERROR_HANDLER(InvalidAddressScale)
  ERROR_HANDLER(InvalidDisplacement)
  ERROR_HANDLER(InvalidElementIndex)
  ERROR_HANDLER(InvalidLabel)
  ERROR_HANDLER(InvalidImmediate)
  ERROR_HANDLER(InvalidInstruction)
  ERROR_HANDLER(InvalidPhysId)
  ERROR_HANDLER(InvalidRegType)

  #undef ERROR_HANDLER

  Emit_Failed: {
#ifndef ASMJIT_NO_LOGGING
    return EmitterUtils::logInstructionFailed(this, err, instId, options, o0, o1, o2, opExt);
#else
    resetExtraReg();
    resetInstOptions();
    resetInlineComment();
    return reportError(err);
#endif
  }
}

// riscv64::Assembler - Align
// ======================

Error Assembler::align(AlignMode alignMode, uint32_t alignment) {
  // TODO: [RISC-V]
}

// riscv64::Assembler - Events
// =======================

Error Assembler::onAttach(CodeHolder* code) noexcept {
  return Base::onAttach(code);
}

Error Assembler::onDetach(CodeHolder* code) noexcept {
  return Base::onDetach(code);
}

ASMJIT_END_SUB_NAMESPACE

#endif // !ASMJIT_NO_RISCV64
