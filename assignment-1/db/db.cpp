#include <iostream>
#include <map>
#include <tuple>
#include <mutex>
#include <thread>
#include <nlohmann/json.hpp>
#include "db.h"

#define MAX_THREADS 100
using namespace std;
int thread_count = 0;
thread threads[MAX_THREADS];
mutex mute;

DB::DB(){}

bool DB::is_key_exists(string key){
    for (i = data_base.begin(); i != data_base.end(); ++i){
        if (i->first == key){
            return true;
        }
    }
    return false;
}

void DB::setTimeout(callback_func callback, int ttl, string key){
    std::this_thread::sleep_for (std::chrono::seconds(ttl));
    (this->*callback)(key);
}

void DB::join_db_threads(){
    for(int i = 1; i <= thread_count; i++){
        if(threads[i].joinable()) threads[i].join();
    }
}

void DB::set_timer(int ttl, string key){
    if(thread_count<MAX_THREADS){
        threads[++thread_count] = thread(&DB::setTimeout, this, &DB::delete_data, ttl, key);
    }
}

string DB::save_data(string key, string value, int ttl){
    if(!is_key_exists(key)){
        data_base.insert(pair<string, string>(key, value));
        if(ttl >= 0){
            set_timer(ttl,key);
        }
        return "OK";
    }else{
        return "key already exists";
    }
}

string DB::get_data(string key){
    lock_guard<mutex> guard(mute);
    for (i = data_base.begin(); i != data_base.end(); ++i){
        if (i->first == key){
           return i->second;
        }
    }
    return "Key doesn't exists";
}

string DB::update_data(string key, string value){
   lock_guard<mutex> guard(mute);
    if(is_key_exists(key)){
        data_base.at(key) = value;
        return "OK";
    }else{
        return "key does not exists to update, first set the key";
    }
}

string DB::delete_data(string key){
   lock_guard<mutex> guard(mute);
    if(is_key_exists(key)){
        data_base.erase(key);
        return "OK";
    }else{
        return "key does not exists to delete";
    }
}