#ifndef __LISTAO__H
#define __LISTAO__H

//"Nickname" da struct lista
typedef struct lista Lista;

//Função para criar a lista
Lista* criarLista(int (*compara)(void*, void*));

//Função para liberar toda a alocação feita na lista
void destruirLista(Lista *lst);

//Função que retorna o tamanho da lista indicando quantos elementos ela possui
int tamanhoLista(Lista *lst);

//Função que insere de forma ordenada na lista
//Retorna 1 = inserido corretamente e 0 = não inseriu
int inserirLista(Lista *lst, void *elem);

//Função que remove o elemento da posição passada como parâmetro
//Retorna 1 = remoção com sucesso e 0 = caso contrário
int removerLista(Lista *lst, int pos);

//Obtem um elemento na posição desejada
void* obterLista(Lista *lst, int pos);

//É somente uma função auxiliar mesmo para imprimir a lista genérica ordenada
//Imprime a lista passando como parâmetro um ponteiro para função do tipo que se deseja imprimir
void imprime_lst(Lista *lst, void (*printLst)(void*));

#endif