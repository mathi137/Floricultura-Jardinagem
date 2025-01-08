#include <stdlib.h>
#include <string.h>
#define LINE_SIZE 1024

typedef struct
{
    char id[10],
         cpf[11],
         name[255],
         name_enterprise[255],
         phone[16],
         email[255],
         birthdate[16];
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

    while (NULL != fgets(line, sizeof(line), arq))
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

    while (NULL != fgets(line, sizeof(line), arq))
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
    FILE * arq = fopen("DB/Clients.csv", "a");
    char line[1024] = "";

    strcat(line, clt->id);
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


int AlterarCliente(Client *clt, char id[])
{
    FILE * arq = fopen("DB/Clients.csv", "r");
    char line[1024] = "", *iten, *cpf, *name, *name_enterprise, *phone, *email, *birthdate;
    int numLines = 0, target= -1, i=0, size = 0;

    char **fileLines = (char **)malloc(sizeof(char *));

    while (NULL != fgets(line, sizeof(line), arq))
    {
        iten = strtok(line, ";");

        if (strcmp(iten, id) == 0) target = i;

        addSomeStrings(&fileLines, line, &size);

        i++;
    }

    if (target == -1) return 1;

    fclose(arq);

    fileLines[target][strcspn(fileLines[target], "\n")] = 0;
    iten = strtok(fileLines[target], ";");

    cpf = strtok(NULL, ";");
    name = strtok(NULL, ";");
    name_enterprise = strtok(NULL, ";");
    phone = strtok(NULL, ";");
    email = strtok(NULL, ";");
    birthdate = strtok(NULL, ";");
    
    strcpy(line, "");
    
    printf("%s\n", *cpf);

    if (!(clt->cpf))
    {
        strcat(cpf, clt->cpf);
    }
    if (!(clt->name))
    {
        name = clt->name;
    }
    if (!(clt->name_enterprise))
    {
        name_enterprise = clt->name_enterprise;
    }
    if (!(clt->phone))
    {
        phone = clt->phone;
    }
    if (!(clt->email))
    {
        email = clt->email;
    }
    if (!(clt->birthdate))
    {
        birthdate = clt->birthdate;
    }

    printf("%s\n", cpf);

    strcat(line, id);
    strcat(line, ";");
    strcat(line, cpf);
    printf("%s\n", line);
    strcat(line, ";");
    strcat(line, name);
    strcat(line, ";");
    strcat(line, name_enterprise);
    strcat(line, ";");
    strcat(line, phone);
    strcat(line, ";");
    strcat(line, email);
    strcat(line, ";");
    strcat(line, birthdate);
    strcat(line, "\n");
    
    printf("%d\n", target);

    strcpy(fileLines[target], line);
    
    printf("%d\n", target);
    
    arq = fopen("DB/Clients.csv", "w");
    
    printf("%d\n", target);

    for (size_t i = 0; i < size+1; i++)
    {
        fputs(fileLines[i], arq);
    }
    
    printf("%d\n", target);

    fclose(arq);
    return 0;
}


int RemoverCliente(char id[])
{

}
