#ifndef __LIVROS_H__
#define __LIVROS_H__

#include "utils.h"

#define MAX_TITULO 151
#define MAX_AUTOR 201
#define MAX_EDITORA 51

typedef struct {
    int posHead;
    int posTop;
    int posFree;
    int totalLivros;
} BinHeader;

typedef struct {
    int posEsq;
    int posDir;
    int codigo;
    char titulo[MAX_TITULO];
    char autor[MAX_AUTOR];
    char editora[MAX_EDITORA];
    int edicao;
    int ano;
    int exemplares;
    double preco;
} Livro;

Livro lerLivro(FILE *file, const int posicao);
BinHeader lerHeader(FILE *file);
void escreverLivro(FILE *file, const Livro livro, const int posicao);
void escreverHeader(FILE *file, const BinHeader header);
void registrarLivro(const Livro novoLivro);
void cadastrarLivro();
int pesquisarCodigo(FILE *file, int codigo);
void mostrarLivro(Livro livro);
void imprimirDadosLivro();
void listarLivros();
void totalLivros();
void removerLivro();
void carregarArquivo();
void listarRegistrosLivres();
void imprimirArvore();

#endif // __LIVROS_H__