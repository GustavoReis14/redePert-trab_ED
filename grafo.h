typedef struct grafo Grafo;
typedef struct vertice Vertice;

/*fun��es para cria��o/remo��o de um grafo*/
Grafo * grafo_cria ();
void grafo_libera (Grafo * grafo);

/*fun��es para manipula��o dos v�rtices de um grafo*/
int grafo_qtd_vertices (Grafo * grafo);
Vertice ** grafo_busca_vertices_saida(Grafo * grafo, Vertice * vertice, int * cont);
Vertice ** grafo_busca_vertices_entrada(Grafo * grafo, Vertice * vertice, int * cont);


/*fun��es para manipula��o das arestas do grafo*/
void grafo_insere_aresta (Grafo * grafo, Vertice * vertice1, Vertice * vertice2, int peso);
int grafo_busca_aresta(Grafo * grafo, Vertice * vertice1, Vertice * vertice2);

/*fun��es para cria��o de v�rtices*/
Vertice * grafo_cria_vertice(Grafo * grafo, const char nome[]);
Vertice * grafo_retorna_vertice(Grafo * grafo, int indice);
Vertice * grafo_retorna_vertice_por_nome(Grafo* grafo, const char nome[]);

/*fun��es para obten��o dos dados de v�rtices*/
char * grafo_retorna_nome(Vertice * vertice);



/*fun��o para impress�o tabulada a partir de um v�rtice*/
void grafo_imprime_tab (Grafo * grafo, Vertice * vertice);
