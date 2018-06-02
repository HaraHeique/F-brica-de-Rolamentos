#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rolamento.h"


//Contem as informações necessárias de todos os rolamentos
struct rolamento 
{
	char nome[20], tipo[3], ordem[6]; //Tipo é a abreviação do nome do rolamento
	int prioridade, n_processos, proces_atual; //Quantos processamentos a peça tem e qual é o processo atual em que se encontra
	float tChegada, estadiaT, estadiaF, estadiaM; //Tempo chegada do rolamento e as estadias deles nas máquinas para produção
};


Rol* tipoCI(float tChegada) 
{
	//Alocação dinâmica do rolamento do tipo cilindrico
	Rol *rolCi = (Rol*) malloc(sizeof(Rol));
	
	//Atribuindo as informações aos campos
	rolCi->prioridade = LOW;
	rolCi->tChegada = tChegada;
	rolCi->estadiaT = 0.8; //Constante estadia no torno
	rolCi->estadiaF = 0.5; //Constante estadia na fresa
	rolCi->estadiaM = 1.2; //Constante estadia no mandril
	rolCi->n_processos = 4; //Passa por 4 máquinas até o fim da peça
	rolCi->proces_atual = 0; //Começa pelo torno
	strcpy(rolCi->nome, "Cilindrico");
	strcpy(rolCi->tipo, "ci");
	strcpy(rolCi->ordem, "TFTM");
	
	return rolCi;
}


Rol* tipoCO(float tChegada) 
{
	//Alocação dinâmica do rolamento do tipo conico
	Rol *rolCo = (Rol*) malloc(sizeof(Rol));
	
	//Atribuindo as informações aos campos
	rolCo->prioridade = MEDIUM;
	rolCo->tChegada = tChegada;
	rolCo->estadiaT = 1.8; //Constante estadia no torno
	rolCo->estadiaF = 0.0; //Constante estadia na fresa: 0.0 porque não vai para a fresa
	rolCo->estadiaM = 2.1; //Constante estadia no mandril
	rolCo->n_processos = 3; //Passa por 3 máquinas até o fim da peça
	rolCo->proces_atual = 0; //Começa pelo torno
	strcpy(rolCo->nome, "Conico");
	strcpy(rolCo->tipo, "co");
	strcpy(rolCo->ordem, "TMT");
	
	return rolCo;
}


Rol* tipoEA(float tChegada) 
{
	//Alocação dinâmica do rolamento do tipo aço
	Rol *rolEa = (Rol*) malloc(sizeof(Rol));
	
	//Atribuindo as informações aos campos
	rolEa->prioridade = HIGH;
	rolEa->tChegada = tChegada;
	rolEa->estadiaT = 1.0; //Constante estadia no torno
	rolEa->estadiaF = 0.5; //Constante estadia na fresa
	rolEa->estadiaM = 1.4; //Constante estadia no mandril
	rolEa->n_processos = 3; //Passa por 3 máquinas até o fim da peça
	rolEa->proces_atual = 0; //Começa pela fresa
	strcpy(rolEa->nome, "Esferico Aco");
	strcpy(rolEa->tipo, "ea");
	strcpy(rolEa->ordem, "FMT");
	
	return rolEa;
}


Rol* tipoET(float tChegada) 
{
	//Alocação dinâmica do rolamento do tipo titanio
	Rol *rolEt = (Rol*) malloc(sizeof(Rol));
	
	//Atribuindo as informações aos campos
	rolEt->prioridade = HIGH;
	rolEt->tChegada = tChegada;
	rolEt->estadiaT = 1.6; //Constante estadia no torno
	rolEt->estadiaF = 0.6; //Constante estadia na fresa
	rolEt->estadiaM = 1.5; //Constante estadia no mandril
	rolEt->n_processos = 5; //Passa por 5 máquinas até o fim da peça
	rolEt->proces_atual = 0; //Começa pela fresa
	strcpy(rolEt->nome, "Esferico Titanio");
	strcpy(rolEt->tipo, "et");
	strcpy(rolEt->ordem, "FMTFT");
	
	return rolEt;
}


Rol* tipoROL(Rol* (*CriaRol)(float), float tChegada) 
{
	return CriaRol(tChegada);
}


//Liberação do rolamento da alocação dinâmica
void liberar_rol(Rol* pRol) 
{
	free(pRol);
}


//Retorna a prioridade do rolamento
int prior_rol(Rol* pRol) 
{
	return pRol->prioridade;
}


//Retorna o tempo de chegada do rolamento
float tChegada_rol(Rol* pRol) 
{
	return pRol->tChegada;
}


//Retorna o tipo do rolamento
char* tipo_rolamento(Rol* pRol) 
{
	//Como pRol->tipo é um string então *tipo aponta para primeira posição dela
	char *tipo = pRol->tipo;
	return tipo;
}


//Retorna a máquina do processo atual
char ordProcesso_Atual(Rol* pRol) 
{
	int proces_atual = pRol->proces_atual; //Pega o valor do processo atual
	char maqAtual = pRol->ordem[proces_atual]; //atribui o caracter que representa a máquina do proce. atual
	
	return maqAtual;
}


float estadiaT_rol(Rol* pRol) 
{
	return pRol->estadiaT;
}


float estadiaF_rol(Rol* pRol) 
{
	return pRol->estadiaF;
}


float estadiaM_rol(Rol* pRol) 
{
	return pRol->estadiaM;
}


//Atualiza o processo de fabricação do rolamento para a próxima etapa
int atualizar_procesRol(Rol* pRol) 
{
	pRol->proces_atual++; //Passa para o próxima etapa do processo, ou seja, máquina de fabricação
	
	//Caso o processo atual seja menor que o número de processos totais então ainda há processos pedentes
	//até a fabricação do rolamento
	if (pRol->proces_atual < pRol->n_processos)
		return 1;
	
	return 0; //Não tem mais processos a serem realizados, ou seja, o rolamento foi fabricado
}


//Retorna um inteiro que indica se elemento é maior, menor ou igual
//Essa função é passada tendo um ponteiro para função que aponta para ela quando chamada
int compara_tchegada(void *v1, void *v2) 
{
	Rol *pRol1 = v1, *pRol2 = v2;
	
	if (pRol1->tChegada >= pRol2->tChegada)
		return 1;
	else
		return 0;
}


//Impressão do rolamento
void imprime_rol(void* pRol) 
{
	//Não há necessidade de casting para tipo rolamento porque esse ponteiro void* passado como parâmetro já aponta para uma struct do tipo rolamento
	Rol *pRol_Aux = pRol;  
	printf("%s         %d  %.2f\n", pRol_Aux->tipo, pRol_Aux->prioridade, pRol_Aux->tChegada);
}
