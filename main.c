#include <stdio.h>
// #include "client.h"
#include "utils/product.h"
#include "utils/payment.h"
#include "utils/crudUsers.h"

/*
*  Função: main
*  Descrição: Função principal.
*  Retorna:
*      0 quando o programa encerrar.
*/
int main(void) 
{
    // Client clients = { NULL, 0 };
    Products products = { NULL, 0 };
    Payments payments = { NULL, 0 };

    // Criar os respectivos arquivos
    // if (!create_clients_file()) return 0;
    if (!create_products_file()) return 0;
    if (!create_payments_file()) return 0;

    int option;

    do
    {
        printf("\nEscolha um menu:\n");
        printf("1: Cliente\n2: Produto\n3: Pagamento\n0: Sair\n");

        scanf("%d", &option);

        switch (option)
        {
        case 1:
            client_menu();
            break;

        case 2:
            product_menu(&products);
            break;

        case 3:
            payment_menu(&payments, &products);
            break;
        
        default:
            break;
        }
    } 
    while (option != 0);

    printf("FIM DO PROGRAMA\n");

    // free_clients(&clients);
    free_products(&products);
    free_payments(&payments);    

    return 0;
}