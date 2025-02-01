#include "../include/DataBase.h"

DataBase DataBase::single;
bool DataBase::is_first_time = true;

DataBase& DataBase::get_instance(const Config& config){
    if(is_first_time){
        is_first_time = false;
        single.driver = mysql::get_mysql_driver_instance();
        single.con = single.driver->connect(config.db_host + ":" + to_string(config.db_port), config.user_name, config.user_password);
        single.con->setSchema("http_and_db");
    }
    return single;
}

vector<User>& DataBase::get_users(){
    PreparedStatement *ps = con->prepareStatement("Select * from users;");
    ResultSet *res = ps->executeQuery();

    vector<User> *temp = new vector<User>();
    while(res->next()){
        User user(res->getInt(1), res->getString(2),res->getString(3), res->getString(4), res->getString(5));
        temp->push_back(user);
    }

    return *temp;
}

void DataBase::add_user(User& user){
    PreparedStatement *ps = con->prepareStatement("Insert into users (name,password,email) values (?, ?, ?);");
    ps->setString(1, user.get_name());
    ps->setString(2, user.get_password());
    ps->setString(3, user.get_email());
    int res = ps->executeUpdate();

    if(res == 0)
        throw SQLException("Something wrong with DB");
}

void DataBase::delete_user(int id){
    PreparedStatement *ps = con->prepareStatement("Delete from users where id = ?;");
    ps->setInt(1, id);
    int res = ps->executeUpdate();

    if(res == 0)
        throw SQLException("Something wrong with DB");
}

void DataBase::delete_user(string name){
    PreparedStatement *ps = con->prepareStatement("Delete from users where name = ?;");
    ps->setString(1, name);
    int res = ps->executeUpdate();

    if(res == 0)
        throw SQLException("Something wrong with DB");
}