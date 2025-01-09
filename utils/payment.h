#ifndef PAYMENT_H
#define PAYMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "product.h"

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

/*
*  Função: generate_payment_id
*  Descrição: Gera um novo ID para o pagamento.
*  Retorna:
*      int: ID gerado.
*/
int generate_payment_id()
{
    return ++payment_id;
}

/*
*  Função: print_payment
*  Descrição: Imprime os detalhes do pagamento.
*  Parâmetros:
*      Payment *pPayment: ponteiro para o objeto do tipo Payment.
*  Retorna:
*      void.
*/
void print_payment(const Payment *pPayment) 
{
    printf("\nID Pagamento: %d\n", pPayment->id);
    printf("CPF: %s\n", pPayment->cpf);
    printf("Data: %s\n", pPayment->date);
    printf("Preco total: %.2f\n", pPayment->total_price);

    int *counts = (int *)calloc(pPayment->pProducts->nProducts, sizeof(int));

    if (counts == NULL) {
        printf("Erro ao alocar memória para o contador de produtos.\n");
        return;
    }

    // Contador ocorrencias de cada produto
    for (int i = 0; i < pPayment->pProducts->nProducts; i++) {
        counts[pPayment->pProducts->pProducts[i].id]++;
    }

    printf("Produtos: ");
    for (int i = 0; i < pPayment->pProducts->nProducts; i++) {
        if (counts[i] > 0) {
            printf("%dx%s ", counts[i], pPayment->pProducts->pProducts[i].name);
        }
    }
    printf("\n");

    free(counts);
}

/*
*  Função: list_payments
*  Descrição: Listar os pagamentos.
*  Parâmetros:
*      Payments *pPayment: ponteiro para o objeto do tipo Payments.
*  Retorna:
*      void.
*/
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

/*
*  Função: scan_payment
*  Descrição: Ler os detalhes do pagamento.
*  Parâmetros:
*      Payment *pPayment: ponteiro para o objeto do tipo Payment.
*      Products *pProducts: ponteiro para o objeto do tipo Products.
*  Retorna:
*      int: 1 se sucesso, 0 se falha.
*/
int scan_payment(Payment *pPayment, Products *pProducts) 
{
    // Verifica se há produtos cadastrados
    if (pProducts->nProducts == 0)
    {
        printf("\nNao ha produtos cadastrados.\n");
        return 0;
    }

    // Gera um novo ID para o pagamento se ainda não existir
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

    // Inicializa o preço total do pagamento
    pPayment->total_price = 0;

    // Aloca memória para os produtos no pagamento, se necessário
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

        // Aloca ou realoca memória para a lista de produtos no pagamento
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

/*
*  Função: scan_edit_payment
*  Descrição: Ler os detalhes do pagamento.
*  Parâmetros:
*      Payment *pPayment: ponteiro para o objeto do tipo Payment.
*  Retorna:
*      void.
*/
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

/*
*  Função: search_payment_by_id
*  Descrição: Busca um pagamento pelo ID.
*  Parâmetros:
*      Payments *pPayments: ponteiro para o objeto do tipo Payments.
*      int payments_index[]: ponteiro para o array de indices dos pagamentos encontrados.
*  Retorna:
*      int: quantidade de pagamentos encontrados.
*/
int search_payment_by_id(Payments *pPayments, int payments_index[]) 
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

/*
*  Função: search_payment_by_cpf
*  Descrição: Busca um pagamento pelo CPF.
*  Parâmetros:
*      Payments *pPayments: ponteiro para o objeto do tipo Payments.
*      int payments_index[]: ponteiro para o array de indices dos pagamentos encontrados.
*  Retorna:
*      int: quantidade de pagamentos encontrados.
*/
int search_payment_by_cpf(Payments *pPayments, int payments_index[]) 
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

/*
*  Função: search_payment_by_price_range
*  Descrição: Busca um pagamento pela faixa de preco.
*  Parâmetros:
*      Payments *pPayments: ponteiro para o objeto do tipo Payments.
*      int payments_index[]: ponteiro para o array de indices dos pagamentos encontrados.
*  Retorna:
*      int: quantidade de pagamentos encontrados.
*/
int search_payment_by_price_range(Payments *pPayments, int payments_index[]) 
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

/*
*  Função: search_payment
*  Descrição: Busca um pagamento.
*  Parâmetros:
*      Payments *pPayments: ponteiro para o objeto do tipo Payments.
*      int payments_index[]: ponteiro para o array de indices dos pagamentos encontrados.
*  Retorna:
*      int: quantidade de pagamentos encontrados.
*/
int search_payment(Payments *pPayments, int payments_index[]) 
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

/*
*  Função: get_all_payments_from_csv
*  Descrição: Obtem todos os pagamentos do arquivo CSV.
*  Parâmetros:
*      char ***lines: ponteiro para o array de ponteiros para as linhas do CSV.
*      int *nLines: ponteiro para o numero de linhas do CSV.
*  Retorna:
*      int: 1 se sucesso, 0 se falha.
*/
int get_all_payments_from_csv(char ***lines, int *nLines) 
{
    const char *file_path = "./DB/pagamentos.csv";

    FILE *file = fopen(file_path, "r");
    if (file == NULL) 
    {
        printf("\nErro ao abrir o arquivo para leitura.\n");
        return 0;
    }

    // Armazena todas as linhas do CSV
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
*  Função: write_payments_to_csv
*  Descrição: Escreve os pagamentos no arquivo CSV.
*  Parâmetros:
*      char **lines: ponteiro para o array de ponteiros para as linhas do CSV.
*      int nLines: numero de linhas do CSV.
*      const char *file_path: caminho do arquivo CSV.
*  Retorna:
*      int: 1 se sucesso, 0 se falha.
*/
int write_payments_to_csv(char **lines, int nLines, const char *file_path)
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
        // Free linha antiga
        free(lines[i]);
    }

    free(lines);
    fclose(file);

    printf("\nProduto inserido ou atualizado com sucesso.\n");

    return 1;
}

/*
*  Função: insert_payment_to_csv
*  Descrição: Insere um pagamento no arquivo CSV.
*  Parâmetros:
*      Payment payment: objeto do tipo Payment.
*      int index: indice do pagamento na lista.
*  Retorna:
*      int: 1 se sucesso, 0 se falha.
*/
int insert_payment_to_csv(Payment payment, int index) 
{
    const char *file_path = "./DB/pagamentos.csv";

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

    if (!write_payments_to_csv(lines, nLines, file_path)) return 0;
    
    return 1;
}

/*
*  Função: remove_payments_from_csv
*  Descrição: Remove pagamentos especificados do arquivo CSV.
*  Parâmetros:
*      int *payments_index: ponteiro para o array de índices dos pagamentos a serem removidos.
*      int nIndex: número de pagamentos a serem removidos.
*  Retorna:
*      int: 1 se sucesso, 0 se falha.
*/
int remove_payments_from_csv(int *payments_index, int nIndex)
{
    const char *file_path = "./DB/pagamentos.csv";

    char **lines = NULL;
    int nLines = 0;

    // Obtem todas as linhas do arquivo CSV
    if (!get_all_payments_from_csv(&lines, &nLines)) return 0;

    // Remove as linhas especificadas pelos índices
    for (int i = 0; i < nIndex; i++) 
    {
        // Incrementa o índice pois o array de índices é zero-based
        int index = payments_index[i] + 1;

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
    if (!write_payments_to_csv(lines, nLines, file_path)) return 0;

    return 1;
}

/*
*  Função: add_payment
*  Descrição: Adiciona um pagamento.
*  Parâmetros:
*      Payments *pPayments: ponteiro para o objeto do tipo Payments.
*      Products *pProducts: ponteiro para o objeto do tipo Products.
*  Retorna:
*      int: 1 se sucesso, 0 se falha.
*/
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

/*
*  Função: remove_payment
*  Descrição: Remove um pagamento.
*  Parâmetros:
*      Payments *pPayments: ponteiro para o objeto do tipo Payments.
*  Retorna:
*      int: 1 se sucesso, 0 se falha.
*/
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

    remove_payments_from_csv(payments_index, nPayments_found);

    printf("\nProduto(s) removido(s) com sucesso.\n");
    return 1;
}

/*
*  Função: edit_payment
*  Descrição: Edita um pagamento.
*  Parâmetros:
*      Payments *pPayments: ponteiro para o objeto do tipo Payments.
*  Retorna:
*      int: 1 se sucesso, 0 se falha.
*/
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

/*
*  Função: export_payment_to_csv
*  Descrição: Exporta os pagamentos para um arquivo CSV.
*  Parâmetros:
*      Payments *pPayments: ponteiro para o objeto do tipo Payments.
*  Retorna:
*      int: 1 se sucesso, 0 se falha.
*/
int export_payment_to_csv(Payments *pPayments, int is_report) 
{
    FILE *file = fopen(is_report ? "./relatorios/pagamentos.csv" : "./DB/pagamentos.csv", "w");

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

/*
*  Função: import_payment_from_csv
*  Descrição: Importa pagamentos de um arquivo CSV.
*  Parâmetros:
*      Payments *pPayments: ponteiro para o objeto do tipo Payments.
*  Retorna:
*      int: 1 se sucesso, 0 se falha.
*/
int import_payment_from_csv(Payments *pPayments) 
{
    char file_name[128];
    char full_path[256];
    
    printf("\nDigite o nome do arquivo CSV: ");
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
        snprintf(full_path, sizeof(full_path), "./DB/%s", file_name);
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
        Payment *temp = (Payment *)realloc(pPayments->pPayments, (pPayments->nPayments + 1) * sizeof(Payment));
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

    if (!export_payment_to_csv(pPayments, 0)) return 0;

    payment_id += pPayments->nPayments;

    return 1;
}

/*
*  Função: create_payments_file
*  Descrição: Cria o arquivo de pagamentos.
*  Parâmetros:
*      void
*  Retorna:
*      int: 1 se sucesso, 0 se falha.
*/
int create_payments_file()
{
    FILE *file = fopen("./DB/pagamentos.csv", "w");

    if (file == NULL)
    {
        printf("\nErro ao abrir o arquivo.\n");
        return 0;
    }   

    fprintf(file, "id,CPF,data,preco_total\n");

    fclose(file);

    return 1;
}

/*
*  Função: free_payments
*  Descrição: Libera a memória alocada para os pagamentos.
*  Parâmetros:
*      Payments *pPayments: ponteiro para o objeto do tipo Payments.
*  Retorna:
*      int: 1 se sucesso, 0 se falha.
*/
int free_payments(Payments *pPayments) 
{
    free(pPayments->pPayments);
    return 0;
}

/*
*  Função: payment_menu
*  Descrição: Menu de pagamentos.
*  Parâmetros:
*      Payments *pPayments: ponteiro para o objeto do tipo Payments.
*      Products *pProducts: ponteiro para o objeto do tipo Products.
*  Retorna:
*      int: 1 se sucesso, 0 se falha.
*/
int payment_menu(Payments *pPayments, Products *pProducts) 
{
    int option;

    do
    {
        printf("\nEscolha uma opcao:\n");
        printf("1: Adicionar pagamento\n2: Remover pagamento\n3: Editar pagamento\n4: Listar pagamentos\n5: Exportar pagamentos para CSV\n0: Sair\n");

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
            export_payment_to_csv(pPayments, 1);
            break;

        // case 6:
            // import_payment_from_csv(pPayments);
            // break;

        default:
            break;
        }
    } 
    while (option != 0);

    return 0;
}

#endif // PAYMENT_H