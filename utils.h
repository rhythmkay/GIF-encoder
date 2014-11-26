#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct lnode *List;

typedef struct lnode
{
    char *carater;
	int indice;
    int len;
	List next;
} List_node;

/* Global variables */
int bit_position, n_pos;
char buffer[1000000000000];

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

/* List Size */
int list_size(List dic);

/* strcmp made by me */
int strcomp(char *one, int one_leng, char *two, int two_leng);

/* Decimal to Binary */
void decimal_to_bin(int num);

/* Know the number of bits two write */
char num_bits(int n);

/* Write bits */
void write_bits(int n, int numero_bits,  FILE* file);
