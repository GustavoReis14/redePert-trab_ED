#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "grafo.h"


struct aresta{
    int peso;
    Vertice * vertice_saida;
    struct aresta * prox_aresta;
};

typedef struct aresta Aresta;

struct vertice{
    char nome[50];
    Vertice * prox_vertice;
    struct aresta * primeira_aresta;
    struct aresta * ultima_aresta;
};

/*composi��o de um grafo*/
struct grafo{
    Vertice * primeiro_vertice;
    Vertice * ultimo_vertice;
};


void grafo_imprime_tab_ (Grafo * grafo, Vertice * vertice1, Vertice * vertice2, int prof);
void grafo_libera_vertices (Vertice * vertice);
void grafo_libera_arestas (Aresta * aresta);


Grafo * grafo_cria()
{
    Grafo * grafo = (Grafo *) malloc(sizeof(Grafo));
    grafo->primeiro_vertice = NULL;
    grafo->ultimo_vertice = NULL;
    return grafo;
}

void grafo_libera (Grafo * grafo){
    grafo_libera_vertices (grafo->primeiro_vertice);
    free(grafo);
}



void grafo_libera_vertices (Vertice * primeiro_vertice){
    Vertice * v = primeiro_vertice;
    while (v!=NULL){
        Aresta * primeira_aresta = v->primeira_aresta;
        grafo_libera_arestas(primeira_aresta);
        Vertice * aux = v->prox_vertice;
        free(v);
        v = aux;
    }
}

void grafo_libera_arestas (Aresta * primeira_aresta){
    Aresta * a = primeira_aresta;
    while (a!=NULL){
        Aresta * aux = a->prox_aresta;
        free(a);
        a = aux;
    }
}

int grafo_qtd_vertices (Grafo * grafo){
    Vertice * vertice = grafo->primeiro_vertice;
    int qtd = 0;
    while (vertice!=NULL){
        qtd++;
        vertice = vertice->prox_vertice;
    }

    return qtd;
}

Vertice ** grafo_busca_vertices_saida(Grafo * grafo, Vertice * vertice, int * cont){

    int num_arestas =0;
    Aresta * aresta = vertice->primeira_aresta;
    while (aresta!=NULL){
        num_arestas++;
        aresta = aresta->prox_aresta;
    }

    Vertice ** vertices = (Vertice **)malloc(sizeof(Vertice * ) * num_arestas);
    int num_aresta = 0;
    aresta = vertice->primeira_aresta;
    while (aresta!=NULL){
        vertices[num_aresta] = aresta->vertice_saida;
        aresta = aresta->prox_aresta;
        num_aresta++;
    }
    *cont = num_arestas;
    return vertices;

}

Vertice ** grafo_busca_vertices_entrada(Grafo * grafo, Vertice * vertice, int * cont){

    int num_arestas =0;


    Vertice * v = grafo->primeiro_vertice;
    while (v!=NULL){
        Aresta* aresta = v->primeira_aresta;
        while (aresta!=NULL){
            if (aresta->vertice_saida == vertice)
                num_arestas++;
            aresta = aresta->prox_aresta;
        }
        v = v->prox_vertice;
   }


    Vertice ** vertices = (Vertice **)malloc(sizeof(Vertice * ) * num_arestas);
    int num_aresta = 0;

    v = grafo->primeiro_vertice;
    while (v!=NULL){
        Aresta* aresta = v->primeira_aresta;
        while (aresta!=NULL){
            if (aresta->vertice_saida == vertice){
                vertices[num_aresta] = v;;
                num_aresta++;
            }
            aresta = aresta->prox_aresta;
        }
        v = v->prox_vertice;
   }


    *cont = num_arestas;
    return vertices;

}

void grafo_insere_aresta (Grafo * grafo, Vertice * vertice1, Vertice * vertice2, int peso){

    Aresta * aresta = (Aresta*) malloc(sizeof(Aresta));
    aresta->peso = peso;
    aresta->vertice_saida = vertice2;
    aresta->prox_aresta = NULL;

    if (vertice1->ultima_aresta!=NULL){
        vertice1->ultima_aresta->prox_aresta = aresta;
        vertice1->ultima_aresta = aresta;
    }
    else {
        vertice1->primeira_aresta = aresta;
        vertice1->ultima_aresta = aresta;
    }
}

int grafo_busca_aresta(Grafo * grafo, Vertice * vertice1, Vertice * vertice2){
    Aresta * aresta = vertice1->primeira_aresta;
    while (aresta!=NULL){
        if (aresta->vertice_saida == vertice2)
            return aresta->peso;
        aresta = aresta->prox_aresta;
    }

    return -1;
}


Vertice * grafo_cria_vertice(Grafo * grafo, const char nome[]){

    Vertice * v = (Vertice *)malloc(sizeof(Vertice));
    strcpy(v->nome, nome);
    v->prox_vertice = NULL;
    v->primeira_aresta = NULL;
    v->ultima_aresta = NULL;

    if (grafo->ultimo_vertice!=NULL){
        grafo->ultimo_vertice->prox_vertice = v;
        grafo->ultimo_vertice = v;
    }
    else {
        grafo->primeiro_vertice = v;
        grafo->ultimo_vertice = v;
    }

    return v;
}


Vertice * grafo_retorna_vertice(Grafo * grafo, int indice){
    Vertice * vertice = grafo->primeiro_vertice;
    int qtd = 0;
    while (vertice!=NULL){
        if (qtd == indice)
            return vertice;
        qtd++;
        vertice = vertice->prox_vertice;
    }
    return NULL;

}

Vertice * grafo_retorna_vertice_por_nome(Grafo* grafo, const char nome[]) {
    Vertice * vertice = grafo->primeiro_vertice;
    while (vertice != NULL) {
        if (!strcmp(vertice->nome, nome)) {
            return vertice;
        }
        vertice = vertice->prox_vertice;
    }
    return NULL;
}

char * grafo_retorna_nome(Vertice * vertice){
    return vertice->nome;
}



void grafo_imprime_tab (Grafo * grafo, Vertice * vertice)
{
   grafo_imprime_tab_(grafo, vertice, NULL, 0);
}


void grafo_imprime_tab_ (Grafo * grafo, Vertice * vertice1, Vertice * vertice2, int prof)
{
   static int tab = 0;
   int x;
   for(x=0;x<tab;x++)
       printf("   ");
   if (vertice2!=NULL)
        printf("%s -- %s (%d)\n", vertice2->nome,vertice1->nome, grafo_busca_aresta(grafo, vertice2,vertice1));

   if (prof==3) return;

   tab+=1;
   int num_vertices = 0;
   Vertice ** vertices = grafo_busca_vertices_saida(grafo, vertice1, &num_vertices);
   int i = 0;
   for (i=0;i<num_vertices;i++){
       grafo_imprime_tab_(grafo, vertices[i], vertice1, prof + 1);
   }

   tab-=1;
}




