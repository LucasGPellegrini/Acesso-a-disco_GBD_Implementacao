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
  // Abre arquivo (escrita)
  FILE * fp;
  fp = fopen ("arquivo.txt", "w");
  if(fp == NULL) {
    return NULL;
  }

  reg registro;
  for(int i = 0; i < nro_registros; i++) {
    // Preenche registro
    reg.nseq = i;
    reg.text = "FUNCAO_MANEIRA";

    // Insere registro no arquivo
    fwrite(&reg, sizeof(struct registro), 1, fp);
  }

  return fp;
}


reg READ_RANDOM(FILE *arq, int nseq) {
  // Teste 0 <= NSEQ <= nro_registros
  if(nsek > ultimo_nseq(arq) || nsek < 0) {
    printf("NSEQ Invalido\n");
    return NULL;
  }

  // Seta o Stream para uma posição nseq
  int offset = nseq * sizeof(struct registro)
  offset -= sizeof(struct registro);
  int flag = fseek(arq, offset, SEEK_SET);

  // le o registro
  reg registro;
  fread(&reg, sizeof(struct registro), 1, arq);

  // Printa o registro
  printf("NSEQ: %d\n TEXT: %s\n\n", reg.nseq, reg.text);
  return reg
}

FILE * ISRT_AT_END(FILE *arq) {
  // Pega NSEQ do ultimo
  int ultimo = ultimo_nseq(arq);

  // Seta Stream para final do arquivo
  fseek(arq, 0L, SEEK_END);

  // Escreve o registro
  reg registro;
  reg.nseq = ultimo + 1;
  reg.text = "FUNCAO_MANEIRA";
  fwrite(&reg, sizeof(struct registro), 1, arq);

  return arq;
}

FILE * UPDATE_RANDOM(FILE *arq, int nseq, int novo_texto) {
  // Teste 0 <= NSEQ <= nro_registros
  if(nsek > ultimo_nseq(arq) || nsek < 0) {
    printf("NSEQ Invalido\n");
    return arq;
  }

  // Seta o Stream para uma posição nseq
  int offset = nseq * sizeof(struct registro)
  int flag = fseek(arq, offset, SEEK_SET);

  // sobrescreve o registro
  reg registro;
  reg.nseq = nseq;
  reg.text = novo_texto;
  fwrite(&reg, sizeof(struct registro), 1, arq);

  return arq;
}

FILE * DELETE_RANDOM(FILE *arq, int nseq) {
  // Teste 0 <= NSEQ <= nro_registros
  if(nsek > ultimo_nseq(arq) || nsek < 0) {
    printf("NSEQ Invalido\n");
    return arq;
  }

  // Seta o Stream para uma posição nseq
  int offset = nseq * sizeof(struct registro)
  int flag = fseek(arq, offset, SEEK_SET);

  // até o fim do arquivo:
    // Pega o prox registro,
    // Subtrai 1 do nseq
    // Guarda na posicao atual
    // Avanca 1 registro
  // LEIO -> VOLTO -> SOBRESCREVO -> AVANÇO (LER e ESCREVER avanca o Stream)
  reg registro;
  while(!feof(arq)) {
    fread(&reg, sizeof(struct registro), 1, arq);
    reg.nseq -= 1;

    offset -= sizeof(struct registro);
    fseek(arq, offset, SEEK_SET);

    fwrite(&reg, sizeof(struct registro), 1, arq);

    offset += 2*(sizeof(struct registro));
  }

  // truncate p/ remover os bytes remanescentes
  long int new_length = file_size(arq) - sizeof(struct registro);
  ftruncate(fileno(arq), new_length);

  return arq;
}

long int file_size(FILE *arq) {
  // Seek até o fim do arquivo
  fseek(arq, 0L, SEEK_END);
  long int sz = ftell(arq);

  // Devolve ponteiro
  fseek(fp, 0L, SEEK_SET);

  return sz;
}

int ultimo_nseq(FILE *arq) {
  // O ultimo nseq = ceil(tamanho_arquivo/sizeof(struct registro))
  long int size = file_size(arq);
  float fultimo = size / sizeof(struct registro);
  int ultimo = (int)fultimo;
  if (fultimo - (int)fultimo > 0) {
      ultimo++;
  }

  return ultimo;
}
