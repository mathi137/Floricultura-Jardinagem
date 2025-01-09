#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

typedef struct
{
    char id[10];
    char cpf[16];
    char name[255];
    char name_enterprise[255];
    char phone[16];
    char email[255];
    char birthdate[16];
} Client;


/*
* Função: ListarClientes
* Descrição: Lista todos os clientes cadastrados no arquivo 'Clients.csv'.
* Retorna:
*   int 0: se houve listagem. 1: caso não haja clientes para listar.
*/
int ListarClientes()
{
    FILE * arq = fopen("DB/Clients.csv", "r");
    char line[1024] = "", *iten;
    int atLeastOne = 0;

    printf("======= Clientes =======\n\n");

    while (fgets(line, sizeof(line), arq))
    {
        line[strcspn(line, "\n")] = 0;
        iten = strtok(line, ";");

        iten = strtok(NULL, ";");
        printf(" -CPF: %s\n", iten);
        iten = strtok(NULL, ";");
        printf(" -Nome: %s\n", iten);
        iten = strtok(NULL, ";");
        printf(" -Nome da Empresa: %s\n", iten);
        iten = strtok(NULL, ";");
        printf(" -Telefone: %s\n", iten);
        iten = strtok(NULL, ";");
        printf(" -Email: %s\n", iten);
        iten = strtok(NULL, ";");
        printf(" -Data de Nascimento: %s\n\n", iten);

        atLeastOne=1;
    }

    if (!atLeastOne) 
    {
        printf("- Sem clientes cadastrados\n\n");
        return 1;
    }

    printf("========================\n\n");
    
    fclose(arq);
    return 0;
}


/*
* Função: CadastrarCliente
* Descrição: Cadastra um cliente no arquivo 'Clients.csv'.
* Parâmetros:
*   Client *clt - Ponteiro para o objeto do tipo Client.
* Retorna:
*   int 1: se o CPF for nulo. 2: caso CPF ja esteja cadastrado. 0: caso o CPF for cadastrado com sucesso.
*/
int CadastrarCliente(Client *clt)
{
    if (strcmp(trim(clt->cpf), "") == 0)
    {
        printf("CPF nao pode ser nulo!\n");
        return 1;
    }

    char *lastID = "0", *cpfConf, _line[1024];

    FILE * arq;

    arq = fopen("DB/Clients.csv", "r");

    while (fgets(_line, sizeof(_line), arq))
    {
        lastID = strtok(_line, ";");
        cpfConf = strtok(NULL, ";");

        if (strcmp(clt->cpf, cpfConf) == 0)
        {
            printf("CPF ja cadastrado!\n");
            return 2;
        }
    }

    fclose(arq);

    arq = fopen("DB/Clients.csv", "a");
    char line[1024] = "";

    int id = atoi(lastID)+1;
    char id_str[10];

    sprintf(id_str, "%d", id);

    strcat(line, id_str);
    strcat(line, ";");
    strcat(line, clt->cpf);
    strcat(line, ";");
    strcat(line, clt->name);
    strcat(line, ";");
    strcat(line, clt->name_enterprise);
    strcat(line, ";");
    strcat(line, clt->phone);
    strcat(line, ";");
    strcat(line, clt->email);
    strcat(line, ";");
    strcat(line, clt->birthdate);
    strcat(line, "\n");

    fputs(line, arq);

    printf("Cliente cadastrado.\n");

    fclose(arq);
    return 0;
}


/*
* Função: AtualizarCliente
* Descrição: atualiza os dados de um cliente ja existente no arquivo 'Clients.csv' baseado em seu CPF.
* Parâmetros:
*   Client *clt - Ponteiro para o objeto do tipo Client.
* Retorna:
*   int 1: Erro ao abrir o arquivo. 2: Erro ao criar o arquivo temporário. 3: CPF nao encontrado. 0: Cliente atualizado com sucesso.
*/
int AtualizarCliente(Client *clt) {
    FILE *arq = fopen("DB/Clients.csv", "r");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    FILE *tempFile = fopen("DB/temp.csv", "w");
    if (tempFile == NULL) {
        printf("Erro ao criar o arquivo temporário.\n");
        fclose(arq);
        return 2;
    }

    char line[1024];
    int found = 0;

    while (fgets(line, sizeof(line), arq)) {
        char *idInLine, *cpfInLine, tempLine[1024];
        strcpy(tempLine, line);
        idInLine = strtok(tempLine, ";");
        cpfInLine = strtok(NULL, ";");

        if (strcmp(cpfInLine, clt->cpf) == 0) {
            found = 1;
            
            fprintf(tempFile, "%s;%s;%s;%s;%s;%s;%s\n", idInLine, clt->cpf, clt->name, 
                                                      clt->name_enterprise, clt->phone, 
                                                      clt->email, clt->birthdate);
        } else {
            
            fputs(line, tempFile);
        }
    }

    fclose(arq);
    fclose(tempFile);

    
    if (!found) {
        printf("Cliente com CPF %s nao encontrado.\n", clt->cpf);
        remove("DB/temp.csv");
        return 3;
    }

    remove("DB/Clients.csv");
    rename("DB/temp.csv", "DB/Clients.csv");

    printf("Cliente atualizado com sucesso.\n");
    return 0;
}


/*
* Função: RemoverCliente
* Descrição: Remove o cadastro de um cliente do arquivo 'Clients.csv'.
* Parâmetros:
*   char *cpf - ponteiro para a variavel cpf
* Retorna:
*   int 1: Erro ao abrir o arquivo. 2: Erro ao criar o arquivo temporário. 3: CPF nao encontrado. 0: Cliente removido com sucesso.
*/
int RemoverCliente(char *cpf) {
    FILE *arq = fopen("DB/Clients.csv", "r");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    FILE *tempFile = fopen("DB/temp.csv", "w"); 
    if (tempFile == NULL) {
        printf("Erro ao criar arquivo temporário.\n");
        fclose(arq);
        return 2;
    }

    char line[1024];
    int found = 0;


    while (fgets(line, sizeof(line), arq)) {
        char *idInLine, *cpfInLine, tempLine[1024];
        strcpy(tempLine, line);
        idInLine = strtok(tempLine, ";");
        cpfInLine = strtok(NULL, ";");

        if (strcmp(cpfInLine, cpf) != 0) {
            fputs(line, tempFile);
        } else {
            found = 1;
        }
    }

    fclose(arq);
    fclose(tempFile);

    if (!found) {
        printf("Cliente com CPF %s nao encontrado.\n", cpf);
        remove("DB/temp.csv");
        return 3;
    }

    remove("DB/Clients.csv");
    rename("DB/temp.csv", "DB/Clients.csv");

    printf("Cliente com CPF %s removido com sucesso.\n", cpf);
    return 0;
}

/*
* Função: client_menu
* Descrição: É chamada na main. Controla o menu de produtos.
* Retorna:
*   int: 1 se sucesso, 0 se falha.
*/
int client_menu()
{
    int option;
    char cpf[10];
    Client clt;
    do
    {
        printf("\nEscolha uma opcao:\n");
        printf("1: Listar clientes\n2: Cadastrar cliente\n3: Atualizar cliente pelo CPF\n4: Remover cliente\n0: Sair\n");
        printf("->: ");

        scanf("%d", &option);

        printf("\n");

        switch (option)
        {
        case 1:
            ListarClientes();
            break;

        case 2:
            getchar();
            printf("CPF: ");
            gets(clt.cpf);
            printf("Nome: ");
            gets(clt.name);
            printf("Nome da Empresa: ");
            gets(clt.name_enterprise);
            printf("Telefone: ");
            gets(clt.phone);
            printf("Email: ");
            gets(clt.email);
            printf("Data de Nascimento: ");
            gets(clt.birthdate);
            CadastrarCliente(&clt);
            break;

        case 3:
            getchar();
            printf("CPF: ");
            gets(clt.cpf);
            printf("Nome: ");
            gets(clt.name);
            printf("Nome da Empresa: ");
            gets(clt.name_enterprise);
            printf("Telefone: ");
            gets(clt.phone);
            printf("Email: ");
            gets(clt.email);
            printf("Data de Nascimento: ");
            gets(clt.birthdate);
            AtualizarCliente(&clt);
            break;

        case 4:
            getchar();
            printf("Digite o CPF do Cliente: ");
            gets(cpf);
            RemoverCliente(cpf);
            break;

        default:
            printf("Opcao inexistente!\n");
            break;
        }
    } 
    while (option != 0);

    return 0;
}