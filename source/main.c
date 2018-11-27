#include <stdio.h>
#include <stdlib.h>

#define MEMORY_SIZE (((1ULL) << 15) + 8)
#define STACK_SIZE ((1ULL) << 16)
#define resolve(x) ((x < (1ULL << 15)) ? x : cpu->memory[x])
#define HALT 0
#define CONTINUE 1
#define MOD_VAL 32768

typedef unsigned char Byte;
typedef unsigned short Word;

typedef struct {
  Word memory[MEMORY_SIZE];
  Word stack[STACK_SIZE];
  unsigned int pc;
  unsigned int sp;
} CPU;

int load(CPU* cpu, char* file_name) {
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
    cpu->memory[a] = cpu->stack[cpu->sp--];
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
    if (resolve(b))
      cpu->pc = resolve(a);
    break;
  case 8: // JT
    a = fetch(cpu);
    b = fetch(cpu);
    if (!resolve(b))
      cpu->pc = resolve(a);
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
    cpu->memory[a] = ~resolve(b);
    break;
  case 15: // READ
    a = fetch(cpu);
    b = fetch(cpu);
    cpu->memory[a] = resolve(b);
    break;
  case 16: // WRITE
    a = fetch(cpu);
    b = fetch(cpu);
    cpu->memory[a] = b;
    break;
  case 17: // CALL
    a = fetch(cpu);
    cpu->stack[cpu->sp++] = cpu->pc;
    cpu->pc = a;
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

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage: %s <binary>\n", argv[0]);
    return EXIT_FAILURE;
  }

  CPU* cpu = malloc(sizeof(CPU));
  if (load(cpu, argv[1])) {
    printf("Error: reading binary failed\n");
    free(cpu);
    return EXIT_FAILURE;
  }

  while (execute(cpu, fetch(cpu)))
    ;

  free(cpu);
  return EXIT_SUCCESS;
}
