#!/usr/bin/python3
from sys import argv;
from os import system;
from random import randint

while 1:
    try:
        cantidad_de_nodos = input('Random nodes : ');
        cantidad_de_nodos = int(cantidad_de_nodos);
        if ( cantidad_de_nodos > 30 ):
            raise ValueError;
        else:
            break;
    except ValueError:
        print('Invalid nodes count (it can not be grater than 30) ...');

cadena = './a.out';
for i in [randint(1,50) for i in range(cantidad_de_nodos + 1)]:
    cadena = cadena+ ' ' + str(i);
system("gcc main.c");
system(cadena);
