#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>

#include "size.h"
#include "registro.h"
#include "mtwister.h"

// uma chamada de seed apenas!
int SEED_SETTED;
MTRand r;

void SEET_SEED_(){
	srand(time(NULL));
	SEED_SETTED = rand() + 1;
	r = seedRand(SEED_SETTED);
}

void RANDOM_REG_(char* reg_text){
	char diff = 'Z' - 'A';
	for(int i = 0; i < TEXT_SIZE; i++)
		reg_text[i] = 'A' + rand() % diff;
}

int CREATE_HEAP_FILE(entry_number_t nro_registros){
	// Abre arquivo (escrita)
	FILE *fp;
	fp = fopen("arquivo", "wb");
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
			remove("arquivo");
			return 0;
		}
	}

	fclose(fp);

	return 1;
}

int READ_REG(FILE *arq, entry_number_t nseq, Registro reg){
	if(arq == NULL) return 0;

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
if(arq == NULL) return 0;

	// Acha a quantidade de registro no momento
	entry_number_t rqtd;
	FILE_REG_NUM(arq, &rqtd);

	if(!SEED_SETTED) SEET_SEED_();

	// Pega um aleatorio em [0..1] (MersenneTwister)
	double rn = genRand(&r);

	// Acha nseq aleatorio
	entry_number_t nseq = (rqtd - 1) * rn;

	// Le um registro aleatorio
	return READ_REG(arq, nseq, reg);
}

int ISRT_AT_END(FILE *arq){
if(arq == NULL) return 0;

	// Seta stream para o ultimo registro
	fseek(arq, -(long int)sizeof(struct registro), SEEK_END);

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
	if(arq == NULL || reg == NULL) return 0;

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
	double rn = genRand(&r);

	// Acha nseq aleatorio
	entry_number_t nseq = (rqtd - 1) * rn;

	// Registro aleatorio
	reg->nseq = nseq;
	RANDOM_REG_(reg->text);

	// Update um registro aleatorio
	return UPDATE_REG(arq, reg);
}

int DELETE_REG(FILE *arq, entry_number_t nseq, Registro reg){
	if(arq == NULL || reg == NULL) return 0;

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

	registro.nseq = UINT_MAX;

	fseek(arq, nseq * sizeof(struct registro), SEEK_SET);

	fwrite(&registro, sizeof(struct registro), 1, arq);

	rewind(arq);

	return 1;
}

int DELETE_RANDOM(FILE *arq, Registro reg){
	if(arq == NULL || reg == NULL) return 0;

	// Acha a quantidade de registro no momento
	entry_number_t rqtd;
	FILE_REG_NUM(arq, &rqtd);

	if(!SEED_SETTED) SEET_SEED_();

	// Pega um aleatorio em [0..1] (MersenneTwister)
	double rn = genRand(&r);

	// Acha nseq aleatorio
	entry_number_t nseq = (rqtd - 1) * rn;

	// Delete um registro aleatorio
	return DELETE_REG(arq, nseq, reg);
}

int CALC_SIZE_RAM(int ram_size, int ratio, entry_number_t *rnum){
	if(ram_size <= 0 || rnum == NULL) return 0;

	*rnum = ratio * (((ram_size * G) / sizeof(struct registro)) + 1);
	return 1;
}

int FILE_REG_NUM(FILE *arq, entry_number_t* rnum){
	if(arq == NULL || rnum == NULL) return 0;

	entry_number_t seek = ftell(arq);

	fseek(arq, 0, SEEK_END);
	*rnum = ftell(arq) / sizeof(struct registro);
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

int SEQUENTIAL_SWEEP(FILE *arq, int rnum_by_page, entry_number_t *valid_registers, int *num_of_pages) {
	if(arq == NULL || rnum_by_page <= 0) return 0;
	rewind(arq);

	*num_of_pages = -1; 
	*valid_registers = 0; 

	Registro buffer = (Registro) malloc(rnum_by_page * sizeof(struct registro));

	int read = 0;
	do {
		for(int i = 0; i < read; i++)
			if(buffer[i].nseq != UINT_MAX)
				(*valid_registers)++;
		
		(*num_of_pages)++;

		read = fread(buffer, sizeof(struct registro), rnum_by_page, arq);
	} while(read);

	return 1;
}

int RANDOM_SWEEP(FILE *arq, entry_number_t qtd_reg, entry_number_t *valid_registers) {
	if(arq == NULL || valid_registers == NULL || qtd_reg <= 0) return 0;

	struct registro reg;
	*valid_registers = 0;

	for(unsigned int i = 0; i < qtd_reg; i++)
		if(READ_RANDOM(arq, &reg))
			if(reg.nseq != UINT_MAX)
				(*valid_registers)++;

	return 1;
}

int GENERATE_FIRST_100KB(FILE *arq, entry_number_t *rnum) {
	if(arq == NULL) return 0;
	// Abre arquivo (escrita)
	FILE *first_100KB;
	first_100KB = fopen("first_100KB", "wb");
	if(first_100KB == NULL) return 0;

	entry_number_t rnum_to_copy = 100 * K / sizeof(struct registro) + 1;
	Registro buffer = (Registro) malloc(rnum_to_copy * sizeof(struct registro));
	int read = fread(buffer, sizeof(struct registro), rnum_to_copy, arq);

	for(int i = 0; i < read; i++) {
		if(!fwrite(&buffer[i], sizeof(struct registro), 1, first_100KB)) {
			fclose(first_100KB);
			remove("first_100KB");
			return 0;
		}
	}

	FILE_REG_NUM(first_100KB, rnum);
	fclose(first_100KB);

	return 1;
}

int PRINT_REG(Registro reg){
	if(reg  == NULL)
		return 0;
	printf("NSEQ: %u\n", reg->nseq);
	printf("NSEQ: %s\n", reg->text);
	
	return 1;
}
