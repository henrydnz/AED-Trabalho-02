#include "livros.h"

void registrarLivro(const Livro novoLivro){
    FILE *file = abrirArquivo();
    BinHeader header = lerHeader(file);

    // caso 1 - arvore vazia
    if(header.posHead == -1){
        header.posHead = header.posTop;
        escreverLivro(file, novoLivro, header.posTop);
        header.posTop++;
        escreverHeader(file, header);
        fclose(file);
        return;
    }

    // caso 2 - arvore nao vazia
    // procura o pai do livro a registrar
    Livro livroAtual;
    int posAtual = header.posHead;
    int posPai = -1;
    while(posAtual != -1){
        posPai = posAtual;
        livroAtual = lerLivro(file, posAtual);
        posAtual = (novoLivro.codigo > livroAtual.codigo) ? 
                    livroAtual.posDir : 
                    livroAtual.posEsq;
    }

    //altera pai, escreve filho
    Livro livroPai = lerLivro(file, posPai);
    (novoLivro.codigo > livroPai.codigo)?
        (livroPai.posDir = header.posTop):
        (livroPai.posEsq = header.posTop);

    escreverLivro(file, livroPai, posPai);
    escreverLivro(file, novoLivro, header.posTop);

    //altera header
    header.posTop++;
    escreverHeader(file, header);

    fclose(file);
}

void cadastrarLivro(){
    printf("CADASTRAR LIVRO\n\n");

    Livro novoLivro;

    printf("Codigo: ");
    scanf("%d", &(novoLivro.codigo));
    limparBuffer();

    printf("Titulo: ");
    fgets(novoLivro.titulo, MAX_TITULO, stdin);
    limparBuffer();

    printf("Autor: ");
    fgets(novoLivro.autor, MAX_AUTOR, stdin);
    limparBuffer();

    printf("Editora: ");
    fgets(novoLivro.editora, MAX_EDITORA, stdin);
    limparBuffer();

    printf("Numero da edicao: ");
    scanf("%d", &(novoLivro.edicao));
    limparBuffer();

    printf("Ano: ");
    scanf("%d", &(novoLivro.ano));
    limparBuffer();

    printf("Numero de exemplares: ");
    scanf("%d", &(novoLivro.exemplares));
    limparBuffer();

    printf("Preco: ");
    scanf("%lf", &(novoLivro.edicao));
    limparBuffer();

    novoLivro.posDir = -1;
    novoLivro.posEsq = -1;

    registrarLivro(novoLivro);
}

void imprimirDadosLivro(){
    printf("IMPRIMIR DADOS POR CODIGO");
}

void listarLivros(){
    printf("LISTA DOS LIVROS CADASTRADOS");
}

void totalLivros(){
    printf("TOTAL DE LIVROS CADASTRADOS");
}

void removerLivro(){
    printf("REMOVER LIVRO");
}

void carregarArquivo(){
    printf("CARREGAR ARQUVO");
}

void listarRegistrosLivres(){
    printf("LISTA DE REGISTROS LIVRES");
}

void imprimirArvore(){
    printf("IMPRIMIR ARVORE POR NIVEIS");
}
