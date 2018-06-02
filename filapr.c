#include <stdio.h>
#include <stdlib.h>
#include "filapr.h"



//Struct nó que armazena os nós que existem dentro da lista
struct nodo 
{
	void *info; //Armazena os itens(informações) que estarão dentro do nó
	int prior; //Indica a prioridade do pedido realizado
	struct nodo *prox, *ant; //Ponteiros que apontam para o próximo e anterior nó da lista
};

//Struct lista que é a lista em si
struct lista 
{
	int tam; //Inteiro que armazena o tamanho da lista
	struct nodo *ini, *fim; //São os ponteiros diferenciais da lista. Apontam a início(cabeça) e o final da lista(último elemento)
};

typedef struct nodo No;


//Criar a lista onde tanto o ponteiro de início e fim apontam para o nulo
Fila* criar_fila() 
{
	Fila *lst; //Definindo um ponteiro do tipo Fila
	lst = (Fila*) malloc(sizeof(Fila)); //Fazendo a alocação dinâmica da lista
	
	//Checar se houve a alocação de memória corretamente
	if (lst == NULL)
	{
		exit(1); //Aborda o programa
	}
	
	lst->ini = lst->fim = NULL; //Tanto o inicio quanto o fim da lista apontam para o NULL
	lst->tam = 0; //Coloca o tamanho da lista para 0;
	
	return lst;
}


//Liberar a lista depois de ser utilizada pois ela foi alocada dinamicamente na memória
void libera_fila(Fila *lst) 
{
	//Checar primeiramente se a lista está foi alocada corretamente
	if (lst != NULL) 
	{
		No *noAux; //Cria um nó auxiliar que percorra pelo nós da lista e irá liberar os nós na determinada posição de memória
		
		//Percorrer toda a lista e liberá-la
		while (lst->ini != NULL) 
		{
			noAux = lst->ini; //Recebe o nó que o inicio da lista está apontando
			//O início da lista aponta para o próximo nó
			//Poderia ser abaixo também lst->ini = noAux->prox
			lst->ini = lst->ini->prox; 
			free(noAux->info); //Liberar também a memória alocada do campo informação
			free(noAux); //Vai liberando todos os nós da lista
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
	//Caso a lista não seja alocada retorna 0, pois não é possível alocar nada
	if (lst == NULL)
		return 0;
	
	//Aloca o nó que conterá as informações para ser colocada na lista que será uma fila de prioridades
	No *no_inser = (No*) malloc(sizeof(No));
	
	//Coloca o ponteiro do conteudo passado como parâmetro no nó alocado dinamicamente
	no_inser->info = info;
	no_inser->prior = prior; //Coloca a prioridade no nó criado
	
	//Checar se a lista(fila) está vazia ou não
	//Caso esteja fazia só é necessário colocar o elemento na lista
	if (lst->tam == 0) 
	{
		no_inser->prox = no_inser->ant = NULL; //Como é o primeiro então sempre apontará para o nulo
		lst->ini = lst->fim = no_inser; //Tanto o inicio quanto o final da lista apontarão para o único nó da lista
	}
	
	//Se a prioridade é a menor de todas ou igual a última da fila entao será inserida no final da fila
	else if (prior <= lst->fim->prior)	
	{
		no_inser->ant = lst->fim; //O nó de inserção será colocado no final da fila
		no_inser->prox = NULL; //Como será o último apontará para o nulo
		lst->fim->prox = no_inser; //O penúltimo nó apontará para o último agora
		lst->fim = no_inser; //E o ponteiro do fim da lista aponta para o novo último nó da fila
	}
		
	//Senão a prioridade está acima do última da fila
	else
	{
		No *noAux; //Cria um nó auxiliar para percorrer a fila e comparar as prioridades dos nós da fila
		noAux = lst->ini; //Recebe o nó apontado pelo início da fila
			
		//Percorre a fila até que ache um prioridade menor que a passada como parâmetro
		while (noAux->prior >= prior)
		{
			noAux = noAux->prox;
		}
		
		//Se o nó auxiliar parar em uma determinada posição então quer dizer ele deve entrar nela
		No *noAux_prev = noAux->ant; //Um nó auxiliar que recebe o anterior do nó auxiliar
				
		no_inser->prox = noAux; //O ponteiro próximo do nó inserido aponta para onde o nó auxiliar parou
		no_inser->ant = noAux_prev; //O ponteiro anterior aponta para o nó anterior ao nó auxiliar
		noAux->ant = no_inser; //O ponteiro anterior do nó auxiliar aponta para o nó de inserção
				
		if (noAux_prev != NULL)
			//O ponteiro prox do nó anterior ao nó auxiliar aponta para o de inserção
			noAux_prev->prox = no_inser;
				
		else
			lst->ini = no_inser;
	}
	
	lst->tam++; //Aumenta mais um no tamanho da lista
	
	return 1;
}


//Remover o primeiro nó de uma fila, pois na fila o primeiro a entrar é também o primeiro a sair
int remover_fila(Fila *lst) 
{
	//Checar se a houve problema de alocação ou a lista está vazia então não tem como remover
	if ((lst == NULL) || (lst->tam == 0))
		return 0;
	
	//O nó remoção irá receber o primeiro nó da lista que será removido
	No *no_rem = lst->ini; 
	
	//Se houver somente um elemento na lista tanto o início quanto fim apontam para nulo
	if (lst->tam == 1)
		lst->ini = lst->fim = NULL;
	
	//Senão há mais de um nó na lista
	else 
	{
		lst->ini = no_rem->prox; //O inicio da lista aponta para o próximo nó
		lst->ini->ant = NULL; //E o ponteiro anterior do nó do inicio da lista aponta para o nulo
	}
	
	free(no_rem);  //Libera o nó de remoção
	lst->tam--; //Diminui o tamanho da lista
	
	return 1;
}


//Pega o campo info do void* da posição desejada e retorna ele 
void* obterPos(Fila *lst, int pos) 
{
	if ((lst == NULL) || (pos < 0 || pos > (lst->tam)-1) || (lst->tam == 0))
		exit(2);  //Aborda programa
	
	No *noAux = lst->ini; //Recebe o nó inicial da lista
	
	//Percorre a lista até a posição passada como parâmetro
	for(int i=0; i!=pos; i++) 
	{
		noAux = noAux->prox;
	}
	
	//Retorna o void* no campo info da struct nodo
	return noAux->info;
}


//Pega o campo info do void* da posição desejada e retorna ele 
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
		return; //Termina a execução da função porque não será impresso nada
	
	//Declarar um nó auxiliar para percorrer a lista
	No *noAux = lst->ini;
	
	//Imprimir todos os elementos da lista até que o nó auxiliar atinja o nulo
	while (noAux != NULL) 
	{
		printLst(noAux->info); //Chama o ponteiro da função passada como parâmetro e seu parâmetro que no caso é um void*
		noAux = noAux->prox;
	}
}





