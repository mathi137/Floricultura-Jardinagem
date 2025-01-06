#include <stdlib.h>
#include <string.h>


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
    FILE * arq = fopen("DB/Clients.csv", "r+");
    char line[1024] = "", *iten, *cpf, *name, *name_enterprise, *phone, *email, *birthdate,
    *fileLines[1024];
    int numLines = 0, target, i;

    while (NULL != fgets(line, sizeof(line), arq))
    {
        line[strcspn(line, "\n")] = 0;
        iten = strtok(line, ";");

        if (strcmp(iten, id) == 0) target = i;

        i++;
    }

    cpf = strtok(NULL, ";");
    name = strtok(NULL, ";");
    name_enterprise = strtok(NULL, ";");
    phone = strtok(NULL, ";");
    email = strtok(NULL, ";");
    birthdate = strtok(NULL, ";");
    
    strcpy(line, "");

    if (!(clt->cpf))
    {
        cpf = clt->cpf;
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

    strcat(line, id);
    strcat(line, ";");
    strcat(line, cpf);
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
    
    fclose(arq);
    return 0;
}


int RemoverCliente(char id[])
{

}
