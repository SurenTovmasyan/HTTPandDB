#include "../include/HTTP_Client.h"

void HTTP_Client::start(){
    cout << "Getting configuration instance" << endl;
    config = Config::get_instance();
    cout << "Starting client" << endl;

    domain = "http://" + config.host + ":" + to_string(config.port);
    cout << "Creating domain for connection" << endl;
    client = make_unique<Client>(domain);
}

void HTTP_Client::stop(){
    cout << "Stopping client's work" << endl;
    client->stop();
    cout << "Client stopped successfully!" << endl;
}

void HTTP_Client::authorization(){
    string name, password;
    cout << "Please, insert your accaunt's name and password." << endl;
    cout << "    Name: ";
    cin >> name;
    cout << "Password: ";
    cin >> password;

    cout << "Sending POST request to " << config.host << ":" << config.port << endl;
    Params params;
    params.emplace("name", name);
    params.emplace("password", password);
    Result res = client->Post("/check_user", params);

    if(res && res->status == 200){
        json j;
        j = json::parse(res->body);
        working_user = j["user"];
        if(working_user.get_privilege() == "NONE"){
            cout << "Incorrect name or password." << endl;
            cout << "Would you like to create accaunt? (y/n): ";
            char accepting;
            cin >> accepting;
            if(tolower(accepting) == 'y'){
                create_user();
                authorization();
                return;
            }
            else{
                authorization();
                return;
            }
        }
    }
    else{
        cout << "Request failed!" << endl;
        exit(1);
    }
}

void HTTP_Client::create_user(){
    cout << "To create an accaunt you need to insert some information." << endl;
    User new_user;
    string name, password, c_password, email;

    do{
        cout << "                Name of accaunt: ";
        cin >> name;
        try{
            new_user.set_name(name);
        } catch(invalid_argument* ex){
                cout << "Error: " << ex->what() << endl;
        } 
    } while(new_user.get_name() == "");

    do{
        char accepting;
        cout << "Do you want to add email? (y/n): ";
        cin >> accepting;
        if(tolower(accepting) == 'y'){
            try{
                cout << "                           Mail: ";
                cin >> email;
                new_user.set_email(email);
            } catch(invalid_argument* ex){
                cout << "Error: " << ex->what() << endl;
            } 
        }
        else {
            new_user.set_email(name + "@example.com");
            break;
        }
    } while(new_user.get_email() == "");

    do{
        cout << "                       Password: ";
        cin >> password;

        do{
            cout << "               Confirm password: ";
            cin >> c_password;
            if(c_password != password)
                cout << "Confirmation password and actual password are not equal!" << endl;
        } while (c_password != password);

        try{
            new_user.set_password(password);
        } catch(invalid_argument* ex){
            cout << "Error: " << ex->what() << endl;
        } 
    } while(new_user.get_password() == "");

    cout << "Sending PUT request to " << config.host << ':' << config.port << endl;
    json j;
    j["user"] = new_user;
    auto res = client->Put("/add_user", j.dump(), "application/json");
    if(res && res->status == 200){
        cout << "User added successfully!" << endl << endl;
    }
    else if(res && res->status == 400){
        cout << res->body << endl << endl;
        create_user();
    }
    else cout << "Something has gone wrong in server..." << endl << endl;
}

void HTTP_Client::get_users(){
    cout << "Sending GET request to " << config.host << ":" << config.port << endl;
    auto res = client->Get("/get_users");
    json j = json::parse(res->body);
    vector<User> users = j["users"];
    for (User user : users){
        cout << '\t' << user.to_string() << endl;
    }
    cout << "Count of users: " << users.size() << endl;
}

void HTTP_Client::to_main_page(){
    if(working_user.get_privilege() == "ADMIN"){
        cout << "Welcome, " + working_user.get_name() << endl;

        while(true){
            cout << endl;
            cout << "    1.Get all users" << endl;
            cout << "    2.Delete user" << endl;
            cout << "    3.Add user" << endl;
            cout << "    4.Show acc. information" << endl;
            cout << "    (Some other integer).Exit" << endl;
            cout << "Insert operation number to work with users: " ;
        
            short acception;
            cin >> acception;
            switch (acception)
            {
            case 1: 
                get_users();
                break;

            case 2: 
                delete_user();
                break;

            case 3: 
                create_user();
                break;

            case 4: 
                show_user_info();
                break;

            default:
                return;
            }   
        }
    }
    else {
        cout << "Welcome, " + working_user.get_name() << endl;

        while(true){
            cout << endl;
            cout << "    1.Show acc. information" << endl;
            cout << "    (Some other integer).Exit" << endl;
            cout << "Insert operation number to work with users: " ;
        
            short acception;
            cin >> acception;
            switch (acception)
            {
            case 1: 
                show_user_info();
                break;

            default:
                return;
            }   
        }
    }
}

void HTTP_Client::show_user_info(){
    cout << "The authorizied user information is below:" << endl << endl;
    cout << "    " << working_user.to_string() << endl;
}

void HTTP_Client::delete_user(){
    short acception;
    cout << "\t1.By ID" << endl;
    cout << "\t2.By name" << endl;
    cout << "\t(Some othet character).Back" << endl;
    cout << "Insert how do you want ot delete user: ";
    cin >> acception;

    json j;
    switch (acception)
    {
    case 1:
        {
            int id;
            cout << "Insert id of the user you want to delete: ";
            cin >> id;

            j["way"] = "id";
            j["id"] = id;
        }        
        break;

    case 2:
        {
            string name;
            cout << "Insert name of the user you want to delete: ";
            cin >> name;

            j["way"] = "name";
            j["name"] = name;
        }        
        break;
    
    default:
        return;
    }
    auto res = client->Delete("/delete_user", j.dump(), "application/json");
    if(res && res->status == 200)
        cout << "User deleted successfully!" << endl << endl;
    else 
        cout << "Something went wrong..." << endl;
}