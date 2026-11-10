#include "client.h"

int main(void)
{
    client_init();
    publish("miss","175144");
    //subscribe("hello");
    return 0;
}
