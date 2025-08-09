#ifndef __LIVROS_H__
#define __LIVROS_H__

#include "utils.h"

#define MAX_TITULO 151
#define MAX_AUTOR 201
#define MAX_EDITORA 51

/// @brief Struct para cabeça do arquivo binário.
typedef struct {
    int posHead;
    int posTop;
    int posFree;
    int totalLivros;
} BinHeader;

/// @brief Struct para o nó de livro.
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

/// @brief Lê um livro no arquivo binário em uma posição.
/// @param file
/// @param posicao 
/// @return Uma cópia do struct Livro lido.
/// @pre Ponteiro deve apontar para arquivo. Livro deve existir na posição.
Livro lerLivro(FILE *file, const int posicao);

/// @brief Lê a header do arquivo binário.
/// @param file - Ponteiro para o arquivo
/// @return Uma cópia do struct BinHeader lido.
/// @pre Ponteiro deve apontar para arquivo. Header deve existir.
BinHeader lerHeader(FILE *file);

/// @brief Escreve um livro no arquivo binário
/// @param file 
/// @param livro 
/// @pre Ponteiro deve apontar para arquivo
/// @post Arquivo é alterado com a informação do livro escrito.
void escreverLivro(FILE *file, const Livro livro);

/// @brief Escreve/reescreve header no arquivo binário.
/// @param file 
/// @param header 
/// @pre Ponteiro deve apontar para arquivo
/// @post Arquivo é alterado com a informação da header escrita.
void escreverHeader(FILE *file, const BinHeader header);

/// @brief Lógica para registrar um livro no arquivo binário 
///        como uma árvore binária de busca.
/// @param novoLivro 
/// @pre Arquivo deve existir. 
void registrarLivro(Livro novoLivro);

/// @brief Perguntas para o cadastro de um novo livro no sistema.
/// @post Um struct Livro é montado com as informações e enviado para a função de
///       registro de livros.
void cadastrarLivro();

/// @brief Pesquisa a posição de um livro com um certo código na árvore binária.
/// @param file 
/// @param codigo 
/// @return Inteiro representando a posição do livro com o código dado.
///         Caso não encontre, ou existam dois livros de mesmo código, 
///         então retorna -1.
/// @pre Ponteiro deve apontar para arquivo.
int pesquisarCodigo(FILE *file, int codigo);

/// @brief Estrutura para mostragem de informações de um livro.
/// @param livro 
/// @post Imprime na tela as informações do livro.
void mostrarLivro(Livro livro); 

/// @brief Pede um código, pesquisa e mostra informações sobre o livro respectivo.
/// @post Imprime na tela as informações do livro ou avisa caso ele não exista.
void imprimirDadosLivro(); 

/// @brief Estrutura para mostragem de informaçõe de um livro compacta.
///        Usada para mostragem da lista de livros cadastrados.
/// @param livro 
void mostrarCompacto(Livro livro);

/// @brief Função recursiva ajudante para a função de listar os livros do sistema.
/// @param file 
/// @param posicao 
/// @pre Ponteiro deve apontar para arquivo.
void mostrarInOrdem(FILE *file, int posicao); 

/// @brief Mostra a lista de todos os livros cadastrados no sistema em ordem crescente de código.
void listarLivros();

/// @brief Imprime na tela um inteiro representando o total de livros cadastrados no sistema.
void totalLivros();

/// @brief Função helper para copiar as características de um livro para outro.
///        Ponteiros não são copiados.
/// @param dest 
/// @param source 
/// @post O livro destino recebe as informações do livro source, exceto os ponteiros.
void copiarLivro(Livro *dest, Livro source);

/// @brief Pede um código de livro ao usuário e depois performa a exclusão do livro do sistema,
///        mantendo a lógica da árvore binária intacta.
/// @post O livro com o código respectivo será removido e a árvore binária será reestruturada
///       mantendo sua lógica.
void removerLivro();

/// @brief Imprime uma lista de registros livres em forma de uma lista encadeada.
void listarRegistrosLivres();

/// @brief Imprime na tela uma representação minimalista da estrutura atual da árvore binária.
void imprimirArvore();

#endif // __LIVROS_H__