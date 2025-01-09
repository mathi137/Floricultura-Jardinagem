#ifndef PRODUCT_H
#define PRODUCT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

#define NAME_SIZE 128
#define DESCRIPTION_SIZE 256

static int product_id = 0;

typedef struct 
{
    int id;
    char name[NAME_SIZE];
    char description[DESCRIPTION_SIZE];
    float price;
} Product;

typedef struct 
{
    Product *pProducts;
    int nProducts;
} Products;

/*
*  Função: generate_product_id
*  Descrição: Gera um novo ID para o produto.
*  Retorna:
*      int: ID gerado.
*/
int generate_product_id()
{
    return ++product_id;
}


/*
*  Função: print_product
*  Descrição: Imprime os detalhes do produto.
*  Parâmetros:
*      Product *pProduct: ponteiro para o objeto do tipo Product.
*  Retorna:
*      void.
*/
void print_product(const Product *pProduct) 
{
    printf("\nID Produto: %d\n", pProduct->id);
    printf("Nome: %s\n", pProduct->name);
    printf("Descricao: %s\n", pProduct->description);
    printf("Preco: %.2f\n", pProduct->price);

}

/*
*  Função: list_products
*  Descrição: Listar os produtos.
*  Parâmetros:
*      Products *pProduct: ponteiro para o objeto do tipo Products.
*  Retorna:
*      void.
*/
void list_products(const Products *pProducts) 
{
    if (pProducts->nProducts == 0) 
    {
        printf("\nNao ha produtos para listar.\n");
        return;
    }

    printf("\n====== Produto ======\n");
    for (int i = 0; i < pProducts->nProducts; i++)
        print_product(pProducts->pProducts+i);
    printf("\n=====================\n");

}

/*
*  Função: list_products_alfabetic_order
*  Descrição: Listar os produtos em ordem alfabetica.
*  Parâmetros:
*      Products *pProduct: ponteiro para o objeto do tipo Products.
*  Retorna:
*      void.
*
*/
void list_products_alfabetic_order(const Products *pProducts) 
{
    if (pProducts->nProducts == 0) 
    {
        printf("\nNao ha produtos para listar.\n");
        return;
    }

    // Cria um array temporario para armazenar os produtos
    Product *products = (Product *)malloc(pProducts->nProducts * sizeof(Product));
    if (products == NULL) 
    {
        printf("\nErro ao alocar memoria\n");
        return;
    }
    // Copia os produtos para o array temporario
    memcpy(products, pProducts->pProducts, pProducts->nProducts * sizeof(Product));

    // Ordena os produtos em ordem alfabetica
    for (int i = 0; i < pProducts->nProducts - 1; i++) 
    {
        for (int j = 0; j < pProducts->nProducts - i - 1; j++) 
        {
            if (strcmp(products[j].name, products[j+1].name) > 0) 
            {
                // Troca os produtos
                Product temp = products[j];
                products[j] = products[j+1];
                products[j+1] = temp;
            }
        }
    }

    // Imprime os produtos ordenados
    printf("\n====== Produto ======\n");
    for (int i = 0; i < pProducts->nProducts; i++)
        print_product(products+i);
    printf("\n=====================\n");

    // Libera a memoria do array temporario
    free(products);
}

/*
*  Função: scan_product
*  Descrição: Escaneia os detalhes do produto.
*  Parâmetros:
*      Product *pProduct: ponteiro para o objeto do tipo Product.
*  Retorna:
*      void.
*/
void scan_product(Product *pProduct) 
{
    if (pProduct->id == 0)
        pProduct->id = generate_product_id();

    printf("\nDigite o nome do produto: ");
    getchar();
    fgets(pProduct->name, NAME_SIZE, stdin);
    trim(pProduct->name);

    printf("Digite a descricao do produto: ");
    fgets(pProduct->description, DESCRIPTION_SIZE, stdin);
    trim(pProduct->description);
    
    printf("Digite o preco do produto: ");
    scanf("%f", &pProduct->price);
}

/*
*  Função: search_product_by_id
*  Descrição: Busca um produto pelo ID.
*  Parâmetros:
*      Products *pProducts: ponteiro para o objeto do tipo Products.
*      int products_index[]: ponteiro para o array de indices dos produtos encontrados.
*  Retorna:
*      int: quantidade de produtos encontrados.
*/
int search_product_by_id(Products *pProducts, int products_index[]) 
{
    int id;
    printf("\nDigite o ID do produto: ");
    scanf("%d", &id);

    int count = 0;
    for (int i = 0; i < pProducts->nProducts; i++) 
    {
        if (pProducts->pProducts[i].id == id) 
        {
            products_index[count] = i;
            count++;
        }
    }

    return count;
}

/*
*  Função: search_product_by_name    
*  Descrição: Busca um produto pelo nome.
*  Parâmetros:
*      Products *pProducts: ponteiro para o objeto do tipo Products.
*      int products_index[]: ponteiro para o array de indices dos produtos encontrados.
*  Retorna:
*      int: quantidade de produtos encontrados.
*/
int search_product_by_name(Products *pProducts, int products_index[]) 
{
    char name[NAME_SIZE];
    printf("\nDigite o nome do produto: ");
    getchar();
    fgets(name, NAME_SIZE, stdin);
    trim(name);

    int count = 0;
    for (int i = 0; i < pProducts->nProducts; i++) 
    {
        if (strcmp(pProducts->pProducts[i].name, name) == 0) 
        {
            products_index[count] = i;
            count++;
        }
    }

    return count;
}

/*
*  Função: search_product_by_price_range    
*  Descrição: Busca um produto pelo nome.
*  Parâmetros:
*      Products *pProducts: ponteiro para o objeto do tipo Products.
*      int products_index[]: ponteiro para o array de indices dos produtos encontrados.
*  Retorna:
*      int: quantidade de produtos encontrados.
*/
int search_product_by_price_range(Products *pProducts, int products_index[]) 
{
    float min_price, max_price;
    printf("\nDigite o preco minimo: ");
    scanf("%f", &min_price);
    printf("Digite o preco maximo: ");
    scanf("%f", &max_price);

    int count = 0;
    for (int i = 0; i < pProducts->nProducts; i++) 
    {
        if (pProducts->pProducts[i].price >= min_price && pProducts->pProducts[i].price <= max_price) 
        {
            products_index[count] = i;
            count++;
        }
    }

    return count;
}

/*
*  Função: search_product    
*  Descrição: Busca um produto.
*  Parâmetros:
*      Products *pProducts: ponteiro para o objeto do tipo Products.
*      int products_index[]: ponteiro para o array de indices dos produtos encontrados.
*  Retorna:
*      int: quantidade de produtos encontrados.
*/
int search_product(Products *pProducts, int products_index[]) 
{
    printf("\nOpcoes de busca:\n");
    printf("1: Buscar por ID\n2: Buscar por nome\n3: Buscar por faixa de preco\n");

    int option;

    do
    {
        printf("\nEscolha uma opcao: ");
        scanf("%d", &option);
        
        switch (option)
        {
        case 1:
            return search_product_by_id(pProducts, products_index);

        case 2:
            return search_product_by_name(pProducts, products_index);;

        case 3:
            return search_product_by_price_range(pProducts, products_index);

        default:
            break;
        }
    } while (option < 1 || option > 3);

    return 0;
}

/*
*  Função: list_search_product    
*  Descrição: Listar os produtos.
*  Parâmetros:
*      Products *pProducts: ponteiro para o objeto do tipo Products.
*  Retorna:
*      int: 1 se sucesso, 0 se falha.
*/
int list_search_product(Products *pProducts) 
{
    int products_index[pProducts->nProducts];
    // search_product coleta os indices dos produtos a serem removidos e retorna o numero de produtos encontrados
    int nProducts_found = search_product(pProducts, products_index);

    if (nProducts_found == 0) 
    {
        printf("\nProduto nao encontrado.\n");
        return 0;
    }

    Products *pProducts_found = (Products *)malloc(sizeof(Products));
    pProducts_found->nProducts = nProducts_found;
    pProducts_found->pProducts = (Product *)malloc(nProducts_found * sizeof(Product));

    for (int i = 0; i < nProducts_found; i++) 
    {
        pProducts_found->pProducts[i] = pProducts->pProducts[products_index[i]];
    }

    list_products(pProducts_found);

    return 0;
}

/*
*  Função: get_all_products_from_csv
*  Descrição: Obtem todos os produtos do arquivo CSV.
*
*  Parâmetros:
*      char ***lines: ponteiro para o array de ponteiros para as linhas do CSV.
*      int *nLines: ponteiro para o numero de linhas do CSV.
*
*  Retorna:
*      int: 1 se sucesso, 0 se falha.
*/
int get_all_products_from_csv(char ***lines, int *nLines)
{
    const char *file_path = "./DB/produtos.csv";
    FILE *file = fopen(file_path, "r");
    if (file == NULL) 
    {
        printf("\nErro ao abrir o arquivo para leitura.\n");
        return 0;
    }

    // Ler todas as linhas do CSV
    char buffer[512];

    while (fgets(buffer, sizeof(buffer), file)) 
    {
        // Aloca memoria para uma nova linha
        char *line = strdup(buffer);
        if (line == NULL) 
        {
            printf("\nErro ao alocar memória para as linhas.\n");
            fclose(file);
            return 0;
        }

        // Aloca ou realoca memoria para o array de linhas
        char **temp = (char **)realloc(*lines, ((*nLines) + 1) * sizeof(char *));
        if (temp == NULL) 
        {
            printf("\nErro ao realocar memória para as linhas.\n");
            free(line);
            fclose(file);
            return 0;
        }
        *lines = temp;
        (*lines)[(*nLines)++] = line;
    }

    fclose(file);

    return 1;
}

/*
*  Função: write_products_to_csv    
*  Descrição: Escreve os produtos no arquivo CSV.
*  Parâmetros:
*      char **lines: ponteiro para o array de ponteiros para as linhas do CSV.
*      int nLines: numero de linhas do CSV.
*      const char *file_path: caminho do arquivo CSV.
*  Retorna:
*      int: 1 se sucesso, 0 se falha.
*  
*  Nota: A função libera a memória alocada para as linhas do CSV.
*/
int write_products_to_csv(char **lines, int nLines, const char *file_path)
{
    if (lines == NULL)
    {
        printf("\nNao ha linhas para escrever.\n");
        return 0;
    }

    FILE *file = fopen(file_path, "w");
    if (file == NULL) 
    {
        printf("\nErro ao abrir o arquivo para escrita.\n");
        for (int i = 0; i < nLines; i++) free(lines[i]);
        free(lines);
        return 0;
    }

    for (int i = 0; i < nLines; i++) 
    {
        fprintf(file, "%s", lines[i]);
        // Free em cada linha apos escrita
        free(lines[i]); 
    }

    free(lines);
    fclose(file);

    printf("\nProduto inserido ou atualizado com sucesso.\n");

    return 1;
}

/*
*  Função: insert_product_to_csv    
*  Descrição: Insere um produto no arquivo CSV.
*  Parâmetros:
*      Product product: objeto do tipo Product.
*      int index: indice do produto na lista.
*  Retorna:
*      int: 1 se sucesso, 0 se falha.
*/
int insert_product_to_csv(Product product, int index) 
{
    const char *file_path = "./DB/produtos.csv";

    char **lines = NULL;
    int nLines = 0;

    if (!get_all_products_from_csv(&lines, &nLines)) return 0;

    // Modificar uma linha especifica ou adiciona se o index for maior que o tamanho atual
    char new_line[1024];
    snprintf(new_line, sizeof(new_line), "%d,%s,%s,%.2f\n", product.id, product.name, product.description, product.price);

    if (index < nLines) 
    {
        // Free linha antiga
        free(lines[index]);
        // Troca por uma nova linha
        lines[index] = strdup(new_line); 
    } 
    else 
    {
        // Adicionar um nova linha se o index for maior que o tamanho atual
        char **temp = (char **)realloc(lines, (nLines + 1) * sizeof(char *));
        if (temp == NULL) 
        {
            printf("\nErro ao realocar memória para nova linha.\n");
            for (int i = 0; i < nLines; i++) 
                free(lines[i]);
            free(lines);
            return 0;
        }
        lines = temp;
        lines[nLines++] = strdup(new_line);
    }

    if (!write_products_to_csv(lines, nLines, file_path)) return 0;
    
    return 1;
}

/*
*  Função: remove_products_from_csv    
*  Descrição: Remove um produto do arquivo CSV.
*  Parâmetros:
*      int products_index[pProducts->nProducts]: ponteiro para o array de indices dos produtos encontrados.
*      int nIndex: indice do produto na lista.
*  Retorna:
*      int: 1 se sucesso, 0 se falha.
*/
int remove_products_from_csv(int *products_index, int nIndex)
{
    const char *file_path = "./DB/produtos.csv";

    char **lines = NULL;
    int nLines = 0;

    // Obtem todas as linhas do arquivo CSV
    if (!get_all_products_from_csv(&lines, &nLines)) return 0;

    // Remove as linhas especificadas pelos índices
    for (int i = 0; i < nIndex; i++) 
    {
        // Incrementa o índice pois o array de índices é zero-based
        int index = products_index[i] + 1;
        printf("\nIndex: %d\n", index);

        // Move as linhas subsequentes para preencher o espaço da linha removida
        for (int j = index; j < nLines - i - 1; j++) 
        {
            lines[j] = lines[j + 1];
        }
    }
    
    // Atualiza o número de linhas
    nLines -= nIndex;

    // Realoca o array de linhas para o novo tamanho
    char **temp = (char **)realloc(lines, nLines * sizeof(char *));
    if (temp == NULL) 
    {
        printf("\nErro ao realocar memória para nova linha.\n");
        // Libera as linhas antes de retornar
        for (int i = 0; i < nLines; i++) 
            free(lines[i]);
        free(lines);
        return 0;
    }
    
    lines = temp;

    // Escreve as linhas atualizadas de volta no arquivo CSV
    if (!write_products_to_csv(lines, nLines, file_path)) return 0;

    return 1;
}

/*
*  Função: add_product    
*  Descrição: Adiciona um produto.
*  Parâmetros:
*      Products *pProducts: ponteiro para o objeto do tipo Products.
*  Retorna:
*      int: 1 se sucesso, 0 se falha.
*/
int add_product(Products *pProducts) 
{
    Product product = { 0 };
    scan_product(&product);

    if (product.name[0] == '\0') 
    {
        printf("\nNome invalido.\n");
        return 0;
    }

    for (int i = 0; i < pProducts->nProducts; i++) 
    {
        if (strcmp(product.name, pProducts->pProducts[i].name) == 0) 
        {
            printf("\nProduto ja cadastrado.\n");
            return 0;
        }
    }

    if (product.description[0] == '\0') 
    {
        printf("\nDescricao invalida.\n");
        return 0;
    }
    if (product.price < 0)
    {
        printf("\nPreco invalido.\n");
        return 0;
    }

    if (pProducts->pProducts == NULL) 
    {
        pProducts->pProducts = (Product *)malloc(sizeof(Product));
        
        if (pProducts->pProducts == NULL) 
        {
            printf("\nErro ao alocar memoria.\n");
            return 0;
        }
        
        pProducts->nProducts = 0;
        pProducts->pProducts[pProducts->nProducts] = product;
    }
    else 
    {
        Product *temp = (Product *)realloc(pProducts->pProducts, (pProducts->nProducts + 1) * sizeof(Product));
        
        if (temp == NULL) 
        {
            printf("\nErro ao alocar memoria.\n");
            return 0;   
        }

        pProducts->pProducts = temp;
        pProducts->pProducts[pProducts->nProducts] = product;
    }

    pProducts->nProducts++;

    insert_product_to_csv(product, pProducts->nProducts);

    return 1;
}

/*
*  Função: remove_product    
*  Descrição: Remove um produto.
*  Parâmetros:
*      Products *pProducts: ponteiro para o objeto do tipo Products.
*  Retorna:
*      int: 1 se sucesso, 0 se falha.
*/
int remove_product(Products *pProducts) 
{
    if (pProducts->nProducts == 0) 
    {
        printf("\nNao ha produto para remover.\n");
        return 0;
    }

    int products_index[pProducts->nProducts];
    // search_product coleta os indices dos produtos a serem removidos e retorna o numero de produtos encontrados
    int nProducts_found = search_product(pProducts, products_index);

    if (nProducts_found == 0) 
    {
        printf("\nProduto nao encontrado.\n");
        return 0;
    }

    for (int i = 0; i < nProducts_found; i++) 
    {
        int index = products_index[i];

        for (int j = index; j < pProducts->nProducts - i - 1; j++) 
        {
            pProducts->pProducts[j] = pProducts->pProducts[j + 1];
        }

        // pProducts->pProducts[index] = pProducts->pProducts[pProducts->nProducts - i - 1];
    }
    
    pProducts->nProducts -= nProducts_found;
    pProducts->pProducts = (Product *)realloc(pProducts->pProducts, pProducts->nProducts * sizeof(Product));

    remove_products_from_csv(products_index, nProducts_found);

    printf("\nProduto(s) removido(s) com sucesso.\n");
    return 1;
}

/*
*  Função: edit_product    
*  Descrição: Edita um produto.
*  Parâmetros:
*      Products *pProducts: ponteiro para o objeto do tipo Products.
*  Retorna:
*      int: 1 se sucesso, 0 se falha.
*/
int edit_product(Products *pProducts)
{
    if (pProducts->nProducts == 0)
    {
        printf("\nNao ha produtos para editar.\n");
        return 0;
    }

    int index;
    printf("\nDigite o indice do produto a ser editado: ");
    scanf("%d", &index);

    if (index - 1 < 0 || index - 1 >= pProducts->nProducts)
    {
        printf("\nCodigo do produto e invalido. Tente novamente.\n");
        return 0;
    }

    Product edit_product = { 0 };
    scan_product(&edit_product);

    if (edit_product.price < 0)
    {
        printf("\nPreco invalido.\n");
        return 0;
    }

    // Verifica se valor informado pelo usuario não é vazio, caso contrario, o valor anterior se mantem 
    if (edit_product.name[0] != '\0')
    {
        strcpy(pProducts->pProducts[index - 1].name, edit_product.name);
    }
    // Verifica se valor informado pelo usuario não está vazio, caso contrario, o valor anterior se mantem
    if (edit_product.description[0] != '\0')
    {
        strcpy(pProducts->pProducts[index - 1].description, edit_product.description);
    }

    pProducts->pProducts[index - 1].price = edit_product.price;

    insert_product_to_csv(pProducts->pProducts[index - 1], index);

    return 1;
}

/*
*  Função: export_products_to_csv    
*  Descrição: Exporta os produtos para um arquivo CSV.
*  Parâmetros:
*      Products *pProducts: ponteiro para o objeto do tipo Products.
*  Retorna:
*      int: 1 se sucesso, 0 se falha.
*/
int export_products_to_csv(Products *pProducts, int is_report) 
{
    FILE *file = fopen(is_report ? "./relatorios/produtos.csv" : "./DB/produtos.csv", "w");

    if (file == NULL) 
    {
        printf("\nErro ao abrir o arquivo.\n");
        return 0;
    }

    fprintf(file, "id,nome,descricao,preco\n");

    for (int i = 0; i < pProducts->nProducts; i++) 
    {
        fprintf(file, "%d,%s,%s,%.2f\n", pProducts->pProducts[i].id, pProducts->pProducts[i].name, pProducts->pProducts[i].description, pProducts->pProducts[i].price);
    }

    printf("\nProdutos exportados com sucesso.\n");

    fclose(file);

    return 1;
}

/*
*  Função: import_products_from_csv    
*  Descrição: Importa os produtos de um arquivo CSV.
*  Parâmetros:
*      Products *pProducts: ponteiro para o objeto do tipo Products.
*  Retorna:
*      int: 1 se sucesso, 0 se falha.
*
*  Essa função importa os produtos de um arquivo CSV e os armazena na 
*  estrutura Products. Caso o usuario deseje sobrescrever os dados, a 
*  função desaloca a memória antiga e aloca uma nova memória para os 
*  produtos.
*/
int import_products_from_csv(Products *pProducts) 
{
    char file_name[128];
    char full_path[256];
    
    printf("\nDigite o nome do arquivo CSV: ");
    getchar();
    fgets(file_name, 128, stdin);
    trim(file_name);

    if (file_name[0] == '\0') 
    {
        strcpy(full_path, "./relatorios/produtos_temp.csv");
    }
    else
    {
        snprintf(full_path, sizeof(full_path), "./DB/%s", file_name);
    }

    // Verifica se a memoria foi alocada para os produtos
    if (pProducts->pProducts == NULL) 
    {
        // Aloca memoria para o array de produtos
        pProducts->pProducts = (Product *)malloc(sizeof(Product));
        if (pProducts->pProducts == NULL) {
            fprintf(stderr, "Erro ao alocar memória.\n");
            return 0;
        }
        pProducts->nProducts = 0;
    } 
    else 
    {
        // Caso o usuario deseja sobrescrever os dados dos produtos
        printf("\nDeseja sobrescrever os dados? (s/n): ");
        char option;
        scanf(" %c", &option);

        // Se o usuario escolher sobrescrever os dados
        if (option == 's' || option == 'S') 
        {
            // Desaloca memoria antiga
            free(pProducts->pProducts); 
            // Aloca nova memoria
            pProducts->pProducts = (Product *)malloc(sizeof(Product)); 
            if (pProducts->pProducts == NULL) {
                fprintf(stderr, "Erro ao alocar memória.\n");
                return 0;
            }
            pProducts->nProducts = 0;
        }
    }

    FILE *file = fopen(full_path, "r");
    if (file == NULL) 
    {
        fprintf(stderr, "Erro ao abrir o arquivo para leitura: %s\n", full_path);
        return 0;
    }

    // Pula a primeira linha (cabecalho)
    char buffer[256];
    if (!fgets(buffer, sizeof(buffer), file)) 
    {
        fprintf(stderr, "Erro ao ler o arquivo.\n");
        fclose(file);
        return 0;
    }

    Product product;
    while (fscanf(file, "%d,%128[^,],%256[^,],%f\n", &product.id, product.name, product.description, &product.price) == 4) 
    {
        Product *temp = (Product *)realloc(pProducts->pProducts, (pProducts->nProducts + 1) * sizeof(Product));
        if (temp == NULL) {
            fprintf(stderr, "Erro ao realocar memória.\n");
            fclose(file);
            return 0;
        }
        pProducts->pProducts = temp;

        pProducts->pProducts[pProducts->nProducts] = product;
        pProducts->nProducts++;
    }

    printf("\nProdutos importados com sucesso. Total: %d\n", pProducts->nProducts);
    fclose(file);

    if (!export_products_to_csv(pProducts, 0)) return 0;

    product_id += pProducts->nProducts;

    return 1;
}

/*
*  Função: create_products_file    
*  Descrição: Cria o arquivo de produtos.
*  Parâmetros:
*      void
*  Retorna:
*      int: 1 se sucesso, 0 se falha.
*/
int create_products_file()
{
    FILE *file = fopen("./DB/produtos.csv", "w");

    if (file == NULL)
    {
        printf("\nErro ao abrir o arquivo.\n");
        return 0;
    }   

    fprintf(file, "id,nome,descricao,preco\n");

    fclose(file);

    return 1;
}

/*
*  Função: free_products    
*  Descrição: Libera a memoria alocada para os produtos.
*  Parâmetros:
*      Products *pProducts: ponteiro para o objeto do tipo Products.
*  Retorna:
*      int: 1 se sucesso, 0 se falha.
*/
int free_products(Products *pProducts) 
{
    free(pProducts->pProducts);
    return 0;
}

/*
*  Função: product_menu    
*  Descrição: Menu de produtos.
*  Parâmetros:
*      Products *pProducts: ponteiro para o objeto do tipo Products.
*  Retorna:
*      int: 1 se sucesso, 0 se falha.
*/
int product_menu(Products *pProducts) 
{
    int option;

    do
    {
        printf("\nEscolha uma opcao:\n");
        printf("1: Adicionar produto\n2: Remover produto\n3: Editar produto\n4: Listar produtos\n5: Listar produtos alfabeticamente\n6: Buscar produto\n7: Exportar produtos para CSV\n8: Importar produtos de CSV\n0: Sair\n");

        scanf("%d", &option);

        switch (option)
        {
        case 1:
            add_product(pProducts);
            break;

        case 2:
            remove_product(pProducts);
            break;

        case 3:
            edit_product(pProducts);
            break;

        case 4:
            list_products(pProducts);
            break;

        case 5:
            list_products_alfabetic_order(pProducts);
            break;
        
        case 6:
            list_search_product(pProducts);
            break;

        case 7:
            export_products_to_csv(pProducts, 1);
            break;

        case 8:
            import_products_from_csv(pProducts);
            break;

        default:
            break;
        }
    } 
    while (option != 0);

    return 0;
}

#endif // PRODUCT_H