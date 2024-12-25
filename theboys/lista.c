//TAD lista de números inteiros
//Carlos Souto - GRR20231951 - Nov/2024
//
//Implementação do TAD
//Implementação com lista encadeada dupla não-circular

#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

struct lista_t *lista_cria ()
{
    struct lista_t *lst;
    lst = malloc (sizeof(struct lista_t));
    if (lst == NULL) //Erro: Falha de alocacao
        return NULL;
    
    lst->prim = NULL;
    lst->ult = NULL;
    lst->tamanho = 0;

    return lst;
}

struct lista_t *lista_destroi (struct lista_t *lst)
{
    if (lst == NULL) 
        return NULL; //Retorna NULL para lista NULL
    
    struct item_t *item_atual = lst->prim;
    struct item_t *item_prox;

    while (item_atual != NULL) //Percorre a lista liberando cada item
    {
        item_prox = item_atual->prox;
        free (item_atual);
        item_atual = item_prox;
    }

    free (lst);
    lst = NULL;

    return NULL;
}

int lista_insere (struct lista_t *lst, int item, int pos)
{
    if (lst == NULL)
        return -1; //Erro: Lista NULL
    
    if (pos == -1 || pos > lst->tamanho) //Ajuste caso posicao = -1 ou maior que a lista (Coloca no final)
        pos = lst->tamanho;

    if (pos < 0)
        return -1; //Erro: Posicao invalida

    struct item_t *novo_item;
    novo_item = malloc(sizeof(struct item_t));
    if (novo_item == NULL)
        return -1; //Erro: Falha na alocacao

    novo_item->valor = item;
    novo_item->ant = NULL;
    novo_item->prox = NULL;

    lst->tamanho++;

    if (lst->tamanho == 1) //Lista vazia
    {
        lst->prim = novo_item;
        lst->ult = novo_item;
        return lst->tamanho;
    }

    if (pos == 0) //Insere no comeco
    {
        novo_item->prox = lst->prim;
        lst->prim->ant = novo_item;
        lst->prim = novo_item;
        return lst->tamanho;
    }

    if (pos == lst->tamanho -1) //Insere no final
    {
        novo_item->ant = lst->ult;
        lst->ult->prox = novo_item;
        lst->ult = novo_item;
        return lst->tamanho;
    }

    struct item_t *item_atual = lst->prim;
    for (int i = 0; i < pos; i++) //Encontra a posicao correta
        item_atual = item_atual->prox;
    
    novo_item->prox = item_atual; //Insere na posicao correta
    novo_item->ant = item_atual->ant;
    item_atual->ant->prox = novo_item;
    item_atual->ant = novo_item;

    return lst->tamanho;
}

int lista_retira (struct lista_t *lst, int *item, int pos)
{
    if (lst == NULL || item == NULL)
        return -1; //Erro: Lista ou item NULL

    if (pos == -1)
        pos = lst->tamanho - 1;

    if (pos < 0 || pos >= lst->tamanho)
        return -1; //Erro: Posicao fora dos limites

    struct item_t *item_atual = lst->prim;
    for (int i = 0; i < pos; i++)
        item_atual = item_atual->prox;

    lst->tamanho--;
    *item = item_atual->valor;
    
    if (lst->tamanho == 0) //Lista vazia
    {
        lst->prim = NULL;
        lst->ult = NULL;
        free(item_atual);
        item_atual = NULL;
        return lst->tamanho;
    }

    if (pos == 0) //Retira da primeira posicao
    {
        lst->prim = item_atual->prox;
        lst->prim->ant = NULL;
        free(item_atual);
        item_atual = NULL;
        return lst->tamanho;
    }

    if (pos == lst->tamanho) //Retira da ultima posicao
    {
        lst->ult = item_atual->ant;
        lst->ult->prox = NULL;
        free (item_atual);
        item_atual = NULL;
        return lst->tamanho;
    }

    item_atual->ant->prox = item_atual->prox;
    item_atual->prox->ant = item_atual->ant;
    free(item_atual);

    return lst->tamanho;
}

int lista_consulta (struct lista_t *lst, int *item, int pos)
{
    if (lst == NULL || item == NULL)
        return -1; //Erro: Lista ou item NULL
    
    if (pos == -1)
        pos = lst->tamanho - 1;

    if (pos < 0 || pos >= lst->tamanho)
        return -1; //Erro: Posicao fora dos limites
    
    struct item_t *item_atual = lst->prim;
    for (int i = 0; i < pos; i++)
        item_atual = item_atual->prox;
    
    *item = item_atual->valor;

    return lst->tamanho;
}

int lista_procura (struct lista_t *lst, int valor)
{
    if (lst == NULL)
        return -1; //Erro: Lista NULL
    
    struct item_t *item_atual = lst->prim;
    int pos = 0;
    while (item_atual != NULL)
    {
        if (item_atual->valor == valor)
            return pos;
        item_atual = item_atual->prox;
        pos++;
    }

    return -1; //Item nao encontrado
}

int lista_tamanho (struct lista_t *lst)
{
    if (lst == NULL)
        return -1; //Erro: Lista NULL
    
    return lst->tamanho;
}

void lista_imprime (struct lista_t *lst)
{
    if (lst == NULL || lst->prim == NULL)
        return;
    
    struct item_t *item_atual = lst->prim;

    while (item_atual->prox != NULL)
    {
        printf("%d ", item_atual->valor);
        item_atual = item_atual->prox;
    }

    printf ("%d", item_atual->valor);
}