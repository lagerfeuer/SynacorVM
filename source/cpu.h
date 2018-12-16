#ifndef SYNACOR_CPU_H
#define SYNACOR_CPU_H
#include "util.h"

int load(CPU* cpu, char* file_name);

Word fetch(CPU* cpu);

unsigned int execute(CPU* cpu, Word opcode);

#endif /* SYNACOR_CPU_H */
