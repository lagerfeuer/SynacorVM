#include "cpu.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG
FILE* logfile;
#endif

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
