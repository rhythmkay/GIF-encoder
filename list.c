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
        aux->indice = 0;
        aux->next = NULL;
    }
    return aux;
}

List destroi_lista(List lista)
{
    List temp_ptr;
    while (lista_vazia (lista) == 0)
    {
        temp_ptr = lista;
        lista= lista->next;
        free (temp_ptr);
    }
    free(lista);
    return NULL;
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
        if (strcmp(temp->carater, chave) == 0)
        {
            return 1;
        }

        temp = temp->next;
    }

    return 0;
}

void insere_lista (List lista, char *carater_g)
{
    List no = (List) malloc (sizeof (List_node));
    List ptr = lista;
    if (no != NULL)
    {
        printf("CHar:\n");
        printf("%s\n", carater_g);
        printf("\n");
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
        printf("Char: %s Indice: %i\n", l->carater, l->indice);
        l=l->next;
    }
}
