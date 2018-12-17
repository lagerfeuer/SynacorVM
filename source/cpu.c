#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FILE* logfile;

int load(CPU* cpu, char* file_name) {
  memset(cpu->memory, 0, MEMORY_SIZE);
  FILE* binary = fopen(file_name, "rb");
  if (binary == NULL)
    return EXIT_FAILURE;

  fseek(binary, 0, SEEK_END);
  long file_size = ftell(binary);
  if (file_size == -1)
    goto FAIL_READ;
  // MEMORY_SIZE is Word, file_size is Byte
  if (MEMORY_SIZE * 2 + 1 < ((unsigned long)file_size))
    goto FAIL_READ;
  rewind(binary);
  cpu->size = file_size / 2;

  int i = 0;
  int byte;
  unsigned int lo, hi;
  Word instruction;
  for (;;) {
    byte = fgetc(binary);
    if (byte == EOF)
      break;
    lo = byte;
    byte = fgetc(binary);
    if (byte == EOF)
      goto FAIL_READ;
    hi = byte;
    instruction = (hi << 8) | lo;
    cpu->memory[i++] = instruction;
  }

  fclose(binary);
  return EXIT_SUCCESS;

FAIL_READ:
  fclose(binary);
  return EXIT_FAILURE;
}

Word fetch(CPU* cpu) { return cpu->memory[cpu->pc++]; };

unsigned int execute(CPU* cpu, Word opcode) {
#ifdef DEBUG
  fprintf(logfile, "[*] Executing: %hu (PC: %u)\n", opcode, cpu->pc - 1);
#endif
  Word a, b, c;
  switch (opcode) {
  case 0: // HALT
    return HALT;
  case 1: // SET
    a = fetch(cpu);
    b = fetch(cpu);
    cpu->memory[a] = resolve(b);
    break;
  case 2: // PUSH
    a = fetch(cpu);
    cpu->stack[cpu->sp++] = resolve(a);
    break;
  case 3: // POP
    if (cpu->sp == 0) {
      printf("POP but stack is empty\n");
      abort();
    }
    a = fetch(cpu);
    cpu->memory[a] = cpu->stack[--cpu->sp];
    break;
  case 4: // EQ
    a = fetch(cpu);
    b = fetch(cpu);
    c = fetch(cpu);
    cpu->memory[a] = (resolve(b) == resolve(c)) ? 1 : 0;
    break;
  case 5: // GT
    a = fetch(cpu);
    b = fetch(cpu);
    c = fetch(cpu);
    cpu->memory[a] = (resolve(b) > resolve(c)) ? 1 : 0;
    break;
  case 6: // JMP
    a = fetch(cpu);
    cpu->pc = resolve(a);
    break;
  case 7: // JT
    a = fetch(cpu);
    b = fetch(cpu);
    if (resolve(a))
      cpu->pc = resolve(b);
    break;
  case 8: // JF
    a = fetch(cpu);
    b = fetch(cpu);
    if (!resolve(a))
      cpu->pc = resolve(b);
    break;
  case 9: // ADD
    a = fetch(cpu);
    b = fetch(cpu);
    c = fetch(cpu);
    cpu->memory[a] = (resolve(b) + resolve(c)) % MOD_VAL;
    break;
  case 10: // MULT
    a = fetch(cpu);
    b = fetch(cpu);
    c = fetch(cpu);
    cpu->memory[a] = (resolve(b) * resolve(c)) % MOD_VAL;
    break;
  case 11: // MOD
    a = fetch(cpu);
    b = fetch(cpu);
    c = fetch(cpu);
    cpu->memory[a] = resolve(b) % resolve(c);
    break;
  case 12: // AND
    a = fetch(cpu);
    b = fetch(cpu);
    c = fetch(cpu);
    cpu->memory[a] = resolve(b) & resolve(c);
    break;
  case 13: // OR
    a = fetch(cpu);
    b = fetch(cpu);
    c = fetch(cpu);
    cpu->memory[a] = resolve(b) | resolve(c);
    break;
  case 14: // NOT
    a = fetch(cpu);
    b = fetch(cpu);
    cpu->memory[a] = (~resolve(b)) & 0x7FFF;
    break;
  case 15: // READ
    a = fetch(cpu);
    b = fetch(cpu);
    cpu->memory[a] = cpu->memory[resolve(b)];
    break;
  case 16: // WRITE
    a = fetch(cpu);
    b = fetch(cpu);
    cpu->memory[resolve(a)] = resolve(b);
    break;
  case 17: // CALL
    a = fetch(cpu);
    cpu->stack[cpu->sp++] = cpu->pc;
    cpu->pc = resolve(a);
    break;
  case 18: // RET
    if (cpu->sp == 0)
      return HALT;
    cpu->pc = cpu->stack[--cpu->sp];
    break;
  case 19: // OUT
    a = fetch(cpu);
    printf("%c", resolve(a));
    break;
  case 20: // IN
    a = fetch(cpu);
    int val = getchar();
    if (val == EOF)
      return HALT;
    cpu->memory[a] = val;
    break;
  case 21: // NOOP
    break;
  default:
    printf("Unsupported Opcode: %hu\n", opcode);
    abort();
  }
  return CONTINUE;
}
