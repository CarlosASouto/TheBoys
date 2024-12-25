//Definicao das entidades e declaracao das funcoes necessarias
//para um mundo simulado com super-herois
//
//Carlos Souto - GRR20231951 - Nov/2024

#ifndef MUNDO_H
#define MUNDO_H

#include "conjunto.h"

#define T_INICIO 0
#define T_FIM_DO_MUNDO 525600
#define N_TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS (N_HABILIDADES * 5)
#define N_BASES (N_HEROIS / 5)
#define N_MISSOES (T_FIM_DO_MUNDO / 100)


typedef struct coordenada coord;
typedef struct heroi heroi;
typedef struct base base;
typedef struct mundo mundo;
typedef struct missao missao;

struct coordenada               // Tipo de dado que representa uma coordenada no plano cartesiano
{
    int x;
    int y;
};

struct heroi
{
    int id;                     //número inteiro ≥ 0 que identifica unicamente o herói
    struct cjto_t *habilidades; //conjunto de habilidades que o herói possui. Cada habilidade é representada por um número inteiro ≥ 0
    int paciencia;              //número inteiro ≥ 0 que indica quão paciente uma pessoa é
    int velocidade;             //número inteiro ≥ 0 indicando a velocidade de deslocamento de um herói
    int exp;                    //número inteiro ≥ 0 que indica o número de missões em que o herói já participou
    int base;                   //ID da base onde o herói se encontra no momento
    int vivo;                   //Flag que indica se o herois esta vivo (1 = vivo, 0 = morto)
    int missao_morte;           //Indica a missao em que o heroi morreu
};

struct base
{
    int id;                     //número inteiro ≥ 0 que identifica cada base
    int max;                    //número máximo de heróis naquela base
    struct cjto_t *presentes;   //conjunto dos IDs dos heróis que estão atualmente na base
    struct lista_t *espera;     //fila onde os heróis esperam para poder entrar na base
    coord local;                //localização da base
    int missoes;                //numero de missoes realizados pela base
    int fila_max;               //numero que indica qual foi a fila maxima de uma base
};

struct missao
{
    int id;                     //número inteiro ≥ 0 que identifica a missão
    struct cjto_t *habilidades; //conjunto de habilidades necessárias para cumprir a missão;
    int perigo;                 //nível de perigo da missão
    coord local;                //localização da missão
    int tentativa;              //numero da tentativa atual de realizar a missao
    int cumprida;               //Flag que indica se a missao foi cumprida (1 = cumprida, 0 = nao cumprida)
};

struct mundo
{
    int n_herois;               //número total de heróis no mundo
    heroi herois[N_HEROIS];     //vetor representando todos os heróis
    int n_bases;                //número total de bases no mundo
    base bases[N_BASES];        //vetor representando todas as bases
    int n_missoes;              //número total de missões a cumprir
    missao missoes[N_MISSOES];  //vetor representando todas as missões
    int n_habilidades;          //número de habilidades distintas possíveis
    coord tamanho_mundo;        //coordenadas máximas do plano cartesiano 
    int relogio;                //número inteiro ≥ 0 indicando o tempo atual no mundo. Cada unidade de tempo no mundo simulado representa 1 minuto de tempo real.
    int eventos;                 //Contador de eventos tratados no mundo
};
//Sorteia um numero aleatorio entre min e max inclusive
int aleat (int min, int max);

//Imprime os atributos de um heroi
void imprime_heroi (heroi h);

//Imprime os atributos de uma base
void imprime_base (base b);

//Inicializa todos os atributos do mundo (Mundo, herois, bases e missoes)
//Retorna um ponteiro para o mundo em sucesso e NULL em caso de erro
mundo *mundo_cria();

//Destroi o mundo liberando todos os espaços de memoria alocados
//Retorna NULL;
mundo *mundo_destroi (mundo *m);

#endif