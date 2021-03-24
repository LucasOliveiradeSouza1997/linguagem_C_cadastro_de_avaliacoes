//Guilherme Almeida Mascarenhas
//Lucas Oliveira de Souza
//2º ADS - Manhã

#include "funcoes.h"
#include <locale.h>

int main(){
	setlocale(LC_ALL, "Portuguese");

	char op;
	do {
		imprime_menu();
		char str_auxiliar[3] = ""; // para validar a opção
		scanf("%2[^\n]", str_auxiliar);
		limpa_buffer();
		if (valida_opcao_menu(str_auxiliar)){
			op = str_auxiliar[0];
			switch (op) {
				case '1': cadastrar_alunos();
                        break;
				case '2': listar_alunos();
						break;
				case '3': alterar_aluno();
						break;
				case '4': consultar_aluno();
						break;
			    case '5': cadastrar_avaliacoes();
						break;
				case '6': listar_avaliacoes();
						break;
				case '0': break;
			}
		}
	} while (op != '0');

	return 0;
}
