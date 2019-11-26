#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "grafo.h"

typedef struct REDE REDE_AUXILIAR;
typedef struct LISTA LISTA_CAMINHO;

//rede auxiliar para acesso aos vertices
REDE_AUXILIAR* cria_rede();
LISTA_CAMINHO* cria_lista();
Vertice* retorna_primeira_tarefa(REDE_AUXILIAR* aux);
Vertice* retorna_ultima_tarefa(REDE_AUXILIAR* aux);
void acha_caminho_critico(Grafo* redePert,REDE_AUXILIAR* aux,Vertice* percorre);
void rede_libera(Grafo* redePert,REDE_AUXILIAR* aux);

//inserção e criação da rede
void insere_tarefas(Grafo* redePert,FILE* conteudoRede,REDE_AUXILIAR* aux );