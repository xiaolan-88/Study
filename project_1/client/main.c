#include "client.h"

int main(void)
{
    client_init();
    //publish("hello","ABCDE");
    subscribe("miss");

    while(1){
        
    }
    return 0;
}
