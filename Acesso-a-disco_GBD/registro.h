#ifndef REGISTRO_H_INCLUDED
#define REGISTRO_H_INCLUDED

#include "size.h"
#pragma pack(1)

struct registro{
  entry_number_t nseq;
  char text[TEXT_SIZE];
};

typedef struct registro* Registro;

int CREATE_HEAP_FILE(entry_number_t nro_registros); // OK
int READ_REG(FILE *arq, entry_number_t nseq, Registro reg); // OK
int READ_RANDOM(FILE *arq, Registro reg); // OK
int ISRT_AT_END(FILE *arq); // OK
int UPDATE_REG(FILE *arq, Registro reg); // OK
int UPDATE_RANDOM(FILE *arq, Registro reg); // OK
int DELETE_REG(FILE *arq, entry_number_t nseq, Registro reg); // OK
int DELETE_RANDOM(FILE *arq, Registro reg); // OK
int CALC_SIZE_RAM(int ram_size, int ratio, entry_number_t *rnum);

// Quantidade de registros atualmente no arquivo
int FILE_REG_NUM(FILE *arq, entry_number_t *rnum); // OK?

// Tamanho do arquivo
int FILE_SIZE(FILE *arq, memory_size_t *arq_size); // OK?

#endif // REGISTRO_H_INCLUDED
