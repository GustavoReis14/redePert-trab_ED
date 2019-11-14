#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "grafo.h"

typedef struct REDE REDE_AUXILIAR;

//rede auxiliar para acesso aos vertices
REDE_AUXILIAR* cria_rede();
Vertice* retorna_primeira_tarefa(REDE_AUXILIAR* aux);
Vertice* retorna_ultima_tarefa(REDE_AUXILIAR* aux);
void caminho_critico(Grafo* redePert,REDE_AUXILIAR* aux,Vertice* percorre);
void rede_libera(REDE_AUXILIAR* aux);

//inserção e criação da rede
void insere_tarefas(Grafo* redePert,FILE* conteudoRede,REDE_AUXILIAR* aux );