#include "livros.h"

void registrarLivro(Livro novoLivro){
    FILE *file = fopen(LIVROS_BIN, "rb");

    if (file == NULL) {
        printf("Erro ao abrir %s\n", LIVROS_BIN);
        return;
    }

    BinHeader header;
    fread(&header, sizeof(BinHeader), 1, file);

    novoLivro.posDir = -1;
    novoLivro.posEsq = -1;

    // caso 1 - arvore vazia

    if(header.posHead == -1){
        header.posHead = header.posTop;
        fseek(file, sizeof(BinHeader) + header.posTop * sizeof(Livro), SEEK_SET);
        fwrite(&novoLivro, sizeof(Livro), 1, file);
        header.posTop++;
        fseek(file, 0, SEEK_SET);
        fwrite(&header, sizeof(BinHeader), 1, file);
        fclose(file);
        return;
    }

    // caso 2 - arvore nao vazia

    // procura o pai do livro a registrar
    Livro livroAtual;
    int posAtual = header.posHead;
    int posPai = -1;
    while(posAtual != -1){
        // atualiza pai pra posicao atual
        posPai = posAtual;
        // le livro atual
        fseek(file, sizeof(BinHeader) + posAtual * sizeof(Livro), SEEK_SET);
        fread(&livroAtual, sizeof(Livro), 1, file);
        // atualiza posicao atual
        posAtual = (novoLivro.codigo > livroAtual.codigo) ? livroAtual.posDir : livroAtual.posEsq;
    }

    // le pai
    Livro livroPai;
    fseek(file, sizeof(BinHeader) + posPai * sizeof(Livro), SEEK_SET);
    fread(&livroPai, sizeof(Livro), 1, file);
    // atualiza ligacoes do pai
    if(novoLivro.codigo > livroPai.codigo)
        livroPai.posDir = header.posTop;
    else 
        livroPai.posEsq = header.posTop;
    // reescreve pai
    fseek(file, sizeof(BinHeader) + posPai * sizeof(Livro), SEEK_SET);
    fwrite(&livroPai, sizeof(Livro), 1, file);

    // escreve novo livro
    fseek(file, sizeof(BinHeader) + header.posTop * sizeof(Livro), SEEK_SET);
    fwrite(&novoLivro, sizeof(Livro), 1, file);

    //atualiza header.posTop
    header.posTop++;
    //reescreve header
    fseek(file, 0, SEEK_SET);
    fwrite(&header, sizeof(BinHeader), 1, file);
    
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
