#include "livros.h"

Livro lerLivro(FILE *file, const int posicao){
    Livro livro;
    fseek(file, sizeof(BinHeader) + posicao * sizeof(Livro), SEEK_SET);
    fread(&livro, sizeof(Livro), 1, file);
    return livro;
}

BinHeader lerHeader(FILE *file){
    BinHeader header;
    fseek(file, 0, SEEK_SET);
    fread(&header, sizeof(BinHeader), 1, file);
    return header;
}

void escreverLivro(FILE *file, const Livro livro, const int posicao){
    fseek(file, sizeof(BinHeader) + posicao * sizeof(Livro), SEEK_SET);
    fwrite(&livro, sizeof(Livro), 1, file);
}

void escreverHeader(FILE *file, const BinHeader header){
    fseek(file, 0, SEEK_SET);
    fwrite(&header, sizeof(BinHeader), 1, file);
}

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
    header.totalLivros++;
    escreverHeader(file, header);

    fclose(file);

    printf("\"%s\" foi registrado.", novoLivro.titulo);
}

void cadastrarLivro(){
    printf("CADASTRAR LIVRO\n\n");

    Livro novoLivro;

    printf("Codigo: ");
    scanf("%d", &(novoLivro.codigo));
    limparBuffer();

    printf("Titulo: ");
    lerStr(novoLivro.titulo, MAX_TITULO);

    printf("Autor: ");
    lerStr(novoLivro.autor, MAX_AUTOR);

    printf("Editora: ");
    lerStr(novoLivro.editora, MAX_EDITORA);

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
    scanf("%lf", &(novoLivro.preco));
    limparBuffer();

    novoLivro.posDir = -1;
    novoLivro.posEsq = -1;

    registrarLivro(novoLivro);
}

int pesquisarCodigo(FILE *file, int codigo){
    BinHeader header = lerHeader(file);
    int posCod, posAtual = header.posHead;
    Livro livroAtual;

    while(posAtual != -1){
        posCod = posAtual;
        livroAtual = lerLivro(file, posAtual);
        if(codigo > livroAtual.codigo)
            posAtual = livroAtual.posDir;
        if(codigo < livroAtual.codigo)
            posAtual = livroAtual.posEsq;
        if(codigo == livroAtual.codigo)
            return posCod;
    }

    return -1;
}

void mostrarLivro(Livro livro){
    printf("\n--- \"%s\", de %s ---\n", livro.titulo, livro.autor);
    printf("Editora: %s\n", livro.editora);
    printf("Edicao: %d\n", livro.edicao);
    printf("ano: %d\n", livro.ano);
    printf("Exemplares disp.: %d\n", livro.exemplares);
    printf("Preco: %.2lf", livro.preco);
}

void imprimirDadosLivro(){
    printf("IMPRIMIR DADOS POR CODIGO\n\n");

    FILE *file = abrirArquivo();
    int codigo, posCodigo;
    printf("Digite o codigo do livro: ");
    scanf("%d", &codigo);
    limparBuffer();

    posCodigo = pesquisarCodigo(file, codigo);
    if(posCodigo == -1) {
        printf("Esse codigo nao existe!\n");
        fclose(file);
        return;
    }

    Livro livro = lerLivro(file, posCodigo);
    mostrarLivro(livro);
    fclose(file);
}

void mostrarCompacto(Livro livro){
    printf("--- \"%s\" de %s\n", livro.titulo, livro.autor);
    printf("Codigo: %d\n", livro.codigo);
    printf("Exemplares disp.: %d\n\n", livro.exemplares);
}

void mostrarInOrdem(FILE *file, int posicao){
    if(posicao == -1) return;

    Livro livro = lerLivro(file, posicao);
    mostrarInOrdem(file, livro.posEsq);
    mostrarCompacto(livro);
    mostrarInOrdem(file,livro.posDir);
}

void listarLivros(){
    printf("LISTA DOS LIVROS CADASTRADOS\n\n");

    FILE *file = abrirArquivo();
    BinHeader header = lerHeader(file);

    if(header.posHead == -1){
        printf("Nenhum livro cadastrado.\n");
        return;
    }

    mostrarInOrdem(file, header.posHead);
    fclose(file);
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
