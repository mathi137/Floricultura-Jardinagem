#include <stdio.h>
#include "utils\crudUsers.h"

int main(int argc, char const *argv[])
{
    ListarClientes();
    ListarCliente("3");

    Client clt = {"123", "123", "123", "123", "123", "123", "123"};
    // AlterarCliente(&clt, "3");
    return 0;
}
