#include "../include/HTTP_Server.h"

Server HTTP_Server::server;
Config HTTP_Server::config;
DataBase HTTP_Server::db;

void HTTP_Server::start(){
    cout << "Starting server" << endl;
    cout << "Getting configuration instance" << endl;
    config = Config::get_instance();

    cout << "Connnecting to DB" << endl;
    try{
        db = DataBase::get_instance(config);
    } catch (runtime_error &ex){
        cout << "Error: " << ex.what() << endl;
    }
    cout << "Connected to DB successfully" << endl;

    bind_points();

    cout << "Listening to clients" << endl << endl;
    server.listen(config.client_host, config.client_port);
}

void HTTP_Server::stop(){
    cout << "Stoping server" << endl;
    server.stop();
    cout << "Server stopped successfully!" << endl;
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
    cout << "Got request of checking user N:" << name << ", P:" << password << endl;

    vector<User> users = db.get_users();
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
        cout << "Found user with same name and password" << endl;
    }
    else{
        cout << "Didn't found user with same name and password" << endl;
    }
        
    json j;
    j["user"] = temp;
    res.set_content(j.dump(), "application/json");
    cout << "Sent response" << endl;
}

void HTTP_Server::get_users(const Request &req, Response &res){
    cout << "Got request of getting users"<< endl;
    json j;
    cout << "Getting users from DB" << endl;
    j["users"] = db.get_users();

    cout << "Sending JSON file with users" << endl;
    res.set_content(j.dump(), "application/json");
}

void HTTP_Server::add_user(const Request &req, Response &res){
    cout << "Got request of adding an user" << endl;
    json j = json::parse(req.body);
    User new_user = j["user"];
    cout << new_user.to_string();
    try{
        db.add_user(new_user);
        res.status = 200;
    } catch(SQLException& ex){
        cout << "DB error: " << ex.what();
        res.set_content(ex.what(), "text/plain");
        res.status = 500;
    }
    cout << "User added to DB" << endl;
}

void HTTP_Server::delete_user(const Request &req, Response &res){
    cout << "Got request of deleting user" << endl;
    json j = json::parse(req.body);
    string way = j["way"];
    if(way == "id"){
        int id = j["id"];
        try{
            db.delete_user(id);
            res.status = 200;
        } catch(SQLException ex){
            cout << "DB error: " << ex.what();
            res.status = 500;
        }
    }
    else{
        string name = j["name"];
        try{
            db.delete_user(name);
            res.status = 200;
        } catch(SQLException& ex){
            cout << "DB error: " << ex.what();
            res.set_content(ex.what(), "text/plain");
            res.status = 500;
        }
    }
    cout << "User deleted from DB" << endl;
}