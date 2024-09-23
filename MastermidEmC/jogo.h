#ifndef JOGO_H
#define JOGO_H

#define CodeBreaker 0
#define CodeMaker   1

struct JOGO {
  int nslots;
  int ncolors;
  int ntries;

};

void  cria_jogo_Mastermind(int nslots, int ncolors, int ntries);
int joga_Mastermind();

#endif // JOGO_H_INCLUDED
