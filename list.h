typedef struct lnode *List;

typedef struct lnode
{
    int *carater;
    int comprimento;
	int indice;
	List next;
} List_node;

/* Cria uma lista */
List cria_lista (void);

/* Verifica se a lista está vazia */
int lista_vazia(List lista);

/* Procura na lista um elemento pelo 'carater' da struct*/
int procura_lista (List lista, int *chave, int comp);

/* Insere na lista o 'carater' incrementando o indice automaticamente */
void insere_lista (List lista, int *carater, int comp);

/* Imprime a lista */
void imprime_lista (List lista);

/* Get Index */
int get_index(List lista, int *chave, int comp);

/* Carater Comparator */
/* Returns 1 if they are different and 0 if they are equal */
int caratercmp(int *lista1, int length1, int *lista2, int length2);
