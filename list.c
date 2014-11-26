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

int procura_lista (List lista, char *chave)
{
    List temp = lista->next;

    while (temp != NULL)
    {
        if (strcomp(temp->carater, chave) == 0)
        {
            return 1; /* Found */
        }
        
        temp = temp->next;
    }

    return 0; /* Not found */
}

void insere_lista (List lista, char *carater_g)
{
    List no = (List) malloc (sizeof (List_node));
    List ptr = lista;
    if (no != NULL)
    {
        no->carater = (char *)malloc(strlen(carater_g)*sizeof(char));
        strcpy(no->carater, carater_g);
        no->next=NULL;
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
    
    while (l != NULL)
    {
        printf("Char: %i Indice: %i\n", (int)l->carater[0], l->indice);
        l=l->next;
    }
}

int get_index(List lista, char *c) 
{
    List ptr = lista->next;

    while (ptr != NULL)
    {
        if (strcomp(ptr->carater, c) == 0)
        {
            return ptr->indice;
        }
        ptr = ptr->next;
    }

    return 0; /* c not found */
}

int strcomp(char *one, char *two)
{
    int i;

    if (strlen(one) == strlen(two))
    {
        for (i = 0; i < strlen(one); i++)
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
