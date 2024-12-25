//Implementacao das funcoes relacionadas as 
//entidades do trabalho The Boys 2024/2
//
//Carlos Souto - GRR20231951 - Nov/2024

#include <stdlib.h>
#include <stdio.h>
#include "mundo.h"
#include "lista.h"

int aleat (int min, int max)
{
    return (rand() % (max - min + 1)) + min;
}

//Inicializa os atributos do heroi aleatoriamente. (A experiencia comeca zerada e a base começa como -1)
//Retorna 1 em caso de sucesso e retorna 0 em caso de erro
int heroi_cria (int id, heroi *novo)
{
    int quant_habilidades, habilidade;
    struct cjto_t *cjto_habilidades;
    
    quant_habilidades = aleat (1, 3);

    cjto_habilidades = cjto_cria(N_HABILIDADES);

    if (cjto_habilidades == NULL)
    {
        printf("Erro: Nao foi possivel criar o conjunto de habilidades do heroi.\n");
        return 0;
    }

    //Loop que sorteia as habilidades do heroi ate completar o conjunto
    for (int i = 0; i < quant_habilidades; i++)
    {
        habilidade = aleat (0, N_HABILIDADES - 1);

        //Loop que garante que nao terao habilidades repetidas
        while (cjto_pertence (cjto_habilidades, habilidade))
            habilidade = aleat (0, N_HABILIDADES - 1);
        
        if (cjto_insere (cjto_habilidades, habilidade) == -1)
        {
            printf("Erro: Nao foi possivel inserir a habilidade no conjunto(heroi)\n");
            return 0;
        }
    }

    //Inicializa o heroi
    novo->habilidades = cjto_habilidades;
    novo->id = id;
    novo->exp = 0;
    novo->paciencia = aleat (0, 100);
    novo->velocidade = aleat (50, 5000);
    novo->vivo = 1;

    return 1;
}

//Libera a memoria que foi alocada para o conjunto de habilidades de um heroi
void heroi_destroi (heroi *h)
{
    if (h == NULL)
        return;

    cjto_destroi (h->habilidades);

    return;
}

void imprime_heroi (heroi h)
{
    if (h.vivo)
        {
            printf("HEROI %2d VIVO  PAC %3d VEL %4d EXP %4d HABS [", h.id, h.paciencia, h.velocidade, h.exp);
            cjto_imprime (h.habilidades);
            printf("]\n");
            return;
        }
    printf("HEROI %2d MORTO PAC %3d VEL %4d EXP %4d HABS [", h.id, h.paciencia, h.velocidade, h.exp);
    cjto_imprime (h.habilidades);
    printf("]\n");
}

//Inicializa os atributos da base e a retorna no ponteiro *nova
//Retorna 1 em caso de sucesso e retorna 0 em caso de erro
int base_cria (int id, base *nova)
{
    struct cjto_t *presentes;
    struct lista_t *fila;

    fila = lista_cria();
    if (fila == NULL)
    {
        printf("Erro: Nao foi possivel alocar memoria para a fila de espera.\n");
        return 0;
    }

    nova->max = aleat (3, 10);
    presentes = cjto_cria (N_HEROIS);

    nova->id = id;
    nova->local.x = aleat (0, N_TAMANHO_MUNDO - 1);
    nova->local.y = aleat (0, N_TAMANHO_MUNDO - 1);
    nova->presentes = presentes;
    nova->espera = fila;
    nova->missoes = 0;
    nova->fila_max = 0;

    return 1;
}

//Libera a memoria alocada para a fila de espera e conjunto de presentes de uma base
void base_destroi (base *b)
{
    if (b == NULL)
        return;
    
    lista_destroi (b->espera);
    cjto_destroi (b->presentes);

    return;
}

void imprime_base (base b)
{
    printf("BASE %2d LOT %2d FILA MAX %2d MISSOES %d\n", b.id, b.max, b.fila_max, b.missoes);
}

//Inicializa os atributos da base e a retorna no ponteiro *nova
//Retorna 1 em caso de sucesso e retorna 0 em caso de erro
int missao_cria (int id, missao *nova)
{
    int quant_habilidades, habilidade;
    struct cjto_t *cjto_habilidades;

    quant_habilidades = aleat (6, 10);

    cjto_habilidades = cjto_cria(N_HABILIDADES);
    if (cjto_habilidades == NULL)
    {
        printf("Erro: Nao foi possivel criar o conjunto de habilidades da missao.\n");
        return 0;
    }

    //Loop que sorteia as habilidades da missao ate completar o conjunto
    for (int i = 0; i < quant_habilidades; i++)
    {
        habilidade = aleat (0, N_HABILIDADES - 1);

        //Loop que garante que nao terao habilidades repetidas
        while (cjto_pertence (cjto_habilidades, habilidade))
            habilidade = aleat (0, N_HABILIDADES - 1);
        
        if (cjto_insere (cjto_habilidades, habilidade) == -1)
        {
            printf("Erro: Nao foi possivel inserir a habilidade no conjunto(missao)\n");
            return 0;
        }
    }

    nova->habilidades = cjto_habilidades;
    nova->id = id;
    nova->local.x = aleat (0, N_TAMANHO_MUNDO - 1);
    nova->local.y = aleat (0, N_TAMANHO_MUNDO - 1);
    nova->perigo = aleat (0, 100);
    nova->tentativa = 1;
    nova->cumprida = 0;

    return 1;
}

//Libera a memoria alocada para o conjunto de habilidades de uma missao
void missao_destroi (missao *m)
{
    if (m == NULL)
        return;
    
    cjto_destroi (m->habilidades);

    return;
}

mundo *mundo_cria()
{
    int i;
    mundo *mundo;
    mundo = malloc(sizeof(struct mundo));
    if (mundo == NULL)
        return NULL;

    mundo->n_habilidades = N_HABILIDADES;
    mundo->tamanho_mundo.x = N_TAMANHO_MUNDO;
    mundo->tamanho_mundo.y = N_TAMANHO_MUNDO;
    mundo->relogio = T_INICIO;
    mundo->eventos = 0;

    mundo->n_herois = N_HEROIS;
    for (i = 0; i < mundo->n_herois; i++)
        if (!heroi_cria (i, &mundo->herois[i]))
        {
            printf("Erro: Nao foi possivel criar heroi.\n");
            return NULL;
        }

    mundo->n_bases = N_BASES;
    for (i = 0; i < mundo->n_bases; i++)
        if (!base_cria (i, &mundo->bases[i]))
        {
            printf("Erro: Nao foi possivel criar base.\n");
            return NULL;
        }

    mundo->n_missoes = N_MISSOES;
    for (i = 0; i < mundo->n_missoes; i++)
        if (!missao_cria (i, &mundo->missoes[i]))
        {
            printf("Erro: Nao foi possivel criar missao.\n");
            return NULL; 
        }

    return mundo;
}


mundo *mundo_destroi (mundo *m)
{
    if (m == NULL)
        return NULL;
    
    for (int i = 0; i < m->n_missoes; i++)
        missao_destroi (&m->missoes[i]);
    for (int i = 0; i < m->n_bases; i++)
        base_destroi (&m->bases[i]);
    for (int i = 0; i < m->n_herois; i++)
        heroi_destroi (&m->herois[i]);
    
    free (m);
    
    return NULL;
}