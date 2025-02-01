#ifndef CONFIG
#define CONFIG

#include <nlohmann/json.hpp> 
#include <fstream>
#include <iostream>
#include <cstring>

using json = nlohmann::json;
using namespace std;

class Config{
public:
    string client_host, db_host;
    string user_name, user_password;
    int client_port, db_port;

    Config() = default;

    static Config& get_instance();

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Config, client_host, client_port, user_name, user_password, db_host, db_port);
private:
    static Config single;
    static bool is_firts_time;
};

#endif //CONFIG