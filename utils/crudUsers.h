#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LINE_SIZE 1024

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


void addSomeStrings(char ***input, char newLine[1024], int *currentSize)
{
    char **data;
    
    data = (char **) realloc(*input, (*currentSize+1) * sizeof(char *));
    data[*currentSize] = (char *) malloc(LINE_SIZE*sizeof(char *));
    strcpy(data[*currentSize], newLine);
    currentSize++;
}

// feito
int ListarCliente(char id[])
{
    FILE * arq = fopen("DB/Clients.csv", "r");
    char line[1024] = "", *iten;

    while (fgets(line, sizeof(line), arq))
    {
        line[strcspn(line, "\n")] = 0;
        iten = strtok(line, ";");
        if (strcmp(id, iten) != 0) continue;
        while (iten != NULL)
        {
            printf("-%s-", iten);
            iten = strtok(NULL, ";");
        }
        printf("%s\n", "");
    }
    
    fclose(arq);
    return 0;
}

// feito
int ListarClientes()
{
    FILE * arq = fopen("DB/Clients.csv", "r");
    char line[1024] = "", *iten;

    while (fgets(line, sizeof(line), arq))
    {
        line[strcspn(line, "\n")] = 0;
        iten = strtok(line, ";");

        while (iten != NULL)
        {
            printf("-%s-", iten);
            iten = strtok(NULL, ";");
        }
        printf("%s\n", "");
    }
    
    fclose(arq);
    return 0;
}

// feito
int InserirCliente(Client *clt)
{
    char *lastID = "0", *cpfConf, _line[1024];

    FILE * arq;

    arq = fopen("DB/Clients.csv", "r");

    while (fgets(_line, sizeof(_line), arq))
    {
        lastID = strtok(_line, ";");
        cpfConf = strtok(NULL, ";");

        if (strcmp(clt->cpf, cpfConf) == 0)
        {
            perror("CPF ja cadastrado!");
            return 1;
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

    fclose(arq);
    return 0;
}


// Feito
int AlterarCliente(Client *clt, char idClt[]) {
    FILE *arq = fopen("DB/Clients.csv", "r");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    char tempLine[1024];

    while (fgets(tempLine, sizeof(tempLine), arq))
    {
        char *tempID = strtok(tempLine, ";");
        char *cpfConf = strtok(NULL, ";");

        if (strcmp(clt->cpf, cpfConf) == 0)
        {
            perror("CPF ja existente!");
            return 1;
        }
    }

    FILE *tempFile = fopen("DB/temp.csv", "w");
    if (tempFile == NULL) {
        printf("Erro ao criar o arquivo temporário.\n");
        fclose(arq);
        return 1;
    }

    char line[1024];
    int found = 0;

    while (fgets(line, sizeof(line), arq)) {
        char idInLine[10];
        sscanf(line, "%9[^;];", idInLine);

        if (strcmp(idInLine, idClt) == 0) {
            found = 1;
            
            fprintf(tempFile, "%s;%s;%s;%s;%s;%s;%s\n", idClt, clt->cpf, clt->name, 
                                                      clt->name_enterprise, clt->phone, 
                                                      clt->email, clt->birthdate);
        } else {
            
            fputs(line, tempFile);
        }
    }

    fclose(arq);
    fclose(tempFile);

    
    if (!found) {
        printf("Cliente com ID %s não encontrado.\n", clt->id);
        remove("DB/temp.csv");
        return 1;
    }

    remove("DB/Clients.csv");
    rename("DB/temp.csv", "DB/Clients.csv");

    printf("Cliente atualizado com sucesso.\n");
    return 0;
}


// Feito
int RemoverCliente(const char *id) {
    FILE *arq = fopen("DB/Clients.csv", "r");
    if (arq == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    FILE *tempFile = fopen("DB/temp.csv", "w"); 
    if (tempFile == NULL) {
        printf("Erro ao criar arquivo temporário.\n");
        fclose(arq);
        return 1;
    }

    char line[1024];
    int found = 0;


    while (fgets(line, sizeof(line), arq)) {
        char idInLine[10];
        sscanf(line, "%9[^;];", idInLine);  

        if (strcmp(idInLine, id) != 0) {
            fputs(line, tempFile);
        } else {
            found = 1;
        }
    }

    fclose(arq);
    fclose(tempFile);

    if (!found) {
        printf("Cliente com ID %s não encontrado.\n", id);
        remove("DB/temp.csv");
        return 1;
    }

    remove("DB/Clients.csv");
    rename("DB/temp.csv", "DB/Clients.csv");

    printf("Cliente com ID %s removido com sucesso.\n", id);
    return 0;
}