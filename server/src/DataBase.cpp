#include "../include/DataBase.h"

DataBase DataBase::single;
bool DataBase::is_first_time = true;

DataBase& DataBase::get_instance(const Config& config){
    if(is_first_time){
        single.logger = spdlog::stderr_color_mt("DataBase");
        spdlog::set_pattern("%^[%Y-%m-%d %H:%M:%S] [%l] %v%$");
        single.logger->info("Creating connection with DB server on " + config.db_host + ":" + to_string(config.db_port) + ".");
        is_first_time = false;
        single.driver = mysql::get_mysql_driver_instance();

        try{
            single.con = single.driver->connect(config.db_host + ":" + to_string(config.db_port), config.user_name, config.user_password);
        } catch (const SQLException ex){
            single.logger->critical("DB server is offed or unavailable.");
            exit(1);
        }

        single.con->setSchema("http_and_db");
    }
    return single;
}

vector<User>& DataBase::get_users(){
    logger->info("Preparing and executing query to get all users.");
    PreparedStatement *ps = con->prepareStatement("Select * from users;");
    ResultSet *res = ps->executeQuery();

    vector<User> *temp = new vector<User>();
    while(res->next()){
        User user(res->getInt(1), res->getString(2),res->getString(3), res->getString(4), res->getString(5));
        temp->push_back(user);
    }
    logger->info("Preparing vector<User> to return.");
    logger->info("Everything went successfully!");
    return *temp;
}

void DataBase::add_user(const User& user){
    logger->info("Checking name availability for gotten user.");
    if(check_name_availability(user.get_name())){
        PreparedStatement *ps = con->prepareStatement("Insert into users (name,password,email) values (?, ?, ?);");
        ps->setString(1, user.get_name());
        ps->setString(2, user.get_password());
        ps->setString(3, user.get_email());
        int res = ps->executeUpdate();
        logger->info("Preparing and executing update of inserting new user.");

        if(res == 0){
            logger->error("DB couldn't execute update.");
            throw SQLException();
        }
        logger->info("New user add to DB.");
    }
    else{
        logger->error("Gotten user's name isn't available.");
        throw invalid_argument("name");
    }
}

bool DataBase::check_name_availability(const string& name){
    logger->info("Preparing and executing query of getting names.");
    PreparedStatement *ps = con->prepareStatement("Select name from users;");
    auto res = ps->executeQuery();
    while(res->next()){
        if(res->getString(1) == name){
            logger->info("An user with the same name is found.");
            return false;
        }
    }
    logger->info("An user with the same name isn't found.");
    return true;
}

void DataBase::delete_user(int id){
    logger->info("Preparing and executing update of deleting user by ID.");
    PreparedStatement *ps = con->prepareStatement("Delete from users where id = ?;");
    ps->setInt(1, id);
    int res = ps->executeUpdate();

    logger->info("User deleted successfully.");
    if(res == 0){
        logger->error("DB couldn't execute update.");
        throw SQLException();
    }
}

void DataBase::delete_user(string name){
    logger->info("Preparing and executing update of deleting user by Name.");
    PreparedStatement *ps = con->prepareStatement("Delete from users where name = ?;");
    ps->setString(1, name);
    int res = ps->executeUpdate();
    
    logger->info("User deleted successfully.");
    if(res == 0){
        logger->error("DB couldn't execute update.");
        throw SQLException();
    }
}