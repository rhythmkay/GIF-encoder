typedef struct lnode *List;

typedef struct lnode
{
    char *carater;
	int indice;
	List next;
} List_node;

List cria_lista (void);

List destroi_lista (List lista);

int lista_vazia(List lista);

void procura_lista (List lista, char *chave, List *ant, List *actual);

void elimina_lista (List lista, char *carater_g);

void insere_lista (List lista, char *carater_g);

void imprime_lista (List lista);
