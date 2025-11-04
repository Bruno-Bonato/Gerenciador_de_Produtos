#ifndef TIPOS_H //Verifica se a biblioteca já foi incluída
#define TIPOS_H //Se não, inclui a biblioteca

enum Tipo_produto{ //Tag para controle do tipo de produto
    ALIMENTO,
    BEBIDA,
    HIGIENE,
    LIMPEZA,
    VESTUARIO
};

struct Alimento{ // Estrutura específica para alimentos
    char validade[11];
    float peso;
};

struct Bebida { // Estrutura específica para bebidas
    float teor_alcoolico; // Em porcentagem
    int ml;
};

struct Higiene { // Estrutura específica para produtos de higiene
    float pH; 
    char tipo_pele[20]; // Ex: oleosa, seca, mista
};

struct Limpeza { // Estrutura específica para produtos de limpeza
    char componentes_quimicos[50];
    int periculosidade; // 1 a 5
};

struct Vestuario { // Estrutura específica para vestuário
    char tamanho[4]; // Ex: P, M, G
    char material[20];
};

struct Produto{ //Estrutura geral dos produtos
    enum Tipo_produto tipo;
    int id;
    char nome[50];
    float preco;
    int quantidade;
    char marcas[5][20];
    int status; // 1 - ativo, 0 - removido
    union{
        struct Alimento alimento;
        struct Vestuario vestuario;
        struct Limpeza limpeza;
        struct Bebida bebida;
        struct Higiene higiene;
    };
};

#endif //Fim do bloco condiconal da biblioteca
