#ifndef SERVER
#define SERVER

#include <nlohmann/json.hpp> 
#include <fstream>
#include <iostream>
#include <cstring>
#include "../../general/include/User.h"
#include "../libs/cpp-httplib/httplib.h"
#include <vector>
#include "Config.h"
#include "DataBase.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

using json = nlohmann::json;
using namespace std;
using namespace httplib;

class HTTP_Server{
public:
    HTTP_Server() = default;

    void start();
    void stop();

private:
    static Server server;
    static Config config;
    static shared_ptr<spdlog::logger> logger;
    static DataBase db;

    void bind_points();
    static void add_user(const Request&, Response&);
    static void check_user(const Request&, Response&);
    static void get_users(const Request&, Response&);
    static void delete_user(const Request&, Response&);

    HTTP_Server(HTTP_Server& other) = delete;
    HTTP_Server(HTTP_Server&& other) = delete;

    HTTP_Server& operator=(HTTP_Server& other) = delete;
    HTTP_Server& operator=(HTTP_Server&& other) = delete;
};

#endif //SERVER