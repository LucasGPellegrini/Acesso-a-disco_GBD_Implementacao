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
#define MAXOP 2

int main()
{
  int t, op;
  entry_number_t nro_registros;

  do{
      do{
          printf("LISTA DE OPCOES:\n\n");
          printf(" [1] Criar arquivo.\n");
          printf(" [2] Sair.\n");
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
            if(!CALC_SIZE_RAM(t, &nro_registros)){
                printf("Valor invalido.\n\n");
                break;
            }

            if(!CREATE_HEAP_FILE(nro_registros)){
                printf("Ocorreu algum erro ao criar arquivo.\n\n");
                break;
            }

            printf("Arquivo criado com sucesso!\n\n");
            break;
      }  

  } while(op != MAXOP);
  
  system(limpar);
  return 0;
}