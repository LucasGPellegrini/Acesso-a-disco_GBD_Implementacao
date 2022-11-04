#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

#define TRUE 1
#define MAXOP 7

int main()
{
  int t, op;
  entry_number_t nro_registros;
  struct registro reg;
  int blocks_sizes[4] = {1, 1000, 10000, 100000};

  do{
      do{
          printf("LISTA DE OPCOES:\n\n");
          printf(" [1] Criar Arquivo.\n");
          printf(" [2] Ler Registro Aleatório.\n");
          printf(" [3] Inserir Registro Aleatório (Final).\n");
          printf(" [4] Atualizar Registro Aleatório.\n");
          printf(" [5] Deletar Registro Aleatório.\n");
          printf(" [6] Experimentos com varredura sequencial.\n");
          printf(" [7] Sair.\n");
          printf("\nDigite uma das opcoes: ");
          
          scanf("%d", &op);
          system(limpar);
          setbuf(stdin, NULL);
          
          if(op < 1 || op > MAXOP){
              printf("Opcao invalida! Digite novamente!\n\n");
          }
      } while(op < 1 || op > MAXOP);
      
      if(op == MAXOP){
          break;
      }
      
      switch(op){
      	case 1:
	        printf("Entre com o tamanho da RAM (em GBs): ");
	        scanf("%d", &t);
	        if(!CALC_SIZE_RAM(t, 5, &nro_registros)){
	            printf("Valor invalido.\n\n");
	            break;
	        }

	        if(!CREATE_HEAP_FILE(nro_registros)){
	            printf("Ocorreu algum erro ao criar arquivo.\n\n");
	            break;
	        }

	        printf("Arquivo criado com sucesso!\n\n");
	        break;

      	case 2:
      		if(!READ_RANDOM(fopen("arquivo", "rb"), &reg)){
    			printf("Erro!\n");
    			break;
      		}

      		printf("NSEQ: %lu\n", reg.nseq);
   			printf("TEXT: %s\n", reg.text);
   			printf("Registro lido com sucesso!\n\n");
   			break;
   			
   		case 3:
   			if(!ISRT_AT_END(fopen("arquivo", "rb+"))){
   			    printf("Erro!\n");
   				break;
   			}
   			printf("Registro inserido com sucesso!\n\n");
   			break;

  		case 4:
      		if(!UPDATE_RANDOM(fopen("arquivo", "rb+"), &reg)){
    			printf("Erro!\n");
    			break;
      		}

      		printf("NSEQ: %lu\n", reg.nseq);
   			printf("TEXT: %s\n", reg.text);
   			printf("Registro atualizado com sucesso!\n\n");
   			break;

   		case 5:
   			if(!DELETE_RANDOM(fopen("arquivo", "rb+"), &reg)){
    			printf("Erro!\n");
    			break;
      		}

      		printf("NSEQ: %lu\n", reg.nseq);
   			printf("TEXT: %s\n", reg.text);
   			printf("Registro deletado com sucesso!\n\n");
   			break;
        case 6:    
   			for(int i = 0; i < 4; i++) {
                entry_number_t valid_registers;
                int num_of_pages;
                double time;

                if(SEQUENTIAL_SWEEP(fopen("arquivo", "rb"), blocks_sizes[i], &valid_registers, &num_of_pages, &time)) {
                    printf("VARREDURA SEQUENCIAL - BLOCOS DE %d REGISTRO(s)\n\n", blocks_sizes[i]);
                    printf("Registros validos: %lu\n", valid_registers);
   			        printf("Numero de paginas lidas: %d\n", num_of_pages);
   			        printf("Tempo para processamento: %lf\n\n", time);
                }
            }
            break;
      }

  } while(op != MAXOP);
  
  system(limpar);
  return 0;
}
