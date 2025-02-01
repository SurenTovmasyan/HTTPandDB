#ifndef CLIENT
#define CLIENT

#include <iostream>
#include <cstring>
#include <nlohmann/json.hpp> 
#include <fstream>
#include <vector>
#include "../../general/include/User.h"
#include "Config.h"
#include <cctype>
#include "../libs/cpp-httplib/httplib.h"

using namespace std;
using json = nlohmann::json;
using namespace httplib;

class HTTP_Client{
public:
    HTTP_Client() = default;

    void start();
    void stop();

    void authorization();
    void to_main_page();

private:
    User working_user;
    unique_ptr<Client> client;
    Config config;
    string domain;

    void create_user();
    void get_users();
    void delete_user();
    void show_user_info();

    HTTP_Client(HTTP_Client& other) = delete;
    HTTP_Client(HTTP_Client&& other) = delete;

    HTTP_Client& operator=(HTTP_Client& other) = delete;
    HTTP_Client& operator=(HTTP_Client&& other) = delete;
};

#endif //CLIENT