//Implementacao da LEF
//Carlos Souto - GRR20231951 - Nov/2024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LEF.h"

lef *cria_LEF ()
{
    lef *lista;

    lista = malloc(sizeof(lef));
    lista->prim = NULL;

    return lista;
}

lef *destroi_LEF (lef *lista)
{
    if (lista == NULL)
        return NULL;
    
    evento *atual = lista->prim;
    evento *prox;

    while (atual != NULL)
    {
        prox = atual->prox;
        free (atual);
        atual = prox;
    }

    free (lista);

    return NULL;
}

evento cria_evento (char nome[], int tempo, int info1, int info2)
{
    evento novo;

    strcpy(novo.nome, nome);
    novo.tempo = tempo;
    novo.info1 = info1;
    novo.info2 = info2;
    novo.prox = NULL;

    return novo;
}

int insere_evento (lef *lista, evento novo)
{
    evento *atual, *anterior, *novo_evento;
    if (lista == NULL)
    {
        printf("Erro: Lista ou evento NULL.\n");
        return 0; 
    }

    novo_evento = malloc(sizeof(evento));
    if (novo_evento == NULL)
    {
        printf("Erro: Nao foi possivel alocar memoria para o novo evento.\n");
        return 0;
    }

    *novo_evento = novo;

    atual = lista->prim;
    anterior = NULL;
    //Encontra o local correto para inserir o evento
    while ((atual != NULL) && (atual->tempo <= novo_evento->tempo))
    {
        anterior = atual;
        atual = atual->prox;
    }

    //Lista vazia ou com um evento
    if (anterior == NULL)
    {
        novo_evento->prox = lista->prim;
        lista->prim = novo_evento;
        return 1;
    }

    //Lista com mais de um evento
    novo_evento->prox = anterior->prox;
    anterior->prox = novo_evento;

    return 1;
}

evento retira_evento (lef *lista)
{
    evento retirado;
    evento *temp;

    retirado = *lista->prim;

    temp = lista->prim;
    lista->prim = lista->prim->prox;

    free (temp);
    temp = NULL;

    return retirado;
}

void nome_evento (char nome[], evento e)
{
    strcpy (nome, e.nome);
}

int tempo_evento (evento e)
{
    return e.tempo;
}

int info1_evento (evento e)
{
    return e.info1;
}

int info2_evento (evento e)
{
    return e.info2;
}