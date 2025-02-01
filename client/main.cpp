#include "include/HTTP_Client.h"
#include "../general/include/User.h"

int main(){
    HTTP_Client client;

    client.start();

    client.authorization();

    client.to_main_page();

    client.stop();
    
    return 0;
}