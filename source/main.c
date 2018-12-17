#include "cpu.h"
#include "disassembler.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef DEBUG
FILE* logfile;
#endif

int main(int argc, char* argv[]) {
  if (argc < 2) {
    printf("Usage: %s [-d] <binary>\n\n", argv[0]);
    printf("Disassemble: %s -d <binary>\n", argv[0]);
    printf("  Writes disassembled code to <binary>.s\n");
    return EXIT_FAILURE;
  }

  CPU* cpu = malloc(sizeof(CPU));
  if (load(cpu, argv[argc - 1])) {
    printf("Error: reading binary failed\n");
    free(cpu);
    return EXIT_FAILURE;
  }

  if (strcmp(argv[1], "-d") == 0) {
    char* file_name = strdup(argv[argc - 1]);
    char* dot = index(file_name, '.');
    if (dot != NULL) {
      *(++dot) = 's';
      *(++dot) = '\0';
    } else {
      char* tmp = file_name;
      file_name = malloc(strlen(tmp) + 3);
      strncpy(file_name, tmp, strlen(tmp) + 1);
      free(tmp);
      strncat(file_name, ".s", 2);
    }
    disassemble(cpu, file_name);
    free(file_name);
    free(cpu);
    return EXIT_SUCCESS;
  }

#ifdef DEBUG
  logfile = fopen("synacorVM.log", "w+");
  if (logfile == NULL) {
    fprintf(stderr, "Could not open logfile (synacorVM.log)\n");
    abort();
  }
#endif

  while (execute(cpu, fetch(cpu)))
    ;

  free(cpu);
#ifdef DEBUG
  fclose(logfile);
#endif
  return EXIT_SUCCESS;
}
