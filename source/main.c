#include <stdio.h>
#include <stdlib.h>

#define MEMORY_SIZE (((1ULL) << 15) + 8)
#define STACK_SIZE ((1ULL) << 16)
#define resolve(x) ((x < (1ULL << 15)) ? x : cpu->memory[x])
#define CONTINUE 1;
#define

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
    return CONTINUE;
  case 1: // SET
    a = fetch(cpu);
    b = fetch(cpu);
    cpu->memory[a] = resolve(b);
    return CONTINUE;
  case 2: // PUSH
    a = fetch(cpu);
    cpu->stack[cpu->sp++] = resolve(a);
    return CONTINUE;
  case 3: // POP
    if (cpu->sp == 0) {
      printf("POP but stack is empty\n");
      abort();
    }
    a = fetch(cpu);
    cpu->memory[a] = cpu->stack[cpu->sp--];
  case 4: // EQ
    a = fetch(cpu);
    b = fetch(cpu);
    c = fetch(cpu);
    cpu->memory[a] = (resolve(b) == resolve(c)) ? 1 : 0;
    return CONTINUE;
  case 5: // GT
    a = fetch(cpu);
    b = fetch(cpu);
    c = fetch(cpu);
    cpu->memory[a] = (resolve(b) > resolve(c)) ? 1 : 0;
    return CONTINUE;
  case 6: // JMP
    a = fetch(cpu);
    cpu->pc = resolve(a);
    return CONTINUE;
  default:
    printf("Unsupported Opcode: %hu\n", opcode);
    abort();
  }
  return 0;
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

  while (execute(cpu, fetch(cpu)) == 0)
    ;

  free(cpu);
  return EXIT_SUCCESS;
}
