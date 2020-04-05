#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include "db.h"

using namespace std;

DB::DB(){}

bool DB::is_key_exists(string key){
    map<string, string>::iterator i;
    for (i = user_data.begin(); i != user_data.end(); ++i){
        if (i->first == key){
            return true;
        }
    }
    return false;
}

string DB::save_data(string key, string value){
    if(!is_key_exists(key)){
        user_data.insert(pair<string, string>(key, value));
        return "OK";
    }else{
        return "key already exists";
    }
}

string DB::get_data(string key){
    map<string, string>::iterator i;
    for (i = user_data.begin(); i != user_data.end(); ++i){
        if (i->first == key){
            return i->second;
        }
    }
    return "key does not exists";
}

string DB::update_data(string key, string value){
    if(is_key_exists(key)){
        user_data.at(key) = value;
        return "OK";
    }else{
        return "key does not exists to update, first set the key";
    }
}

string DB::delete_data(string key){
    if(is_key_exists(key)){
        user_data.erase(key);
        return "OK";
    }else{
        return "key does not exists to delete";
    }
}