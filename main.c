#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Defining structs

typedef struct
{
    int index;
    int status;
    char name[128];
    char phone[16];
    char email[128];
    char birthdate[16];
    char password[128];
} User;

typedef struct
{
    User *pUsers;
    int users_length;
} Users;

// End of defining structs

// Defining functions 

void get_user_data(User *user)
{
    printf("=== Cadastro ===\n");
    printf("Insira os dados do usuario:\n");
    user->status = 1;
    printf("Nome: ");
    scanf("%s", user->name);
    printf("Telefone: ");
    scanf("%s", user->phone);
    printf("Email: ");
    scanf("%s", user->email);
    printf("Data de nascimento: ");
    scanf("%s", user->birthdate);
    printf("Senha: ");
    scanf("%s", user->password);
    printf("========================\n\n");
}

void get_login_data(User *user)
{   
    printf("=== Login ===\n");
    printf("Insira o nome e a senha:\n");
    printf("Nome: ");
    scanf("%s", user->name);
    printf("Senha: ");
    scanf("%s", user->password);
    printf("========================\n\n");
}

int create_user(Users *pUsers_struct)
{
    User *user;
    get_user_data(user);

    if (pUsers_struct->users_length == 0)
        pUsers_struct->pUsers = (User *)malloc(sizeof(User));
    else
        pUsers_struct->pUsers = (User *)realloc(pUsers_struct->pUsers, (pUsers_struct->users_length + 1) * sizeof(User));
    
    pUsers_struct->users_length++;

    if (pUsers_struct->pUsers == NULL)
    {
        printf("Erro ao alocar memoria");
        return 1;
    }
    
    user->index = pUsers_struct->users_length - 1;
    pUsers_struct->pUsers[pUsers_struct->users_length - 1] = *user;
    return 0;
}

void disable_user(Users *pUsers_struct, int index)
{
    pUsers_struct->pUsers[index].status = 0;
    printf("Conta desativada\n\n");
}

void enable_user(Users *pUsers_struct, int index)
{
    pUsers_struct->pUsers[index].status = 1;
    printf("Conta reativada\n\n");
}

void update_user(Users *pUsers_struct, int index)
{
    get_user_data(&(pUsers_struct->pUsers[index]));
    printf("Dados atualizados\n\n");
}

int login(Users *pUsers_struct, User **pCurrent_user)
{
    User user;
    get_login_data(&user);

    for (int i = 0; i < pUsers_struct->users_length; i++)
    {
        if (strcmp(pUsers_struct->pUsers[i].name, user.name) == 0)
        {
            if (strcmp(pUsers_struct->pUsers[i].password, user.password) == 0)
            {
                *pCurrent_user = &(pUsers_struct->pUsers[i]);
                printf("Login efetuado com sucesso\n\n");
                return 0;
            }
            else
            {
                printf("Senha incorreta\n\n");
                return 1;
            }
        }
    }

    printf("Usuario nao encontrado\n\n");
    return 1;
}

void singup(Users *pUsers_struct, User **pCurrent_user)
{
    create_user(pUsers_struct);
    login(pUsers_struct, pCurrent_user);
}

void print_user(User user) {
    printf("=== Dados do Usuario ===\n");
    printf("Indice: %d\n", user.index);
    printf("Nome: %s\n", user.name);
    printf("Email: %s\n", user.email);
    printf("Telefone: %s\n", user.phone);
    printf("Status: %s\n", user.status ? "Ativo" : "Inativo");
    printf("Aniversario: %s\n", user.birthdate);
    printf("========================\n\n");
}

// End of defining functions

int main()
{
    // Initializing variables
    Users *pUsers_struct;
    pUsers_struct->users_length = 0;

    int option;

    while (1) {
        User *pCurrent_user = NULL;
        
        while (pCurrent_user == NULL)
        {
            printf("Crie uma conta ou faça o login:\n");
            printf("1: Criar conta\n2: Fazer login\n");

            scanf("%d", &option);

            switch (option)
            {
                case 0:
                    printf("FIM DO PROGRAMA\n"); 
                    // free(pUsers_struct->pUsers);
                    // free(pUsers_struct);
                    return 0;  

                case 1:
                    singup(pUsers_struct, &pCurrent_user);
                    break;

                case 2:
                    login(pUsers_struct, &pCurrent_user);
                    break;              

                default:
                    break;
            }
        }

        do
        {
            printf("Escolha uma opcao:\n");
            printf("1: Editar dados\n2: Desativar conta\n3: Reativar conta\n4: Imprimir dados\n5: Logout\n0: Sair\n");

            scanf("%d", &option);

            switch (option)
            {
                case 0:
                    free(pUsers_struct->pUsers);
                    free(pUsers_struct);
                    return 0;

                case 1:
                    update_user(pUsers_struct, pCurrent_user->index);
                    break;

                case 2:
                    disable_user(pUsers_struct, pCurrent_user->index);
                    break;

                case 3:
                    enable_user(pUsers_struct, pCurrent_user->index);
                    break;

                case 4:
                    print_user(*pCurrent_user);
                    break;
                
                default:
                    break;
            }
        } while (option != 5);
    }

    // Freeing memory
    // free(pUsers_struct->pUsers);
    // free(pUsers_struct);

    printf("FIM DO PROGRAMA\n");

    return 0;
}