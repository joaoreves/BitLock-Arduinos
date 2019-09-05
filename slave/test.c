#include <stdlib.h>
#include <stdio.h>

int main()
{
    char *key = "wi4142oen24io14qwfen23n55253225235moaosnfinwe00";

    char *string = "Open. Alfredo Cagado.";

    char send[1024];

    for (int i = 0; i < sizeof(key); i++)
    {
        send[i] = string[i] + key [sizeof(key) - i];
        printf("%02X", send[i]);
    }

    printf("\n");
}
