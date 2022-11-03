/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
C#, OCaml, VB, Perl, Swift, Prolog, Javascript, Pascal, HTML, CSS, JS
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>

typedef unsigned long long int memory_size_t;
typedef unsigned long int entry_number_t;
#define TEXT_SIZE 46

const memory_size_t K = 1024;
const memory_size_t M = 1048576;
const memory_size_t G = 1073741824;

struct registro{
  
  char text[TEXT_SIZE];
  entry_number_t nseq;
};

int CALC_SIZE_RAM(int ram_size, int ratio, entry_number_t *rnum){
  if(ram_size <= 0 || rnum == NULL) return 0;

  *rnum = ratio * (((ram_size * G) / sizeof(struct registro)) + 1);
  return 1;
}



int main()
{
    entry_number_t nro_registros;
    CALC_SIZE_RAM(16, 5, &nro_registros);
    printf("%lu\n\n", sizeof(struct registro));
    printf("%lu", nro_registros);

    return 0;
}
