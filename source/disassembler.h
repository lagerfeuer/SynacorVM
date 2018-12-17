#ifndef SYNACOR_DISASM_H
#define SYNACOR_DISASM_H
#include "cpu.h"
#include "util.h"

#include <stdio.h>

#define REG_PREFIX(x) ((x < (1U << 15)) ? "" : "$")
#define DISASM_VAL(x) ((x < (1U << 15)) ? x : x - (1U << 15))

void disassemble(CPU* cpu, const char* file_name);

void resolve_opcode(FILE* asm_file, CPU* cpu, Word opcode);

#endif /* SYNACOR_DISASM_H */
