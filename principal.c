#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "filapr.h"
#include "listaO.h"
#include "rolamento.h"
#include "maquina.h"

//Quantidade de filas para as máquinas. No caso são 3: Torno, Fresa e Mandril
#define NFILAS_MAQ 3 

//Quantidade de máquinas totais de cada tipo a fábrica dispõe a usar
//2 Tornos, 1 fresa e mandril
#define NT_TORNO 100
#define NT_FRESA 100
#define NT_MANDRIL 100


//Indicam quantas máquinas estão sendo usadas no momento
struct N_maquinasUso 
{
	unsigned int nTorno, nFresa, nMandril;
};

//Coleta as informações dos rolamentos produzidos
struct informacoesRol 
{
	//Número de rolamentos cilindrico, conico, esférico aço e titânico e tempo total deles
	unsigned int nCI, nCO, nEA, nET; 
	float tCI, tCO, tEA, tET;
};

//Protótipos de structs usadas no principal
typedef struct N_maquinasUso NEquipUso;
typedef struct informacoesRol InfoRol;


//Protótipos das funções usadas no principal
float chegadaPedido(float param);
float tempoMaquina(float param);
Fila** criarF_maquinas();
void pedidoRol_inicial(Lista *pedidosROL);
void maquinasUsadas_inicial(struct N_maquinasUso *maq);
int checa_atualiza_pedido_ou_maq(Lista *pedidosROL, Lista *MaquinasUso, double *tFabrica);
void atualizar_pedidoROL(Lista *pedidosROL, Rol *rol_Atualizar, double tFabrica, int *n);
void inserir_Rolamento(Fila **f_maq, Lista *MaquinasUso, Rol *p_atual, NEquipUso *maq, double tFabrica);
void atualizar_maqAtual(Fila **f_maq, Lista *MaquinasUso, Equip *maq_atual, NEquipUso *maq, double tFabrica);
int proxima_maquina(Rol *rolp);
void inicializa_coletaDados(InfoRol *dadosRol);
FILE* inicializa_rolConstruidos();
void rolConstruidos_Dados(FILE *lst_RolFabricados, Rol *rolFabricado, double tFabrica);
void coletaDados(Rol *rolDados, InfoRol *coletaInfo_rol, double tFabrica);
void liberarF_maquinas(Fila **f_maq);
void liberar_MaquinasUso(Lista *MaquinasUso);
void imprimirF_maquinas(Fila **f_maq);
void imprimir_salvar_info(InfoRol *dadosRol, FILE* lst_RolFabricados);



int main(int argc, char **argv) 
{
	//Checar os argumentos de entrada
	if (argc != 2) 
	{
		printf("Cuidado com os argumentos.\n");
		exit(1); //Aborda a execução do programa
	}
	
	Fila **filas_maq;  //Vetor de filas das máquinas
	Lista *pedidosROL, *MaquinasUso; //Lista de pedidos rolamentos e Lista de máquinas sendo utilizadas
	Rol *rol_processar, *p_atual;  //pedido do rolamento atual(de menor tempo)
	Equip *maq_atual; //máquina de menor tempo final de processamento
	NEquipUso Nmaq; //Indica quantas máquinas de cada tipo está sendo usada no momento
	InfoRol coletaInfo_rol; //Coletar informações sobre os rolamentos fabricados
	time_t tInicio, tFim; //Calcular o tempo de produção dos rolamentos
	FILE *lst_RolFabricados; //Esse arquivo guarda os rolamentos correntes construidos
	int condicao_fabrica, nEsfericos = 1; //Conta o número de esfericos
	double tSimulacao = atof(argv[1]), tFabrica = 0.00, t_execucao = 0.00;
	
	//Calcular o tempo de execução do programa desde o início do programa
	tInicio = time(NULL);
	
	/*Criando as filas das máquinas que ficam dentro de um vetor
	Cada fila é uma posição do vetor*/
	filas_maq = criarF_maquinas();
	
	/*Cria uma lista de máquinas que são instanciadas na fila somente quando estão em uso, 
	além de serem inseridas de forma ordenada em relação ao tempo final de processamento da peça*/
	MaquinasUso = criarLista(compara_tFimProcesso);
	//Inicializa a quantidade máquinas usadas no momento corrente. No caso nenhuma usada então tudo 0
	maquinasUsadas_inicial(&Nmaq);
	//Inicializa os dados colocando o valor padrão que é 0(zero)
	inicializa_coletaDados(&coletaInfo_rol);
	//Abre o arquivo que contém os rolamentos correntes fabricados
	lst_RolFabricados = inicializa_rolConstruidos();
	
	/*Cria uma lista de rolamentos pedidos que será inserida de forma ordenada da chegada do pedido, 
	por isso passada a função como parâmetro*/
	pedidosROL = criarLista(compara_tchegada);
	pedidoRol_inicial(pedidosROL); //Pedidos dos três rolamentos iniciais
	
	//Retorna 1 para operações no pedidoROL ou 0 para MaquinasUso, além de atualizar o tempo da fábrica
	condicao_fabrica = checa_atualiza_pedido_ou_maq(pedidosROL, MaquinasUso, &tFabrica);
	
	//Faz a simulação enquanto não chega no tempo da simulação de entrada
	while (tFabrica <= tSimulacao) 
	{
		//condicao_fabrica == 1 faz atualizações relacionadas a lista de pedidos dos rolamentos
		if (condicao_fabrica) 
		{
			//Pega pedido de menor tempo
			p_atual = obterLista(pedidosROL, 0);
			//Atualiza o próximo pedido na lista de pedidos
			atualizar_pedidoROL(pedidosROL, p_atual, tFabrica, &nEsfericos);
			/*Insere o rolamento ou na fila de prioridade das máquinas ou diretamente na máquina
			dependendo da situação*/
			inserir_Rolamento(filas_maq, MaquinasUso, p_atual, &Nmaq, tFabrica);
		}
		//condicao_fabrica == 0 faz atualizações relacionadas a máquinas que estão em uso
		else			
		{
			//Pega a máquina com menor tempo de finalização de processamento
			maq_atual = obterLista(MaquinasUso, 0);
			//Pega o rolamento que foi processado na maquina atual
			rol_processar = obterPeca(maq_atual);
			
			/*Caso retorna 1 porque a rolamento tem uma próxima máquina de processamento e atualiza essa próxima etapa
			do rolamento Caso retorna 0 porque acabou de fabricar a peça/rolamento*/
			if (proxima_maquina(rol_processar))
				//Usa mesma função de inserir no rolamento para a peça que está dentro da máquina atual
				inserir_Rolamento(filas_maq, MaquinasUso, rol_processar, &Nmaq, tFabrica);
			else 
			{
				rolConstruidos_Dados(lst_RolFabricados, rol_processar, tFabrica);
				coletaDados(rol_processar, &coletaInfo_rol, tFabrica);
				liberar_rol(rol_processar);
			}
			//A função abaixo decide se a instância da máquina atual é liberada ou se continua pegando a próxima peça na fila
			atualizar_maqAtual(filas_maq, MaquinasUso, maq_atual, &Nmaq, tFabrica);
		}
		//Retorna 1 para operações no pedidoROL ou 0 para MaquinasUso, além de atualizar o tempo da fábrica
		condicao_fabrica = checa_atualiza_pedido_ou_maq(pedidosROL, MaquinasUso, &tFabrica);
	}
	
	//Imprimindo e salvando informações em um arquivo e fechando o arquivo a lista de rolamentos fabricados
	imprimir_salvar_info(&coletaInfo_rol, lst_RolFabricados); 
	
	liberarF_maquinas(filas_maq); //Liberando as filas das máquinas
	destruirLista(pedidosROL); //Liberando a lista de pedidos dos rolamentos
	liberar_MaquinasUso(MaquinasUso); //Liberando a lista de máquinas em uso
	
	//Tempo final de execução do final do programa
	tFim = time(NULL);
	//Tempo final de execução
	t_execucao = difftime(tFim, tInicio);
	printf("\n\n Tempo de Execução da produção: %f\n", t_execucao);
	
	return 0;
}


//Gera o tempo de chegada de um novo pedido
float chegadaPedido(float param)
{
 	float u=0;
	
	do 
	{
		u = (float) (rand()%RAND_MAX) / RAND_MAX;
	} while ((u==0) || (u==1));
	
	return (float) (-param * log (u));
}


//Retorna o tempo de processamento em uma máquina específica
float tempoMaquina(float param) 
{
	return 2.0 * param * rand() / (RAND_MAX + 1.0);
}


//Instânciam as três filas de prioridades das máquinas
Fila** criarF_maquinas() 
{
	//Fazendo a alocação dinâmica da fila das máquinas
	//No caso são 3 filas para três tipos de máquinas diferentes
	Fila **f_maq = (Fila**) malloc(NFILAS_MAQ * sizeof(Fila*));
	
	if (f_maq == NULL)
		exit(2); //Erro de alocação aborda o programa
	
	//Loop para alocar cada posição do vetor das filas
	//No caso está sendo criado as filas do torno, fresa e mandril
	for (int i=0; i < NFILAS_MAQ; i++)
		f_maq[i] = criar_fila();
	
	return f_maq;
}


//Libera a alocação dinâmica filas criadas
void liberarF_maquinas(Fila **f_maq)  
{
	if (f_maq != NULL) 
	{
		//Liberando cada fila, ou seja, torno, fresa e mandril
		for (int i=0; i < NFILAS_MAQ; i++)
			libera_fila(f_maq[i]);
	
		free(f_maq); //Logo depois liberando o vetor que armazena as filas das máquinas
	}	
}


//Serve principalmente para liberar as máquinas que não acabam de processar a peça porque o tempo de fábrica
//ultrapassa o de simulação
void liberar_MaquinasUso(Lista *MaquinasUso) 
{
	//Se a lista de maquinas em uso tiver elementos percorrer eles para liberar o malloc dos rolamentos
	if (tamanhoLista(MaquinasUso)) 
	{
		Equip *maqAux; 
		Rol *rol_rem; //rolamento auxiliar de remoção/liberar
		for (int i=0; i < tamanhoLista(MaquinasUso); i++) 
		{
			maqAux = obterLista(MaquinasUso, i);
			rol_rem = obterPeca(maqAux); //Obtém o rolamento que está inserido na máquina
			liberar_rol(rol_rem); //libera o rolamento alocado dinamicamente
		}
	}
	destruirLista(MaquinasUso); //Libera a lista normalmente
}


/*Colocará na lista de pedidos de forma ordenada, assim quando a fábrica 
chegar no tempo do primeiro pedido é necessário pegar o que se encontra na primeira lista*/
void pedidoRol_inicial(Lista *pedidosROL) 
{
	//Primeiro rolamento esférico criado é aço por causa dos 90%
	inserirLista(pedidosROL, tipoROL(tipoEA, chegadaPedido(TESF)));
	inserirLista(pedidosROL, tipoROL(tipoCI, chegadaPedido(TCIL)));
	inserirLista(pedidosROL, tipoROL(tipoCO, chegadaPedido(TCON)));
}


//Inicializa o número de máquinas que está sendo usada no momento corrente
void maquinasUsadas_inicial(struct N_maquinasUso *maq) 
{
	maq->nTorno = 0; maq->nFresa = 0; maq->nMandril = 0;
}


/*Checa se vai realizar as instruções que tem relação com a lista de pedidos ou com as máquinas em uso
O escolhido é aquele que tem o menor tempo imediatamente acima do tempo de fábrica,
além de atualizar o tempo da fábrica para o menor tempo*/
int checa_atualiza_pedido_ou_maq(Lista *pedidosROL, Lista *MaquinasUso, double *tFabrica) 
{
	Rol *rolAux = obterLista(pedidosROL, 0); //Obtendo o menor tempo dos rolamentos pedidos
	
	//Verifica se tem pelo menos uma máquina em uso, pois caso não tenha a lista de pedidos que será executada
	if (tamanhoLista(MaquinasUso) > 0) 
	{
		Equip *maqAux = obterLista(MaquinasUso, 0); //Obtendo o menor tempo do fim de processamento de uma das máquinas
		
		/*Caso tempo de chegada do rolamento menor ou igual ao tempo final de fabricação da peça
		então será feito outro pedido, caso contrário a máquina acaba a produção de uma determinada peça */
		if (tFinalProc_peca(maqAux) < tChegada_rol(rolAux))
		{
			*tFabrica = tFinalProc_peca(maqAux);
			return 0; //MaquinasUso que será executado
		}
	}

	*tFabrica = tChegada_rol(rolAux);
	return 1; //pedidosROL que será executado
} 


//Renova/atualiza a lista de pedidos após o de menor tempo ser colocado na fila das máquinas
void atualizar_pedidoROL(Lista *pedidosROL, Rol *rol_Atualizar, double tFabrica, int *n) 
{
	//Função retorna um ponteiro para char do tipo do rolamento para ser atualizado
	char *tipo = tipo_rolamento(rol_Atualizar);
	//É atualizado com o novo tempo de chegada do rolamento que é atualizado na lista de pedidos
	float novoTempo = tFabrica;
	
	removerLista(pedidosROL, 0); //Remove da lista para colocar um novo e renovar a lista
	
	//Faz o switch case para identificar o tipo do rolamento a ser inserido e o novo tempo chegada
	//tipo[1] é possível diferenciar os tipos de cada rolamento
	switch (tipo[1]) 
	{
		//Tipo ea = esférico aço
		case 'a':
			novoTempo += chegadaPedido(TESF);
			if (*n != 9) 
			{
				inserirLista(pedidosROL, tipoROL(tipoEA, novoTempo));
				*n = *n+1; //Conta mais um do tipo aço
				break;
			}
			//Se tem 9 do tipo esférico aço então tem que criar o décimo como titânico 
			inserirLista(pedidosROL, tipoROL(tipoET, novoTempo));
			break;
		
		//Tipo ci = cilindrico
		case 'i':
			novoTempo += chegadaPedido(TCIL);
			inserirLista(pedidosROL, tipoROL(tipoCI, novoTempo));
			break;
		
		//Tipo co = conico
		case 'o':
			novoTempo += chegadaPedido(TCON);
			inserirLista(pedidosROL, tipoROL(tipoCO, novoTempo));
			break;
		
		//Tipo et = esférico titanico
		//Como titanica aparece 10% das vezes entao o proximo que ele cria é aco
		case 't':
			novoTempo += chegadaPedido(TESF);
			inserirLista(pedidosROL, tipoROL(tipoEA, novoTempo));
			*n = 1; //Já vai para 1 porque já é criado um novo rolamento aço 
			break;
		
		//Algum erro ocorreu
		default:
			printf("Erro! x1\n");
			exit(3);
	}
}


//Checa e insere o pedido do rolamento ou na fila de espera de prioridades ou diretamente na máquina
void inserir_Rolamento(Fila **f_maq, Lista *MaquinasUso, Rol *p_atual, NEquipUso *maq, double tFabrica) 
{
	//Retorna o caractere representante de qual a máquina atual que o rolamento deve entrar
	char TipoMaq = ordProcesso_Atual(p_atual);
	
	/* 
	maq[0] = fila do torno
	maq[1] = fila da fresa
	maq[2] = fila do mandril
	*/
	switch (TipoMaq) 
	{
		case 'T':
			//Todos os tornos disponíveis estão em uso, logo insere na fila de espera das maquinas
			if (maq->nTorno == NT_TORNO)
				inserir_fila(f_maq[0], p_atual, prior_rol(p_atual));
			//Caso contrário insere na máquina diretamente
			else 
			{
				inserirLista(MaquinasUso, GeraMaq(MaqTorno, p_atual, tFabrica+tempoMaquina(estadiaT_rol(p_atual))));
				maq->nTorno++; 
			}
	
			break;
		
		case 'F':
			//Todos as fresas disponíveis estão em uso, logo insere na fila de espera das maquinas
			if (maq->nFresa == NT_FRESA)
				inserir_fila(f_maq[1], p_atual, prior_rol(p_atual));
			//Caso contrário insere na máquina diretamente
			else 
			{
				inserirLista(MaquinasUso, GeraMaq(MaqFresa, p_atual, tFabrica+tempoMaquina(estadiaF_rol(p_atual))));
				maq->nFresa++;
			}
				
			break;
		
		case 'M':
			//Todos os mandril disponíveis estão em uso, logo insere na fila de espera das maquinas
			if (maq->nMandril == NT_MANDRIL)
				inserir_fila(f_maq[2], p_atual, prior_rol(p_atual));
			//Caso contrário insere na máquina diretamente
			else 
			{
				inserirLista(MaquinasUso, GeraMaq(MaqMandril, p_atual, tFabrica+tempoMaquina(estadiaM_rol(p_atual))));
				maq->nMandril++;
			}
			break;
		
		default:
			printf("Erro! x2\n");
			exit(5);
	}
}


/*Decide se a máquina atual de menor tempo de processamento vai pegar uma nova peça na fila se exister
Caso não exista ela é liberada porque as máquinas só existem quando são utilizadas*/
void atualizar_maqAtual(Fila **f_maq, Lista *MaquinasUso, Equip *maq_atual, NEquipUso *maq, double tFabrica) 
{
	//Pega o tipo da máquina (torno, fresa ou mandril)
	char maqt = tipoMaq(maq_atual);
	
	//Remove a instância da máquina na lista ordenada, para caso seja inserida novamente é de forma ordenada
	//caso a fila não esteja vazia
	removerLista(MaquinasUso, 0);
	
	switch (maqt) 
	{
		case 'T': //Torno
			if (tamanho_fila(f_maq[0])) //Tem elementos na fila
			{
				//obterIni é uma função da filapr que pega no início da fila o elemento
				Atualizar_Maq(maq_atual, obterIni(f_maq[0]), tFabrica+tempoMaquina(estadiaT_rol(obterIni(f_maq[0]))));
				inserirLista(MaquinasUso, maq_atual);
				remover_fila(f_maq[0]);
				break;
			}
			//Caso contrário apaga a existencia da máquina porque não tem peças na fila para ser fabricada
			liberar_Maq(maq_atual);
			maq->nTorno--; //Libera uma máquina torno para ser usada
			break;
			
		case 'F': //Fresa
			if (tamanho_fila(f_maq[1])) //Tem elementos na fila
			{
				//obterIni é uma função da filapr que pega no início da fila o elemento
				Atualizar_Maq(maq_atual, obterIni(f_maq[1]), tFabrica+tempoMaquina(estadiaF_rol(obterIni(f_maq[1]))));
				inserirLista(MaquinasUso, maq_atual);
				remover_fila(f_maq[1]);
				break;
			}
			//Caso contrário apaga a existencia da máquina porque não tem peças na fila para ser fabricada
			liberar_Maq(maq_atual);
			maq->nFresa--; //libera uma fresa para ser usada
			break;
		
		case 'M': //Mandril
			if (tamanho_fila(f_maq[2])) //Tem elementos na fila
			{
				//obterIni é uma função da filapr que pega no início da fila o elemento
				Atualizar_Maq(maq_atual, obterIni(f_maq[2]), tFabrica+tempoMaquina(estadiaM_rol(obterIni(f_maq[2]))));
				inserirLista(MaquinasUso, maq_atual);
				remover_fila(f_maq[2]);
				break;
			}
			//Caso contrário apaga a existencia da máquina porque não tem peças na fila para ser fabricada
			liberar_Maq(maq_atual);
			maq->nMandril--; //libera um mandril para ser usado
			break;
		
		default:
			printf("Erro! x6\n");
			exit(6);
	}
}


//Retorna 1 se existir próxima máquina para o rolamento que está na maquina atual, caso contrário retorna 0
int proxima_maquina(Rol *rolp) 
{	
	//Função que além de atualizar a proxima etapa de processamento do rolamento da máquina
	//retorna um inteiro responsável por indicar se o rolamento foi fabricado
	int continua_fabricacao = atualizar_procesRol(rolp);
	
	if (continua_fabricacao)
		return 1; //Continua o processamento do rolamento nas máquinas
	
	return 0; //Finalizado a fabricação rolamento
}


//Inicializa para retirar o lixo de dentro das variáveis
void inicializa_coletaDados(InfoRol *dadosRol)
{
	dadosRol->nCI = 0; dadosRol->nCO = 0; dadosRol->nEA = 0; dadosRol->nET = 0;
	dadosRol->tCI = 0.0; dadosRol->tCO = 0.0; dadosRol->tEA = 0.0; dadosRol->tET = 0.0;
}


//Inicializa abrindo o arquivo dos rolamentos correntes construidos
FILE* inicializa_rolConstruidos() 
{
	FILE *lst_RolFabricados = fopen("Rolamentos_Fabricados.txt", "w");
	
	fprintf(lst_RolFabricados, "Tipo       Tempo final de processamento\n");
	
	return lst_RolFabricados;
}


//Coletar os dados do rolamento corrente construido
void rolConstruidos_Dados(FILE *lst_RolFabricados, Rol *rolFabricado, double tFabrica) 
{
	fprintf(lst_RolFabricados, "%s           %.2f\n", tipo_rolamento(rolFabricado), tFabrica-tChegada_rol(rolFabricado));
}


//Coleta informações que serão impressas no final do tempo de simulação e escrito em um arquivo de saída
void coletaDados(Rol *rolDados, InfoRol *coletaInfo_rol, double tFabrica) 
{
	char *tipoRol = tipo_rolamento(rolDados); //Pega o tipo do rolamento
	
	switch (tipoRol[1]) 
	{
		case 'a': //Esférico aço
			coletaInfo_rol->nEA++;
			coletaInfo_rol->tEA += (tFabrica - tChegada_rol(rolDados));
			break;
			
		case 'o': //conico
			coletaInfo_rol->nCO++;
			coletaInfo_rol->tCO += (tFabrica - tChegada_rol(rolDados));
			break;
		
		case 'i': //cilindrico
			coletaInfo_rol->nCI++;
			coletaInfo_rol->tCI += (tFabrica - tChegada_rol(rolDados));
			break;
		
		case 't': //Esférico titânico
			coletaInfo_rol->nET++;
			coletaInfo_rol->tET += (tFabrica - tChegada_rol(rolDados));
			break;
		
		default:
			printf("Erro! x12");
			exit(7); //Aborda o programa
	}
}


//Fazer a impressão das filas das máquinas
void imprimirF_maquinas(Fila **f_maq) 
{
	char nome_filas[NFILAS_MAQ][10] = {"Torno", "Fresa", "Mandril"};
	
	//Loop para imprimir as filas dentro do vetor de filas das máquinas
	for (int i=0; i < NFILAS_MAQ; i++)
	{
		printf("%s:\n", nome_filas[i]);
		imprime_fila(f_maq[i], imprime_rol);
		printf("\n\n");
	}
}


//Imprime os dados coletados sobre os rolamentos criados e fecha o arquivo que registra todos os rolamentos fabricados
void imprimir_salvar_info(InfoRol *dadosRol, FILE *lst_RolFabricados) 
{
	//Abrindo arquivo será escrito as informações sobre os rolamentos
	FILE *info_saida = fopen("Resumo_rolamentos.txt", "w");
	
	
	printf("Tipo rolamento     Nº pecas    Tempo medio construcao\n");
	printf("Cilindrico         %d           %.2f\n", dadosRol->nCI, (dadosRol->tCI)/dadosRol->nCI);
	printf("Conico             %d           %.2f\n", dadosRol->nCO, (dadosRol->tCO)/dadosRol->nCO);
	printf("Esferico Aco       %d           %.2f\n", dadosRol->nEA, (dadosRol->tEA)/dadosRol->nEA);
	printf("Esferico Titanio   %d           %.2f\n", dadosRol->nET, (dadosRol->tET)/dadosRol->nET);
	
	//Checa se houve erro de abertura do arquivo
	if (info_saida == NULL) 
	{
		printf("Erro de gravacao!");
		exit(8); //Aborda o programa
	}
	
	fprintf(info_saida, "Tipo rolamento     Nº pecas    Tempo medio construcao\n");
	fprintf(info_saida, "Cilindrico         %d           %.2f\n", dadosRol->nCI, (dadosRol->tCI)/dadosRol->nCI);
	fprintf(info_saida, "Conico             %d           %.2f\n", dadosRol->nCO, (dadosRol->tCO)/dadosRol->nCO);
	fprintf(info_saida, "Esferico Aco       %d           %.2f\n", dadosRol->nEA, (dadosRol->tEA)/dadosRol->nEA);
	fprintf(info_saida, "Esferico Titanio   %d           %.2f\n", dadosRol->nET, (dadosRol->tET)/dadosRol->nET);
	
	fclose(info_saida);
	fclose(lst_RolFabricados);
}
