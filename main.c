#include <stdio.h>
#include <string.h>

enum Tipo_produto{ //Tag para controle do tipo de produto
    ALIMENTO,
    BEBIDA,
    HIGIENE,
    LIMPEZA,
    VESTUARIO
};

struct Produto{ //Estrutura do produto
    enum Tipo_produto tipo;
    int id;
    char nome[50];
    float preco;
    int quantidade;
    char marcas[5][20];
    int status; // 1 - ativo, 0 - removido
    union{
        char validade[11]; // alimentos, bebidas e limpeza
        char tamanho[4];  // vestuário
    };
};

void limpar_buffer() { // Função para limpar o buffer de entrada
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void cadastrarProduto(FILE *arq){
    struct Produto produto;
    int tipoProduto;

    fseek(arq, 0, SEEK_END); // Vai para o fim do arquivo
    long pos = ftell(arq);
    produto.id = (pos / sizeof(produto)) + 1; // ID automático com base no tamanho do arquivo
    produto.status = 1; // Produto ativo

    printf("\n=-=-= Cadastro de Novo Produto =-=-=\n");
    printf("Escolha o tipo de produto:\n");
    printf("0 - Alimento\n");
    printf("1 - Bebida\n");
    printf("2 - Higiene\n");
    printf("3 - Limpeza\n");
    printf("4 - Vestuário\n");
    printf("Tipo: ");
    scanf("%d", &tipoProduto);
    limpar_buffer();

    produto.tipo = tipoProduto;

    printf("Digite o nome do produto: ");
    fgets(produto.nome, 50, stdin);
    produto.nome[strcspn(produto.nome, "\n")] = '\0';

    printf("Digite o preço: ");
    scanf("%f", &produto.preco);
    limpar_buffer();

    printf("Digite a quantidade: ");
    scanf("%d", &produto.quantidade);
    limpar_buffer();

    switch (produto.tipo) {
        case ALIMENTO:
        case BEBIDA:
            printf("Digite a validade (dd/mm/aaaa): ");
            fgets(produto.validade, 11, stdin);
            produto.validade[strcspn(produto.validade, "\n")] = '\0';
            break;
        case VESTUARIO:
            printf("Digite o tamanho (ex: P, M, G): ");
            fgets(produto.tamanho, 4, stdin);
            produto.tamanho[strcspn(produto.tamanho, "\n")] = '\0';
            break;
        default:
            memset(produto.validade, 0, sizeof(produto.validade)); //Preenche todo o campo do produto.validade como 0000...
            break;
    }
    limpar_buffer();
    printf("Digite até 5 marcas (pressione Enter para encerrar antes):\n");
    for (int i = 0; i < 5; i++) {
        printf("Marca %d: ", i + 1);
        fgets(produto.marcas[i], 20, stdin);
        produto.marcas[i][strcspn(produto.marcas[i], "\n")] = '\0';
        if (produto.marcas[i][0] == '\0') break;
    }

    fwrite(&produto, sizeof(produto), 1, arq);
    printf("Produto cadastrado com sucesso! ID: %d\n", produto.id);
}

void listarProdutos(FILE *arq) {
    struct Produto produto;
    rewind(arq); // Volta para o início do arquivo

    printf("\n=-=-= Lista de Produtos =-=-=\n");
    int contador = 0;
    while (fread(&produto, sizeof(produto), 1, arq) == 1) {
        printf("\nID: %d\n", produto.id);
        printf("Nome: %s\n", produto.nome);
         printf("Preço: %.2f\n", produto.preco);
        printf("Quantidade: %d\n", produto.quantidade);

        switch (produto.tipo) {
            case ALIMENTO: printf("Tipo: Alimento\n"); break;
            case BEBIDA: printf("Tipo: Bebida\n"); break;
            case HIGIENE: printf("Tipo: Higiene\n"); break;
            case LIMPEZA: printf("Tipo: Limpeza\n"); break;
            case VESTUARIO: printf("Tipo: Vestuário\n"); break;
        }

        if (produto.tipo == ALIMENTO || produto.tipo == BEBIDA) printf("Validade: %s\n", produto.validade);
        else if (produto.tipo == VESTUARIO) printf("Tamanho: %s\n", produto.tamanho);
        if (produto.status == 1)  printf("Status: Ativo\n");
        else if(produto.status == 0)  printf("Status: Inativo\n");
        printf("Marcas: ");
        for (int i = 0; i < 5 && produto.marcas[i][0] != '\0'; i++) {
            printf("%s", produto.marcas[i]);
            if (i < 4 && produto.marcas[i+1][0] != '\0') printf(", ");
        }
        printf("\n=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
        contador++;
    }

    if (contador == 0)
        printf("Nenhum produto ativo encontrado.\n");
}

void buscarProduto(FILE *arq) {
    struct Produto produto;
    char nomeBusca[50];
    int encontrado = 0;

    printf("\nDigite o nome do produto: ");
    while (getchar() != '\n'); // limpar buffer antes da leitura
    fgets(nomeBusca, 50, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    rewind(arq);
    printf("\n--- Resultados da Busca ---\n");

    while (fread(&produto, sizeof(produto), 1, arq) == 1) {
        if (strstr(produto.nome, nomeBusca)) {
            switch (produto.tipo){
                case ALIMENTO:
                case BEBIDA:
                case LIMPEZA:
                    printf("\nID: %d | Nome: %s | Preço: %.2f | Quantidade: %d | Validade:%s |Status: ", 
                   produto.id, produto.nome, produto.preco, produto.quantidade, produto.validade);
                   break;
                case VESTUARIO:
                    printf("\nID: %d | Nome: %s | Preço: %.2f | Quantidade: %d | Tamanho: %s |Status: ", 
                   produto.id, produto.nome, produto.preco, produto.quantidade, produto.tamanho);
                   break;
                case HIGIENE:
                    printf("\nID: %d | Nome: %s | Preço: %.2f | Quantidade: %d |Status: ", 
                   produto.id, produto.nome, produto.preco, produto.quantidade);
                   break;
            }
            if (produto.status == 1) printf("Ativo\n");
            else if (produto.status == 0) printf("Inativo\n");
            printf("Marcas: ");
            for (int i = 0; i < 5 && produto.marcas[i][0] != '\0'; i++) {
                printf("%s", produto.marcas[i]);
                if (i < 4 && produto.marcas[i+1][0] != '\0') printf(", ");
            }
            encontrado = 1;
        }
    }

    if (!encontrado)
        printf("Nenhum produto encontrado com o termo '%s'.\n", nomeBusca);
}

void editarProduto(FILE *arq){
    struct Produto produto;
    int id;
    printf("\nDigite o ID do produto: "); // Pede o ID do produto a ser editado 
    scanf("%d", &id);
    limpar_buffer();

    long pos = (id - 1) * sizeof(produto); // Calcula a posição do produto no arquivo
    fseek(arq, pos, SEEK_SET);
    if (fread(&produto, sizeof(produto), 1, arq) != 1 || produto.status == 0) { // Verifica se o produto existe e está ativo
        printf("Produto não encontrado!\n");
        return;
}

    printf("Produto atual:\n"); // Exibe as informações atuais do produto
    printf("ID: %d | Nome: %s | Preço: %.2f\n", produto.id, produto.nome, produto.preco);

    printf("Marcas atuais: ");
    for (int i = 0; i < 5 && produto.marcas[i][0] != '\0'; i++) { // Exibe as marcas atuais
        printf("%s", produto.marcas[i]);
        if (i < 4 && produto.marcas[i+1][0] != '\0') printf(", ");
    }

    // Solicita as novas informações do produto

    printf("\nDigite o novo nome (deixe em branco para manter o atual): ");
    fgets(produto.nome, 50, stdin);

    printf("Digite o novo preço (ou -1 para manter o atual): ");
    float novoPreco;
    scanf("%f", &novoPreco);
    limpar_buffer();

    if (novoPreco >= 0) {
        produto.preco = novoPreco;
    }

    printf("Digite o nova quantidade (ou -1 para manter o atual): ");
    int novaQuantidade;
    scanf("%d", &novaQuantidade);
    limpar_buffer();

    if (novaQuantidade >= 0) {
        produto.quantidade = novaQuantidade;
    }

    switch (produto.tipo){
        case ALIMENTO:
        case BEBIDA:
        case LIMPEZA:
            printf("Digite a nova validade (deixe em branco para manter a atual): ");
            fgets(produto.validade, 11, stdin);
            break;
        case VESTUARIO:
            printf("Digite o novo tamanho (deixe em branco para manter o atual): ");
            fgets(produto.tamanho, 4, stdin);
            break;
        default:
            break;
    }

    for (int i = 0; i< 5; i++){
        printf("Digite a nova marca %d (deixe em branco para manter a atual): ", i + 1);
        char novaMarca[20];
        fgets(novaMarca, 20, stdin);
        if (novaMarca[0] != '\n') {
            strcpy(produto.marcas[i], novaMarca);
        }
    }

    // Atualiza o produto no arquivo
    fseek(arq, pos, SEEK_SET);
    fwrite(&produto, sizeof(produto), 1, arq);

    printf("Produto atualizado com sucesso!\n");
}
void removerProduto(FILE *arq){
    struct Produto produto;
    int id;
    printf("\nDigite o ID do produto: "); // Pede o ID do produto a ser removido
    scanf("%d", &id);
    limpar_buffer();

    long pos = (id - 1) * sizeof(produto); // Calcula a posição do produto no arquivo
    fseek(arq, pos, SEEK_SET); 
    if (fread(&produto, sizeof(produto), 1, arq) != 1 || produto.status == 0) { // Verifica se o produto existe e está ativo
        printf("Produto não encontrado!\n");
        return;
    } else {
        produto.status = 0; // Marca o produto como removido
        fseek(arq, pos, SEEK_SET);
        fwrite(&produto, sizeof(produto), 1, arq);
        printf("Produto removido com sucesso!\n");
    }
}

const char *ARQUIVO = "produtos.bin"; // Nome do arquivo binário

int main() {
    FILE *arq;
    int opcao;

    arq = fopen(ARQUIVO, "rb+"); // Abre o arquivo em modo leitura e escrita
    if (!arq) {
        arq = fopen(ARQUIVO, "wb+");
        if (!arq) {
            printf("Erro ao abrir o arquivo!\n");
            return 1;
        }
    }

    do{ // Loop do menu principal
        printf("\n-=-= GERENCIADOR DE PRODUTOS =-=-=\n");
        printf("1 - Cadastrar novo produto\n");
        printf("2 - Listar produtos\n");
        printf("3 - Buscar produto (por nome)\n");
        printf("4 - Editar informações de um produto\n");
        printf("5 - Remover produto\n");
        printf("6 - Sair\n");
        printf("-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch(opcao){
            case 1:
                cadastrarProduto(arq);
                break;

            case 2:
                listarProdutos(arq);
                break;

            case 3:
                buscarProduto(arq);
                break;
                
            case 4:
                editarProduto(arq);
                break;

            case 5:
                removerProduto(arq);
                break;

            case 6:
                printf("Encerrando o programa. Até logo!\n");
                break;

            default:
                printf("Erro! Tente novamente\n");
        }
    }while(opcao != 6); // enquanto for diferente de 6 (encerramento) vai repetir menu

    fclose(arq); // Fecha o arquivo
    return 0;
}
