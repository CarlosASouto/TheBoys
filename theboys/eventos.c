//Implementacao dos eventos do mundo
//Carlos Souto - GRR20231951 - Nov/2024

#include <stdio.h>
#include <math.h>
#include "eventos.h"
#include "lista.h"

int eventos_iniciais (lef *lista, mundo *m)
{
    int i, base_inicial, tempo;
    evento novo;

    //Evento inicial que define quando os herois chegarao em bases aleatoriamente
    for (i = 0; i < m->n_herois; i++)
    {
        base_inicial = aleat (0, m->n_bases - 1);
        tempo = aleat (0, 4320);
        novo = cria_evento ("CHEGA", tempo, i, base_inicial);
        if (insere_evento (lista, novo) == 0)
        {
            printf("Erro ao inserir evento.\n");
            return 0;
        }
    }

    //Evento inicial que define para qual momento as missoes serao agendadas aleatoriamente
    for (i = 0; i < m->n_missoes; i++)
    {
        tempo = aleat (0, T_FIM_DO_MUNDO);
        novo = cria_evento ("MISSAO", tempo, i, 0);
        if (insere_evento (lista, novo) == 0)
        {
            printf("Erro ao inserir evento.\n");
            return 0;
        }
    }

    //Evento inicial que agenda o fim do mundo
    tempo = T_FIM_DO_MUNDO;
    novo = cria_evento ("FIM", tempo, 0, 0);
    if (insere_evento (lista, novo) == 0)
        {
            printf("Erro ao inserir evento.\n");
            return 0;
        }

    return 1;
}

int evento_chega (lef *lista, mundo *m, int tempo, int id_heroi, int id_base)
{
    evento novo;
    int n_presentes, fila, aguarda;

    m->eventos++;

    n_presentes = cjto_card (m->bases[id_base].presentes);
    fila = lista_tamanho (m->bases[id_base].espera);

    //Atualiza a base do heroi
    m->herois[id_heroi].base = id_base;

    //Verifica se o heroi vai aguardar na fila
    if ((n_presentes < m->bases[id_base].max) && (fila == 0))
        aguarda = 1;
    else
        aguarda = (m->herois[id_heroi].paciencia) > (10 * fila);

    //Insere os proximos eventos baseado se o heroi vai aguardar ou nao
    if (aguarda)
    {
        printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) ESPERA\n", tempo, id_heroi, id_base, n_presentes, m->bases[id_base].max);
        novo = cria_evento ("ESPERA", tempo, id_heroi, id_base);
        if (insere_evento (lista, novo) == 0)
        {
            printf("Erro ao inserir evento.\n");
            return 0;
        }

        return 1;
    }

    printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) DESISTE\n", tempo, id_heroi, id_base, n_presentes, m->bases[id_base].max);
    novo = cria_evento ("DESISTE", tempo, id_heroi, id_base);
    if (insere_evento (lista, novo) == 0)
    {
        printf("Erro ao inserir evento.\n");
        return 0;
    }

    return 1;
}

int evento_espera (lef *lista, mundo *m, int tempo, int id_heroi, int id_base)
{
    evento novo;
    int fila;

    m->eventos++;
    
    //Insere o heroi na fila de espera da base
    fila = lista_insere (m->bases[id_base].espera, id_heroi, -1);
    if (fila == -1)
    {
        printf("Erro ao adicionar o heroi na fila.\n");
        return 0;
    }

    //Verifica se o tamanho da fila atual é maior que a fila maxima que a base ja teve
    if (fila > m->bases[id_base].fila_max)
        m->bases[id_base].fila_max = fila;
    
    //Insere o evento AVISA na LEF
    printf("%6d: ESPERA HEROI %2d BASE %d (%2d)\n", tempo, id_heroi, id_base, fila - 1);
    novo = cria_evento ("AVISA", tempo, id_base, 0);
    if (insere_evento (lista, novo) == 0)
    {
        printf("Erro ao inserir evento.\n");
        return 0;
    }

    return 1;
}

int evento_desiste (lef *lista, mundo *m, int tempo, int id_heroi, int id_base)
{
    evento novo;
    int destino;

    m->eventos++;

    //Decide o novo destino do heroi
    destino = aleat (0, m->n_bases - 1);

    printf("%6d: DESIST HEROI %2d BASE %d\n", tempo, id_heroi, id_base);

    //Insere o evento VIAJA na LEF
    novo = cria_evento ("VIAJA", tempo, id_heroi, destino);
    if (insere_evento (lista, novo) == 0)
    {
        printf("Erro ao inserir evento.\n");
        return 0;
    }
    
    return 1;
}

int evento_avisa (lef *lista, mundo *m, int tempo, int id_base)
{
    evento novo;
    int n_presentes, fila, vagas, h_removido;

    m->eventos++;

    n_presentes = cjto_card (m->bases[id_base].presentes);
    fila = lista_tamanho (m->bases[id_base].espera);

    vagas = m->bases[id_base].max - n_presentes;

    printf("%6d: AVISA  PORTEIRO BASE %d (%2d/%2d) FILA [", tempo, id_base, n_presentes, m->bases[id_base].max);
    lista_imprime (m->bases[id_base].espera);
    printf("]\n");

    //Enquanto ainda houver vagas na base e herois na fila, admite o primeiro heroi da fila na base
    while ((vagas > 0) && (fila > 0))
    {
        fila = lista_retira (m->bases[id_base].espera, &h_removido, 0);
        if (fila == -1)
        {
            printf("Erro ao retirar heroi da fila.\n");
            return 0;
        }

        if (cjto_insere (m->bases[id_base].presentes, h_removido) == -1)
        {
            printf("Erro ao inserir o heroi no conjunto dos presentes.\n");
            return 0;
        }
        vagas--;

        printf("%6d: AVISA  PORTEIRO BASE %d ADMITE %2d\n", tempo, id_base, h_removido);
        novo = cria_evento ("ENTRA", tempo, h_removido, id_base);
        if (insere_evento (lista, novo) == 0)
        {
            printf("Erro ao inserir o evento.\n");
            return 0;
        }
    }

    return 1;
}

int evento_entra (lef *lista, mundo *m, int tempo, int id_heroi, int id_base)
{
    evento novo;
    int tempo_permanencia, n_presentes;

    m->eventos++;

    n_presentes = cjto_card (m->bases[id_base].presentes);

    //Calcula o tempo que o heroi vai ficar na base e adiciona o evento de saida
    tempo_permanencia = 15 + m->herois[id_heroi].paciencia * aleat(1, 20);
    novo = cria_evento ("SAI", tempo + tempo_permanencia, id_heroi, id_base);
    if (insere_evento (lista, novo) == 0)
    {
        printf("Erro ao inserir o evento.\n");
        return 0;
    }

    printf("%6d: ENTRA  HEROI %2d BASE %d (%2d/%2d) SAI %d\n", tempo, id_heroi, id_base, n_presentes, m->bases[id_base].max, tempo + tempo_permanencia);

    return 1;
}

int evento_sai (lef *lista, mundo *m, int tempo, int id_heroi, int id_base)
{
    evento novo;
    int destino, n_presentes;

    //Verifica se o heroi esta vivo
    if (!m->herois[id_heroi].vivo)
        return 1;

    m->eventos++;

    //Retira o heroi do conjunto dos presentes na base
    n_presentes = cjto_retira (m->bases[id_base].presentes, id_heroi);
    if (n_presentes == -1)
    {
        printf("Erro ao retirar herois do conjunto de presentes na base.\n");
        return 0;
    }

    //Decide o novo destino do heroi
    destino = aleat (0, m->n_bases - 1);

    printf("%6d: SAI    HEROI %2d BASE %d (%2d/%2d)\n", tempo, id_heroi, id_base, n_presentes, m->bases[id_base].max);

    //Insere o envento VIAJA na LEF
    novo = cria_evento ("VIAJA", tempo, id_heroi, destino);
    if (insere_evento (lista, novo) == 0)
    {
        printf("Erro ao inserir evento.\n");
        return 0;
    }

    //Insere o evento AVISA na LEF
    novo = cria_evento ("AVISA", tempo, id_base, 0);
    if (insere_evento (lista, novo) == 0)
    {
        printf("Erro ao inserir evento.\n");
        return 0;
    }

    return 1;
}


//Calcula a distancia cartesiana entre duas cordenadas e retorna a distancia
int calcula_distancia (coord local1, coord local2)
{
    return sqrt ((pow (local1.x - local2.x, 2)) + pow(local1.y - local2.y, 2));
}

int evento_viaja (lef *lista, mundo *m, int tempo, int id_heroi, int id_base_destino)
{
    evento novo;
    int distancia, duracao;
    heroi h = m->herois[id_heroi];
    base b_atual = m->bases[h.base];
    base b_destino = m->bases[id_base_destino];

    m->eventos++;

    //Calcula a distancia da base atual do heroi ate a base destino e a duracao da viagem baseado na velocidade do heroi
    distancia = calcula_distancia(b_atual.local, b_destino.local);
    duracao = distancia / h.velocidade;

    printf("%6d: VIAJA  HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d\n", tempo, id_heroi, b_atual.id, id_base_destino, distancia, h.velocidade, tempo + duracao);

    //Insere o evento CHEGA na LEF
    novo = cria_evento ("CHEGA", tempo + duracao, id_heroi, id_base_destino);
    if (insere_evento (lista, novo) == 0)
    {
        printf("Erro ao inserir evento.\n");
        return 0;
    }

    return 1;
}

int evento_morre (lef *lista, mundo *m, int tempo, int id_heroi, int id_base)
{
    evento novo;

    m->eventos++;

    printf("%6d: MORRE  HEROI %2d MISSAO %d\n", tempo, id_heroi, m->herois[id_heroi].missao_morte);

    //Retira o heroi do conjunto de presentes na base
    if (cjto_retira (m->bases[id_base].presentes, id_heroi) == -1)
    {
        printf("Erro ao retirar heroi da base.\n");
        return 0;
    }

    //Atualiza o status do heroi para morto
    m->herois[id_heroi].vivo = 0;

    //Insere o evento AVISA na LEF
    novo = cria_evento ("AVISA", tempo, id_base, 0);
    if (insere_evento (lista, novo) == 0)
    {
        printf("Erro ao inserir evento.\n");
        return 0;
    }

    return 1;
}

//Ordena o primeiro vetor por BubbleSort e replica as trocas no segundo vetor
void ordena_bases (int distancias[], int bases[], int tamanho)
{
    if (tamanho == 1)
        return;

    for (int i = 0; i < tamanho - 1; i++)
        if (distancias[i] > distancias[i + 1])
        {
            int temp1 = distancias[i];
            distancias[i] = distancias[i + 1];
            distancias[i + 1] = temp1;

            //Replicando a troca no segundo vetor
            int temp2 = bases[i];
            bases[i] = bases[i + 1];
            bases[i + 1] = temp2;
        }
    
    ordena_bases (distancias, bases, tamanho - 1);
}

int evento_missao (lef *lista, mundo *m, int tempo, int id_missao)
{
    evento novo;
    int i, distancia, BMP, risco, distancias [m->n_bases], bases_ordenadas[m->n_bases];
    struct cjto_t *uniao_hab, *temp;

    m->eventos++;

    printf("%6d: MISSAO %d TENT %d HAB REQ: [", tempo, id_missao, m->missoes[id_missao].tentativa);
    cjto_imprime (m->missoes[id_missao].habilidades);
    printf("]\n");

    //Preenche um vetor com as distancias das bases em relacao a missao e um outro vetor com os ids das bases na ordem de 0 ate N_BASES
    for (i = 0; i < N_BASES; i++)
    {
        distancia = calcula_distancia (m->missoes[id_missao].local, m->bases[i].local);
        distancias[i] = distancia;
        bases_ordenadas[i] = i;
    }

    //Ordena os vetores "distancias" e "bases_ordenadas" por distancia da menor para a maior
    ordena_bases (distancias, bases_ordenadas, m->n_bases);

    i = 0;
    BMP = 0;

    //Loop que verifica se existe uma BMP
    while ((i < m->n_bases) && (BMP == 0))
    {
        uniao_hab = cjto_cria (m->n_habilidades);

        printf("%6d: MISSAO %d BASE %d DIST %d HEROIS [", tempo, id_missao, m->bases[bases_ordenadas[i]].id, distancias[i]);
        cjto_imprime (m->bases[bases_ordenadas[i]].presentes);
        printf("]\n");

        //Verifica quais os herois estao presentes em uma base e adiciona suas habilidades ao conjunto uniao
        for (int j = 0; j < m->n_herois; j++)
        {
            if ((cjto_pertence (m->bases[bases_ordenadas[i]].presentes, j)) && (m->herois[j].vivo))
            {
                printf("%6d: MISSAO %d HAB HEROI %2d: [", tempo, id_missao, m->herois[j].id);
                cjto_imprime (m->herois[j].habilidades);
                printf("]\n");

                temp = cjto_uniao (uniao_hab, m->herois[j].habilidades);
                cjto_destroi (uniao_hab);
                uniao_hab = cjto_copia (temp);
                cjto_destroi (temp);
            }
        }

        printf("%6d: MISSAO %d UNIAO HAB BASE %d: [", tempo, id_missao, m->bases[bases_ordenadas[i]].id);
        cjto_imprime (uniao_hab);
        printf("]\n");

        //Verifica se a base atual eh uma BMP
        if (cjto_contem (uniao_hab, m->missoes[id_missao].habilidades))
            BMP = 1;
        else
            cjto_destroi (uniao_hab);
        i++;
    }

    //Caso haja uma BMP
    if (BMP)
    {
        //Decrementa o valor de i para que i represente a BMP
        i--;
        
        m->missoes[id_missao].cumprida = 1;

        printf("%6d: MISSAO %d CUMPRIDA BASE %d HABS: [", tempo, id_missao, m->bases[bases_ordenadas[i]].id);
        cjto_imprime (uniao_hab);
        printf("]\n");

        //Incrementa o numero de missoes realizados pela base
        m->bases[bases_ordenadas[i]].missoes++;

        //Calcula o risco da missao para um heroi baseado em seus atributos e verifica se o heroi morre ou ganha experiencia na missao
        for (int j = 0; j < m->n_herois; j++)
        {
            if ((cjto_pertence (m->bases[bases_ordenadas[i]].presentes, j)) && (m->herois[j].vivo))
            {
                risco = m->missoes[id_missao].perigo / (m->herois[j].paciencia + m->herois[j].exp + 1.0);
                if (risco > aleat (0, 30))
                {
                    m->herois[j].missao_morte = id_missao;
                    novo = cria_evento ("MORRE", tempo, m->herois[j].id, m->bases[bases_ordenadas[i]].id);
                    if (insere_evento (lista, novo) == 0)
                    {
                        printf("Erro ao inserir evento.\n");
                        return 0;
                    }
                }
                m->herois[j].exp++;
            }
        }

        cjto_destroi (uniao_hab);
        return 1;
    }

    //Caso nao haja uma BMP
    printf("%6d: MISSAO %d IMPOSSIVEL\n", tempo, id_missao);

    //Agenda a missao para 1 dia depois do tempo atual
    novo = cria_evento ("MISSAO", tempo + 24*60, id_missao, 0);
    if (insere_evento (lista, novo) == 0)
    {
        printf("Erro ao inserir evento.\n");
        return 0;
    }

    m->missoes[id_missao].tentativa++;

    return 1;
}

void evento_fim(lef *lista, mundo *m, int tempo)
{
    int i, missoes_cumpridas, tentativas_min, tentativas_max, tentativas, mortes;
    float taxa_missoes, media_tentativas, mortalidade;

    printf ("%6d: FIM\n", tempo);

    //Imprime todos os herois e atributos
    for (i = 0; i < m->n_herois; i++)
        imprime_heroi (m->herois[i]);

    //Imprime todas as bases e atributos
    for (i = 0; i < m->n_bases; i++)
        imprime_base (m->bases[i]);

    printf("EVENTOS TRATADOS: %d\n", m->eventos);

    missoes_cumpridas = 0;
    tentativas_min = 0;
    tentativas_max = 1;
    tentativas = 0;

    //Contabiliza quantas missoes foram cumpridas e qual foi o numero minimo e maximo para todas as missoes
    for (i = 0; i < m->n_missoes; i++)
    {
        if (m->missoes[i].cumprida)
        {
            missoes_cumpridas++;
            tentativas += m->missoes[i].tentativa;
            if(m->missoes[i].tentativa < tentativas_min || tentativas_min == 0)
                tentativas_min = m->missoes[i].tentativa;
            if(m->missoes[i].tentativa > tentativas_max)
                tentativas_max = m->missoes[i].tentativa;
        }
    }
    
    taxa_missoes = (float)missoes_cumpridas / N_MISSOES * 100;
    media_tentativas = tentativas / missoes_cumpridas;

    printf("MISSOES CUMPRIDAS: %d/%d (%.1f%%)\n", missoes_cumpridas, m->n_missoes, taxa_missoes);
    printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f\n", tentativas_min, tentativas_max, media_tentativas);
    
    mortes = 0;
    for (i = 0; i < m->n_herois; i++)
        if (!m->herois[i].vivo)
            mortes++;
    
    mortalidade = (float)mortes / m->n_herois * 100;

    printf("TAXA MORTALIDADE: %.1f%%\n", mortalidade);

    lista = destroi_LEF (lista);
    m = mundo_destroi (m);
}
