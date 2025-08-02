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
        header.totalLivros++;
        escreverHeader(file, header);
        fclose(file);
        return;
    }

    // caso 2 - arvore nao vazia
    // procura pai do novo livro e o altera, escreve o novo livro
    Livro livroAtual, livroPai;
    int posAtual = header.posHead;
    int posPai = -1;
    while(posAtual != -1){
        posPai = posAtual;
        livroAtual = lerLivro(file, posAtual);
        posAtual = (novoLivro.codigo > livroAtual.codigo) ? 
                    livroAtual.posDir : 
                    livroAtual.posEsq;
    }
    livroPai = lerLivro(file, posPai);
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
    lerInt(&novoLivro.codigo);

    printf("Titulo: ");
    lerStr(novoLivro.titulo, MAX_TITULO);

    printf("Autor: ");
    lerStr(novoLivro.autor, MAX_AUTOR);

    printf("Editora: ");
    lerStr(novoLivro.editora, MAX_EDITORA);

    printf("Numero da edicao: ");
    lerInt(&novoLivro.edicao);

    printf("Ano: ");
    lerInt(&novoLivro.ano);

    printf("Numero de exemplares: ");
    lerInt(&novoLivro.exemplares);

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

    int codigo;
    printf("Digite o codigo do livro: ");
    lerInt(&codigo);

    int posCodigo = pesquisarCodigo(file, codigo);
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
    printf("\n--- \"%s\" de %s\n", livro.titulo, livro.autor);
    printf("Codigo: %d\n", livro.codigo);
    printf("Exemplares disp.: %d\n", livro.exemplares);
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
    printf("TOTAL DE LIVROS CADASTRADOS\n\n");
    
    FILE *file = abrirArquivo();
    BinHeader header = lerHeader(file);

    printf("O total de livros cadastrados eh %d\n", header.totalLivros);

    fclose(file);
}

// helper functions pra removerLivro();

// int pesquisarPai(FILE *file, int codigo){
//     BinHeader header = lerHeader(file);
//     Livro livroAtual;
//     int posAtual = header.posHead;
//     int posPai = -1;
//     while(posAtual != -1){
//         livroAtual= lerLivro(file, posAtual);
//         if(livroAtual.codigo == codigo)
//             return posPai;
//         posPai = posAtual;
//         posAtual = (codigo > livroAtual.codigo)?
//                     livroAtual.posDir:
//                     livroAtual.posEsq;
//     }
//     return -1;  // nao chega aq
// }

// int pesquisarMenor(FILE *file, int posInicio){
//     Livro livroAtual;
//     int posAtual = posInicio;
//     int posMM = posInicio;
//     while(posAtual != -1) {
//         posMM = posAtual;
//         livroAtual = lerLivro(file, posMM);
//         posAtual = livroAtual.posEsq;
//     }
//     return posMM;
// }

void removerLivro(){
    // printf("REMOVER LIVRO\n\n");

    // FILE *file = abrirArquivo();
    // BinHeader header = lerHeader(file);

    // if(header.posHead == -1){
    //     printf("Não existem livros cadastrados!\n");
    //     fclose(file);
    //     return;
    // }

    // int codigo;
    // printf("Digite o codigo para remover: ");
    // lerInt(&codigo);

    // int posRemocao = pesquisarCodigo(file, codigo);
    // if(posRemocao == -1){
    //     printf("Esse codigo nao existe!\n");
    //     fclose(file);
    //     return;
    // }

    // 
    // Livro livroRemocao = lerLivro(file, posRemocao);

    // if (livroRemocao.posDir != -1 && livroRemocao.posEsq != -1) {
    //     int posSucessor = pesquisarMenor(file, livroRemocao.posDir);
    //     Livro livroSucessor = lerLivro(file, posSucessor);
    //     strcpy(livroRemocao.titulo, livroSucessor.titulo);
    //     strcpy(livroRemocao.autor, livroSucessor.autor);
    //     strcpy(livroRemocao.editora, livroSucessor.editora);
    //     livroRemocao.ano = livroSucessor.ano;
    //     livroRemocao.codigo = livroSucessor.codigo;
    //     livroRemocao.edicao = livroSucessor.edicao;
    //     livroRemocao.exemplares = livroSucessor.exemplares;
    //     livroRemocao.preco = livroSucessor.preco;
    //     escreverLivro(file, livroRemocao, posRemocao);
    //     posRemocao = posSucessor;
    // }

    // int posFilhoUnico = (livroRemocao.posEsq != -1) ? livroRemocao.posEsq : livroRemocao.posDir;

    // int posPai = pesquisarPai(file, livroRemocao.codigo);
    // if(posPai != -1){
    //     Livro livroPai = lerLivro(file, posPai);
    //     if (livroPai.posDir == posRemocao)
    //         livroPai.posDir = posFilhoUnico;
    //     else 
    //         livroPai.posEsq = posFilhoUnico;
    //     escreverLivro(file, livroPai, posPai);
    // }

    // header.posFree = posRemocao;
    // header.totalLivros--;
    // escreverHeader(file, header);

    // printf("Livro de codigo %d removido com sucesso.\n", codigo);
    // fclose(file);
}

void listarRegistrosLivres(){
    printf("LISTA DE REGISTROS LIVRES");
}

void imprimirArvore(){
    printf("IMPRIMIR ARVORE POR NIVEIS\n\n");

    FILE *file = abrirArquivo();
    BinHeader header = lerHeader(file);
    
    int fila[header.totalLivros];
    int cabeca = 0, cauda = 0;

    fila[cauda++] = header.posHead;
    while(cabeca < cauda){
        int count = cauda - cabeca;
        for(int i=0;i<count;++i){
            int pos = fila[cabeca++];
            Livro livro = lerLivro(file, pos);
            printf("%d ", livro.codigo);
            if(livro.posEsq != -1) fila[cauda++] = livro.posEsq;
            if(livro.posDir != -1) fila[cauda++] = livro.posDir;
        }
        printf("\n");
    }
}
