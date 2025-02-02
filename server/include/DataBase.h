#ifndef DATABASE
#define DATABASE

#include <nlohmann/json.hpp> 
#include <fstream>
#include <iostream>
#include <cstring>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include "Config.h"
#include "../../general/include/User.h"
#include <vector>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

using json = nlohmann::json;
using namespace std;
using namespace sql;

class DataBase{
private:
    Connection *con;
    mysql::MySQL_Driver *driver;
    shared_ptr<spdlog::logger> logger;
    static DataBase single;
    static bool is_first_time;
public:
    static DataBase& get_instance(const Config&);
    
    vector<User>& get_users(); 
    bool check_name_availability(const string&);
    void add_user(const User&);
    void delete_user(int);
    void delete_user(string);
};

#endif //DATABASE