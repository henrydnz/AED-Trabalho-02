#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "livros.h"

#define LIVROS_BIN "livros.bin"

void menu();
void mostrarMenu();
void refresh();
void pausa();
void limparBuffer();
void lerStr(char *str, const int size);
void lerInt(int *i);
FILE *abrirArquivo();
void criarBin();
void trim(char *str);
void carregarArquivo();
void formatarSistema();

#endif // __UTILS_H__