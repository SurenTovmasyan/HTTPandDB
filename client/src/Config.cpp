#include "../include/Config.h"

Config Config::single;
bool Config::is_firts_time = true;

Config& Config::get_instance(){
    if(is_firts_time){
        is_firts_time = false;
        ifstream config_file("../config.json");
        json config_json;
        config_file >> config_json;
        single = config_json.get<Config>();
    }
    return single;
}