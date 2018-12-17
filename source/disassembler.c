#include "disassembler.h"

#include <stdbool.h>
#include <stdio.h>

void disassemble(CPU* cpu, const char* file_name) {
  FILE* asm_file = fopen(file_name, "w+");
  /* bool halt = false; */
  while (cpu->pc < cpu->size) {
    Byte opcode = fetch(cpu);

    /* if (halt && opcode == 0) */
    /*   break; */
    /* if (!halt && opcode == 0) */
    /*   halt = true; */

    resolve_opcode(asm_file, cpu, opcode);
  }
  fclose(asm_file);
}

// TODO create labels PRINT{1,2,3} and instead of fprintf everywhere,
// just put goto's to these labels, since all the variables (pc,a,b,c) are in
// scope, PLUS another variable for the name of the opcode
void resolve_opcode(FILE* asm_file, CPU* cpu, Word opcode) {
  Word a, b, c;
  unsigned int pc = cpu->pc;
  switch (opcode) {
  case 0: // HALT
    fprintf(asm_file, "%05x: %s\n", pc, "halt");
    break;
  case 1: // SET
    a = fetch(cpu);
    b = fetch(cpu);
    fprintf(asm_file, "%05x: %s %u %s%u\n", pc, "set ", a, REG_PREFIX(b),
            DISASM_VAL(b));
    break;
  case 2: // PUSH
    a = fetch(cpu);
    cpu->stack[cpu->sp++] = resolve(a);
    fprintf(asm_file, "%05x: %s %s%u\n", pc, "push", REG_PREFIX(a),
            DISASM_VAL(a));
    break;
  case 3: // POP
    a = fetch(cpu);
    fprintf(asm_file, "%05x: %s %s%u\n", pc, "pop ", REG_PREFIX(a),
            DISASM_VAL(a));
    break;
  case 4: // EQ
    a = fetch(cpu);
    b = fetch(cpu);
    c = fetch(cpu);
    fprintf(asm_file, "%05x: %s %s%u %s%u %s%u\n", pc, "eq  ", REG_PREFIX(a),
            DISASM_VAL(a), REG_PREFIX(b), DISASM_VAL(b), REG_PREFIX(c),
            DISASM_VAL(c));
    break;
  case 5: // GT
    a = fetch(cpu);
    b = fetch(cpu);
    c = fetch(cpu);
    fprintf(asm_file, "%05x: %s %s%u %s%u %s%u\n", pc, "gt  ", REG_PREFIX(a),
            DISASM_VAL(a), REG_PREFIX(b), DISASM_VAL(b), REG_PREFIX(c),
            DISASM_VAL(c));
    break;
  case 6: // JMP
    a = fetch(cpu);
    fprintf(asm_file, "%05x: %s %s%u\n", pc, "jmp ", REG_PREFIX(a),
            DISASM_VAL(a));
    break;
  case 7: // JT
    a = fetch(cpu);
    b = fetch(cpu);
    fprintf(asm_file, "%05x: %s %s%u %s%u\n", pc, "jt  ", REG_PREFIX(a),
            DISASM_VAL(a), REG_PREFIX(b), DISASM_VAL(b));
    break;
  case 8: // JF
    a = fetch(cpu);
    b = fetch(cpu);
    fprintf(asm_file, "%05x: %s %s%u %s%u\n", pc, "jt  ", REG_PREFIX(a),
            DISASM_VAL(a), REG_PREFIX(b), DISASM_VAL(b));
    break;
  case 9: // ADD
    a = fetch(cpu);
    b = fetch(cpu);
    c = fetch(cpu);
    fprintf(asm_file, "%05x: %s %s%u %s%u %s%u\n", pc, "add ", REG_PREFIX(a),
            DISASM_VAL(a), REG_PREFIX(b), DISASM_VAL(b), REG_PREFIX(c),
            DISASM_VAL(c));
    break;
  case 10: // MULT
    a = fetch(cpu);
    b = fetch(cpu);
    c = fetch(cpu);
    fprintf(asm_file, "%05x: %s %s%u %s%u %s%u\n", pc, "mult", REG_PREFIX(a),
            DISASM_VAL(a), REG_PREFIX(b), DISASM_VAL(b), REG_PREFIX(c),
            DISASM_VAL(c));
    break;
  case 11: // MOD
    a = fetch(cpu);
    b = fetch(cpu);
    c = fetch(cpu);
    fprintf(asm_file, "%05x: %s %s%u %s%u %s%u\n", pc, "mod ", REG_PREFIX(a),
            DISASM_VAL(a), REG_PREFIX(b), DISASM_VAL(b), REG_PREFIX(c),
            DISASM_VAL(c));
    break;
  case 12: // AND
    a = fetch(cpu);
    b = fetch(cpu);
    c = fetch(cpu);
    fprintf(asm_file, "%05x: %s %s%u %s%u %s%u\n", pc, "and ", REG_PREFIX(a),
            DISASM_VAL(a), REG_PREFIX(b), DISASM_VAL(b), REG_PREFIX(c),
            DISASM_VAL(c));
    break;
  case 13: // OR
    a = fetch(cpu);
    b = fetch(cpu);
    c = fetch(cpu);
    fprintf(asm_file, "%05x: %s %s%u %s%u %s%u\n", pc, "or  ", REG_PREFIX(a),
            DISASM_VAL(a), REG_PREFIX(b), DISASM_VAL(b), REG_PREFIX(c),
            DISASM_VAL(c));
    break;
  case 14: // NOT
    a = fetch(cpu);
    b = fetch(cpu);
    fprintf(asm_file, "%05x: %s %s%u %s%u\n", pc, "not ", REG_PREFIX(a),
            DISASM_VAL(a), REG_PREFIX(b), DISASM_VAL(b));
    break;
  case 15: // READ
    a = fetch(cpu);
    b = fetch(cpu);
    fprintf(asm_file, "%05x: %s %s%u %s%u\n", pc, "rmem", REG_PREFIX(a),
            DISASM_VAL(a), REG_PREFIX(b), DISASM_VAL(b));
    break;
  case 16: // WRITE
    a = fetch(cpu);
    b = fetch(cpu);
    fprintf(asm_file, "%05x: %s %s%u %s%u\n", pc, "wmem", REG_PREFIX(a),
            DISASM_VAL(a), REG_PREFIX(b), DISASM_VAL(b));
    break;
  case 17: // CALL
    a = fetch(cpu);
    fprintf(asm_file, "%05x: %s %s%u\n", pc, "call", REG_PREFIX(a),
            DISASM_VAL(a));
    break;
  case 18: // RET
    a = fetch(cpu);
    fprintf(asm_file, "%05x: %s %s%u\n", pc, "call", REG_PREFIX(a),
            DISASM_VAL(a));
    break;
  case 19: // OUT
    a = fetch(cpu);
    fprintf(asm_file, "%05x: %s %s%u\t; %c\n", pc, "out ", REG_PREFIX(a),
            DISASM_VAL(a), (a < (1U << 15)) ? resolve(a) : ' ');
    break;
  case 20: // IN
    a = fetch(cpu);
    fprintf(asm_file, "%05x: %s %s%u\n", pc, "in  ", REG_PREFIX(a),
            DISASM_VAL(a));
    break;
  case 21: // NOOP
    fprintf(asm_file, "%05x: %s\n", pc, "noop");
    break;
  default:
    fprintf(asm_file, "%05x: dw %u\n", pc, opcode);
    break;
  }
}
