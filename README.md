# Gerenciador de Produtos



## Domínio da Aplicação  

Este projeto trata-se de um sistema de **gerenciamento de produtos** desenvolvido em linguagem C, com interface de terminal. O domínio é o controle de cadastro, edição, remoção e listagem de produtos para uma pequena aplicação/estoque de produtos.



## Informações que serão gerenciadas  

No sistema são armazenadas informações de cada produto, tais como:

- Identificador único do produto (ID)  

- Nome do produto  

- Preço unitário

- Quantidade em estoque  

- Marcas

- Status do produto (ex: 1 - ativo e 0 - removido)



Os dados são persistidos em um arquivo binário para permitir leitura/escrita entre execuções.



## Operações que serão desenvolvidas  

O sistema permite executar as seguintes operações principais:

- **Cadastrar** novo produto: inserir um novo registro de produto com seus dados.  

- **Listar** todos os produtos: exibir todos os produtos registrados.  

- **Buscar** produto: localizar produto(s) por algum critério (por exemplo nome ou ID).  

- **Editar** produto: alterar dados de um produto existente (nome, quantidade, preço, etc).  

- **Remover** produto: excluir um produto do registro.  

- **Persistência**: gravar os dados em arquivo binário e ler na inicialização para manter o histórico de produtos.
