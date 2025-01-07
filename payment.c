#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// #include "utils.c"

#define CPF_SIZE 16
#define DATE_SIZE 16

static int payment_id = 0;

typedef struct 
{
    int id;
    char cpf[CPF_SIZE];
    char date[DATE_SIZE];
    Products *pProducts;
    float total_price;
} Payment;

typedef struct 
{
    Payment *pPayments;
    int nPayments;
} Payments;

int generate_payment_id()
{
    return ++payment_id;
}

void print_payment(const Payment *pPayment) 
{
    printf("\nID Produto: %d\n", pPayment->id);
    printf("CPF: %s\n", pPayment->cpf);
    printf("Data: %s\n", pPayment->date);
    printf("Preco total: %.2f\n", pPayment->total_price);

    printf("Produtos: ");
    for (int i = 0; i < pPayment->pProducts->nProducts; i++)
        printf("%s ", pPayment->pProducts->pProducts[i].name);
    printf("\n");
}

void list_payments(const Payments *pPayments) 
{
    if (pPayments->nPayments == 0) 
    {
        printf("\nNao ha pagamentos para listar.\n");
        return;
    }

    printf("\n====== Pagementos ======\n");
    for (int i = 0; i < pPayments->nPayments; i++)
        print_payment(pPayments->pPayments+i);
    printf("\n=====================\n");
}

int scan_payment(Payment *pPayment, Products *pProducts) 
{
    if (pProducts->nProducts == 0)
    {
        printf("\nNao ha produtos cadastrados.\n");
        return 0;
    }

    if (pPayment->id == 0)
        pPayment->id = generate_payment_id();

    printf("\nDigite o CPF do cliente: ");
    getchar();
    fgets(pPayment->cpf, CPF_SIZE, stdin);
    trim(pPayment->cpf);

    printf("Digite a data do pagamento (DD-MM-AAAA): ");
    fgets(pPayment->date, DATE_SIZE, stdin);
    trim(pPayment->date);

    printf("\nENTRANDO NO PROCESSAMENTO DE PRODUTOS\n");

    pPayment->total_price = 0;

    if (pPayment->pProducts == NULL) 
    {
        pPayment->pProducts = (Products *)malloc(sizeof(Products));
        if (pPayment->pProducts == NULL) 
        {
            printf("\nErro ao alocar memoria para os produtos no pagamento.\n");
            return 0;
        }
        pPayment->pProducts->pProducts = NULL;
        pPayment->pProducts->nProducts = 0;
    }

    int index;

    do
    {
        printf("Insira o ID do produto (0 para sair): ");
        scanf("%d", &index);

        if (index < 0 || index > pProducts->nProducts) 
        {
            printf("\nID invalido, tente novamente!\n");
            continue;
        }

        if (index == 0)
            break;

        Product *temp;

        if (pPayment->pProducts->pProducts == NULL)
            temp = (Product *)malloc(sizeof(Product));
        else 
            temp = (Product *)realloc(pPayment->pProducts->pProducts, (pPayment->pProducts->nProducts + 1) * sizeof(Product));

        if (temp == NULL) 
        {
            printf("\nNao foi possivel alocar memoria para o produtos.\n");
            return 0;
        }

        pPayment->pProducts->pProducts = temp;

        // Adiciona produto a lista de pagamento
        pPayment->pProducts->pProducts[pPayment->pProducts->nProducts] = pProducts->pProducts[index - 1];
        pPayment->pProducts->nProducts++;

        // Atualiza preco total
        pPayment->total_price += pProducts->pProducts[index - 1].price;
    } 
    while (1);

    printf("\nProdutos inseridos com sucesso! Total: %d\n", pPayment->pProducts->nProducts);

    return 1;
}

void scan_edit_payment(Payment *pPayment) 
{
    if (pPayment->id == 0)
        pPayment->id = generate_payment_id();

    printf("\nDigite o CPF do produto: ");
    getchar();
    fgets(pPayment->cpf, CPF_SIZE, stdin);
    trim(pPayment->cpf);

    printf("Digite a Data do produto: ");
    fgets(pPayment->date, DATE_SIZE, stdin);
    trim(pPayment->date);
}

int search_payment_by_id(Payments *pPayments, int payments_index[pPayments->nPayments]) 
{
    int id;
    printf("\nDigite o ID do produto: ");
    scanf("%d", &id);

    int count = 0;
    for (int i = 0; i < pPayments->nPayments; i++) 
    {
        if (pPayments->pPayments[i].id == id) 
        {
            payments_index[count] = i;
            count++;
        }
    }

    return count;
}

int search_payment_by_cpf(Payments *pPayments, int payments_index[pPayments->nPayments]) 
{
    char cpf[CPF_SIZE];
    printf("\nDigite o CPF do produto: ");
    getchar();
    fgets(cpf, CPF_SIZE, stdin);
    trim(cpf);

    int count = 0;
    for (int i = 0; i < pPayments->nPayments; i++) 
    {
        if (strcmp(pPayments->pPayments[i].cpf, cpf) == 0) 
        {
            payments_index[count] = i;
            count++;
        }
    }

    return count;
}

int search_payment_by_price_range(Payments *pPayments, int payments_index[pPayments->nPayments]) 
{
    float min_price, max_price;
    printf("\nDigite o preco minimo: ");
    scanf("%f", &min_price);
    printf("Digite o preco maximo: ");
    scanf("%f", &max_price);

    int count = 0;
    for (int i = 0; i < pPayments->nPayments; i++) 
    {
        if (pPayments->pPayments[i].total_price >= min_price && pPayments->pPayments[i].total_price <= max_price) 
        {
            payments_index[count] = i;
            count++;
        }
    }

    return count;
}

int search_payment(Payments *pPayments, int payments_index[pPayments->nPayments]) 
{
    printf("\nOpcoes de busca:\n");
    printf("1: Buscar por ID\n2: Buscar por CPF\n3: Buscar por faixa de preco\n");

    int option;

    do
    {
        printf("\nEscolha uma opcao: ");
        scanf("%d", &option);
        
        switch (option)
        {
        case 1:
            return search_payment_by_id(pPayments, payments_index);

        case 2:
            return search_payment_by_cpf(pPayments, payments_index);

        case 3:
            return search_payment_by_price_range(pPayments, payments_index);

        default:
            break;
        }
    } while (option < 1 || option > 3);

    return 0;
}

int get_all_payments_from_csv(char ***lines, int *nLines) 
{
    const char *file_path = "./relatorios/pagamentos.csv";

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

int write_payments_to_csv(char **lines, int nLines, const char *file_path)
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

int insert_payment_to_csv(Payment payment, int index) 
{
    const char *file_path = "./relatorios/pagamentos.csv";

    char **lines = NULL;
    int nLines = 0;

    if (!get_all_payments_from_csv(&lines, &nLines)) return 0;

    // Modificar uma linha especifica ou adiciona se o index for maior que o tamanho atual
    char new_line[1024];
    snprintf(new_line, sizeof(new_line), "%d,%s,%s,%.2f\n", payment.id, payment.cpf, payment.date, payment.total_price);

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

    if (!write_payments_to_csv(lines, nLines, file_path)) return 0;
    
    return 1;
}

int remove_payments_from_csv(int *payments_index, int nIndex)
{
    const char *file_path = "./relatorios/pagamentos.csv";

    char **lines = NULL;
    int nLines = 0;

    if (!get_all_payments_from_csv(&lines, &nLines)) return 0;

    // O ultimo elemento passa para a possicao do produto removido
    for (int i = 0; i < nIndex; i++) 
    {
        int index = payments_index[i] + 1;

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

    if (!write_payments_to_csv(lines, nLines, file_path)) return 0;

    return 1;
}

int add_payment(Payments *pPayments, Products *pProducts) 
{
    Payment payment = { 0 };
    if (!scan_payment(&payment, pProducts)) return 0;

    if (payment.cpf[0] == '\0') 
    {
        printf("\nCPF invalido.\n");
        return 0;
    }
    if (payment.date[0] == '\0') 
    {
        printf("\nData invalida.\n");
        return 0;
    }
    if (payment.total_price < 0)
    {
        printf("\nPreco invalido.\n");
        return 0;
    }

    Payment *temp;

    if (pPayments->pPayments == NULL) 
        temp = (Payment *)malloc(sizeof(Payment));
    else 
        temp = (Payment *)realloc(pPayments->pPayments, (pPayments->nPayments + 1) * sizeof(Payment));

    if (temp == NULL) 
    {
        printf("\nErro ao alocar memoria.\n");
        return 0;   
    }

    pPayments->pPayments = temp;
    pPayments->pPayments[pPayments->nPayments] = payment;
    pPayments->nPayments++;

    insert_payment_to_csv(payment, pPayments->nPayments);

    return 1;
}

int remove_payment(Payments *pPayments) 
{
    if (pPayments->nPayments == 0) 
    {
        printf("\nNao ha produto para remover.\n");
        return 0;
    }

    int payments_index[pPayments->nPayments];
    // search_payment coleta os indices dos pagamentos a serem removidos e retorna o numero de pagamentos encontrados
    int nPayments_found = search_payment(pPayments, payments_index);

    if (nPayments_found == 0) 
    {
        printf("\nProduto nao encontrado.\n");
        return 0;
    }

    for (int i = 0; i < nPayments_found; i++) 
    {
        int index = payments_index[i];

        for (int j = index; j < pPayments->nPayments - i - 1; j++) 
        {
            pPayments->pPayments[j] = pPayments->pPayments[j + 1];
        }
    }
    
    pPayments->nPayments -= nPayments_found;
    pPayments->pPayments = (Payment *)realloc(pPayments->pPayments, pPayments->nPayments * sizeof(Payment));

    if (pPayments->pPayments == NULL) return 0;

    remove_payments_from_csv(payments_index, nPayments_found);

    printf("\nProduto(s) removido(s) com sucesso.\n");
    return 1;
}

int edit_payment(Payments *pPayments)
{
    if (pPayments->nPayments == 0)
    {
        printf("\nNao ha pagamentos para editar.\n");
        return 0;
    }

    int index;
    printf("\nDigite o indice do produto a ser editado: ");
    scanf("%d", &index);

    if (index - 1 < 0 || index - 1 >= pPayments->nPayments)
    {
        printf("\nCodigo do produto e invalido. Tente novamente.\n");
        return 0;
    }

    Payment edit_payment = { 0 };
    scan_edit_payment(&edit_payment);

    if (edit_payment.total_price < 0)
    {
        printf("\nPreco invalido.\n");
        return 0;
    }

    // Verifica se valor informado pelo usuario não é vazio, caso contrario, o valor anterior se mantem 
    if (edit_payment.cpf[0] != '\0')
    {
        strcpy(pPayments->pPayments[index - 1].cpf, edit_payment.cpf);
    }
    // Verifica se valor informado pelo usuario não está vazio, caso contrario, o valor anterior se mantem
    if (edit_payment.date[0] != '\0')
    {
        strcpy(pPayments->pPayments[index - 1].date, edit_payment.date);
    }

    pPayments->pPayments[index - 1].total_price = edit_payment.total_price;

    insert_payment_to_csv(pPayments->pPayments[index - 1], index);

    return 1;
}

int export_payment_to_csv(Payments *pPayments) 
{
    FILE *file = fopen("./relatorios/pagamentos.csv", "w");

    if (file == NULL) 
    {
        printf("\nErro ao abrir o arquivo.\n");
        return 0;
    }

    fprintf(file, "id,CPF,data,preco_total\n");

    for (int i = 0; i < pPayments->nPayments; i++) 
    {
        fprintf(file, "%d,%s,%s,%.2f\n", pPayments->pPayments[i].id, pPayments->pPayments[i].cpf, pPayments->pPayments[i].date, pPayments->pPayments[i].total_price);
    }

    printf("\npagamentos exportados com sucesso.\n");

    fclose(file);

    return 1;
}

int import_payment_from_csv(Payments *pPayments) 
{
    char file_name[128];
    char full_path[256];
    
    printf("\nDigite o CPF do arquivo CSV: ");
    getchar();
    fgets(file_name, 128, stdin);
    trim(file_name);

    if (file_name[0] == '\0') 
    {
        strcpy(full_path, "./relatorios/pagamentos_temp.csv");
    }
    else
    {
        // Build the full file path safely
        snprintf(full_path, sizeof(full_path), "./relatorios/%s", file_name);
    }

    if (pPayments->pPayments == NULL) 
    {
        // Aloca memoria para o array de pagamentos
        pPayments->pPayments = (Payment *)malloc(sizeof(Payment));
        if (pPayments->pPayments == NULL) {
            fprintf(stderr, "Erro ao alocar memória.\n");
            return 0;
        }
        pPayments->nPayments = 1;
    } 
    else 
    {
        // Caso o usuario deseja sobrescrever os dados dos pagamentos
        printf("\nDeseja sobrescrever os dados? (s/n): ");
        char option;
        scanf(" %c", &option);

        // Se o usuario escolher sobrescrever os dados
        if (option == 's' || option == 'S') 
        {
            // Desaloca memoria antiga
            free(pPayments->pPayments); 
            // Aloca nova memoria
            pPayments->pPayments = (Payment *)malloc(sizeof(Payment)); 
            if (pPayments->pPayments == NULL) {
                fprintf(stderr, "Erro ao alocar memória.\n");
                return 0;
            }
            pPayments->nPayments = 1;
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

    Payment payment;
    while (fscanf(file, "%d,%16[^,],%16[^,],%f\n", &payment.id, payment.cpf, payment.date, &payment.total_price) == 4) 
    {
        Payment *temp = realloc(pPayments->pPayments, (pPayments->nPayments + 1) * sizeof(Payment));
        if (temp == NULL) {
            fprintf(stderr, "Erro ao realocar memória.\n");
            fclose(file);
            return 0;
        }
        pPayments->pPayments = temp;

        pPayments->pPayments[pPayments->nPayments] = payment;
        pPayments->nPayments++;
    }

    printf("\npagamentos importados com sucesso. Total: %d\n", pPayments->nPayments);
    fclose(file);

    if (!export_payment_to_csv(pPayments)) return 0;

    payment_id += pPayments->nPayments;

    return 1;
}

int create_payments_file()
{
    FILE *file = fopen("./relatorios/pagamentos.csv", "w");

    if (file == NULL)
    {
        printf("\nErro ao abrir o arquivo.\n");
        return 0;
    }   

    fprintf(file, "id,CPF,data,preco_total\n");

    fclose(file);

    return 1;
}

int free_payments(Payments *pPayments) 
{
    free(pPayments->pPayments);
    return 0;
}

int payment_menu(Payments *pPayments, Products *pProducts) 
{
    int option;

    do
    {
        printf("\nEscolha uma opcao:\n");
        printf("1: Adicionar pagamento\n2: Remover pagamento\n3: Editar pagamento\n4: Listar pagamentos\n5: Exportar pagamentos para CSV\n6: Importar pagamentos de CSV\n0: Sair\n");

        scanf("%d", &option);

        switch (option)
        {
        case 1:
            add_payment(pPayments, pProducts);
            break;

        case 2:
            remove_payment(pPayments);
            break;

        case 3:
            edit_payment(pPayments);
            break;

        case 4:
            list_payments(pPayments);
            break;

        case 5:
            export_payment_to_csv(pPayments);
            break;

        case 6:
            import_payment_from_csv(pPayments);
            break;

        default:
            break;
        }
    } 
    while (option != 0);

    return 0;
}