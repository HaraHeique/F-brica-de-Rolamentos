#include <stdio.h>
#include <stdlib.h>
#include "filapr.h"



//Struct n� que armazena os n�s que existem dentro da lista
struct nodo 
{
	void *info; //Armazena os itens(informa��es) que estar�o dentro do n�
	int prior; //Indica a prioridade do pedido realizado
	struct nodo *prox, *ant; //Ponteiros que apontam para o pr�ximo e anterior n� da lista
};

//Struct lista que � a lista em si
struct lista 
{
	int tam; //Inteiro que armazena o tamanho da lista
	struct nodo *ini, *fim; //S�o os ponteiros diferenciais da lista. Apontam a in�cio(cabe�a) e o final da lista(�ltimo elemento)
};

typedef struct nodo No;


//Criar a lista onde tanto o ponteiro de in�cio e fim apontam para o nulo
Fila* criar_fila() 
{
	Fila *lst; //Definindo um ponteiro do tipo Fila
	lst = (Fila*) malloc(sizeof(Fila)); //Fazendo a aloca��o din�mica da lista
	
	//Checar se houve a aloca��o de mem�ria corretamente
	if (lst == NULL)
	{
		exit(1); //Aborda o programa
	}
	
	lst->ini = lst->fim = NULL; //Tanto o inicio quanto o fim da lista apontam para o NULL
	lst->tam = 0; //Coloca o tamanho da lista para 0;
	
	return lst;
}


//Liberar a lista depois de ser utilizada pois ela foi alocada dinamicamente na mem�ria
void libera_fila(Fila *lst) 
{
	//Checar primeiramente se a lista est� foi alocada corretamente
	if (lst != NULL) 
	{
		No *noAux; //Cria um n� auxiliar que percorra pelo n�s da lista e ir� liberar os n�s na determinada posi��o de mem�ria
		
		//Percorrer toda a lista e liber�-la
		while (lst->ini != NULL) 
		{
			noAux = lst->ini; //Recebe o n� que o inicio da lista est� apontando
			//O in�cio da lista aponta para o pr�ximo n�
			//Poderia ser abaixo tamb�m lst->ini = noAux->prox
			lst->ini = lst->ini->prox; 
			free(noAux->info); //Liberar tamb�m a mem�ria alocada do campo informa��o
			free(noAux); //Vai liberando todos os n�s da lista
		}
		free(lst); //Libera a lista
	}
}


//Retorna o tamanho da Fila
int tamanho_fila(Fila *lst) 
{
	if (lst != NULL)
		return lst->tam;
	
	return -1;
}


///Insere em uma fila de prioridades
int inserir_fila(Fila *lst, void *info, int prior) 
{
	//Caso a lista n�o seja alocada retorna 0, pois n�o � poss�vel alocar nada
	if (lst == NULL)
		return 0;
	
	//Aloca o n� que conter� as informa��es para ser colocada na lista que ser� uma fila de prioridades
	No *no_inser = (No*) malloc(sizeof(No));
	
	//Coloca o ponteiro do conteudo passado como par�metro no n� alocado dinamicamente
	no_inser->info = info;
	no_inser->prior = prior; //Coloca a prioridade no n� criado
	
	//Checar se a lista(fila) est� vazia ou n�o
	//Caso esteja fazia s� � necess�rio colocar o elemento na lista
	if (lst->tam == 0) 
	{
		no_inser->prox = no_inser->ant = NULL; //Como � o primeiro ent�o sempre apontar� para o nulo
		lst->ini = lst->fim = no_inser; //Tanto o inicio quanto o final da lista apontar�o para o �nico n� da lista
	}
	
	//Se a prioridade � a menor de todas ou igual a �ltima da fila entao ser� inserida no final da fila
	else if (prior <= lst->fim->prior)	
	{
		no_inser->ant = lst->fim; //O n� de inser��o ser� colocado no final da fila
		no_inser->prox = NULL; //Como ser� o �ltimo apontar� para o nulo
		lst->fim->prox = no_inser; //O pen�ltimo n� apontar� para o �ltimo agora
		lst->fim = no_inser; //E o ponteiro do fim da lista aponta para o novo �ltimo n� da fila
	}
		
	//Sen�o a prioridade est� acima do �ltima da fila
	else
	{
		No *noAux; //Cria um n� auxiliar para percorrer a fila e comparar as prioridades dos n�s da fila
		noAux = lst->ini; //Recebe o n� apontado pelo in�cio da fila
			
		//Percorre a fila at� que ache um prioridade menor que a passada como par�metro
		while (noAux->prior >= prior)
		{
			noAux = noAux->prox;
		}
		
		//Se o n� auxiliar parar em uma determinada posi��o ent�o quer dizer ele deve entrar nela
		No *noAux_prev = noAux->ant; //Um n� auxiliar que recebe o anterior do n� auxiliar
				
		no_inser->prox = noAux; //O ponteiro pr�ximo do n� inserido aponta para onde o n� auxiliar parou
		no_inser->ant = noAux_prev; //O ponteiro anterior aponta para o n� anterior ao n� auxiliar
		noAux->ant = no_inser; //O ponteiro anterior do n� auxiliar aponta para o n� de inser��o
				
		if (noAux_prev != NULL)
			//O ponteiro prox do n� anterior ao n� auxiliar aponta para o de inser��o
			noAux_prev->prox = no_inser;
				
		else
			lst->ini = no_inser;
	}
	
	lst->tam++; //Aumenta mais um no tamanho da lista
	
	return 1;
}


//Remover o primeiro n� de uma fila, pois na fila o primeiro a entrar � tamb�m o primeiro a sair
int remover_fila(Fila *lst) 
{
	//Checar se a houve problema de aloca��o ou a lista est� vazia ent�o n�o tem como remover
	if ((lst == NULL) || (lst->tam == 0))
		return 0;
	
	//O n� remo��o ir� receber o primeiro n� da lista que ser� removido
	No *no_rem = lst->ini; 
	
	//Se houver somente um elemento na lista tanto o in�cio quanto fim apontam para nulo
	if (lst->tam == 1)
		lst->ini = lst->fim = NULL;
	
	//Sen�o h� mais de um n� na lista
	else 
	{
		lst->ini = no_rem->prox; //O inicio da lista aponta para o pr�ximo n�
		lst->ini->ant = NULL; //E o ponteiro anterior do n� do inicio da lista aponta para o nulo
	}
	
	free(no_rem);  //Libera o n� de remo��o
	lst->tam--; //Diminui o tamanho da lista
	
	return 1;
}


//Pega o campo info do void* da posi��o desejada e retorna ele 
void* obterPos(Fila *lst, int pos) 
{
	if ((lst == NULL) || (pos < 0 || pos > (lst->tam)-1) || (lst->tam == 0))
		exit(2);  //Aborda programa
	
	No *noAux = lst->ini; //Recebe o n� inicial da lista
	
	//Percorre a lista at� a posi��o passada como par�metro
	for(int i=0; i!=pos; i++) 
	{
		noAux = noAux->prox;
	}
	
	//Retorna o void* no campo info da struct nodo
	return noAux->info;
}


//Pega o campo info do void* da posi��o desejada e retorna ele 
void* obterIni(Fila *lst) 
{
	if (lst->tam > 0 && lst != NULL)
		//Retorna o primeiro elemento da lista
		return lst->ini->info;
	
	exit(3); //Aborda o programa
}


void imprime_fila(Fila *lst, void (*printLst)(void*)) 
{
	if (lst == NULL || lst->tam == 0)
		return; //Termina a execu��o da fun��o porque n�o ser� impresso nada
	
	//Declarar um n� auxiliar para percorrer a lista
	No *noAux = lst->ini;
	
	//Imprimir todos os elementos da lista at� que o n� auxiliar atinja o nulo
	while (noAux != NULL) 
	{
		printLst(noAux->info); //Chama o ponteiro da fun��o passada como par�metro e seu par�metro que no caso � um void*
		noAux = noAux->prox;
	}
}





