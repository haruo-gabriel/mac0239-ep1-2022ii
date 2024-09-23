ALL: mastermind run_mastermind_test

run_mastermind_test: mastermind
	./mastermind 4 7 10 

mastermind: mastermind.o jogo.o codemaker.o codebreaker.o
	gcc -o mastermind mastermind.o jogo.o codemaker.o codebreaker.o


mastermind.o: mastermind.c jogo.h
	gcc -c mastermind.c

jogo.o: jogo.c jogo.h codemaker.h
	gcc -c jogo.c

codemaker.o: codemaker.c codemaker.h
	gcc -c codemaker.c

codebreaker.o: codebreaker.c codebreaker.h
	gcc -c codebreaker.c
