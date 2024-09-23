#include <stdio.h> 

#include "jogo.h"
#include "codemaker.h"
#include "codebreaker.h"


struct JOGO _jogo;

void  cria_jogo_Mastermind(int nslots, int ncolors, int ntries){
  _jogo.nslots = nslots;
  _jogo.ncolors = ncolors;
  _jogo.ntries = ntries;

  cria_CodeMaker(nslots, ncolors);
  cria_CodeBreaker(nslots, ncolors);
  printf("Jogo Iniciado\n");
  
}

int joga_Mastermind() {
  /*    Runs the game for the specified number of tries.
        Returns True if the CodeBreaker guessed the code within the given tries,
        otherwise returns False
  */
  for( int i=0; i <  _jogo.ntries; i++ ) {
    int* guess = codebreaker_guess();
    printGuess( guess, _jogo.nslots );
    int* feedback = codemaker_feedback(guess);
    if( convert_feedback(feedback) ){
      //printf("\nGuesses: %d\n", i+1);
      return CodeBreaker;
    }
  }

  return CodeMaker;
}

