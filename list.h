typedef struct lnode *List;

typedef struct lnode
{
    char *carater;
	int indice;
	List next;
} List_node;

/* Cria uma lista */
List cria_lista (void);

/* Destroi uma lista */
List destroi_lista (List lista);

/* Verifica se a lista está vazia */
int lista_vazia(List lista);

/* Procura na lista um elemento pelo 'carater' da struct*/
void procura_lista (List lista, char *chave, List *ant, List *actual);

/* Elimina elemento da lista pelo 'carater' da struct */
void elimina_lista (List lista, char *carater_g);

/* Insere na lista o 'carater' incrementando o indice automaticamente */
void insere_lista (List lista, char *carater_g);

/* Imprime a lista */
void imprime_lista (List lista);
