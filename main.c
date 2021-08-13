#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tools.h"

#define MAX_STR_LEN 10


int main(int argc, char **argv)
{
    int number;
    int i;
    char *cadena        = NULL;
    arbol *arbol_op     = crear_arbol();
    lista *nodos        = crear_lista();
    argc -= 1;
    if ( argc > 0 )
    {
        for ( i = 1; i <= argc; i++ )
        {
            procesar_elemento_en_arbol(atoi(argv[i]), arbol_op, nodos );
        }
    }
    while (1)
    {
        system("clear");
        if ( arbol_op->raiz != NULL )
            mostrar_arbol( arbol_op->raiz );
        while ( 1 )
        {
            printf("\n\n\n\n\n\n\n\n\n\n\n\n\n");
            printf("-> ");
            if ( cadena != NULL )
            {
                free(cadena);
                cadena = NULL;
            }
            cadena = string_input(MAX_STR_LEN);
            if ( is_number(cadena, MAX_STR_LEN) )
            {
                number = atoi(cadena);
                break;
            }
            printf("\nNumero invalido !");
        }
        procesar_elemento_en_arbol(number, arbol_op, nodos);
    }
    return 0;
}
