#ifndef __FILAPR__H
#define __FILAPR__H

//Fazendo o typedef da struct lista que o usuário vai declarar no principal
typedef struct lista Fila;

//Criar a fila
Fila* criar_fila();

//Liberar a Fila criada
void libera_fila(Fila *lst);

//Tamanho da Fila
int tamanho_fila(Fila *lst);

//Inserir na Fila que no caso é uma fila de prioridades
//Retorna 1 se inseriu corretamente e 0 caso não tenha conseguido inserir
int inserir_fila(Fila *lst, void *info, int prior);

//Remover na fila
//Retorna 1 se inseriu corretamente e 0 caso não tenha conseguido inserir
int remover_fila(Fila *lst);

//Pegar um elemento na posição passada como parâmetro
//No caso retorna o ponteiro do elemento void*
void* obterPos(Fila *lst, int pos);

//Pega as informações do elemento da primeira posição da fila
void* obterIni(Fila *lst);

//Imprime os elementos da lista passando como parâmetro o ponteiro de uma função
void imprime_fila(Fila *lst, void (*printLst)(void*));

#endif