#include "funcoes.h"

void cadastrar_alunos(){
	char op;

	do {
		FILE * arq;

		if ((arq = fopen(ARQ_ALUNOS_BIN, "ab")) == NULL) {
			fprintf(stderr, "Erro: não foi possível abrir o arquivo %s!\n", ARQ_ALUNOS_BIN);
			return;
		}
		aluno var_aluno;

		char str_auxiliar[3] = ""; // validar opcao cadastrar outro aluno
		char estagio_auxiliar[3] = ""; // validar o campo estágio
		char nome_auxiliar[42] = ""; // 42 para validar se o nome digitado possui apenas 40 caracteres

		fseek(arq, 0, SEEK_END); // necessário no Windows
		var_aluno.cod_aluno = ftell(arq) / sizeof(aluno) + 1;

		printf("\n\t   ------------------ Cadastrar Novo Aluno -----------------");
		printf("\n\n\t   Código do aluno: %d\n", var_aluno.cod_aluno);
		do {
			printf("\t   Nome do aluno: ");
			scanf("%41[^\n]", nome_auxiliar);
			limpa_buffer();
		}while(!(valida_nome(nome_auxiliar)));
		strcpy(var_aluno.nome, nome_auxiliar);

		do{
			printf("\n\t   Digite:");
			printf("\n\n\t   <1> Iniciante\n\t   <2> Intermediário\n\t   <3> Avançado\n");
			printf("\n\t   Opção: ");
			scanf("%2[^\n]", estagio_auxiliar);
			limpa_buffer();
		}while(!(valida_estagio(estagio_auxiliar)));
		var_aluno.estagio = converte_estagio_em_int(estagio_auxiliar);

		fwrite(&var_aluno, sizeof(aluno), 1, arq);
		printf("\n\t  **** Aluno gravado com sucesso! ****\n");
		fclose(arq);

		do{
			printf("\n\t   Deseja cadastrar outro aluno? [S/N]: ");
			scanf("%2[^\n]", str_auxiliar);
			limpa_buffer();
			if(str_auxiliar[0] == 83 || str_auxiliar[0] == 78){ // transformarr 'S' em 's' ou 'N' em 'n'
				str_auxiliar[0] += 32;
			}
		}while(!(valida_opcao_cadastrar_outro_aluno(str_auxiliar)));
		op = str_auxiliar[0];

	}while(op != 'n' );
	printf("\n\n");
}

void listar_alunos(void){

	char estagio_auxiliar[3]="";

	printf("\n\t   ---------------- Listar aluno(s) de um estágio  ---------------\n\n");
	printf("\t   Digite:");
	printf("\n\n\t   <1> Iniciante\n\t   <2> Intermediário\n\t   <3> Avançado\n");
	printf("\n\t   Opção: ");
	scanf("%2[^\n]", estagio_auxiliar);
	limpa_buffer();

	if (valida_estagio(estagio_auxiliar)){
		char * estagios[] = {"Iniciante", "Intermediário", "Avançado"};
		int leitura_estagio;
		char str_auxiliar[3] = "";
		char op;
		FILE * arq, * out;
		aluno var_aluno;
		leitura_estagio = converte_estagio_em_int(estagio_auxiliar);
		printf("\n");

		if ((arq = fopen(ARQ_ALUNOS_BIN, "rb")) == NULL) {
			printf("\n\t   **** Não há nenhum aluno cadastrado! ****\n\n");
			return;
		}
		printf("\t------------------------------------------------------------------\n");
		printf("\t#Código | Nome do aluno                            | Estágio      \n");
		printf("\t------------------------------------------------------------------\n");
		while (fread(&var_aluno, sizeof(aluno), 1, arq) > 0) {
			if (var_aluno.estagio == leitura_estagio){
				printf("\t%07d | %-40.40s | %s\n", var_aluno.cod_aluno, var_aluno.nome, estagios[var_aluno.estagio - 1]);
			}
		}
		printf("\t------------------------------------------------------------------\n");

		do{
			printf("\n\t   Deseja gerar um relatório? [S/N]: ");
			scanf("%2[^\n]", str_auxiliar);
			limpa_buffer();
			if(str_auxiliar[0] == 83 || str_auxiliar[0] == 78){ // transformarr 'S' em 's' ou 'N' em 'n'
				str_auxiliar[0] += 32;
			}
		}while(!(valida_opcao_cadastrar_outro_aluno(str_auxiliar)));

		op = str_auxiliar[0];

		if(op == 's'){
			if ((out = fopen(ARQ_LISTAGEM_DE_ALUNOS_TXT, "w")) == NULL) {
				fprintf(stderr, "\t   Erro: não foi possível abrir o arquivo %s!\n", ARQ_LISTAGEM_DE_ALUNOS_TXT);
				fclose(arq);
				return;
			}
			fprintf(out, "\t------------------------------------------------------------------\n");
			fprintf(out, "\t#Código | Nome do aluno                            | Estágio      \n");
			fprintf(out,"\t------------------------------------------------------------------\n");
			rewind(arq);
			while (fread(&var_aluno, sizeof(aluno), 1, arq) > 0) {
				if (var_aluno.estagio == leitura_estagio){
					fprintf(out, "\t%07d | %-40.40s | %s\n", var_aluno.cod_aluno, var_aluno.nome, estagios[var_aluno.estagio - 1]);
				}
			}
			fprintf(out,"\t------------------------------------------------------------------\n");

			fclose(out);
			printf("\n\t   Arquivo %s gerado com  sucesso!\n", ARQ_LISTAGEM_DE_ALUNOS_TXT);
		}
		fclose(arq);
	}
	printf("\n");
}

void alterar_aluno(void){

	FILE * arq;
	aluno var_aluno;
	int codigo = 0;
	char estagio_auxiliar[3] = ""; // auxiliar para validações
	char nome_auxiliar[42] = ""; // 42 para validar se o nome digitado possui apenas 40 caracteres
	char * estagios[] = {"Iniciante", "Intermediário", "Avançado"};

	if ((arq = fopen(ARQ_ALUNOS_BIN, "rb+")) == NULL) {
		printf("\n\t   **** Não há nenhum aluno cadastrado! ****\n\n");
		return;
	}
	printf("\n\t   ------------------ Alterar dados de um aluno -------------\n\n");

	printf("\t   Código do aluno: ");
	scanf("%d", &codigo);
	limpa_buffer();

	fseek(arq, (codigo - 1) * sizeof(aluno), SEEK_SET);
	fread(&var_aluno, sizeof(aluno), 1, arq);

	if (codigo > 0 && !feof(arq) && codigo == var_aluno.cod_aluno ) { //codigo >= 2.106.185.887 as vezes gera um erro que modifica o codigo, por isso foi utilizado a expressao: codigo == var_aluno.cod_aluno
		printf("\n\t   Nome do aluno: %s\n", var_aluno.nome);
		printf("\t   Estágio: %s\n", estagios[var_aluno.estagio - 1]);

		do {
			printf("\n\t   Novo nome do aluno: ");
			scanf("%41[^\n]", nome_auxiliar);
			limpa_buffer();
		}while(!(valida_nome(nome_auxiliar)));
		strcpy(var_aluno.nome, nome_auxiliar);

		do{
			printf("\n\t   Digite:");
			printf("\n\n\t   <1> Iniciante\n\t   <2> Intermediário\n\t   <3> Avançado\n");
			printf("\n\t   Opção: ");
			scanf("%2[^\n]", estagio_auxiliar);
			limpa_buffer();
		}while(!(valida_estagio(estagio_auxiliar)));
		var_aluno.estagio = converte_estagio_em_int(estagio_auxiliar);
		var_aluno.cod_aluno = codigo;

		fseek(arq, -sizeof(aluno), SEEK_CUR); // volta para o início da estrutura anterior
		fwrite(&var_aluno, sizeof(aluno), 1, arq);
		printf("\n\t   **** Alterações realizadas com sucesso! ****\n\n");
	}
	else {
		printf("\n\t   Erro: não há nenhum aluno cadastrado com esse código.\n\n\n");
	}
	fclose(arq);
}


void consultar_aluno(void){

	FILE * arq, * arq2;
	aluno var_aluno;
	avaliacao ava;
	int codigo = 0;
	char estagio_auxiliar[3] = "";
	char nome_auxiliar[42] = ""; // 42 para validar se o nome digitado possui apenas 40 caracteres
	char * estagios[] = {"Iniciante", "Intermediário", "Avançado"};

	if ((arq = fopen(ARQ_ALUNOS_BIN, "rb+")) == NULL) {
		printf("\n\t   **** Não há nenhum aluno cadastrado! ****\n\n");
		return;
	}
	if ((arq2 = fopen(ARQ_AVALIACOES_BIN, "rb")) == NULL) {
		fprintf(stderr, "\n\t   **** Não há nenhuma avaliação cadastrada. ****\n\n\n");
		fclose(arq);
		return;
	}
	printf("\n\t   ------------------ Consultar um aluno -------------------\n\n");

	printf("\t   Código do aluno: ");
	scanf("%d", &codigo);
	limpa_buffer();

	fseek(arq, (codigo - 1) * sizeof(aluno), SEEK_SET);
	fread(&var_aluno, sizeof(aluno), 1, arq);

	if (codigo > 0 && !feof(arq) && codigo == var_aluno.cod_aluno ) { //codigo >= 2.106.185.887 as vezes gera um erro que modifica o codigo, por isso foi utilizado a expressao: codigo == var_aluno.cod_aluno
		printf("\n\t   Nome do aluno: %s\n", var_aluno.nome);
		printf("\t   Estágio: %s\n\n", estagios[var_aluno.estagio - 1]);
		printf("\t   ------------------------\n");
		printf("\t        Data     |   Nota  \n");
		printf("\t   ------------------------\n");
		while (fread(&ava, sizeof(avaliacao), 1, arq2) > 0) {
			if (ava.cod_aluno == codigo){
				printf("\t    %02d/%02d/%04d   |   %-2.2f \n", ava.data.dia, ava.data.mes, ava.data.ano, ava.nota);
			}
		}
		printf("\t   ------------------------\n");
	}
	else {
		printf("\n\t   Erro: não há nenhum aluno cadastrado com esse código.\n\n\n");
	}
	fclose(arq);
	fclose(arq2);
	printf("\n\n");
}


void cadastrar_avaliacoes(void){

	FILE * arq, * arq2;
	avaliacao ava;
	aluno var_aluno;
	int codigo = 0;
	float nota = 0;
	char aux_nota[10];
	char data_auxiliar[12] = "";
	char * estagios[] = {"Iniciante", "Intermediário","Avançado"};

	if ((arq = fopen(ARQ_ALUNOS_BIN, "rb")) == NULL) {
		printf("\n\t   **** Não há nenhum aluno cadastrado! ****\n\n");
		return;
	}

	printf("\n\t   ------------------- Cadastrar Avaliação ------------------\n\n");

	printf("\t   Código do aluno: ");
	scanf("%d", &codigo);
	limpa_buffer();

	fseek(arq, (codigo - 1) * sizeof(aluno), SEEK_SET);
	fread(&var_aluno, sizeof(aluno), 1, arq);

	if (codigo > 0 && !feof(arq) && codigo == var_aluno.cod_aluno ) { //codigo >= 2.106.185.887 as vezes gera um erro que modifica o codigo, por isso foi utilizado a expressao: codigo == var_aluno.cod_aluno

		if ((arq2 = fopen(ARQ_AVALIACOES_BIN, "ab")) == NULL) {
			fprintf(stderr, "Erro: não foi possível abrir o arquivo %s!\n", ARQ_AVALIACOES_BIN);
			fclose(arq);
		return;
		}

		printf("\n\t   Nome do aluno: %s\n", var_aluno.nome);
		printf("\t   Estágio: %s\n\n", estagios[var_aluno.estagio-1]);

		do {
			printf("\t   Digite uma data no formato dd/mm/aaaa: ");
			scanf("%d/%d/%d",&ava.data.dia, &ava.data.mes, &ava.data.ano );
			limpa_buffer();
		}while(!(valida_data(ava.data)));

		do {
			printf("\n\t   Digite uma nota entre 0 e 10: ");
			scanf("%9[^\n]",aux_nota);
			limpa_buffer();
		}while(!(valida_nota(aux_nota,&nota)));

		ava.nota = nota;
		ava.cod_aluno = codigo;

		fwrite(&ava, sizeof(avaliacao), 1, arq2);
		printf("\n\t   **** Avaliação gravada com sucesso! ****\n\n\n");

		fclose(arq2);

	}
	else {
		printf("\n\t   Erro: não há nenhum aluno cadastrado com esse código.\n\n\n");
	}
	fclose(arq);
}

void listar_avaliacoes(void){

	FILE * arq, * arq2;
	avaliacao ava;
	aluno var_aluno;
	tdata data1, data2, data_aux;
	bool nenhuma_data = true;
	char data_auxiliar[12] = "";
	char nome[41];

	if ((arq = fopen(ARQ_ALUNOS_BIN, "rb")) == NULL) {
		printf("\n\t   **** Não há nenhum aluno cadastrado! ****\n\n");
		return;
	}
	if ((arq2 = fopen(ARQ_AVALIACOES_BIN, "rb")) == NULL) {
		fprintf(stderr, "\n\t   **** Não há nenhuma avaliação cadastrada! ****\n\n\n");
		fclose(arq);
		return;
	}

	printf("\n\t   ------------------- Listar Avaliações -------------------\n\n");

	do {
		printf("\t   Digite uma data no formato dd/mm/aaaa: ");
		scanf("%d/%d/%d",&data1.dia, &data1.mes, &data1.ano );
		limpa_buffer();
	}while(!(valida_data(data1)));

	do {
		printf("\t   Digite uma outra data no formato dd/mm/aaaa: ");
		scanf("%d/%d/%d",&data2.dia,&data2.mes,&data2.ano );
		limpa_buffer();
	}while(!(valida_data(data2)));

	if (compara_datas(data1, data2) == 1){
		data_aux = data1;
		data1 = data2;
		data2 = data_aux;
	}

	while (fread(&ava, sizeof(avaliacao), 1, arq2) > 0) {
		if (!(compara_datas(ava.data, data1) == -1 || compara_datas(ava.data, data2) == 1)){
			if (nenhuma_data){ // apenas para imprimir o menu se existir avaliação dentro do período
				printf("\n\t --------------------------------------------------------------");
				printf("\n\t                      Nome                |    Data    | Nota");
				printf("\n\t --------------------------------------------------------------\n");
			}
			nenhuma_data = false;
			fseek(arq, (ava.cod_aluno - 1) * sizeof(aluno), SEEK_SET);
			fread(&var_aluno, sizeof(aluno), 1, arq);
			strcpy(nome, var_aluno.nome);
			printf("\t %-40.40s | %02d/%02d/%04d | %.2f\n", nome, ava.data.dia, ava.data.mes, ava.data.ano, ava.nota);
		}
	}
	if (nenhuma_data){
		printf("\n\t   **** Nenhuma data encontrada nesse período. ****\n\n\n");
	}else{
		printf("\t --------------------------------------------------------------\n\n\n");
	}
	fclose(arq);
	fclose(arq2);
}

int compara_datas(tdata dt1, tdata dt2){

	if(dt1.ano > dt2.ano){
		return 1;
	}
	if (dt1.ano < dt2.ano){
		return -1;
	}
	if(dt1.mes > dt2.mes){
		return 1;
	}
	if (dt1.mes < dt2.mes){
		return -1;
	}
	if(dt1.dia > dt2.dia){
		return 1;
	}
	if (dt1.dia < dt2.dia){
		return -1;
	}
	return 0;
}

bool valida_nome(const char * nome){
	int i = 0;
	while(nome[i] != '\0'){
		i++;
	}
	if(i == 0 || i > 40){ //i == 0: o nome está em branco; i > 40: estourou a capacidade de 41 caracteres contando o '\n'
		printf("\t   Erro: digite um nome com até 40 caracteres.\n");
		return false;
	}
	return true;
}

void limpa_buffer(){
	char c;
	while((c=getchar())!= '\n');
}

bool valida_opcao_menu(const char * str){

	if (str[0] < '0' || str[0] > '6' || str[1] != '\0'){
		printf("\n\t   **** Opção inválida! ****\n\n\n");
		return false;
	}
	return true;
}

void imprime_menu(){

	printf("\t   |--------------------------------------------------------|\n");
	printf("\t   |\t\t       CADASTRO DE ALUNOS                   |       \n");
	printf("\t   |--------------------------------------------------------|\n");
	printf("\t   | <1> Cadastrar alunos                                   |\n");
	printf("\t   | <2> Listar os alunos de um estágio cadastrado          |\n");
	printf("\t   | <3> Alterar um aluno                                   |\n");
	printf("\t   | <4> Consultar um aluno                                 |\n");
	printf("\t   | <5> Cadastrar avaliações                               |\n");
	printf("\t   | <6> Listar avaliações                                  |\n");
	printf("\t   | <0> Sair do sistema                                    |\n");
	printf("\t   |--------------------------------------------------------|");
	printf("\n\t    Opção: ");
}

bool valida_estagio(const char * str){

	if (str[0] < '1' || str[0] > '3' || str[1] != '\0'){
		printf("\n\t   Opção inválida para o estágio!\n");
		return false;
	}
	return true;
}

int converte_estagio_em_int(const char * str){

	if(str[0] == '1'){
		return 1;
	}
	else if(str[0] == '2'){
		return 2;
	}
	return 3;
}

bool valida_opcao_cadastrar_outro_aluno(const char * str){

	if ((str[0] == 's' || str[0] == 'n' )&& str[1] == '\0'){
		return true;
	}
	printf("\n\t   Erro: digite apenas 'S' ou 'N'\n\t   Obs: sem distinção entre maiúsculas e minúsculas.\n");
	return false;
}

bool valida_data(tdata data){

	if (data.dia < 1 || data.mes < 1 || data.mes >12 || data.ano < 1997){ // data mí­nima 01/01/1997
		printf("\t   Erro: data inválida. Digite novamente (data mínima: 01/01/1997).\n");
		return false;
	}
	if (data.mes == 1 || data.mes == 3 || data.mes == 5 || data.mes == 7 || data.mes == 8 || data.mes == 10  || data.mes == 12  ){
		if(data.dia > 31 ){ // meses com 31 dias
			printf("\t   Erro: data inválida. Digite novamente (data mínima: 01/01/1997).\n");
			return false;
		}
	}else if (data.mes == 4 || data.mes == 6 || data.mes == 9 || data.mes == 11  ){ // meses com 30 dias
		if(data.dia > 30 ){
			printf("\t   Erro: data inválida. Digite novamente (data mínima: 01/01/1997).\n");
			return false;
		}
	}else{//fevereiro
		bool bissexto = data.ano % 4 == 0;
		if (data.dia > 28 + bissexto){
			printf("\t   Erro: data inválida. Digite novamente (data mínima: 01/01/1997).\n");
			return false;
		}
	}

	return true;
}

bool valida_nota(const char * str , float * nota){

	int i = 0,ponto=0;

	for (i=0;str[i]!= '\0';i++ ){

		if(ponto == 2){
			printf("\t   Erro: Nota inválida\n");
			return false;
		}
		if (str[i] == '.'){
			ponto++;
		}
		else if (str[i] < '0' || str[i] > '9'){
			printf("\t   Erro: Nota inválida\n");
			return false;
		}
	}

	if(i == 0 ){ // nao foi digitado nenhum valor para a nota
		printf("\t   Erro: Nota inválida\n");
		return false;
	}

	*nota = atof(str);
	if(*nota < 0 || *nota > 10){
		printf("\t   Erro: Nota inválida\n");
		return false;
	}
	return true;

}
