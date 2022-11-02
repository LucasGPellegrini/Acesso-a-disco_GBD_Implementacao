#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "size.h"
#include "registro.h"
#include "mtwister.h"

// uma chamada de seed apenas!
int SEED_SETTED;

void SEET_SEED_(){
  srand(time(NULL));
  SEED_SETTED = rand() + 1;
}


void RANDOM_REG_(char* reg_text){
  char diff = 'Z' - 'A';
  for(int i = 0; i < TEXT_SIZE; i++)
    reg_text[i] = 'A' + rand() % diff;
}

int CREATE_HEAP_FILE(entry_number_t nro_registros){
  // Abre arquivo (escrita)
  FILE *fp;
  fp = fopen("arquivo.txt", "w");
  if(fp == NULL) return 0;

  if(!SEED_SETTED) SEET_SEED_();

  struct registro reg;
  for(entry_number_t i = 0; i < nro_registros; i++){
    // Preenche registro
    reg.nseq = i;
    RANDOM_REG_(reg.text);

    // Insere registro no arquivo
    if(!fwrite(&reg, sizeof(struct registro), 1, fp)){
      fclose(fp);
      remove("arquivo.txt");
      return 0;
    }
  }

  fclose(fp);

  return 1;
}


int READ_REG(FILE *arq, entry_number_t nseq, Registro reg){
  if(arq == NULL || reg == NULL || nseq < 0) return 0;

  // Acha a quantidade de registro no momento
  entry_number_t rqtd;
  FILE_REG_NUM(arq, &rqtd);

  // Se o numero sequencial for maior ou
  // igual a qtd acima, nseq invalido
  if(nseq >= rqtd) return 0;

  // Seta o Stream para uma posição nseq
  fseek(arq, nseq * sizeof(struct registro), SEEK_SET);

  // le o registro
  struct registro registro;
  if(!fread(&registro, sizeof(struct registro), 1, arq)) return 0;

  *reg = registro;

  rewind(arq);

  return 1;
}

int READ_RANDOM(FILE *arq, Registro reg){
  if(arq == NULL || reg == NULL) return 0;

  // Acha a quantidade de registro no momento
  entry_number_t rqtd;
  FILE_REG_NUM(arq, &rqtd);

  if(!SEED_SETTED) SEET_SEED_();

  // Pega um aleatorio em [0..1] (MersenneTwister)
  MTRand r = seedRand(SEED_SETTED);
  getRand(&r);
  
  // Acha nseq aleatorio
  entry_number_t nseq = floor((rqtd - 1) * r);

  // Le um registro aleatorio
  READ_REG(arq, nseq, reg);

  return 1;
}

int ISRT_AT_END(FILE *arq){
  if(arq == NULL) return 0;

  // Seta stream para o ultimo registro
  fseek(arq, - sizeof(struct registro), SEEK_END);

  // le o ultimo registro
  struct registro reg;
  fread(&reg, sizeof(struct registro), 1, arq);

  // Escreve o registro
  reg.nseq++;
  RANDOM_REG_(reg.text);
  if(!fwrite(&reg, sizeof(struct registro), 1, arq)) return 0;

  rewind(arq);
  return 1;
}

int UPDATE_REG(FILE *arq, Registro reg){
  if(arq == NULL || reg == NULL || nseq < 0) return 0;

  // Acha a quantidade de registro no momento
  entry_number_t rqtd;
  FILE_REG_NUM(arq, &rqtd);

  // Se nseq eh maior ou igual a rqtd, invalido
  if(reg->nseq >= rqtd) return 0;

  fseek(arq, reg->nseq * sizeof(struct registro), SEEK_SET);
  if(!fwrite(reg, sizeof(struct registro), 1, arq)) return 0;

  rewind(arq);

  return 1;
}

int UPDATE_RANDOM(FILE *arq, Registro reg){
  if(arq == NULL || reg == NULL) return 0;

  // Acha a quantidade de registro no momento
  entry_number_t rqtd;
  FILE_REG_NUM(arq, &rqtd);

  if(!SEED_SETTED) SEET_SEED_();

  // Pega um aleatorio em [0..1] (MersenneTwister)
  MTRand r = seedRand(SEED_SETTED);
  getRand(&r);
  
  // Acha nseq aleatorio
  entry_number_t nseq = floor((rqtd - 1) * r);

  // Registro aleatorio
  reg->nseq = nseq;
  RANDOM_REG_(reg->text);

  // Update um registro aleatorio
  READ_REG(arq, nseq, reg);

  return 1;
}

int DELETE_REG(FILE *arq, entry_number_t nseq, Registro reg){
  if(arq == NULL || reg == NULL || nseq < 0) return 0;

  // Acha a quantidade de registro no momento
  entry_number_t rqtd;
  FILE_REG_NUM(arq, &rqtd);

  // Se nseq eh maior ou igual a rqtd, invalido
  if(nseq >= rqtd) return 0;

    // Seta stream para o registro nseq
  fseek(arq, nseq * sizeof(struct registro), SEEK_SET);

  struct registro registro;
  if(!fread(&registro, sizeof(struct registro), 1, arq)) return 0;

  // Faz uma copia do registro retirado
  *reg = registro;

  entry_number_t offset;
  size_t size_registro = sizeof(struct registro);

  while(!feof(arq)){
    fread(&registro, size_registro, 1, arq);
    
    registro.nseq -= 1;

    offset -= size_registro;
    fseek(arq, offset, SEEK_CUR);

    fwrite(&registro, size_registro, 1, arq);

    offset += 2 * size_registro;
  }

  // Truncate p/ remover os bytes remanescentes
  memory_size_t novo_tamanho = FILE_SIZE(arq) - size_registro;
  ftruncate(fileno(arq), novo_tamanho);

  return 1;
}

int DELETE_RANDOM(FILE *arq, Registro reg){
  if(arq == NULL || reg == NULL) return 0;

  // Acha a quantidade de registro no momento
  entry_number_t rqtd;
  FILE_REG_NUM(arq, &rqtd);

  if(!SEED_SETTED) SEET_SEED_();

  // Pega um aleatorio em [0..1] (MersenneTwister)
  MTRand r = seedRand(SEED_SETTED);
  getRand(&r);
  
  // Acha nseq aleatorio
  entry_number_t nseq = floor((rqtd - 1) * r);

  // Delete um registro aleatorio
  DELETE_REG(arq, nseq, reg);

  return 1;
}

int FILE_REG_NUM(FILE *arq, entry_number_t* rnum){
  if(arq == NULL || rnum == NULL) return 0;

  entry_number_t seek = ftell(arq);

  fseek(arq, 0, SEEK_END);
  *nqtd = ftell(arq) / sizeof(struct registro);
  fseek(arq, 0, seek);

  return 1;
}


int FILE_SIZE(FILE *arq, memory_size_t *arq_size){
  if(arq == NULL || arq_size == NULL) return 0;

  entry_number_t rnum;
  FILE_REG_NUM(arq, &rnum);
  *arq_size = rnum * sizeof(struct registro);

  return 1;
}
