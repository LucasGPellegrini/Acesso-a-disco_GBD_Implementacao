#ifndef REGISTRO_H_INCLUDED
#define REGISTRO_H_INCLUDED
typedef struct registro reg;

FILE * CREATE_HEAP_FILE(long int nro_registros);
reg READ_RANDOM(FILE *arq, int nseq);
FILE * ISRT_AT_END(FILE *arq);
FILE * UPDATE_RANDOM(FILE *arq, int nseq, int novo_texto);
FILE * DELETE_RANDOM(FILE *arq, int nseq);

#endif // REGISTRO_H_INCLUDED
