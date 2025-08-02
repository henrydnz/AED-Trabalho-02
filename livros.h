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
    int totalExemplares;
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

Livro lerLivro(FILE *file, const int posicao);  //feito
BinHeader lerHeader(FILE *file);    //feito
void escreverLivro(FILE *file, const Livro livro, const int posicao);   //feito
void escreverHeader(FILE *file, const BinHeader header);    //feito
void registrarLivro(const Livro novoLivro); //feito
void cadastrarLivro();  //feito
int pesquisarCodigo(FILE *file, int codigo);    //feito
void mostrarLivro(Livro livro); //feito
void imprimirDadosLivro();  //feito
void mostrarCompacto(Livro livro);  //feito
void mostrarInOrdem(FILE *file, int posicao);   //feito
void listarLivros();    //feito
void totalLivros();
void removerLivro();
void carregarArquivo();
void listarRegistrosLivres();
void imprimirArvore();

#endif // __LIVROS_H__