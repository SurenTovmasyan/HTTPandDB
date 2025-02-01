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
    string host;
    int port;

    Config() = default;

    static Config& get_instance();

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Config, host, port);
private:
    static Config single;
    static bool is_firts_time;
};

#endif //CONFIG