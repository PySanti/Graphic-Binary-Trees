// by Santiago De Andrade 12/08/2021
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//* ---------------------------------- MACROS

#define ROOT_X_POSITION  90
#define ROOT_Y_POSITION  5
#define NODE_X_SPACE     5
#define NODE_Y_SPACE     2
#define DERECHA          -1
#define IZQUIERDA        -2
#define NEWLINE          '\n'
#define NULL_CHAR        '\0'   
#define X                0
#define Y                1
#define TIME             2
#define DEBUG_MSG \
printf("\n\n-------------------------------------------- SIGO AQUI --------------------------------------------\n\n"); \
sleep(TIME); \


//* ---------------------------------- ESTRUCTURAS

typedef struct tree_node
{
    int element;
    struct tree_node *derecha;
    struct tree_node *izquierda;
    struct tree_node *padre;
    int position[2];
} tree_node;
typedef struct arbol
{
    tree_node *raiz;
} arbol;


typedef struct list_node 
{
    struct tree_node *element;
    struct list_node *siguiente;
    struct list_node *anterior;
} list_node;
typedef struct lista
{
    list_node *cabeza;
} lista;




//* ---------------------------------- PROTOTIPOS (ARBOLES)
// crea un tree_node y lo retorna, recibe el elemento del tree_node
tree_node *crear_tree_node(int element);

// crea un arbol y lo retorna
arbol *crear_arbol(void);

// Muestra el arbol, recibe la raiz del mismo
void mostrar_arbol(tree_node *tree_node);

// Tramita el proceso de insercion del nuevo_elemento en arbol, retorna false en caso de que el elemento ya este repetido
int insertar_elemento_arbol(tree_node *nuevo_elemento, arbol *arbol);

// le asigna la posicion a nuevo_tree_node, direccion_de_insercion seria DERECHA si es un hijo derecho e IZQUIERDA en caso contrario 
void asignar_posicion(tree_node *nuevo_tree_node, int direccion_de_insercion);

// actualiza la posicion de todos los hijos de uno de los tree_nodes
void actualizar_posicion_de_descendencia(tree_node *tree_node_principal, int direccion_de_movimiento);

// actualizar las posiciones de los nodos del arbol ante la insercion de un nuevo nodo
void actualizar_posiciones_de_arbol( tree_node *nuevo_nodo, arbol *arbol_op, lista *nodos );


// lleva a cabo todas las operaciones necesarias para la insercion de un nodo en el arbol, la unica funcion que reune a funciones de listas y arboles
void procesar_elemento_en_arbol(int number, arbol *arbol_op, lista *nodos );


// retorna true en caso de que element sea un nodo hijo de principal
int es_descendiente(  tree_node *wanted_node, tree_node *principal );

// retorna aquel nodo que coincida con algun descendiente de principal, en caso de uqe no coincida con nadie, se retorna NULL
tree_node *alguna_descendencia_coincide( tree_node *principal, lista *nodos );

// en caso de que nodo_a_evaluar coincida con otro nodo, se retornara la referencia al mismo, NULL en caso contrario
tree_node *coincide(tree_node *nodo_a_evaluar, lista *nodos);



//* ---------------------------------- PROTOTIPOS (LISTAS)
lista *crear_lista(void);
void agregar_nodo_lista(list_node *nuevo_list_node, lista *lista_operativa);
void mostrar_lista(lista *lista_operativa);
list_node *crear_list_node(tree_node *nuevo_tree_node);





//* ---------------------------------- PROTOTIPOS (OTROS)
// funcion creada para la entrada de cadenas, recibe la longitud de la cadena que se desea introducir y retorna la referencia a la misma. Tener en cuenta que limpiara el buffer y eliminara el salto de linea automaticamente
char *string_input(int str_len);

// recibe una cadena y retorna true en caso de que la cadena sea un numero
int is_number(char cadena[], int len);

void gotoxy(int x, int y);


//* ---------------------------------- DEFINICIONES (ARBOLES)
int es_descendiente(  tree_node *wanted_node, tree_node *principal )
{
    tree_node *nodo_actual = principal;
    while ( nodo_actual != NULL )
    {
        if ( wanted_node == nodo_actual )
        {
            return 1;
        }
        if ( wanted_node->element > nodo_actual->element )
        {
            nodo_actual = nodo_actual->derecha;
        }
        else 
        {
            nodo_actual = nodo_actual->izquierda;
        }
    }
    return 0;
}
tree_node *alguna_descendencia_coincide( tree_node *principal, lista *nodos )
{
    if ( nodos->cabeza != NULL )
    {
        list_node *curr_node = nodos->cabeza;
        tree_node *coincidencia = NULL;
        while ( curr_node != NULL )
        {
            if ( (es_descendiente(curr_node->element, principal)) && (( coincidencia =  coincide(curr_node->element, nodos) ) != NULL))
            {
                return coincidencia;
            }
            curr_node = curr_node->siguiente;
        }
        return NULL;
    }
}
void procesar_elemento_en_arbol(int number, arbol *arbol_op, lista *nodos)
{
    tree_node *nuevo_nodo = crear_tree_node( number );
    if (insertar_elemento_arbol( nuevo_nodo, arbol_op ) == 0)
    {
        return;
    }
    else 
    {
        agregar_nodo_lista( crear_list_node( nuevo_nodo ), nodos );
        if ( coincide( nuevo_nodo, nodos) != NULL )
        {
            int movimiento              = nuevo_nodo->padre->position[ X ] - nuevo_nodo->position[ X ];
            tree_node *nodo_padre       = nuevo_nodo->padre;
            tree_node *nodo_coincidente = NULL;
            tree_node *nodo_actual      = NULL;
            actualizar_posicion_de_descendencia(nodo_padre, movimiento);
            while (1)
            {
                while ( ( nodo_coincidente =  coincide( nodo_padre, nodos) ) != NULL)
                {
                    if ( es_descendiente(nodo_coincidente, nodo_padre->derecha) )
                    {
                        movimiento = -movimiento;
                        actualizar_posicion_de_descendencia(nodo_padre, movimiento);
                        nodo_padre->position[X] += movimiento;
                        if ( nodo_padre->izquierda != NULL )
                            actualizar_posicion_de_descendencia(nodo_padre->izquierda, movimiento);
                        break;
                    }
                    else if ( es_descendiente(nodo_coincidente, nodo_padre->izquierda ) )
                    {
                        movimiento = -movimiento;
                        actualizar_posicion_de_descendencia(nodo_padre, movimiento);
                        nodo_padre->position[X] += movimiento;
                        if ( nodo_padre->derecha != NULL )
                            actualizar_posicion_de_descendencia(nodo_padre->derecha, movimiento);
                        break;
                    }
                    else 
                    {
                        actualizar_posicion_de_descendencia(nodo_padre, -movimiento);
                        nodo_padre = nodo_padre->padre;
                        actualizar_posicion_de_descendencia(nodo_padre, movimiento);
                    }
                }
                nodo_actual = nodo_padre;
                while ( nodo_actual != NULL )
                {
                    if ( ( nodo_coincidente = alguna_descendencia_coincide(nodo_actual, nodos) ) != NULL )
                    {
                        if ( (nodo_coincidente->izquierda != NULL) && (es_descendiente(nodo_actual, nodo_coincidente->izquierda)) )
                            actualizar_posicion_de_descendencia(nodo_coincidente->izquierda, (movimiento < 0)? (movimiento) :(-movimiento)  );
                        else if ((nodo_coincidente->derecha != NULL) && (es_descendiente(nodo_actual, nodo_coincidente->derecha)) )
                            actualizar_posicion_de_descendencia(nodo_coincidente->derecha, (movimiento < 0)? (-movimiento) : (movimiento))  ;
                    }
                    nodo_actual = nodo_actual->padre;
                }
                break;
            }
        }
    }
}
void actualizar_posicion_de_descendencia(tree_node *tree_node_principal, int cantidad_de_movimiento)
{
    tree_node_principal->position[0] += cantidad_de_movimiento;
    if ( tree_node_principal->derecha != NULL )
    {
        actualizar_posicion_de_descendencia(tree_node_principal->derecha, cantidad_de_movimiento);
    }
    if ( tree_node_principal->izquierda != NULL )
    {
        actualizar_posicion_de_descendencia(tree_node_principal->izquierda, cantidad_de_movimiento);
    }
}
tree_node *coincide(tree_node *nodo_a_evaluar, lista *nodos)
{
    if ( !(nodos->cabeza == NULL) )
    {
        list_node *curr_node = nodos->cabeza;
        while ( curr_node != NULL )
        {
            if ( ( curr_node->element->position[X] == nodo_a_evaluar->position[X] ) && ( curr_node->element != nodo_a_evaluar ))
            {
                return curr_node->element;
            }
            curr_node = curr_node->siguiente;
        }
        return NULL;
    }
}





arbol *crear_arbol()
{
    arbol *new_tree = (arbol *)malloc(sizeof(arbol));
    new_tree->raiz = NULL;
    return new_tree;
}
void asignar_posicion(tree_node *nuevo_tree_node, int direccion_de_insercion)
{
    nuevo_tree_node->position[X] =  (direccion_de_insercion == DERECHA)? (nuevo_tree_node->padre->position[X] + NODE_X_SPACE) : (nuevo_tree_node->padre->position[X] - NODE_X_SPACE);
    nuevo_tree_node->position[Y] = nuevo_tree_node->padre->position[Y] + NODE_Y_SPACE;
}
tree_node *crear_tree_node(int element)
{
    tree_node *nuevo_tree_node = (tree_node *)malloc(sizeof(tree_node));
    nuevo_tree_node->element = element;
    return nuevo_tree_node;
}
void mostrar_arbol(tree_node *tree_node)
{
    if ( tree_node->padre != NULL )
    {
        if ( tree_node->padre->derecha == tree_node )
        {
            gotoxy(tree_node->position[X] , tree_node->position[Y]-1);
            printf("\\");
        }
        else 
        {            
            gotoxy(tree_node->position[X] + 2, tree_node->position[Y]-1);
            printf("/");
        }
    }
    gotoxy(tree_node->position[X], tree_node->position[Y]);
    printf("%i", tree_node->element);
    if (tree_node->derecha != NULL)
    {
        mostrar_arbol(tree_node->derecha);
    }
    if (tree_node->izquierda != NULL)
    {
        mostrar_arbol(tree_node->izquierda);
    }
}
int insertar_elemento_arbol(tree_node *nuevo_tree_node, arbol *arbol)
{
    if (arbol->raiz == NULL)
    {
        arbol->raiz = nuevo_tree_node;
        arbol->raiz->derecha = arbol->raiz->izquierda = arbol->raiz->padre = NULL;
        arbol->raiz->position[X] = ROOT_X_POSITION;
        arbol->raiz->position[Y] = ROOT_Y_POSITION;
    }
    else
    {
        tree_node *tree_node_actual = arbol->raiz;
        while (1)
        {
            if (nuevo_tree_node->element > tree_node_actual->element)
            {
                if ((tree_node_actual->derecha == NULL))
                {
                    tree_node_actual->derecha = nuevo_tree_node;
                    tree_node_actual->derecha->padre = tree_node_actual;
                    tree_node_actual->derecha->derecha = tree_node_actual->derecha->izquierda = NULL;
                    asignar_posicion(tree_node_actual->derecha, DERECHA); 
                    return 1;
                }
                else
                {
                    tree_node_actual = tree_node_actual->derecha;
                }
            }
            else if (nuevo_tree_node->element < tree_node_actual->element)
            {
                if ((tree_node_actual->izquierda == NULL))
                {
                    tree_node_actual->izquierda = nuevo_tree_node;
                    tree_node_actual->izquierda->padre = tree_node_actual;
                    tree_node_actual->izquierda->derecha = tree_node_actual->izquierda->izquierda = NULL;
                    asignar_posicion(tree_node_actual->izquierda, IZQUIERDA);
                    return 1;
                }
                else
                {
                    tree_node_actual = tree_node_actual->izquierda;
                }
            }
            else
            {
                return 0;
            }
        }
    }
}
//* ---------------------------------- DEFINICIONES (OTROS)
void gotoxy(int x, int y)
{
    printf("%c[%d;%df", 0x1B, y, x);
}
char *string_input(int str_len)
{
    char *cadena = (char *) calloc(sizeof(char), str_len);
    int i;
    fgets(cadena, str_len, stdin);
    for ( i = 0; i < str_len; ++i )
    {
        if ( cadena[ i ] ==  NEWLINE )
        {
            cadena[ i ] = NULL_CHAR;
            return cadena;
        }
    }
    while ( ( i = getchar() ) != NEWLINE );
    return cadena;
}
int is_number(char cadena[], int len)
{
    int i;
    for ( i = 0; i <  len; i++)
    {
        if ( !( cadena[ i ] >= '0' && cadena[ i ] <= '9' ) && ( cadena[ i ] != NULL_CHAR ) && ( cadena[ i ] != NEWLINE ))
        {
            return 0;
        }
    }
    return 1;
}
//* ---------------------------------- DEFINICIONES (LISTAS)

lista *crear_lista(void)
{
    lista *nueva_lista = (lista *) malloc(sizeof(lista));
    nueva_lista->cabeza = NULL;
    return nueva_lista;
}
void agregar_nodo_lista(list_node *nuevo_list_node, lista *lista_operativa)
{
    if ( lista_operativa->cabeza == NULL)
    {
        lista_operativa->cabeza = nuevo_list_node;
        lista_operativa->cabeza->siguiente = lista_operativa->cabeza->anterior = NULL;
    }
    else
    {
        list_node *curr_node = lista_operativa->cabeza;
        while ( curr_node->siguiente != NULL )
        {
            curr_node = curr_node->siguiente;
        }
        curr_node->siguiente = nuevo_list_node;
        curr_node->siguiente->anterior = curr_node;
        curr_node->siguiente->siguiente = NULL;
    }
}
void mostrar_lista(lista *lista_operativa)
{
        printf("\n\n");
        list_node *curr_node = lista_operativa->cabeza;
        while ( curr_node != NULL )
        {
            if ( curr_node->siguiente != NULL )
                printf(" ( %i ) -> ", curr_node->element->element);
            else
                printf(" ( %i )    ", curr_node->element->element);
            curr_node = curr_node->siguiente;
        }
        free(curr_node);
}
list_node *crear_list_node(tree_node *nuevo_tree_node)
{
    list_node *new_list_node = (list_node *) malloc(sizeof(list_node));
    new_list_node->siguiente = new_list_node->anterior = NULL;
    new_list_node->element = nuevo_tree_node;
    return new_list_node;
}
