/*funcoes.h */

#ifndef _FUNCOES_H
#define _FUNCOES_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h> // função strlen
#include <stdlib.h> // função atof

#define ARQ_ALUNOS_BIN "alunos.dat"
#define ARQ_AVALIACOES_BIN "avaliacoes.dat"
#define ARQ_LISTAGEM_DE_ALUNOS_TXT "listagem_de_alunos.txt"

typedef struct {
	int cod_aluno;
	char nome[41];
	int estagio;
} aluno;

typedef struct {
	int dia;
	int mes;
	int ano;
} tdata;

typedef struct {
	int cod_aluno;
	tdata data;
	float nota;
} avaliacao;

void cadastrar_alunos(void);
void listar_alunos(void);
void alterar_aluno(void);
void consultar_aluno(void);
void cadastrar_avaliacoes(void);
void listar_avaliacoes(void);
int compara_datas(tdata dt1, tdata dt2);

/* Recebe uma string, de tamanho 42, como argumento: verifica se o nome informado possui 41 caracteres; se possuir
 retorna false (pois o limite do nome é 40 caracteres), retorna false se o nome estiver em branco ou retorna true
caso a entrada seja válida */
bool valida_nome(const char *);

/*funcao para limpar o buffer do teclado após uma leitura*/
void limpa_buffer(void);

/*recebe uma string como argumento para validar se foi digitado apenas um numero entre 0 e 6 e retorna true caso
  a entrada estiver correta ou false se a entrada estiver errada*/
bool valida_opcao_menu(const char *);

/*imprimir o menu principal do programa*/
void imprime_menu(void);

/*recebe uma string como argumento para validar se foi digitado apenas um numero entre 1 e 3,
  sendo 1= Iniciante, 2= Intermediário e 3= Avançado  e retorna true caso
  a entrada estiver correta ou false se a entrada estiver errada*/
bool valida_estagio(const char *);

/*recebe uma string validada, referente ao estagio, e converte o caractere em inteiro (apenas para os
  numeros 1,2 e 3)*/
int converte_estagio_em_int(const char *);

/*recebe uma string, convertida para minúscula como argumento, para validar se a entrada é apenas s ou n,
  retorna true, caso  foi digitado S/s ou N/n, ou false se foi digitado qualquer outro caractere  */
bool valida_opcao_cadastrar_outro_aluno(const char *);

/*recebe uma estrutura do tipo data: verifica se foi digitado apenas números; se a data está  no
formato dd/mm/aaaa e se a data é válida. Retorna, caso a entrada for valida, true ou false se a entrada estiver incorreta. */
bool valida_data(tdata);

/*recebe uma string e verifica se: foi digitado apenas números e no máximo um ponto,a nota pertence ao
intervalo [0,10] e a string nao esta vazia. Retorna, caso a entrada for valida, verdadeiro ou falso, se a entrada
estiver incorreta.Retorna o valor da nota convertida de string para float por meio de um ponteiro */
bool valida_nota(const char *, float * );

#endif
