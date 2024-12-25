//Definicao dos eventos da simulacao do mundo
//Carlos Souto - GRR20231951 - Nov/2024

#ifndef EVENTOS_H
#define EVENTOS_H

#include "LEF.h"
#include "mundo.h"

//Insere na LEF os eventos iniciais do mundo
//Cada heroi chegara em uma base aleatoria em algum momento dos tres primeiros dias de simulação
//Cada missão eh agendada para ocorrer em algum momento da simulação
//O evento FIM eh agendado para o instante final da simulação
//
//Retorna 1 para sucesso ou 0 para erro
int eventos_iniciais (lef *lista, mundo *m);

//Trata a chegada de um heroi em uma base. Pode deixa-lo entrar, adicionar a fila, ou agendar sua desistencia
//Retorna 1 para sucesso ou 0 para erro
int evento_chega (lef *lista, mundo *m, int tempo, int id_heroi, int id_base);

//Insere o heroi na fila de espera da base
//Retorna 1 para sucesso ou 0 para erro
int evento_espera (lef *lista, mundo *m, int tempo, int id_heroi, int id_base);

//Faz o heroi desistir de entrar na base
//Retorna 1 para sucesso ou 0 para erro
int evento_desiste (lef *lista, mundo *m, int tempo, int id_heroi, int id_base);

//Avisa o porteiro da base sobre alteracao nos herois presentes na base para liberar o proximo da fila se necessario
//Retorna 1 para sucesso ou 0 para erro
int evento_avisa (lef *lista, mundo *m, int tempo, int id_base);

//Insere o heroi dentro de uma base agendando sua saida
//Retorna 1 para sucesso ou 0 para erro
int evento_entra (lef *lista, mundo *m, int tempo, int id_heroi, int id_base);

//Retira o heroi da base
//Retorna 1 para sucesso ou 0 para erro
int evento_sai (lef *lista, mundo *m, int tempo, int id_heroi, int id_base);

//Trata a viagem do heroi até uma base, agendando a sua chegada
//Retorna 1 para sucesso ou 0 para erro
int evento_viaja (lef *lista, mundo *m, int tempo, int id_heroi, int id_base_destino);

//Trata a morte do heroi
//Retorna 1 para sucesso ou 0 para erro
int evento_morre (lef *lista, mundo *m, int tempo, int id_heroi, int id_base);

//Trata um evento de missao
//Retorna 1 para sucesso ou 0 para erro
int evento_missao (lef *lista, mundo *m, int tempo, int id_missao);

//Trata o fim do mundo. Destruindo o mundo e imprimindo estatisticas
void evento_fim (lef *lista, mundo *m, int tempo);

#endif