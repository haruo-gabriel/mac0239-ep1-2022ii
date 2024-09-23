#ifndef CODEMAKER_H
#define CODEMAKER_H

struct CODIGO {
  int nslots;
  int ncolors;
  int* codigo;
};

void  cria_CodeMaker(int nslots, int ncolors);
void  mata_CodeMaker();

void printCodigo(int codigo[], int nslots);
int* codemaker_feedback( int* guess);

#endif // CODEMAKER_H_INCLUDED
