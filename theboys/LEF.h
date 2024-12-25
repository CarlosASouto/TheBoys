//Funcoes relacionadas a Lista de Eventos Futuros -LEF
//Carlos Souto - GRR20231951 - Nov/2024

#ifndef LEF
#define LEF

typedef struct evento evento;
typedef struct lef_st lef;

struct evento
{
    char nome[7];
    int tempo;
    int info1;
    int info2;
    evento *prox;
};

struct lef_st
{
    evento *prim;
};

//Cria uma lista de eventos futuros vazia
//Retorna o ponteiro para a lista ou NULL em caso de erro
lef *cria_LEF ();

//Libera a memoria alocada para uma LEF
//Retorna NULL
lef *destroi_LEF (lef *lista);

//Imprime uma LEF
void imprime_LEF (lef *lista);

//Cria um evento com base nos parametros fornecidos
//Retorna o evento criado
evento cria_evento (char nome[], int tempo, int info1, int info2);

//Insere o evento novo na LEF
//Retorna 0 em caso de erro e 1 em sucesso
int insere_evento (lef *lista, evento novo);

//Remove o primeiro evento da LEF e libera memoria alocada
//Retorna o evento retirado
evento retira_evento (lef *lista);

//Retorna o nome do evento atual na string passada como parametro
void nome_evento (char nome[], evento e);

//Retorna o tempo do evento atual
int tempo_evento (evento e);

//Retorna a info 1 do evento atual
int info1_evento (evento e);

//Retorna a info 2 do evento
int info2_evento (evento e);

#endif