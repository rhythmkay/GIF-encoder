typedef struct lnode *List;

typedef struct lnode
{
    char *carater;
	int indice;
    int len;
	List next;
} List_node;

/* Cria uma lista */
List cria_lista (void);

/* Verifica se a lista está vazia */
int lista_vazia(List lista);

/* Procura na lista um elemento pelo 'carater' da struct*/
int procura_lista (List lista, char *chave, int chave_leng);

/* Insere na lista o 'carater' incrementando o indice automaticamente */
void insere_lista (List lista, char *carater_g, int carater_g_leng);

/* Imprime a lista */
void imprime_lista (List lista);

/* Get Index */
int get_index(List lista, char *c, int c_leng);

/* strcmp made by me */
int strcomp(char *one, int one_leng, char *two, int two_leng);
