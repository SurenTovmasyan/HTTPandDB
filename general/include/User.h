#ifndef USER
#define USER

#include <iostream>
#include <cstring>
#include <nlohmann/json.hpp> 
#include <fstream>
#include <vector>

using namespace std;
using json = nlohmann::json;

class User{
public:
    User() = default;
    User(string privilege);
    User(int id, string name, string password, string email, string privilege);

    int get_id() const;

    string get_name() const;
    void set_name(string name);

    string get_password() const;
    void set_password(string password);

    string get_email() const;
    void set_email(string email);

    string get_privilege() const;

    string to_string();

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(User, id, name, password, email, privilege);

private:
    string name, password, email;
    string privilege;
    int id;
};

#endif //USER