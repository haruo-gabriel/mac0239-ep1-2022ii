#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "codebreaker.h"

struct BREAKER _breaker;

void  cria_CodeBreaker(int nslots, int ncolors) {
  _breaker.nslots = nslots;
  _breaker.ncolors= ncolors;
  _breaker.last_guess= (int*) NULL;

  printf("Criando clausulas ...\n");
  cria_clausulas_fixas();
}


struct FORMULA _fml;

void cria_clausulas_fixas() {
  _fml.nclauses = 0;
  for( int i=0; i<MAX_CLAUSES; i++)
    _fml.clauses[i] = (struct CLAUSE *) NULL;

  min_one_color_per_slot();
  max_one_color_per_slot();
}

void min_one_color_per_slot() {
  for(int i=0; i<_breaker.nslots; i++) {
    int* Lits = malloc( _breaker.ncolors * sizeof(int) );
    for( int j=0; j<_breaker.ncolors; ++j )
      Lits[j]=slot_color_to_var(i, j); // att_to_var

    add_clause(_breaker.ncolors, Lits);
  }
}

int slot_color_to_var(int slot, int color) {
  return color * _breaker.nslots + slot + 1;  
}

void add_clause(int nLits, int* Lits) {
  int ind = _fml.nclauses;
  
  _fml.clauses[ind] = malloc(sizeof(struct CLAUSE));
  _fml.clauses[ind]->nLits = nLits;
  _fml.clauses[ind]->Lits  = Lits;
  _fml.nclauses++;
  //printf("%d ", ind);
  //printClause(_fml.clauses[ind]);
}



void max_one_color_per_slot() {
  for( int i=0; i < _breaker.nslots; i++ ) {
    for( int j=0; j < _breaker.ncolors - 1; j++ ) {
      for( int k=j+1; k < _breaker.ncolors; k++ ) {
        int aux1 = slot_color_to_var(i, j);
        int aux2 = slot_color_to_var(i, k);
        int* Lits = malloc( 2 * sizeof(int) );
        Lits[0] = -aux1;
        Lits[1] = -aux2;
        add_clause(2,Lits);
      }
    }
  }
}

int* codebreaker_guess() {
  /* Uses a SAT solver to make a guess */
  int *result = run_solver();
  _breaker.last_guess = result;
  return result;
}

int  convert_feedback(int *feedback) {
  int count = 0, i;

  for( i=0; i < _breaker.nslots; i++ ) 
    if( feedback[i] == 1 )
      count += 1;
  if( count == _breaker.nslots )
    return 1;


  // Generate extra clauses
  int *Lits = malloc( _breaker.nslots * sizeof(int) );
  for (i = 0; i < _breaker.nslots; i++) 
    Lits[i] = - slot_color_to_var(i, _breaker.last_guess[i]);
  add_clause(_breaker.nslots, Lits);

  for( i=0; i < _breaker.nslots; i++ ) {
    int color = _breaker.last_guess[i];

    if (feedback[i] == 1) {
      int *Lits1 = malloc(sizeof(int));
      Lits1[0] = slot_color_to_var(i, color);
      add_clause(1, Lits1);
    }

    else if (feedback[i] == 0){
      for (int j = 0; j < _breaker.nslots; j++){
        int *Lits1 = malloc(sizeof(int));
        Lits1[0] = - slot_color_to_var(j, color);
        add_clause(1, Lits1);
      }
    }

    else{ // if (feedback == 2)
      int *Lits1 = malloc(sizeof(int));
      Lits1[0] = - slot_color_to_var(i, color);
      add_clause(1, Lits1);
      
      int *Lits2 = malloc((_breaker.nslots - 1) * sizeof(int));
      int j, k;
      for (int j = 0, k = 0; j < _breaker.nslots; j++){
        if (j != i){
          Lits2[k] = slot_color_to_var(j, color);
          k += 1;
        }
      }
      add_clause((_breaker.nslots) - 1, Lits2);
    }
  }

  return 0;
}

void printGuess(int c[], int n) {
  printf("Guess: [ ");
  for( int i = 0; i < n; i++ ) 
    printf("%d ", c[i] );
  
  printf("]\n");
}

void printClause(struct CLAUSE* c) {
  printf("CLAUSE: [ ");
  for( int i = 0; i < c->nLits; i++ ) 
    printf("%d ", c->Lits[i] );
  
  printf("]\n");
}


// Solver Functions
struct SOLVER _solver;

int* run_solver() {
  /* Runs the SAT solver with the SAT file */
  // printf( "Running solver ...\n");
  inicia_solver();
  write_problem();
  write_clauses();

  char cmd[1000] = "";
  strcat( cmd, _solver.solver);
  strcat( cmd, " ");
  strcat( cmd, _solver.fname);
  strcat( cmd, " ");
  strcat( cmd, _solver.solver_resp);
  strcat( cmd, " > /dev/null");
  // printf( "Comando: %s\n", cmd );
  system(cmd);
  
  return parse_answer();
}

void inicia_solver() {
  _solver.fname = "/tmp/mastermindsat.cnf";
  _solver.satfile = fopen(_solver.fname, "w");
  _solver.nclauses = 0;
  _solver.solver = SOLVER_CALL;
  _solver.solver_resp = SOLVER_RESP;
  write_header();
  if(!_solver.satfile ) {
    printf("Error creating file\n");
    exit(2);
  }
}

void write_header() {
  /* Writes the header of the SAT file */
  fprintf(_solver.satfile,"c\n");
  fprintf(_solver.satfile,"c Mastermind - SAT\n");
  fprintf(_solver.satfile,"c\n");
}

void write_problem() {
  int nvars = _breaker.nslots * _breaker.ncolors;
  fprintf(_solver.satfile,"p cnf %d %d\n",nvars, _fml.nclauses);
  //fclose(_solver.satfile);
}

void write_clauses() {
  for( int i=0; i<_fml.nclauses; i++) {
    for( int j=0; j<_fml.clauses[i]->nLits; j++) {
      fprintf(_solver.satfile, "%d ", _fml.clauses[i]->Lits[j]);
    }
    fprintf(_solver.satfile, "0\n");
  }
  fclose(_solver.satfile);
}



int *parse_answer() {
  int nvars = _breaker.nslots * _breaker.ncolors;
  errno = 0;
  FILE *ans = fopen( _solver.solver_resp, "r");
  if( !ans ) {
    printf("Erro %d na abertura da resposta ao solver em %s.  Abortando.\n%s",
           errno, _solver.solver_resp, strerror(errno));
    exit(3);
  }
  
  char veredict[10];
  int ret = fscanf(ans, "%s", veredict);
  if( ret==-1 ) {
    printf("Erro %d na leitura da resposta ao solver.  Abortando.\n%s",
           errno, strerror(errno));
    exit(4);
  }
  
  // printf("Veredito: %s\n", veredict);
  if( strcmp (veredict, "UNSAT") == 0 ) {
    printf("UNSAT não devia aconteder aqui!!!\n");
    fflush(stdout);
    return (int*) NULL;
  }

  // Satisfiable
  int answer[nvars];
  int* guess = malloc( _breaker.nslots * sizeof(int) );
  for( int i=0; i < nvars; i++ ) {
    ret = fscanf( ans, "%d", &answer[i]);
    if( ret==-1 ) {
      printf("Erro %d na leitura da resposta ao solver.  Abortando.\n%s",
             errno, strerror(errno));
      exit(5);
    }
    if( answer[i] > 0 ) {
      int slot  = (answer[i]-1)%_breaker.nslots;
      int color = (answer[i]-1)/_breaker.nslots;
      guess[slot] = color;
    }
  }

  return guess;
    
}
