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

using json = nlohmann::json;
using namespace std;
using namespace sql;

class DataBase{
private:
    Connection *con;
    mysql::MySQL_Driver *driver;
    static DataBase single;
    static bool is_first_time;
public:
    static DataBase& get_instance(const Config&);
    
    vector<User>& get_users(); 
    void add_user(User&);
    void delete_user(int);
    void delete_user(string);
};

#endif //DATABASE