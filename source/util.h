#ifndef SYNACOR_UTIL_H
#define SYNACOR_UTIL_H

#define MEMORY_SIZE (((1ULL) << 15) + 8)
#define STACK_SIZE ((1ULL) << 16)
#define resolve(x) ((x < (1ULL << 15)) ? x : cpu->memory[x])
#define HALT 0
#define CONTINUE 1
#define MOD_VAL 32768

typedef unsigned char Byte;
typedef unsigned short Word;

typedef struct _CPU_ {
  Word memory[MEMORY_SIZE];
  Word stack[STACK_SIZE];
  unsigned int pc;
  unsigned int sp;
} CPU;

#endif /* SYNACOR_UTIL_H */
