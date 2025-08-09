#include "utils.h"

#define MAX_ARQ_NAME 100

/// @brief 
void menu(){
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

/// @brief 
void mostrarMenu(){
    refresh();
    printf("-----------------------------------------------\n");
    printf("          SISTEMA BIBLIOTECA - Menu         \n");
    printf("-----------------------------------------------\n");
    printf(" Selecione uma opcao:\n\n");
    printf(" %-2d - Cadastrar livro\n", 1);
    printf(" %-2d - Pesquisar por codigo\n", 2);
    printf(" %-2d - Listar livros\n", 3);
    printf(" %-2d - Imprimir total\n", 4);
    printf(" %-2d - Remover livro\n", 5);
    printf(" %-2d - Carregar arquivo\n", 6);
    printf(" %-2d - Listar registros livres\n", 7);
    printf(" %-2d - Imprimir arvore por niveis\n", 8);
    printf(" %-2d - Formatar sistema\n\n", 9);
    printf(" %-2d - Sair\n", 0);
    printf("-----------------------------------------------\n");
    printf("\n> ");
}

/// @brief 
void refresh(){
    #if defined(_WIN32) || defined(_WIN64) 
        system("cls");  // comando windows
    #elif defined(__linux__) || defined(__APPLE__)
        system("clear"); // comando linux ou macos
    #endif
}

/// @brief 
void pausa(){
    printf("\n\nPressione Enter para retornar...");
    char c = getchar();
    if(c!='\n') limparBuffer();
}

/// @brief 
void limparBuffer(){
    for(int c; (c = getchar()) != '\n' && c != EOF; );
}

/// @brief 
/// @param str 
/// @param size 
void lerStr(char *str, const int size){
    fgets(str, size, stdin);
    char *newline = strchr(str, '\n');
    if(newline != NULL) *newline = '\0';
}

/// @brief 
/// @param i 
void lerInt(int *i){
    scanf("%d", i);
    limparBuffer();
}

/// @brief 
/// @return 
FILE *abrirArquivo(){
    FILE *file = fopen(LIVROS_BIN, "r+b");

    if (file == NULL) {
        printf("Erro ao abrir %s\n", LIVROS_BIN);
        exit(1);
    }

    return file;
}

/// @brief 
void criarBin(){
    FILE *file = fopen(LIVROS_BIN, "rb");
    if(file != NULL){
        fclose(file); return;
    }

    file = fopen(LIVROS_BIN, "wb");
    if(file == NULL){
        printf("ERRO: Não foi possivel criar %s\n", LIVROS_BIN);
        exit(1);
    }

    BinHeader header;
    header.posHead = -1;
    header.posTop = 0;
    header.posFree = -1;
    header.totalLivros = 0;
    escreverHeader(file, header);
    
    printf("Arquivo %s foi inicializado.\n", LIVROS_BIN);

    fclose(file);
}

/// @brief 
/// @param str 
void trim(char *str) {
    char *inicio = str;
    while (*inicio == ' ') inicio++;
    char *fim = str + strlen(str) - 1;
    while (fim > inicio && (*fim == ' ' || *fim == '\n')) {
        *fim = '\0';
        fim--;
    }
    if (inicio != str)
        memmove(str, inicio, strlen(inicio) + 1);
}

/// @brief 
void carregarArquivo(){

    printf(">> Carregar Arquivo <<");

    char nomeArq[MAX_ARQ_NAME];
    printf("Digite o nome do arquivo:\n");
    lerStr(nomeArq, MAX_ARQ_NAME);

    FILE *file_txt = fopen(nomeArq, "r");
    if(file_txt == NULL){
        printf("O arquivo \"%s\" nao foi encontrado.\n", nomeArq);
        printf("Verifique se o nome digitado está correto\n");
        printf("ou se o arquivo está na pasta correta.");
        return;
    }

    const char *delim = ";";
    char linha[1024];
    // matriz de char pra token
    char tokens[8][MAX_AUTOR];  // max autor é o maior tamanho de token
    Livro l;
    
    while(fgets(linha, sizeof(linha), file_txt) != NULL){
        // tira \n do final
        linha[strcspn(linha, "\n")] = '\0';

        // loop pra preencher tokens da linha
        int tokenCount = 0;
        char *token = strtok(linha, delim);
        while(token != NULL && tokenCount < 8){
            trim(token);
            strncpy(tokens[tokenCount], token, MAX_AUTOR - 1);
            tokens[tokenCount][MAX_AUTOR - 1] = '\0';
            tokenCount++;
            token = strtok(NULL, delim);
        }

        // montar livro
        l.codigo = atoi(tokens[0]);

        strncpy(l.titulo, tokens[1], MAX_TITULO);
        l.titulo[MAX_TITULO-1] = '\0';

        strncpy(l.autor, tokens[2], MAX_AUTOR);
        l.autor[MAX_AUTOR-1] = '\0';

        strncpy(l.editora, tokens[3], MAX_EDITORA);
        l.editora[MAX_EDITORA] = '\0';

        l.edicao = atoi(tokens[4]);

        l.ano = atoi(tokens[5]);

        l.exemplares = atoi(tokens[6]);

        size_t pos = strcspn(tokens[7], ",");
        if (tokens[7][pos] != '\0')
            tokens[7][pos] = '.';
        l.preco = atof(tokens[7]);

        // salvar
        registrarLivro(l);
    }
    fclose(file_txt);
}

/// @brief 
void formatarSistema(){
    printf(">> Formatacao do Sistema <<\n\n");
    remove(LIVROS_BIN); 
    while(!access(LIVROS_BIN, F_OK));
    printf("Arquivo %s foi excluido.\n", LIVROS_BIN);
    criarBin();
}