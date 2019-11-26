#include "pert.h"

typedef struct {
    char origem;
    char destino;
    int custo;
}PERT;


struct REDE{
    Vertice* primeira_tarefa;
    Vertice* ultima_tarefa;
    int temp;
    int caminho_critico;
    int rota;
    int rota_temp;
    int modifica_caminho;
    char* nome_caminho;
    char* nome_dependencia;
    char* novo_caminho;
    int insert_nome;
    int n_dependencia;
    int novo_critico;
    int tam_nome;
};

static void cria_tarefas(Grafo* redePert, FILE* conteudoRede, PERT* tarefa,REDE_AUXILIAR* aux);
static int quantidade_de_tarefas(FILE* conteudoRede);
static char* converte_para_string(char caracter);
static void seta_rede(Grafo* redePert, FILE* conteudoRede, REDE_AUXILIAR* aux, PERT* tarefa);
static void imprime_caminho_critico(Grafo* redePert,REDE_AUXILIAR* aux,Vertice* percorre);
static void trata_caminho(char* palavra);
static void printa_caminho_critico(Grafo* redePert,REDE_AUXILIAR* aux,Vertice* percorre);
static void printa_novo_caminho_critico(Grafo* redePert,REDE_AUXILIAR* aux,Vertice* percorre);
static void remove_dependencia(Grafo* redePert,REDE_AUXILIAR* aux,Vertice* percorre);
static void acha_novo_caminho_critico(Grafo* redePert,REDE_AUXILIAR* aux,Vertice* percorre);


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
    aux->temp = 0;
    aux->caminho_critico = 0;
    aux->rota = 0;
    aux->rota_temp = 0;
    aux->nome_caminho = (char*) malloc (quantidade_de_tarefas(conteudoRede)*2);
    aux->nome_dependencia = (char*) malloc (quantidade_de_tarefas(conteudoRede)*2);
    aux->insert_nome = 0;
    aux->n_dependencia = 0;
    aux->tam_nome = quantidade_de_tarefas(conteudoRede)*2;
    fseek(conteudoRede, 0, SEEK_SET);
    cria_tarefas(redePert,conteudoRede,tarefa,aux);
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

static void cria_tarefas(Grafo* redePert, FILE* conteudoRede, PERT* tarefa, REDE_AUXILIAR* aux){
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
        if (tarefa[i].custo == 0) {
            aux->nome_dependencia[aux->n_dependencia] = tarefa[i].origem;
            aux->n_dependencia++;
        }
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

void acha_caminho_critico(Grafo* redePert,REDE_AUXILIAR* aux,Vertice* percorre) {
    int cont = 0;
    static int chamada = 0;
    chamada++;
    Vertice ** caminhos = grafo_busca_vertices_saida(redePert, percorre, &cont);
    for (int i = 0; i < cont; i++){
        aux->temp += grafo_busca_aresta(redePert, percorre, caminhos[i]);
        if (!strcmp(grafo_retorna_nome(caminhos[i]),grafo_retorna_nome(retorna_ultima_tarefa(aux)))){
            aux->rota_temp++;
            if (aux->temp > aux->caminho_critico) {
                aux->caminho_critico = aux->temp;
                aux->rota = aux->rota_temp;
            }   
        }
        acha_caminho_critico(redePert,aux, caminhos[i]);
        aux->temp -= grafo_busca_aresta(redePert, percorre, caminhos[i]);
    }
    chamada--;
    if (chamada == 0) { 
        printf("Tempo para o caminho critico %dh\n",aux->caminho_critico);
        //printf("Rota que chegou no objetivo %d\n",aux->rota);
        //printf("Numero de dependencia no caminho = %d\nNome da(s) dependencia(s) : %s\n",aux->n_dependencia,aux->nome_dependencia);
        aux->rota_temp = 0;
        printa_caminho_critico(redePert,aux,percorre);
        trata_caminho(aux->nome_caminho);
        for (int i = 0; i < strlen(aux->nome_caminho);i++) {
            printf("%c",aux->nome_caminho[i]);
            if (i + 1 != strlen(aux->nome_caminho)) printf(" -> ");
        }
        printf("\n");
        remove_dependencia(redePert,aux,percorre);
    }
}

void rede_libera(Grafo* redePert,REDE_AUXILIAR* aux) {
    grafo_libera(redePert);
    free(aux->nome_caminho);
    free(aux->novo_caminho);
    free(aux);
}

static void printa_caminho_critico(Grafo* redePert,REDE_AUXILIAR* aux,Vertice* percorre){
    int cont = 0;
    Vertice ** caminhos = grafo_busca_vertices_saida(redePert, percorre, &cont);
    for (int i = 0; i < cont; i++){
        if (!strcmp(grafo_retorna_nome(caminhos[i]),grafo_retorna_nome(retorna_ultima_tarefa(aux)))){
            aux->rota_temp++;
            strcat(aux->nome_caminho, grafo_retorna_nome(caminhos[i]));
        }
        if (aux->rota_temp != aux->rota) printa_caminho_critico(redePert,aux, caminhos[i]);
        if (aux->rota_temp == aux->rota  || aux->rota_temp == 0 ) {
            strcat(aux->nome_caminho, grafo_retorna_nome(percorre));
        }
    }
}

static void trata_caminho(char* palavra) {
    int i = 0;
    int j = strlen(palavra) - 1;
    while (i < j) {
        char temp = palavra[i];
        palavra[i] = palavra[j];
        palavra[j] = temp;
        i++;
        j--;
    }
    int cont = 0;

    while (cont < strlen(palavra)) {
        for(i = 0; i < strlen(palavra); i++) {
            for(j = i + 1; palavra[j] != '\0'; j++){
                if(palavra[j] == palavra[i])  {
                    for(int k = j; palavra[k] != '\0'; k++) {
                        palavra[k] = palavra[k + 1];
                    }
                }
            }
	    }
        cont++;
    }
    
    
}

static void remove_dependencia(Grafo* redePert,REDE_AUXILIAR* aux,Vertice* percorre) {
    if (aux->n_dependencia == 0) {
        printf("A rede pert nao possui dependencias\n");
        return;
    }
    aux->novo_critico = 0;
    aux->temp = 0;
    aux->rota = 0;
    aux->rota_temp = 0;
    aux->novo_caminho = (char*) malloc (aux->tam_nome);
    acha_novo_caminho_critico(redePert,aux,retorna_primeira_tarefa(aux));
}

static void acha_novo_caminho_critico(Grafo* redePert, REDE_AUXILIAR* aux, Vertice* percorre){
    int cont = 0;
    static int chamada = 0;
    chamada++;
    Vertice ** caminhos = grafo_busca_vertices_saida(redePert, percorre, &cont);
    for (int i = 0; i < cont; i++){
        aux->temp += grafo_busca_aresta(redePert, percorre, caminhos[i]);
        if (grafo_busca_aresta(redePert, percorre, caminhos[i]) == 0) continue;
        if (!strcmp(grafo_retorna_nome(caminhos[i]),grafo_retorna_nome(retorna_ultima_tarefa(aux)))){
            aux->rota_temp++;
            if (aux->temp > aux->novo_critico) {
                aux->novo_critico = aux->temp;
                aux->rota = aux->rota_temp;
            }   
        }
        acha_novo_caminho_critico(redePert,aux, caminhos[i]);
        aux->temp -= grafo_busca_aresta(redePert, percorre, caminhos[i]);
    }
    chamada--;
    if (chamada == 0) {
        aux->rota_temp = 0;
        printa_novo_caminho_critico(redePert,aux,percorre);
        trata_caminho(aux->novo_caminho);
        if (!strstr(aux->novo_caminho,aux->nome_caminho)) {
            printf("Tempo para o NOVO caminho critico %dh\n",aux->novo_critico);
            for (int i = 0; i < strlen(aux->novo_caminho);i++) {
                if (aux->novo_caminho[i] >= 65 && aux->novo_caminho[i] <= 90)printf("%c",aux->novo_caminho[i]);
                else break;
                if (i + 1 != strlen(aux->novo_caminho)) printf(" -> ");
            }
            printf("\n");
        }else printf("A remoção de dependencias não altera o caminho critico!\n");
    } 
}

static void printa_novo_caminho_critico(Grafo* redePert,REDE_AUXILIAR* aux,Vertice* percorre){
    int cont = 0;
    Vertice ** caminhos = grafo_busca_vertices_saida(redePert, percorre, &cont);
    for (int i = 0; i < cont; i++){
        if (grafo_busca_aresta(redePert, percorre, caminhos[i]) == 0) continue;
        if (!strcmp(grafo_retorna_nome(caminhos[i]),grafo_retorna_nome(retorna_ultima_tarefa(aux)))){
            aux->rota_temp++;
            strcat(aux->novo_caminho, grafo_retorna_nome(caminhos[i]));
        }
        if (aux->rota_temp != aux->rota) printa_novo_caminho_critico(redePert,aux, caminhos[i]);
        if (aux->rota_temp == aux->rota  || aux->rota_temp == 0 ) {
            strcat(aux->novo_caminho, grafo_retorna_nome(percorre));
        }
    }
}