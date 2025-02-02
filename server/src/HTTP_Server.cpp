#include "../include/HTTP_Server.h"

Server HTTP_Server::server;
Config HTTP_Server::config;
DataBase HTTP_Server::db;
shared_ptr<spdlog::logger> HTTP_Server::logger;

void HTTP_Server::start(){
    logger = spdlog::stdout_color_mt("server");
    spdlog::set_pattern("%^[%Y-%m-%d %H:%M:%S] [%l] %v%$");
    logger->info("Starting server.");
    logger->info("Getting configuration instance.");
    config = Config::get_instance();

    logger->info("Connnecting to DB server.");
    db = DataBase::get_instance(config);
    logger->info("Connected to DB successfully.");

    bind_points();

    logger->info("Listening to clients.");
    server.listen(config.client_host, config.client_port);
}

void HTTP_Server::stop(){
    logger->info("Stoping server.");
    server.stop();
    logger->critical("Server stopped successfully.");
}

void HTTP_Server::bind_points(){
    server.Post("/check_user", HTTP_Server::check_user)
          .Get("/get_users", HTTP_Server::get_users)
          .Put("/add_user", HTTP_Server::add_user)
          .Delete("/delete_user", HTTP_Server::delete_user);
}

void HTTP_Server::check_user(const Request &req, Response &res){
    User temp("NONE");
    string name = req.get_param_value("name");
    string password = req.get_param_value("password");
    logger->info("Got request of checking user N:" + name + ", P:" + password + ".");

    vector<User> users = db.get_users();
    logger->info("Getting users list.");
    bool found_one = false;
    for (User user :users){
        if(user.get_name() == name){
            if(user.get_password() == password){
                found_one = true;
                temp = user;
            }
        }
        if(found_one) break;
    }

    if(found_one){
        logger->info("Found user with same name and password.");
    }
    else{
        logger->error("Didn't found user with same name and password.");
    }
        
    json j;
    j["user"] = temp;
    res.set_content(j.dump(), "application/json");
    logger->info("Sent response.");
}

void HTTP_Server::get_users(const Request &req, Response &res){
    logger->info("Got request of getting users.");
    json j;
    logger->info("Getting users from DB.");
    j["users"] = db.get_users();

    logger->info("Sending JSON file with users.");
    res.set_content(j.dump(), "application/json");
}

void HTTP_Server::add_user(const Request &req, Response &res){
    logger->info("Got request of adding an user.");
    json j = json::parse(req.body);
    User new_user = j["user"];
    logger->info(new_user.to_string());
    try{
        db.add_user(new_user);
        res.status = 200;
    } catch(SQLException& ex){
        logger->error("DB error: " + string(ex.what()) + ".");
        res.set_content(ex.what(), "text/plain");
        res.status = 500;
    } catch(invalid_argument& ex){
        logger->error("Invalid name(already used).");
        res.set_content(ex.what(), "text/plain");
        res.status = 400;
    }
    logger->info("User added to DB.");
}

void HTTP_Server::delete_user(const Request &req, Response &res){
    logger->info("Got request of deleting an user.");
    json j = json::parse(req.body);
    string way = j["way"];
    if(way == "id"){
        int id = j["id"];
        try{
            db.delete_user(id);
            res.status = 200;
        } catch(SQLException ex){
            logger->error("DB error: " + string(ex.what()) + ".");
            res.set_content(ex.what(), "text/plain");
            res.status = 500;
        }
    }
    else{
        string name = j["name"];
        try{
            db.delete_user(name);
            res.status = 200;
        } catch(SQLException& ex){
            logger->error("DB error: " + string(ex.what()) + ".");
            res.set_content(ex.what(), "text/plain");
            res.status = 500;
        }
    }
    logger->info("User deleted from DB.");
}