#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <stdlib.h>

#include "livros.h"

#define LIVROS_BIN "livros.bin"

void menu();
void mostrarMenu();
void criarBin();
void refresh();
void pausa();
void limparBuffer();
void formatarSistema();

#endif // __UTILS_H__