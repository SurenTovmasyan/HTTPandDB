#include "include/HTTP_Server.h"

using namespace httplib;

int main(){
    HTTP_Server srv;
    srv.start();

    srv.stop();
    return 0;
}