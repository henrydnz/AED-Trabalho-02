#include "utils.h"

void menu(){
    criarBin();
    int opcao = -1;
    while(opcao != 0){
        mostrarMenu();
        scanf("%d",&opcao); 
        limparBuffer();
        refresh();
        switch(opcao){
            case 1:
                cadastrarLivro(); 
                break;
            case 2:
                imprimirDadosLivro(); 
                break;
            case 3:
                listarLivros(); 
                break;
            case 4:
                totalLivros(); 
                break; 
            case 5:
                removerLivro(); 
                break; 
            case 6:
                carregarArquivo(); 
                break;
            case 7:
                listarRegistrosLivres(); 
                break; 
            case 8:                
                imprimirArvore(); 
                break; 
            case 9:
                formatarSistema(); 
                break;
            case 0:
                printf("Saindo do sistema. Ate logo!\n\n");
                continue;
            default: 
                printf("Opcao nao reconhecida\n\n");
                break;
        }
        pausa();
    }
}

void mostrarMenu(){
    refresh();
    printf("-----------------------------------------------\n");
    printf("          SISTEMA BIBLIOTECA - Menu         \n");
    printf("-----------------------------------------------\n");
    printf(" Selecione uma opcao:\n\n");
    printf(" %-2d - Cadastrar livro\n", 1);
    printf(" %-2d - Imprimir dados do livro por codigo\n", 2);
    printf(" %-2d - Listar todos os livros\n", 3);
    printf(" %-2d - Mostrar total de livros\n", 4);
    printf(" %-2d - Remover livro\n", 5);
    printf(" %-2d - Carregar arquivo\n", 6);
    printf(" %-2d - Listar registros livres\n", 7);
    printf(" %-2d - Imprimir arvores por niveis\n", 8);
    printf(" %-2d - Formatar sistema\n\n", 9);
    printf(" %-2d - Sair\n", 0);
    printf("-----------------------------------------------\n");
    printf("\n> ");
}

void criarBin(){
    FILE *fileLivros = fopen(LIVROS_BIN, "rb");

    // jah existe, retorna
    if(fileLivros != NULL){
        fclose(fileLivros);
        return;
    }

    // nao existe, cria
    fileLivros = fopen(LIVROS_BIN, "wb");

    // erro ao criar
    if(fileLivros == NULL){
        printf("ERRO: Não foi possivel criar o arquivo binário %s.", LIVROS_BIN);
        exit(1);
    }

    fclose(fileLivros);

    // iniciar header de arquivo binario
    // cria
    BinHeader header;
    header.posHead = -1;
    header.posTop = 0;
    header.posFree = -1;
    header.totalLivros = 0;
    // escreve
    fwrite(&header, sizeof(BinHeader), 1, fileLivros);

    printf("Arquivo %s foi inicializado.", LIVROS_BIN);
}

void refresh(){
    #if defined(_WIN32) || defined(_WIN64) 
        system("cls");  // comando windows
    #elif defined(__linux__) || defined(__APPLE__)
        system("clear"); // comando linux ou macos
    #endif
}

void pausa(){
    printf("\nPressione Enter para retornar...");
    char c = getchar();
    if(c!='\n') limparBuffer();
}

void limparBuffer(){
    for(int c; (c = getchar()) != '\n' && c != EOF; );
}

void formatarSistema(){
    printf("FORMATACAO DO SISTEMA\n\n");
    printf("Excluindo e reinicializando arquivo...\n");
    remove(LIVROS_BIN); criarBin();
    printf("Sistema formatado com sucesso.\n");
}

Livro lerLivro(const FILE *file, const int posicao){
    Livro livro;
    fseek(file, sizeof(BinHeader) + posicao * sizeof(Livro), SEEK_SET);
    fread(&livro, sizeof(Livro), 1, file);
    return livro;
}

BinHeader lerHeader(const FILE *file){
    BinHeader header;
    fseek(file, 0, SEEK_SET);
    fwrite(&header, sizeof(BinHeader), 1, file);
    return header;
}

void escreverLivro(const FILE *file, const Livro livro, const int posicao){
    fseek(file, sizeof(BinHeader) + posicao * sizeof(Livro), SEEK_SET);
    fwrite(&livro, sizeof(Livro), 1, file);
}

void escreverHeader(const FILE *file, const BinHeader header){
    fseek(file, 0, SEEK_SET);
    fwrite(&header, sizeof(BinHeader), 1, file);
}

FILE *abrirArquivo(){
    FILE *file = fopen(LIVROS_BIN, "rb");

    if (file == NULL) {
        printf("Erro ao abrir %s\n", LIVROS_BIN);
        exit(1);
    }

    return file;
}