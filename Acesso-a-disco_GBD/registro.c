#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "registro.h"
#define FILE_SIZE 42949672960
#define QTD_REG 858993459

struct registro {
  int nseq;
  char text[46];
}

// uma chamada de seed apenas!
int srand((unsigned) time(&t));

FILE * CREATE_HEAP_FILE(long int nro_registros) {

}


reg READ_RANDOM(FILE *arq) {

  int pos_aleatorio = rand %

  int flag = fseek(arq, sizeof(struct registro), pos_aleatorio);
}

long int file_size(FILE *arq) {
  // Seek até o fim do arquivo
  fseek(arq, 0L, SEEK_END);
  long int sz = ftell(arq);
  // Devolve ponteiro
  fseek(fp, 0L, SEEK_SET);

  return sz;
}
