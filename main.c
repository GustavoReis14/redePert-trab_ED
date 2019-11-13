#include <stdlib.h>
#include <stdio.h>
#include "grafo.h"
#include "pert.h"

int main () {
    Grafo* redePert = grafo_cria();
    REDE_AUXILIAR* aux = cria_rede();
    FILE* conteudoRede = fopen("redePert.txt","r");
    insere_tarefas(redePert, conteudoRede,aux);
    printf("primeira tarefa : %s\nultima tarefa : %s\n", grafo_retorna_nome(retorna_primeira_tarefa(aux)), grafo_retorna_nome(retorna_ultima_tarefa(aux)));
    caminho_critico(redePert,aux,retorna_primeira_tarefa(aux));
    grafo_libera(redePert);

    free(aux);
    return 1;
}