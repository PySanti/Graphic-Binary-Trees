#include <stdio.h>
#include <stdlib.h>
#include "arboles.h"


typedef struct list_node 
{
    struct tree_node *element;
} list_node;

typedef struct lista
{
    list_node *cabeza;
} lista;

