#include <tuple>
#include <mutex>
#include <thread>
#include "map.h"

#define MAX_THREADS 1000
using namespace std;
int thread_count = 0;
thread threads[MAX_THREADS];
mutex mute;

Map::Map(){}

bool Map::is_key_exists(string key){
    map<string, string>::iterator db_itr;
    for (db_itr = data_base.begin(); db_itr != data_base.end(); ++db_itr){
        if (db_itr->first == key){
            return true;
        }
    }
    return false;
}

void Map::setTimeout(callback_func callback, int ttl, string key){
    std::this_thread::sleep_for (std::chrono::seconds(ttl));
    (this->*callback)(key);
}

void Map::join_db_threads(){
    for(int i = 1; i <= thread_count; i++){
        if(threads[i].joinable()) threads[i].join();
    }
}

void Map::set_timer(int ttl, string key){
    if(thread_count<MAX_THREADS){
        threads[++thread_count] = thread(&Map::setTimeout, this, &Map::delete_data, ttl, key);
    }
}

string Map::save_data(string key, string value, int ttl){
    if(!is_key_exists(key)){
        data_base.insert(pair<string, string>(key, value));
    }else{
        map<string, string>::iterator db_itr;
        db_itr = data_base.find(key);
        if (db_itr != data_base.end()){
            db_itr->second = value;
        }   
    }
    if(ttl >= 0){
        set_timer(ttl,key);
    }
    return "OK";
}

string Map::get_data(string key){
    lock_guard<mutex> guard(mute);
    map<string, string>::iterator db_itr;
    for (db_itr = data_base.begin(); db_itr != data_base.end(); ++db_itr){
        if (db_itr->first == key){
           return db_itr->second;
        }
    }
    return NIL_MSG;
}

int Map::delete_data(string key){
   lock_guard<mutex> guard(mute);
    if(is_key_exists(key)){
        data_base.erase(key);
        return 1;
    }else{
        return 0;
    }
}

void Map::take_backup(){
    fstream map_backup_file("map_backup");
    boost::archive::text_oarchive map_oarch(map_backup_file);
    map_oarch << data_base;

    cout<<"Took map data backup successfully \n";
}

void Map::restore_backup(){
   ifstream map_backup_file("map_backup");
   boost::archive::text_iarchive map_iarch(map_backup_file);
   map_iarch >> data_base;

   cout<<"Restored map data successfully \n";
}