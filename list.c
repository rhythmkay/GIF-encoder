#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

List cria_lista (void)
{
    List aux;
    aux = (List) malloc (sizeof (List_node));
    if (aux != NULL)
    {
        aux->carater = NULL;
        aux->indice = -1;
        aux->next = NULL;
    }
    return aux;
}

int lista_vazia (List lista)
{
    return (lista->next == NULL ? 1 : 0);
}

int procura_lista (List lista, char *chave, int chave_leng)
{
    List temp = lista->next;

    while (temp != NULL)
    {
        if (strcomp(temp->carater, temp->len, chave, chave_leng) == 0)
        {
            return 1; /* Found */
        }
        
        temp = temp->next;
    }

    return 0; /* Not found */
}

void insere_lista (List lista, char *carater_g, int carater_g_leng)
{
    List no = (List) malloc (sizeof (List_node));
    List ptr = lista;
    int i;

    if (no != NULL)
    {
        no->carater = (char *)malloc(strlen(carater_g)*sizeof(char));
        for (i = 0; i < carater_g_leng; i++)
        {
            no->carater[i] = carater_g[i];
        }
        no->next=NULL;
        no->len = carater_g_leng;
        while (ptr != NULL)
        {
            if (ptr->next == NULL)
            {
                no->indice = ptr->indice + 1;
                ptr->next = no;
                break;
            }
            ptr = ptr->next;
        }
    }
}

void imprime_lista (List lista)
{
    List l = lista->next;
    int i;
    
    while (l != NULL)
    {
        printf("Char:");
        for (i = 0; i < l->len; i++)
        {
            printf("%i", (int)l->carater[i]);
        }
        printf(" Indice: %i\n", l->indice);
        l=l->next;
    }
}

int get_index(List lista, char *c, int c_leng) 
{
    List ptr = lista->next;

    while (ptr != NULL)
    {
        if (strcomp(ptr->carater, ptr->len, c, c_leng) == 0)
        {
            return ptr->indice;
        }
        ptr = ptr->next;
    }

    return -1; /* c not found */
}

int strcomp(char *one, int one_leng, char *two, int two_leng)
{
    int i;

    if (one_leng == two_leng)
    {
        for (i = 0; i < one_leng; i++)
        {
            if (one[i] != two[i])
            {
                return 1;
            }
        }

        return 0; /* Strings are equal */
    }

    return 1; /* Strings are different */
}
