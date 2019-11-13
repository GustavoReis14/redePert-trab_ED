#include "pert.h"

typedef struct {
    char origem;
    char destino;
    int custo;
}PERT;

struct REDE{
    Vertice* primeira_tarefa;
    Vertice* ultima_tarefa;
};

static void cria_tarefas(Grafo* redePert, FILE* conteudoRede, PERT* tarefa);
static int quantidade_de_tarefas(FILE* conteudoRede);
static char* converte_para_string(char caracter);
static void seta_rede(Grafo* redePert, FILE* conteudoRede, REDE_AUXILIAR* aux, PERT* tarefa);


void insere_tarefas (Grafo* redePert,FILE* conteudoRede,REDE_AUXILIAR* aux ) {
    if (conteudoRede == NULL) {
        printf("Erro ao abrir o arquivo\n");
        exit(0);
    }
    PERT tarefa[quantidade_de_tarefas(conteudoRede)];
          
    int i = 0;
    while(!feof(conteudoRede)) {
        char inicio, finalTarefa;
        char virg,virg2;
        int tempoExecucao;
        fscanf(conteudoRede," %c %c %c %c %i\n", &inicio,&virg, &finalTarefa,&virg2, &tempoExecucao);
        tarefa[i].origem = inicio;
        tarefa[i].destino = finalTarefa;
        tarefa[i].custo = tempoExecucao;
        //printf("%c %c %i\n", tarefa[i].origem, tarefa[i].destino, tarefa[i].custo);
        i++;
    }
    fseek(conteudoRede, 0, SEEK_SET);
    cria_tarefas(redePert,conteudoRede,tarefa);
    seta_rede(redePert,conteudoRede,aux, tarefa);
    fclose(conteudoRede);
}

REDE_AUXILIAR* cria_rede() {
    REDE_AUXILIAR * aux = (REDE_AUXILIAR *) malloc(sizeof(REDE_AUXILIAR));
    aux->primeira_tarefa = NULL;
    aux->ultima_tarefa = NULL;
    return aux;
}

static int quantidade_de_tarefas(FILE* conteudoRede) {  
    int cont = 0;
    while(!feof(conteudoRede)) {
        char inicio, finalTarefa;
        char virg,virg2;
        int tempoExecucao;
        fscanf(conteudoRede,"%c %c %c %c %i\n", &inicio,&virg, &finalTarefa,&virg2, &tempoExecucao);
        cont++;
    }
    fseek(conteudoRede, 0, SEEK_SET);
    return cont;
}

static void cria_tarefas(Grafo* redePert, FILE* conteudoRede, PERT* tarefa){
    int qt_tarefa = quantidade_de_tarefas(conteudoRede);
    char nomes_das_tarefas[qt_tarefa * 2];
    int cont = 0;
    for (int i = 0; i < qt_tarefa; i++) {
        int j;
        for (j = 0; j < strlen(nomes_das_tarefas); j++) {
            if (tarefa[i].origem == nomes_das_tarefas[j]) break;
        }
        if (j == strlen(nomes_das_tarefas)) nomes_das_tarefas[cont] = tarefa[i].origem, cont++;
        
        for (j = 0; j < strlen(nomes_das_tarefas); j++) {
            if (tarefa[i].destino == nomes_das_tarefas[j]) break;
        }
        if (j == strlen(nomes_das_tarefas)) nomes_das_tarefas[cont] = tarefa[i].destino, cont++;
    }

    for (int i = 0; i < strlen(nomes_das_tarefas); i++) {
        char* nome = converte_para_string(nomes_das_tarefas[i]);
        grafo_cria_vertice(redePert, nome);
        //printf("Vertice %s criado \n", nome);
        free(nome);
    }

    for (int i = 0; i < qt_tarefa; i++) {
        char* nome_origem = converte_para_string(tarefa[i].origem);
        char* nome_chegada = converte_para_string(tarefa[i].destino);
        grafo_insere_aresta (redePert, grafo_retorna_vertice_por_nome(redePert, nome_origem), grafo_retorna_vertice_por_nome(redePert, nome_chegada), tarefa[i].custo);
        //printf("Aresta criada entra %s %s com o peso %i \n", nome_origem, nome_chegada, tarefa[i].custo);
        free(nome_origem);
        free(nome_chegada);
    }

}

static char* converte_para_string(char caracter) {
    char* string = (char*) malloc (2);
    string[0] = caracter;
    string[1] = '\0';
    return string;
}

static void seta_rede(Grafo* redePert, FILE* conteudoRede, REDE_AUXILIAR* aux, PERT* tarefa) {
    int qt_tarefas = quantidade_de_tarefas(conteudoRede);
    for (int i = 0; i < qt_tarefas; i++) {
        int j;
        for (j = 0; j < qt_tarefas; j++) {
            if (tarefa[i].origem == tarefa[j].destino) break;
        }
        if (j == qt_tarefas) {
            char* nome = converte_para_string(tarefa[i].origem);
            aux->primeira_tarefa = grafo_retorna_vertice_por_nome(redePert, nome);
            free(nome);
            break;
        }
    }
    for (int i = 0; i < qt_tarefas; i++) {
        int j;
        for (j = 0; j < qt_tarefas; j++) {
            if (tarefa[i].destino == tarefa[j].origem) break;
        }
        if (j == qt_tarefas) {
            char* nome = converte_para_string(tarefa[i].destino);
            aux->ultima_tarefa = grafo_retorna_vertice_por_nome(redePert, nome);
            free(nome);
            break;
        }
    }

    //printf("tarefa inicial igual a %s\ntarefa final igual a %s\n",grafo_retorna_nome(aux->primeira_tarefa), grafo_retorna_nome(aux->ultima_tarefa));
}

Vertice* retorna_primeira_tarefa(REDE_AUXILIAR* aux) {
    return aux->primeira_tarefa;
}
Vertice* retorna_ultima_tarefa(REDE_AUXILIAR* aux) {
    return aux->ultima_tarefa;
}