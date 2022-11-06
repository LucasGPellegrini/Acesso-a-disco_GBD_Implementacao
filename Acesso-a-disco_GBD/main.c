#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "registro.h"

// checa para saber se é windows OS
// _WIN32 macro
#ifdef _WIN32
    #define limpar "cls"
  
// checa para saber se é linux OS 
// __linux__ macro
#elif __linux__
    #define limpar "clear"
  
#endif

#define MAXOP 9

// Preambulo :)
int tempo_seq_sweep(FILE *arq, int rnum_by_page, entry_number_t *valid_registers, int *num_of_pages, double *time);
int tempo_rnd_sweep(FILE *arq, entry_number_t qtd_reg, entry_number_t *valid_registers, double *time);

int main()
{
	//case 2 - case 5
	struct registro reg;
	//

	//case 1 e case 6
	entry_number_t nro_registros;
	//
	
	//case 6 e case 7
	entry_number_t valid_registers;
	double time;
	//

	int op;
	do {
		printf("LISTA DE OPCOES:\n\n");
		printf(" [1] Criar Arquivo.\n");
		printf(" [2] Ler Registro Aleatório.\n");
		printf(" [3] Inserir Registro Aleatório (Final).\n");
		printf(" [4] Atualizar Registro Aleatório.\n");
		printf(" [5] Deletar Registro Aleatório.\n");
		printf(" [6] Experimentos com Varredura Sequencial.\n");
		printf(" [7] Experimentos com Leitura Aleatoria.\n");
		printf(" [8] Gerar arquivo com os primeiros 100KB do arquivo original.\n");
		printf(" [9] Sair.\n");
		printf("\nDigite uma das opcoes: ");

		scanf("%d", &op);
		system(limpar);
		setbuf(stdin, NULL);
		
		switch(op){
			case MAXOP:
				printf("Encerrando o programa.\n");
				
				//esperando n segundos antes de encerrar o programa
				int n = 3;
				clock_t inicio = clock();
				while((clock() - inicio) * 1000 / CLOCKS_PER_SEC < n * 1000);
								
				break;
				
			case 1:
      {
				int t;
	       		printf("Entre com o tamanho da RAM (em GBs): ");
	       		scanf("%d", &t);
	       		
	       		if(!CALC_SIZE_RAM(t, 5, &nro_registros)){
	           		printf("Valor invalido.\n\n");
	           		break;
	       		}

	       		if(!CREATE_HEAP_FILE(nro_registros)){
	           		printf("Ocorreu algum erro ao criar o arquivo.\n\n");
	           		break;
	       		}

	       		printf("Arquivo criado com sucesso!\n\n");
	       		break;
      }

			case 2:
	    		if(!READ_RANDOM(fopen("arquivo", "rb"), &reg)){
	  				printf("Ocorreu algum erro ao ler aleatoriamente do arquivo.\n");
	  				break;
	    		}

	    		if(PRINT_REG(&reg))
	 				printf("Registro lido com sucesso!\n\n");
	 			break;
	 			
	 		case 3:
	 			if(!ISRT_AT_END(fopen("arquivo", "rb+"))){
	 			    printf("Ocorreu algum erro ao inserir no final do arquivo.\n");
	 				break;
	 			}
	 			printf("Registro inserido com sucesso!\n\n");
	 			break;

			case 4:
	    		if(!UPDATE_RANDOM(fopen("arquivo", "rb+"), &reg)){
	  				printf("Ocorreu algum erro ao atualizar um registro aleatorio do arquivo.\n");
	  				break;
	    		}

	    		if(PRINT_REG(&reg))
	 				printf("Registro atualizado com sucesso!\n\n");
	 			break;

	 		case 5:
	 			if(!DELETE_RANDOM(fopen("arquivo", "rb+"), &reg)){
	  				printf("Ocorreu algum erro ao deletar um registro aleatorio do arquivo!\n");
	  				break;
	    		}

	    		if(PRINT_REG(&reg))
	 				printf("Registro deletado com sucesso!\n\n");
	 			break;
	 			
			case 6:
      {
				int i, num_of_pages, block_sizes[4] = {1, 1000, 10000, 1000000};
				
				do {
					printf("Tamanho de Pagina:\n\n");
					printf(" [0] 1       Registro.\n");
					printf(" [1] 1000    Registros.\n");
					printf(" [2] 10000   Registros.\n");
					printf(" [3] 1000000 Registros.\n");
					printf("\n Opcao: ");

					scanf("%d", &i);
				}while(i < 0 || i > 3);

				FILE *fb = fopen("arquivo", "rb");
				if(tempo_seq_sweep(fb, block_sizes[i], &valid_registers, &num_of_pages, &time)) {
					FILE_REG_NUM(fb, &nro_registros);
					printf("VARREDURA SEQUENCIAL - BLOCOS DE %d REGISTRO(s)\n\n", block_sizes[i]);
					printf("Registros validos: %u\n", valid_registers);
					printf("Registros invalidos: %u\n", (nro_registros - valid_registers));
					printf("Numero de paginas lidas: %d\n", num_of_pages);
					printf("Tempo para processamento: %lf\n\n", time);
				}
				break;
      }
				
			case 7:
      {
				entry_number_t nro_rnd_sweep = 0;
				do {
					printf("Digite a quantidade de registros a ser lida: ");
					scanf("%u", &nro_rnd_sweep);
				} while(nro_rnd_sweep <= 0);

				if(tempo_rnd_sweep(fopen("arquivo", "rb"), nro_rnd_sweep, &valid_registers, &time)) {
					printf("LEITURA ALEATORIA DE %d REGISTRO(s)\n\n", nro_rnd_sweep);
					printf("Registros validos: %u\n", valid_registers);
					printf("Registros invalidos: %u\n", (nro_rnd_sweep - valid_registers));
					printf("Tempo para processamento: %lf\n\n", time);
				}
				break;
      }
				
			case 8:
      {
				entry_number_t rnum_100KB;     
				if(!GENERATE_FIRST_100KB(fopen("arquivo", "rb"), &rnum_100KB)){
					printf("Ocorreu algum erro ao gerar o arquivo com os primeiros 100KB do arquivo original.\n");
					break;
				}

				printf("Arquivo com os primeiros 100KB criado com sucesso!\n");
				printf("Quantidade de registros: %u\n\n", rnum_100KB);
				break;
      }

			default:
				printf("Opcao invalida! Digite novamente!\n\n");
				break;
		}
				
	} while(op != MAXOP);

	system(limpar);
	return 0;
}

// Funcao calcular tempo SEQUENTIAL_SWEEP
int tempo_seq_sweep(FILE *arq, int rnum_by_page, entry_number_t *valid_registers, int *num_of_pages, double *time) {
	struct timespec inicio, fim; 
	clock_gettime(CLOCK_REALTIME, &inicio);

	int flag = SEQUENTIAL_SWEEP(arq, rnum_by_page, valid_registers, num_of_pages);

	clock_gettime(CLOCK_REALTIME, &fim);
	long segundos = fim.tv_sec - inicio.tv_sec;
	long nanosegs = fim.tv_nsec - inicio.tv_nsec;

	*time = (double) segundos + nanosegs*1e-9;
	return flag;
}

// Funcao calcular tempo RANDOM_SWEEP
int tempo_rnd_sweep(FILE *arq, entry_number_t qtd_reg, entry_number_t *valid_registers, double *time) {
	struct timespec inicio, fim; 
	clock_gettime(CLOCK_REALTIME, &inicio);

	int flag = RANDOM_SWEEP(arq, qtd_reg, valid_registers);

	clock_gettime(CLOCK_REALTIME, &fim);
	long segundos = fim.tv_sec - inicio.tv_sec;
	long nanosegs = fim.tv_nsec - inicio.tv_nsec;

	*time = (double) segundos + nanosegs*1e-9;
	return flag;
}
