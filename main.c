#include <stdio.h>
#include <string.h>
#include "tipos.h" // Inclui o arquivo de tipos

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

    switch (produto.tipo) { // Solicita informações específicas com base no tipo de produto
        case ALIMENTO:
            printf("Digite a validade (dd/mm/aaaa): ");
            fgets(produto.alimento.validade, 11, stdin);
            produto.alimento.validade[strcspn(produto.alimento.validade, "\n")] = '\0';
            printf("Digite o peso (kg): ");
            scanf("%f", &produto.alimento.peso);
            limpar_buffer();
            break;

        case BEBIDA:
            printf("Quantos ml tem a bebida: ");
            scanf("%d", &produto.bebida.ml);
            printf("Digite o teor alcoólico (%%): ");
            scanf("%f", &produto.bebida.teor_alcoolico);
            limpar_buffer();
            break;

        case VESTUARIO:
            printf("Digite o tamanho (ex: P, M, G): ");
            fgets(produto.vestuario.tamanho, 4, stdin);
            produto.vestuario.tamanho[strcspn(produto.vestuario.tamanho, "\n")] = '\0';
            printf("Digite o material: ");
            fgets(produto.vestuario.material, 20, stdin);
            produto.vestuario.material[strcspn(produto.vestuario.material, "\n")] = '\0';
            break;

        case LIMPEZA:
            printf("Digite os componentes químicos: ");
            fgets(produto.limpeza.componentes_quimicos, 50, stdin);
            produto.limpeza.componentes_quimicos[strcspn(produto.limpeza.componentes_quimicos, "\n")] = '\0';
            printf("Digite a periculosidade (1 a 5): ");
            scanf("%d", &produto.limpeza.periculosidade);
            limpar_buffer();
            break;
        
        case HIGIENE:
            printf("Digite o pH do produto: ");
            scanf("%f", &produto.higiene.pH);
            limpar_buffer();
            printf("Digite o tipo de pele (ex: oleosa, seca, mista): ");
            fgets(produto.higiene.tipo_pele, 20, stdin);
            produto.higiene.tipo_pele[strcspn(produto.higiene.tipo_pele, "\n")] = '\0';
            break;

        default:
            printf("Tipo de produto inválido!\n");
            break;
    }

    printf("Digite até 5 marcas (pressione Enter para encerrar antes):\n");
    for (int i = 0; i < 5; i++) {
        printf("Marca %d: ", i + 1);
        fgets(produto.marcas[i], 20, stdin);
        produto.marcas[i][strcspn(produto.marcas[i], "\n")] = '\0';
        if (produto.marcas[i][0] == '\0') break;
    }

    fwrite(&produto, sizeof(produto), 1, arq);
    fflush(arq); // Garante que os dados sejam escritos no arquivo
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

        if (produto.tipo == ALIMENTO) { // Exibe informações específicas
            printf("Validade: %s\n", produto.alimento.validade);
            printf("Peso: %.2f kg\n", produto.alimento.peso);
        } else if (produto.tipo == BEBIDA) {
            printf("Teor Alcoólico: %.2f %%\n", produto.bebida.teor_alcoolico);
            printf("Volume: %d ml\n", produto.bebida.ml);
        } else if (produto.tipo == HIGIENE) {
            printf("pH: %.2f\n", produto.higiene.pH);
            printf("Tipo de Pele: %s\n", produto.higiene.tipo_pele);
        } else if (produto.tipo == LIMPEZA) {
            printf("Componentes Químicos: %s\n", produto.limpeza.componentes_quimicos);
            printf("Periculosidade: %d\n", produto.limpeza.periculosidade);
        } else if (produto.tipo == VESTUARIO) {
            printf("Tamanho: %s\n", produto.vestuario.tamanho);
            printf("Material: %s\n", produto.vestuario.material);
        }

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
        if (produto.status == 1 && strstr(produto.nome, nomeBusca)) {
            printf("\nID: %d | Nome: %s | Preço: %.2f | Quantidade: %d\n", 
                   produto.id, produto.nome, produto.preco, produto.quantidade);
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
    char novoNome[50];
    fgets(novoNome, 50, stdin);
    novoNome[strcspn(novoNome, "\n")] = '\0';
    if (strlen(novoNome) > 0) {
        strcpy(produto.nome, novoNome);
    }

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
            printf("Digite a nova validade (dd/mm/aaaa) (deixe em branco para manter a atual): ");
            fgets(produto.alimento.validade, 11, stdin);
            produto.alimento.validade[strcspn(produto.alimento.validade, "\n")] = '\0';
            break;

        case BEBIDA:
            printf("Digite o novo teor alcoólico (%%) (ou -1 para manter o atual): ");
            float novoTeor;
            scanf("%f", &novoTeor);
            limpar_buffer();
            if (novoTeor >= 0) {
                produto.bebida.teor_alcoolico = novoTeor;
            }
            break;

        case VESTUARIO:
            printf("Digite o novo tamanho (ex: P, M, G) (deixe em branco para manter o atual): ");
            fgets(produto.vestuario.tamanho, 4, stdin);
            produto.vestuario.tamanho[strcspn(produto.vestuario.tamanho, "\n")] = '\0';
            break;

        case LIMPEZA:
            printf("Digite os novos componentes químicos (deixe em branco para manter o atual): ");
            fgets(produto.limpeza.componentes_quimicos, 50, stdin);
            produto.limpeza.componentes_quimicos[strcspn(produto.limpeza.componentes_quimicos, "\n")] = '\0';
            break;

        case HIGIENE:
            printf("Digite o novo pH (ou -1 para manter o atual): ");
            float novoPH;
            scanf("%f", &novoPH);
            limpar_buffer();
            if (novoPH >= 0) {
                produto.higiene.pH = novoPH;
            }
            break;
    }

    for (int i = 0; i< 5; i++){
        printf("Digite a nova marca %d (deixe em branco para manter a atual): ", i + 1);
        char novaMarca[20];
        fgets(novaMarca, 20, stdin);
        size_t len = strlen(novaMarca);
        if (len > 0 && novaMarca[len - 1] == '\n') { // // Verifica se o último caractere é '\n' e o troca por '\0'
            novaMarca[len - 1] = '\0';
        } else { // Limpa o buffer se a entrada exceder o tamanho do array
            limpar_buffer();
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

const char *ARQUIVO = "./produtos.bin"; // Nome do arquivo binário

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
