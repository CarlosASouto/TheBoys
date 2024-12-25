//Programa principal do projeto "The Boys - 2024/2"
//Autor: Carlos Souto - GRR20231951 - Nov/2024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mundo.h"
#include "LEF.h"
#include "eventos.h"

// programa principal
int main ()
{
    srand (0);
    int relogio, info1, info2, fim;
    char nome [7];
    evento atual;
    mundo *mundo;
    lef *lef;

    lef = cria_LEF();
    if (lef == NULL)
    {
        printf("Erro: Nao foi possivel alocar memoria para a LEF.\n");
        return 0;
    }
    
    mundo = mundo_cria();

    //Insere os eventos iniciais na LEF
    if (eventos_iniciais (lef, mundo) == 0)
    {
        printf("Erro ao inserir os eventos iniciais.\n");
        return 0;
    }

    relogio = 0;
    fim = 0;

    while (!fim)
    {
        atual = retira_evento (lef);
        relogio = tempo_evento (atual);
        info1 = info1_evento (atual);
        info2 = info2_evento (atual);
        nome_evento(nome, atual);

        if (strcmp (nome, "CHEGA") == 0)
            if (!evento_chega (lef, mundo, relogio, info1, info2))
            {
                printf("Erro ao realizar evento.\n");
                return 0;
            }

        if (strcmp (nome, "ESPERA") == 0)
            if (!evento_espera (lef, mundo, relogio, info1, info2))
            {
                printf("Erro ao realizar evento.\n");
                return 0;
            }

        if (strcmp (nome, "DESISTE") == 0)
            if (!evento_desiste (lef, mundo, relogio, info1, info2))
            {
                printf("Erro ao realizar evento.\n");
                return 0;
            }

        if (strcmp (nome, "AVISA") == 0)
            if (!evento_avisa (lef, mundo, relogio, info1))
            {
                printf("Erro ao realizar evento.\n");
                return 0;
            }

        if (strcmp (nome, "ENTRA") == 0)
            if (!evento_entra (lef, mundo, relogio, info1, info2))
            {
                printf("Erro ao realizar evento.\n");
                return 0;
            }

        if (strcmp (nome, "SAI") == 0)
            if (!evento_sai (lef, mundo, relogio, info1, info2))
            {
                printf("Erro ao realizar evento.\n");
                return 0;
            }

        if (strcmp (nome, "VIAJA") == 0)
            if (!evento_viaja (lef, mundo, relogio, info1, info2))
            {
                printf("Erro ao realizar evento.\n");
                return 0;
            }

        if (strcmp (nome, "MORRE") == 0)
            if (!evento_morre (lef, mundo, relogio, info1, info2))
            {
                printf("Erro ao realizar evento.\n");
                return 0;
            }

        if (strcmp (nome, "MISSAO") == 0)
            if (!evento_missao (lef, mundo, relogio, info1))
            {
                printf("Erro ao realizar evento.\n");
                return 0;
            }

        if (strcmp (nome, "FIM") == 0)
        {
            evento_fim (lef, mundo, relogio);
            fim = 1;
        }
    }
    return 0;
}