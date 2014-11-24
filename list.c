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
        aux->comprimento = 0;
        aux->indice = 0;
        aux->next = NULL;
    }
    return aux;
}

int lista_vazia (List lista)
{
    return (lista->next == NULL ? 1 : 0);
}

int procura_lista (List lista, int *chave, int comp)
{
    List temp = lista->next;

    while (temp != NULL)
    {
        if (caratercmp(temp->carater, temp->comprimento, chave, comp) == 0) /* They are equal */
        {
            return 1;
        }

        temp = temp->next;
    }

    return 0;
}

void insere_lista (List lista, int *carater, int comp)
{
    List no = (List) malloc (sizeof (List_node));
    List ptr = lista;
    int i;

    if (no != NULL)
    {
        no->carater = (int *)malloc(comp*sizeof(int));
        /* Copy carater to list */
        for (i = 0; i < comp; i++)
        {
            no->carater[i] = carater[i];
        }

        no->comprimento = comp;
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
    int i;
    
    while (l != NULL)
    {
        printf("Carater: ");
        for (i = 0; i < l->comprimento; i++)
        {
            printf("%i", l->carater[i]);
        }
        printf("Indice: %i\n", l->indice);
        l=l->next;
    }
}

int get_index(List lista, int *chave, int comp) 
{
    List ptr = lista->next;

    while (ptr != NULL)
    {
        if (caratercmp(ptr->carater, ptr->comprimento, chave, comp) == 0)
        {
            return ptr->indice;
        }
        ptr = ptr->next;
    }

    return 0; /* chave not found */
}

int caratercmp(int *lista1, int length1, int *lista2, int length2)
{
    int i;

    if (length1 == length2)
    {
        for (i = 0; i < length1; i++)
        {
            if (lista1[i] != lista2[i])
            {
                return 1; /* Different */
            }
        }
        
        return 0; /* Equal */
    }

    return 1; /* Different */
}
