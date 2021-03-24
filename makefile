projeto2LG: main.o funcoes.o
	gcc -o projeto2LG main.o funcoes.o 
main.o:	main.c funcoes.h
	gcc -c main.c
funcoes.o:  funcoes.c funcoes.h
	gcc -c funcoes.c