#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "livros.h"

#define LIVROS_BIN "livros.bin"

/// @brief Loop de seleção de opções do Menu Principal do programa.
/// @pre Arquivo binário precisa estar inicializado.
void menu();


/// @brief Estrutura de mostragem do menu principal com as opções.
/// @post O menu principal é impresso no terminal. 
void mostrarMenu();

/// @brief Limpa o terminal quando chamada, usando o comando correto para diferentes OS.
/// @post O terminal é limpo e o cursor volta ao início.
void refresh();

/// @brief Quando chamada, imprime um aviso e aguarda o usuário digitar Enter.
///        Retorna quando lê uma quebra de linha no stdin, resumindo o programa. 
void pausa();

/// @brief Limpa totalmente o buffer de entrada do stdin. 
/// @post Qualquer conteúdo no buffer é apagado. 
void limparBuffer();

/// @brief Função helper para a leitura rápida de uma string.
/// @param str 
/// @param size 
/// @post O parâmetro string passado é modificado com a leitura feita pela entrada do usuário.
void lerStr(char *str, const int size);

/// @brief Função helper para a leitura rápida de um inteiro.
/// @param i 
/// @post O parâmetro inteiro passado é modificado com a lietura feita pela entrada do usuário.
///       Após isso, o buffer é limpo.
void lerInt(int *i);

/// @brief Abre o arquivo binário já inicializado pelo sistema. Aborta se não encontrar
/// @return Um ponteiro para o arquivo binário aberto.
/// @pre O arquivo binário deve ter sido criado.
FILE *abrirArquivo();

/// @brief Cria o arquivo binário livros.bin para utilização do sistema. 
/// @post O arquivo livros.bin é criado na mesma pasta que o source do programa.
void criarBin();

/// @brief Remove os espaços contidos antes e depios da string passada como parâmetro.
/// @param str 
/// @pre Pointer deve ser não nulo.
/// @post Modifica a string passada, removendo os espaços antes e depois. 
void trim(char *str);

/// @brief Pergunta o nome do arquivo para a leitura, e logo após adiciona os livros descritos.
/// @pre O arquivo .txt deve existir, e a sintática deve estar correta.
/// @post Todos os livros descritos no sistema são adicionados ao arquivo binário.
void carregarArquivo();

/// @brief Reinicia totalmente o arquivo binário, perdendo todos os registros.
/// @pre O arquivo binário deve existir. 
/// @post O arquivo binário é excluído e logo após reinicializado novamente.
void formatarSistema();

#endif // __UTILS_H__