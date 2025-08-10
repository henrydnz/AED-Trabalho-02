#include "utils.h"

#define MAX_ARQ_NAME 100

/// @brief Loop de seleção de opções do Menu Principal do programa.
/// @pre Arquivo binário precisa estar inicializado.
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

/// @brief Estrutura de mostragem do menu principal com as opções.
/// @post O terminal é limpo e o menu principal é impresso. 
void mostrarMenu() {
    refresh();
    printf("\n");
    printf("---------------------------> Sistema Biblioteca <---------------------------\n");
    printf("[1] Cadastrar | [2] Pesquisar | [3] Listar | [4] Total    | [5] Remover\n");
    printf("[6] Arquivo   | [7] Livres    | [8] Arvore | [9] Formatar | [0] Sair\n");
    printf("----------------------------------------------------------------------------\n");
    printf("> ");
}

/// @brief Limpa o terminal quando chamada, usando o comando correto para diferentes OS.
/// @post O terminal é limpo e o cursor volta ao início.
void refresh(){
    #if defined(_WIN32) || defined(_WIN64) 
        system("cls");  // comando windows
    #elif defined(__linux__) || defined(__APPLE__)
        system("clear"); // comando linux ou macos
    #endif
}

/// @brief Quando chamada, imprime um aviso e aguarda o usuário digitar Enter.
///        Retorna quando lê uma quebra de linha no stdin, resumindo o programa. 
void pausa(){
    printf("\n----------\nPressione Enter para retornar...");
    char c = getchar();
    if(c!='\n') limparBuffer();
}

/// @brief Limpa totalmente o buffer de entrada do stdin. 
/// @post Qualquer conteúdo no buffer é apagado. 
void limparBuffer(){
    for(int c; (c = getchar()) != '\n' && c != EOF; );
}

/// @brief Função helper para a leitura rápida de uma string.
/// @param str 
/// @param size 
/// @post O parâmetro string passado é modificado com a leitura feita pela entrada do usuário.
void lerStr(char *str, const int size){
    fgets(str, size, stdin);
    char *newline = strchr(str, '\n');
    if(newline != NULL) *newline = '\0';
}

/// @brief Função helper para a leitura rápida de um inteiro.
/// @param i 
/// @post O parâmetro inteiro passado é modificado com a lietura feita pela entrada do usuário.
///       Após isso, o buffer é limpo.
void lerInt(int *i){
    scanf("%d", i);
    limparBuffer();
}

/// @brief Abre o arquivo binário já inicializado pelo sistema. Aborta se não encontrar
/// @return Um ponteiro para o arquivo binário aberto.
/// @pre O arquivo binário deve ter sido criado.
FILE *abrirArquivo(){
    FILE *file = fopen(LIVROS_BIN, "r+b");

    if (file == NULL) {
        printf("Erro ao abrir %s\n", LIVROS_BIN);
        exit(1);
    }

    return file;
}

/// @brief Cria o arquivo binário livros.bin para utilização do sistema. 
/// @post O arquivo livros.bin é criado na mesma pasta que o source do programa.
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

/// @brief Remove os espaços contidos antes e depios da string passada como parâmetro.
/// @param str 
/// @pre Pointer deve ser não nulo.
/// @post Modifica a string passada, removendo os espaços antes e depois. 
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

/// @brief Pergunta o nome do arquivo para a leitura, e logo após adiciona os livros descritos.
/// @pre O arquivo .txt deve existir, e a sintática deve estar correta.
/// @post Todos os livros descritos no sistema são adicionados ao arquivo binário.
void carregarArquivo(){

    printf(">> Carregar Arquivo <<\n\n");

    char nomeArq[MAX_ARQ_NAME];
    printf("Digite o nome do arquivo:\n");
    lerStr(nomeArq, MAX_ARQ_NAME);
    printf("\n");

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

/// @brief Reinicia totalmente o arquivo binário, perdendo todos os registros.
/// @pre O arquivo binário deve existir. 
/// @post O arquivo binário é excluído e logo após reinicializado novamente.
void formatarSistema(){
    printf(">> Formatacao do Sistema <<\n\n");
    remove(LIVROS_BIN); 
    while(!access(LIVROS_BIN, F_OK));
    printf("Arquivo %s foi excluido.\n", LIVROS_BIN);
    criarBin();
}