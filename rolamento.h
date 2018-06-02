#ifndef __ROLAMENTO__H
#define __ROLAMENTO__H

//Definindo constantes para o tempo de chegada dos rolamentos
//Tempos do cilíndrico, cônico e dos esféricos respectivamente
#define TCIL 21.5  
#define TCON 19.1
#define TESF 8.0

//Abaixo estão os níveis de cada prioridade dos rolamentos cilindrico, cônico e esféricos respectivamente
#define LOW 1
#define MEDIUM 2
#define HIGH 3


//Criando um "nickname" para a struct rolamento
typedef struct rolamento Rol;

//Cria rolamento do tipo cilindrico
Rol* tipoCI(float tChegada);

//Cria rolamento do tipo conico
Rol* tipoCO(float tChegada);

//Cria rolamento do tipo esferico aco
Rol* tipoEA(float tChegada);

//Cria rolamento do tipo esferico titanio
Rol* tipoET(float tChegada);

//Cria um dos rolamentos passando a função como parâmetro
//Utiliza ponteiro para função do rolamento
Rol* tipoROL(Rol* (*CriaRol)(float), float tChegada);

//Liberar o rolamento por causa da alocação dinâmica
void liberar_rol(Rol* pRol);

//Retorna a prioridade do pedido feito
int prior_rol(Rol* pRol);

//Retorna o tempo de chegada do pedido feito
float tChegada_rol(Rol* pRol);

//Retorna o tipo do rolamento do pedido feito
//o tipo é uma string logo é retornado o ponteiro para char
char* tipo_rolamento(Rol* pRol);

//Retorna a maquina que o rolamento deve entrar a partir do seu processo atual
char ordProcesso_Atual(Rol* pRol);

//Retorna a constante de estadia no torno
float estadiaT_rol(Rol* pRol);

//Retorna a constate de estadia na fresa
float estadiaF_rol(Rol* pRol);

//Retorna a constante de estadia no mandril
float estadiaM_rol(Rol* pRol);

//Atualiza o processamento do rolamento para a próxima máquina
//Retorna 1 se conseguir atualizar e 0 para caso termine o processo
int atualizar_procesRol(Rol* pRol);

//Retorna 1 para v1>v2 e 0 caso contrário
int compara_tchegada(void *v1, void *v2);

//Impressão dos rolamentos dentro da fila correspondente
void imprime_rol(void* pRol);

#endif