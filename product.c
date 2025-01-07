#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// #include "utils.c"

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

int generate_product_id()
{
    return ++product_id;
}

void print_product(const Product *pProduct) 
{
    printf("\nID Produto: %d\n", pProduct->id);
    printf("Nome: %s\n", pProduct->name);
    printf("Descricao: %s\n", pProduct->description);
    printf("Preco: %.2f\n", pProduct->price);

}

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

void list_products_alfabetic_order(const Products *pProducts) 
{
    if (pProducts->nProducts == 0) 
    {
        printf("\nNao ha produtos para listar.\n");
        return;
    }

    Product *products = malloc(pProducts->nProducts * sizeof(Product));
    if (products == NULL) 
    {
        printf("\nErro ao alocar memoria\n");
        return;
    }
    memcpy(products, pProducts->pProducts, pProducts->nProducts * sizeof(Product));

    for (int i = 0; i < pProducts->nProducts - 1; i++) 
    {
        for (int j = 0; j < pProducts->nProducts - i - 1; j++) 
        {
            if (strcmp(products[j].name, products[j+1].name) > 0) 
            {
                Product temp = products[j];
                products[j] = products[j+1];
                products[j+1] = temp;
            }
        }
    }

    printf("\n====== Produto ======\n");
    for (int i = 0; i < pProducts->nProducts; i++)
        print_product(products+i);
    printf("\n=====================\n");

    free(products);
}

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

int search_product_by_id(Products *pProducts, int products_index[pProducts->nProducts]) 
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

int search_product_by_name(Products *pProducts, int products_index[pProducts->nProducts]) 
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

int search_product_by_price_range(Products *pProducts, int products_index[pProducts->nProducts]) 
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

int search_product(Products *pProducts, int products_index[pProducts->nProducts]) 
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

int get_all_products_from_csv(char ***lines, int *nLines) 
{
    const char *file_path = "./relatorios/produtos.csv";

    // Open the CSV file for reading
    FILE *file = fopen(file_path, "r");
    if (file == NULL) 
    {
        printf("\nErro ao abrir o arquivo para leitura.\n");
        return 0;
    }

    // Read all lines into memory
    char buffer[512];

    while (fgets(buffer, sizeof(buffer), file)) 
    {
        // Allocate memory for a new line
        char *line = strdup(buffer);
        if (line == NULL) 
        {
            printf("\nErro ao alocar memória para as linhas.\n");
            fclose(file);
            return 0;
        }

        char **temp = realloc(*lines, ((*nLines) + 1) * sizeof(char *));
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

int write_products_to_csv(char **lines, int nLines, const char *file_path)
{
    if (lines == NULL)
    {
        printf("\nNao ha linhas para escrever.\n");
        return 0;
    }

    // Write all lines back to the CSV file
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
        free(lines[i]); // Free each line after writing
    }

    free(lines);
    fclose(file);

    printf("\nProduto inserido ou atualizado com sucesso.\n");

    return 1;
}

int insert_product_to_csv(Product product, int index) 
{
    const char *file_path = "./relatorios/produtos.csv";

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
        char **temp = realloc(lines, (nLines + 1) * sizeof(char *));
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

int remove_products_from_csv(int *products_index, int nIndex)
{
    const char *file_path = "./relatorios/produtos.csv";

    char **lines = NULL;
    int nLines = 0;

    if (!get_all_products_from_csv(&lines, &nLines)) return 0;

    // O ultimo elemento passa para a possicao do produto removido
    for (int i = 0; i < nIndex; i++) 
    {
        int index = products_index[i] + 1;

        for (int j = index; j < nLines - i - 1; j++) 
        {
            lines[j] = lines[j + 1];
        }
    }
    
    nLines -= nIndex;

    char **temp = realloc(lines, nLines * sizeof(char *));
    if (temp == NULL) 
    {
        printf("\nErro ao realocar memória para nova linha.\n");
        for (int i = 0; i < nLines; i++) 
            free(lines[i]);
        free(lines);
        return 0;
    }
    
    lines = temp;

    if (!write_products_to_csv(lines, nLines, file_path)) return 0;

    return 1;
}

int add_product(Products *pProducts) 
{
    Product product = { 0 };
    scan_product(&product);

    if (product.name[0] == '\0') 
    {
        printf("\nNome invalido.\n");
        return 0;
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

    if (pProducts->pProducts == NULL) return 0;

    remove_products_from_csv(products_index, nProducts_found);

    printf("\nProduto(s) removido(s) com sucesso.\n");
    return 1;
}

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

int export_products_to_csv(Products *pProducts) 
{
    FILE *file = fopen("./relatorios/produtos.csv", "w");

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
        // Build the full file path safely
        snprintf(full_path, sizeof(full_path), "./relatorios/%s", file_name);
    }

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
        Product *temp = realloc(pProducts->pProducts, (pProducts->nProducts + 1) * sizeof(Product));
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

    if (!export_products_to_csv(pProducts)) return 0;

    product_id += pProducts->nProducts;

    return 1;
}

int create_products_file()
{
    FILE *file = fopen("./relatorios/produtos.csv", "w");

    if (file == NULL)
    {
        printf("\nErro ao abrir o arquivo.\n");
        return 0;
    }   

    fprintf(file, "id,nome,descricao,preco\n");

    fclose(file);

    return 1;
}

int free_products(Products *pProducts) 
{
    free(pProducts->pProducts);
    return 0;
}

int product_menu(Products *pProducts) 
{
    int option;

    do
    {
        printf("\nEscolha uma opcao:\n");
        printf("1: Adicionar produto\n2: Remover produto\n3: Editar produto\n4: Listar produtos\n5: Listar produtos alfabeticamente\n6: Exportar produtos para CSV\n7: Importar produtos de CSV\n0: Sair\n");

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
            export_products_to_csv(pProducts);
            break;

        case 7:
            import_products_from_csv(pProducts);
            break;

        default:
            break;
        }
    } 
    while (option != 0);

    return 0;
}