#include <stdio.h>
#include <stdlib.h>
#include "listaO.h"


//Definindo a struct nó de uma lista duplamente encadeada
typedef struct nodo 
{
	void *elem;
	struct nodo *prox, *ant;
}No;

//Definindo a struct lista que conterá os nós
//typedef da struct lista está no listaG.h porque o usuário deve ter acesso para declarar o tipo lista
struct lista 
{
	int tam;
	No *ini, *fim;
	int (*compara)(void*, void*);
};


Lista* criarLista(int (*compara)(void*, void*))
{
	//Declarar um ponteiro do tipo lista e alocar dinamicamente
	Lista *lst = (Lista*) malloc(sizeof(Lista));
	
	//Checar se foi alocado corretamente
	if (lst == NULL)
		exit(1); //Interrompe a execução do programa porque deu erro
	
	lst->tam = 0;  //A lista começa sem elementos
	lst->ini = lst->fim = NULL; //Tanto inicio como final da fila apontam para o nulo
	lst->compara = compara; //Recebe o ponteira para função compara passada como parâmetro
	
	return lst;
}


void destruirLista(Lista *lst) 
{
	//Criar um ponteiro auxiliar para percorrer a lista e liberá-la por completo
	No *noAux;
	
	if (lst != NULL) 
	{
		//Percorrer toda a lista até que a própria cabeça da lista aponte para o nulo
		//Basicamente tudo aquilo que é dado malloc é dado free
		while (lst->ini != NULL) 
		{
			noAux = lst->ini;  
			lst->ini = lst->ini->prox;
			free(noAux->elem);
			free(noAux);
		}
		free(lst);
	}
}


int tamanhoLista(Lista *lst) 
{
	if (lst != NULL)
		return lst->tam;
	
	return -1;
}


int inserirLista(Lista *lst, void *elem) 
{
	//Caso a lista não tenha sido alocada corretamente
	if (lst == NULL)
		return 0;
	
	//Aloca dinamicamente o nó que conterá o elemento e fará parte da lista
	No *noInser = (No*) malloc(sizeof(No));
	
	noInser->elem = elem; //Atribuindo o elemento no nó
	
	//Caso a lista não contém nós então tanto o inicio quanto o fim da lista apontam para o elemento inserido
	if (lst->tam == 0) 
	{
		noInser->prox = noInser->ant = NULL;
		lst->ini = lst->fim = noInser;
	}
	
	//Senão inserir de maneira ordenada na lista
	else 
	{
		//Colocar no final da lista
		//Retorna 1 se elem > lst->fim->elem
		if (lst->compara(elem, lst->fim->elem)) 
		{
			noInser->prox = NULL;
			noInser->ant = lst->fim;
			lst->fim->prox = noInser;
			lst->fim = noInser;
		}
		//Colocar no início ou no meio da lista
		else 
		{
			No *noComp = lst->ini;
			
			//A comparação(lst->compara) retorna 0 caso ache a posição que o nó alocado deve entrar na lista
			while (lst->compara(elem, noComp->elem))
				noComp = noComp->prox;
			
			noInser->prox = noComp;
			noInser->ant = noComp->ant;
			noComp->ant = noInser;
			
			//Checar se é a primeira posição porque caso seja o nó anterior do noComp for nulo então deve ter essa condição
			if (noInser->ant == NULL)
				lst->ini = noInser;
			else
				noInser->ant->prox = noInser;
		}
	}
	
	lst->tam++; //Aumenta o tamanho da lista
	
	return 1;
}


int removerLista(Lista *lst, int pos) 
{
	//Caso a lista não tenha sido alocada corretamente
	if ((lst == NULL) || (lst->tam == 0) || (pos < 0 || pos > (lst->tam)-1))
		return 0;
	
	//Nó auxiliar que vai apontar para a posição que se deseja liberar(remover)
	No *noRem;
	
	//Se tiver somente um elemento na lista
	if (lst->tam == 1) 
	{
		noRem = lst->ini;
		lst->ini = lst->fim = NULL;
	}
	
	//Se for a última posição
	else if (pos == (lst->tam)-1) 
	{
		noRem = lst->fim;
		lst->fim = noRem->ant;
		lst->fim->prox = NULL;
	}
	
	//Remove em qualquer outra posição
	else 
	{
		noRem = lst->ini;
		//Loop para percorrer os nós da lista até a achar a posição que deseja remover
		for (int i=0; i < pos; i++) 
		{
			noRem = noRem->prox;
		}
		
		noRem->prox->ant = noRem->ant;
		
		//Caso seja nulo então está sendo removido no meio
		if (noRem->ant != NULL)
			noRem->ant->prox = noRem->prox;
		
		//Caso contrário é na primeira posição
		else
			lst->ini = noRem->prox;
	}
	
	
	free(noRem);
	lst->tam--;
	
	return 1;
}


void* obterLista(Lista *lst, int pos) 
{
	//Checar se foi alocado corretamente e se a posição passada existe
	if ((lst == NULL) || (pos < 0 || pos > (lst->tam)-1) || (lst->tam == 0))
		exit(2); //Aborda a chamada da função
	
	No *noAux = lst->ini;
	
	//Percorre a lista até a posição desejada e retorna o ponteiro void* do elemento
	for (int i=0; i < pos; i++) 
	{
		noAux = noAux->prox;
	}
	
	return noAux->elem;
}


void imprime_lst(Lista *lst, void (*printLst)(void*)) 
{
	if (lst == NULL || lst->tam == 0)
		return; //Não imprime nada e acaba logo a chamada da função
	
	//Cria um nó auxiliar para percorrer a lista
	No *noAux = lst->ini;
	
	//Percorre a lista até o nó auxiliar atingir o nulo
	while (noAux != NULL) 
	{
		//Chama o ponteiro para função passada como parâmetro onde printa o elem que o void* aponta
		printLst(noAux->elem);
		noAux = noAux->prox;
	}
}






