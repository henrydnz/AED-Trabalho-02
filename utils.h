#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "livros.h"

#define LIVROS_BIN "livros.bin"

void menu();
void mostrarMenu();
void criarBin();
void refresh();
void pausa();
void limparBuffer();
void formatarSistema();
FILE *abrirArquivo();
void lerStr(char *str, int size);
void lerInt(int *i);

#endif // __UTILS_H__