#ifndef __MAQUINA__H
#define __MAQUINA__H

//Cores que indicam os estados da máquina:
//GREEN = livre e RED = ocupada
#define GREEN 0
#define RED 1

//"Nickname" da struct maquina
//Equip de equipamento
typedef struct maquina Equip;

//Instancia maquina torno quando utilizada
Equip* MaqTorno(void *peca, float tFinalProc);

//Instancia maquina fresa quando utilizada
Equip* MaqFresa(void *peca, float tFinalProc);

//Instancia maquina madril quando utilizada
Equip* MaqMandril(void *peca, float tFinalProc);

//Cria uma das máquinas passando a função como parâmetro
//Utiliza ponteiro para função para aplicar isso
Equip* GeraMaq(Equip* (*TipoMaq)(void*, float), void *peca, float tFinalProc);

/*Atualiza a peça e tempo de processamento de uma máquina instânciada para
ser reutilizada*/ 
void Atualizar_Maq(Equip* Maq, void *peca, float tFinalProc);

//Libera a instância da máquina porque não está sendo utilizada (just in time)
void liberar_Maq(Equip* Maq);

//Retorna o elemento/peça(void*) armazenado na máquina
void* obterPeca(Equip* Maq);

//Retorna o tempo de final de processamento da peça na máquina
float tFinalProc_peca(Equip* Maq);

//Retorna o tipo da máquina
char tipoMaq(Equip* Maq);

//Retorna 1 para v1>v2 e 0 caso contrário
int compara_tFimProcesso(void *v1, void *v2);

//Imprime informações da máquina passada como parâmetro
void imprime_maq(void *Maq);

#endif