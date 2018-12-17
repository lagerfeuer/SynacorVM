#ifndef SYNACOR_CPU_H
#define SYNACOR_CPU_H
#include "util.h"

#include <stdlib.h>

typedef struct _CPU_ {
  Word memory[MEMORY_SIZE];
  Word stack[STACK_SIZE];
  unsigned int pc;
  unsigned int sp;
  unsigned int size;
} CPU;

int load(CPU* cpu, char* file_name);

Word fetch(CPU* cpu);

unsigned int execute(CPU* cpu, Word opcode);

#endif /* SYNACOR_CPU_H */
