#include "../include/User.h"

User::User(string privilege){
    this->privilege = privilege;
}

User::User(int id, string name, string password, string email, string privilege){
    set_email(email);
    set_name(name);
    set_password(password);
    this->id = id;
    this->privilege = privilege;
}

int User::get_id() const{
    return id;
}

string User::get_name() const{
    return name;
}

void User::set_name(string name){
    if(name.size() <= 0 || name.size()>100)
        throw new invalid_argument("Invalid name argument");
    this->name = name;
}

string User::get_password() const{
    return password;
}

void User::set_password(string password){
    if(password.size() < 4)
        throw new invalid_argument("Password is too short");
    else if(password.size() > 100)
        throw new invalid_argument("Password is too long");

    this->password = password;
}

string User::get_email() const{
    return email;
}

void User::set_email(string email){
    if(email.size() < 3)
        throw new invalid_argument("Email is too short");
    else if(email.find("@") == string::npos)
        throw new invalid_argument("Email don't look like an email(no '@')");

    this->email = email;
}

string User::get_privilege() const{
    return privilege;
}

string User::to_string(){
    return "User N" + __cxx11::to_string(id) + " - N: " + name + ", P: " + password + ", E: " + email + ", Priv: " + privilege + ".";
}