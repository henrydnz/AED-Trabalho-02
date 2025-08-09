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

void escreverLivro(FILE *file, const Livro livro){
    fseek(file, sizeof(BinHeader) + livro.pos * sizeof(Livro), SEEK_SET);
    fwrite(&livro, sizeof(Livro), 1, file);
}

void escreverHeader(FILE *file, const BinHeader header){
    fseek(file, 0, SEEK_SET);
    fwrite(&header, sizeof(BinHeader), 1, file);
}

void registrarLivro(Livro novoLivro){
    FILE *file = abrirArquivo();
    BinHeader header = lerHeader(file);
    
    int posNovo; // define posicao do novo livro
    if(header.posFree != -1){   // existe posFree
        posNovo = header.posFree;
        Livro livre = lerLivro(file, posNovo); // contém próximo da lista de livres em posEsq
        header.posFree = livre.posEsq; // atualiza header da lista livre
    } else { // nao existe posFree, usa topo
        posNovo = header.posTop;
        header.posTop++;
    }

    // atribui posicao e inicia ponteiros do novo livro
    novoLivro.pos = posNovo;
    novoLivro.posEsq = -1;
    novoLivro.posDir = -1;
    novoLivro.posPai = -1;

    // caso 1 - arvore vazia
    if(header.posHead == -1)
        header.posHead = posNovo;   // a cabeca eh o novo livro
    else { //caso 2 - nao vazia
        // procura posicao do novo pai do livro
        int posAtual = header.posHead;
        int posPai = -1;
        while(posAtual != -1){
            posPai = posAtual;
            Livro livroAtual = lerLivro(file, posAtual);
            if(novoLivro.codigo > livroAtual.codigo)
                posAtual = livroAtual.posDir;
            else posAtual = livroAtual.posEsq;
        }

        // atualiza ponteiro de pai do novo livro
        novoLivro.posPai = posPai;

        // conecta pai ao novo livro
        Livro livroPai = lerLivro(file, posPai);
        if(novoLivro.codigo > livroPai.codigo)
            livroPai.posDir = posNovo;
        else livroPai.posEsq = posNovo;
        escreverLivro(file, livroPai);
    }

    // escreve novo livro
    escreverLivro(file, novoLivro);

    // atualiza header
    header.totalLivros++;
    escreverHeader(file, header);

    fclose(file);

    printf("> \"%s\" registrado com sucesso.\n", novoLivro.titulo);
}

void cadastrarLivro(){
    printf(">> Cadastrar Livro <<\n\n");

    Livro novoLivro;

    printf("- Codigo: ");
    lerInt(&novoLivro.codigo);

    printf("- Titulo (150): ");
    lerStr(novoLivro.titulo, MAX_TITULO);

    printf("- Autor (200): ");
    lerStr(novoLivro.autor, MAX_AUTOR);

    printf("- Editora (50): ");
    lerStr(novoLivro.editora, MAX_EDITORA);

    printf("- Numero da edicao: ");
    lerInt(&novoLivro.edicao);

    printf("- Ano: ");
    lerInt(&novoLivro.ano);

    printf("- Numero de exemplares: ");
    lerInt(&novoLivro.exemplares);

    printf("- Preco: ");
    scanf("%lf", &(novoLivro.preco));
    limparBuffer();

    registrarLivro(novoLivro);
}

int pesquisarCodigo(FILE *file, int codigo){
    BinHeader header = lerHeader(file);
    int posAtual = header.posHead;

    while(posAtual != -1){
        Livro livroAtual = lerLivro(file, posAtual);
        if(codigo > livroAtual.codigo)
            posAtual = livroAtual.posDir;
        else if(codigo < livroAtual.codigo)
            posAtual = livroAtual.posEsq;
        else if(livroAtual.pos == posAtual)
            return posAtual;
        else {
            printf("Erro ao pesquisar código. Inconsistência de dados na posição %d\n", posAtual);
            return -1;
        }
    }
    return -1;
}

void mostrarLivro(Livro livro) {
    printf("\nTitulo:\n  > %s\n", livro.titulo);
    printf("Autor:\n  > %s\n", livro.autor);
    printf("Editora:\n  > %s\n\n", livro.editora);
    printf("Edicao: %-4d | Ano: %-7d | Exemplares: %-4d | Preco: R$ %.2f\n", 
            livro.edicao, livro.ano, livro.exemplares, livro.preco);
}

void imprimirDadosLivro(){
    printf(">> Pesquisar por Codigo <<\n\n");

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

void mostrarCompacto(Livro livro) {
    printf("--------------------------------------------------------------------\n");
    printf("[%d] ", livro.codigo);
    
    // truncamento de titulo
    const int LARGURA_MAX_TITULO = 55;
    if (strlen(livro.titulo) > LARGURA_MAX_TITULO)
        printf("\"%.*s...\"\n", LARGURA_MAX_TITULO - 3, livro.titulo);
    else
        printf("\"%s\"\n", livro.titulo);
    
    // truncamento de autor
    const int LARGURA_MAX_AUTOR = 40;
    printf("      -> Autor: %.*s%s | Exemplares: %d\n", 
            LARGURA_MAX_AUTOR, 
            livro.autor,
            (strlen(livro.autor) > LARGURA_MAX_AUTOR) ? "..." : "", //truncamento
            livro.exemplares);
}

void mostrarInOrdem(FILE *file, int posicao){
    if(posicao == -1) return;

    Livro livro = lerLivro(file, posicao);
    mostrarInOrdem(file, livro.posEsq);
    mostrarCompacto(livro);
    mostrarInOrdem(file,livro.posDir);
}

void listarLivros(){
    printf(">> Lista de Livros Cadastrados <<\n\n");

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
    printf(">> Total de Livros Cadastrados <<\n\n");
    
    FILE *file = abrirArquivo();
    BinHeader header = lerHeader(file);

    printf("O total de livros cadastrados eh %d\n", header.totalLivros);

    fclose(file);
}

void copiarLivro(Livro *dest, Livro source){
    dest->ano = source.ano;
    strncpy(dest->autor, source.autor, sizeof(dest->autor)-1);
    dest->autor[sizeof(dest->autor) - 1] = '\0';
    dest->codigo = source.codigo;
    dest->edicao = source.edicao;
    strncpy(dest->editora, source.editora, sizeof(dest->editora)-1);
    dest->editora[sizeof(dest->editora) - 1] = '\0';
    dest->exemplares = source.exemplares;
}

void removerLivro(){
    printf(">> Remover Livro <<\n\n");

    FILE *file = abrirArquivo();
    BinHeader header = lerHeader(file);

    if(header.posHead == -1){
        printf("Nao existem livros cadastrados!\n");
        fclose(file);
        return;
    }

    int codigo;
    printf("Digite o codigo para remover: ");
    lerInt(&codigo);

    int posRemocao = pesquisarCodigo(file, codigo);
    if(posRemocao == -1){
        printf("Esse codigo nao existe!\n");
        fclose(file);
        return;
    }

    int posLivre = posRemocao;
    Livro livroRemocao = lerLivro(file, posRemocao);

    // caso 1 - tem dois filhos
    if(livroRemocao.posEsq != -1 && livroRemocao.posDir != -1){
        // pega o sucessor do livro a excluir e seu pai
        int posSucessor = livroRemocao.posDir;
        Livro sucessor = lerLivro(file, posSucessor);
        while (sucessor.posEsq != -1) {
            posSucessor = sucessor.posEsq;
            sucessor = lerLivro(file, posSucessor);
        }

        posLivre = posSucessor;
        copiarLivro(&livroRemocao, sucessor); 

        if(livroRemocao.posDir == posSucessor)
            livroRemocao.posDir = sucessor.posDir;
        
        Livro paiSucessor = lerLivro(file, sucessor.posPai);

        // se o pai do sucessor é o livro a remover
        if (paiSucessor.pos != posRemocao){
            paiSucessor.posEsq = sucessor.posDir;
            escreverLivro(file, paiSucessor);
        }
        // se o sucessor tinha um filho direito, atualiza o pai dele
        if (sucessor.posDir != -1) {
            Livro filhoSucessor = lerLivro(file, sucessor.posDir);
            filhoSucessor.posPai = sucessor.posPai;
            escreverLivro(file, filhoSucessor);
        }
        escreverLivro(file, livroRemocao);
    } else { // caso 2 - tem 0 ou 1 filhos

        int posFilho = (livroRemocao.posEsq != -1) ? livroRemocao.posEsq : livroRemocao.posDir;

        // se o livro a ser removido era a raiz
        if (livroRemocao.posPai == -1) {
            header.posHead = posFilho;
        } else { // se não, atualiza o pai
            Livro livroPai = lerLivro(file, livroRemocao.posPai);
            if (livroPai.posEsq == posRemocao) {
                livroPai.posEsq = posFilho;
            } else {
                livroPai.posDir = posFilho;
            }
            escreverLivro(file, livroPai);
        }

        // se o nó removido tinha um filho, atualiza o ponteiro de pai do filho
        if (posFilho != -1) {
            Livro filho = lerLivro(file, posFilho);
            filho.posPai = livroRemocao.posPai;
            escreverLivro(file, filho);
        }
    }

    // gerencia lista de livres
    // deixa ponteiros do registro nulos
    Livro registroLivre = lerLivro(file, posLivre);
    registroLivre.codigo = -1;
    registroLivre.posEsq = header.posFree; // encadeia a lista
    registroLivre.posDir = -1;
    registroLivre.posPai = -1; 
    escreverLivro(file, registroLivre); 
    // atualiza cabeca
    header.posFree = posLivre;
    header.totalLivros--;
    escreverHeader(file, header);

    fclose(file);
    printf("Livro de codigo %d removido com sucesso.\n", codigo);
}

void listarRegistrosLivres(){
    printf(">> Lista de Registros Livres <<\n\n");

    FILE *file = abrirArquivo();
    BinHeader header = lerHeader(file);

    if(header.posFree == -1) {
        printf("Nao existem posicoes de registros livres.\n");
        return;
    }

    int posAtual = header.posFree;

    printf("Posicoes de registros livres:\n");
    printf("INICIO -> ");
    while(posAtual != -1){
        printf("%d", posAtual);
        Livro livroAtual = lerLivro(file, posAtual);
        if(livroAtual.posEsq != -1) printf(" -> ");
        posAtual = livroAtual.posEsq;
    }
    printf(" -> FIM\n");

    fclose(file);
}

void imprimirArvore(){
    printf(">> Imprimir Arvore por Niveis <<\n\n");

    FILE *file = abrirArquivo();
    BinHeader header = lerHeader(file);

    if(header.posHead == -1) {
        printf("Nao ha livros cadastrados.\n");
        return;
    }
    
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