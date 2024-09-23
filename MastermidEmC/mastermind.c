#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "jogo.h"

int main(int argc, char** argv){

  if( argc != 4 ) {
    printf("usage: mastermind <n espacos> <n cores> <n tentativas>\nAborted.\n");
    exit(1);
  }
  
  int nslots = atoi(argv[1]);
  int ncolors = atoi(argv[2]);
  int ntries = atoi(argv[3]);

  printf("n espacos: %d, n cores: %d, n tentativas: %d.\n",
         nslots, ncolors, ntries);

  cria_jogo_Mastermind(nslots, ncolors, ntries);
  int ganhador = joga_Mastermind();

  if (ganhador == CodeBreaker){
    printf( "CodeBreaker ganhou!\n");
  }
  else {
    printf( "CodeBreaker perdeu :( !!!\n");
  }

  
  return 0;
}

