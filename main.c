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
        char validade[11]; // alimentos e bebidas
        char tamanho[4];  // vestuário
    };
};

void cadastrarProduto(FILE *arq);

void listarProdutos(FILE *arq);

void buscarProduto(FILE *arq);

void editarProduto(FILE *arq){
    struct Produto produto;
    int id;
    printf("\nDigite o ID do produto: "); // Pede o ID do produto a ser editado 
    scanf("%d", &id);
    while(getchar() != '\n');

    long pos = (id - 1) * sizeof(produto); // Calcula a posição do produto no arquivo
    fseek(arq, pos, SEEK_SET);
    if (fread(&produto, sizeof(produto), 1, arq) != 1 || produto.status) {
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

    if (novoPreco >= 0) {
        produto.preco = novoPreco;
    }

    printf("Digite o nova quantidade (ou -1 para manter o atual): ");
    int novaQuantidade;
    scanf("%d", &novaQuantidade);
    while(getchar() != '\n');

    if (novaQuantidade >= 0) {
        produto.quantidade = novaQuantidade;
    }

    switch (produto.tipo){
        case ALIMENTO:
        case BEBIDA:
            printf("Digite a nova validade (deixe em branco para manter a atual): ");
            fgets(produto.validade, 11, stdin);
            break;
        case VESTUARIO:
            printf("Digite o novo tamanho (deixe em branco para manter o atual): ");
            fgets(produto.tamanho, 4, stdin);
            break;
    }

    for (int i = 0; i< 5; i++){
        printf("Digite a nova marca %d (deixe em branco para manter a atual): ", i + 1);
        char novaMarca[20];
        fgets(novaMarca, 20, stdin);
        if (novaMarca[0] != '\n') {
            strcpy(produto.nome, novaMarca);
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
    long pos = (id - 1) * sizeof(produto); // Calcula a posição do produto no arquivo
    fseek(arq, pos, SEEK_SET);
    if (fread(&produto, sizeof(produto), 1, arq) != 1 || produto.status) {
        printf("Produto não encontrado!\n");
        return;
    } else{
        produto.status = 1; // Marca o produto como removido
        fseek(arq, pos, SEEK_SET);
        fwrite(&produto, sizeof(produto), 1, arq); // Atualiza o produto no arquivo
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