#include <stdio.h>

#ifndef CODEBREAKER_H
#define CODEBREAKER_H

#define MAX_CLAUSES 1000000
#define SOLVER_CALL "../minisat/core/minisat"
#define SOLVER_RESP "/tmp/solver_resp.txt"

struct CLAUSE {
  int nLits;
  int* Lits;
};

struct FORMULA {
  int nclauses;
  struct CLAUSE *clauses[MAX_CLAUSES];
};
  
struct BREAKER {
  int nslots;
  int ncolors;
  int* last_guess;
};

struct SOLVER {
  char *fname;
  FILE *satfile;
  int nclauses;
  char *solver;
  char *solver_resp;
};

void cria_CodeBreaker(int nslots, int ncolors);
void printGuess(int c[], int n);
int* codebreaker_guess();
int  convert_feedback(int *feedback);

// private functions
void cria_clausulas_fixas();
void max_one_color_per_slot();
void min_one_color_per_slot();
int slot_color_to_var(int slot, int color); // att_to_var
void add_clause(int nLits, int* Lits);

void printClause(struct CLAUSE* c);

// Solver functions
int* run_solver();
void inicia_solver();
void write_header();
void write_problem();
void write_clauses();
int *parse_answer();


#endif // CODEBREAKER_H_INCLUDED
