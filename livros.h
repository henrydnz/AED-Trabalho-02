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
    int posPai;
    int pos;
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
void escreverLivro(FILE *file, const Livro livro);
void escreverHeader(FILE *file, const BinHeader header);
void registrarLivro(Livro novoLivro);
void cadastrarLivro();
int pesquisarCodigo(FILE *file, int codigo);
void mostrarLivro(Livro livro); 
void imprimirDadosLivro(); 
void mostrarCompacto(Livro livro);
void mostrarInOrdem(FILE *file, int posicao); 
void listarLivros();
void totalLivros();
void copiarLivro(Livro *dest, Livro source);
void removerLivro();
void listarRegistrosLivres();
void imprimirArvore();

#endif // __LIVROS_H__