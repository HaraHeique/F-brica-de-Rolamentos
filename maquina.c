#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "maquina.h"


//Struct que contém as informações das máquinas
struct maquina 
{
	char nome[10], tipo; //Tipo é o caractere que representa a máquina
	int estado; //Se máquina está livre(0) ou ocupada(1)
	float tFinalProces; //Tempo final que a peça é processada na máquina
	void *peca; //Peça genérica que está sendo processada na máquina
};


/*OBS.: As máquinas não são instanciadas desde o início do programa e sim somente quando
são utilizadas. São como se fosse máquinas just in time*/
Equip* MaqTorno(void *peca, float tFinalProc) 
{
	//Alocação da instância da máquina torno
	Equip *maqT = (Equip*) malloc(sizeof(Equip));
	
	strcpy(maqT->nome, "Torno");
	maqT->tipo = 'T';
	maqT->estado = RED; //Como é instanciada quando utilizada então vai para RED = ocupada
	maqT->tFinalProces = tFinalProc;
	maqT->peca = peca;
	
	return maqT;
}


Equip* MaqFresa(void *peca, float tFinalProc) 
{
	//Alocação da instância da máquina fresa
	Equip *maqF = (Equip*) malloc(sizeof(Equip));
	
	strcpy(maqF->nome, "Fresa");
	maqF->tipo = 'F';
	maqF->estado = RED; //Como é instanciada quando utilizada então vai para RED = ocupada
	maqF->tFinalProces = tFinalProc;
	maqF->peca = peca;
	
	return maqF;
}


Equip* MaqMandril(void *peca, float tFinalProc) 
{
	//Alocação da instância da máquina mandril
	Equip *maqM = (Equip*) malloc(sizeof(Equip));
	
	strcpy(maqM->nome, "Mandril");
	maqM->tipo = 'M';
	maqM->estado = RED; //Como é instanciada quando utilizada então vai para RED = ocupada
	maqM->tFinalProces = tFinalProc;
	maqM->peca = peca;
	
	return maqM;
}


Equip* GeraMaq(Equip* (*TipoMaq)(void*, float), void *peca, float tFinalProc) 
{
	return TipoMaq(peca, tFinalProc);
}


//Atualizar a máquina em casos de reutilização
void Atualizar_Maq(Equip* Maq, void *peca, float tFinalProc) 
{
	//Altera somente a peça de fabricação e o tempo de processamento final dela
	Maq->peca = peca;
	Maq->tFinalProces = tFinalProc;
}


//Máquina não utilizada é liberada da memória
void liberar_Maq(Equip* Maq) 
{
	free(Maq);
}


//Retorna peça que está dentro da máquina passada como parâmetro
void* obterPeca(Equip* Maq) 
{
	return Maq->peca;
}


//Retorna o tempo de produção final da determinada peça
float tFinalProc_peca(Equip* Maq) 
{
	return Maq->tFinalProces;
}


//Retorna o tipo da máquina
char tipoMaq(Equip* Maq) 
{
	return Maq->tipo;
}


//Compara o tempo final de processamento da peça na máquina
//Essa função é passada como parâmetro tendo um ponteiro para função que aponta para ela quando chamada
int compara_tFimProcesso(void *v1, void *v2) 
{
	Equip *tMaq1 = v1, *tMaq2 = v2;
	
	//Caso t1 de processamento seja maior retorna 1 caso contrário 0
	if (tMaq1->tFinalProces >= tMaq2->tFinalProces)
		return 1;
	else
		return 0;
}


//Impressão do máquina
void imprime_maq(void *Maq) 
{
	Equip *maqAux = Maq;
	
	printf("%c    %.2f\n", maqAux->tipo, maqAux->tFinalProces);
}
