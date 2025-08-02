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
Livro lerLivro(const FILE *file, const int posicao);
BinHeader lerHeader(const FILE *file);
void escreverLivro(const FILE *file, const Livro livro, const int posicao);
void escreverHeader(const FILE *file, const BinHeader header);
FILE *abrirArquivo();

#endif // __UTILS_H__