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
    for (db_itr = data_base.begin(); db_itr != data_base.end(); ++db_itr){
        if (db_itr->first == key){
            return true;
        }
    }
    return false;
}

bool DB::is_key_exists_in_hmap(string key){
    for (hmap_itr = hash_map.begin(); hmap_itr != hash_map.end(); ++hmap_itr){
        if (hmap_itr->first == key){
            return true;
        }
    }
    return false;
}

bool DB::is_key_exists_in_list(string key){
    for (list_itr = list.begin(); list_itr != list.end(); ++list_itr){
        if (list_itr->first == key){
            return true;
        }
    }
    return false;
}

bool DB::is_field_exists(string key, string field){
    for(int i=0; i< hash_map[key].size(); i++ ){
        if(hash_map[key][i].first == field){
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
    }else{
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

string DB::get_data(string key){
    lock_guard<mutex> guard(mute);
    for (db_itr = data_base.begin(); db_itr != data_base.end(); ++db_itr){
        if (db_itr->first == key){
           return db_itr->second;
        }
    }
    return NIL_MSG;
}

int DB::delete_data(string key){
   lock_guard<mutex> guard(mute);
    if(is_key_exists(key)){
        data_base.erase(key);
        return 1;
    }else{
        return 0;
    }
}

int DB::h_set(string key, string field, string value){
    if(!is_key_exists_in_hmap(key)){
        hash_map.insert(pair<string, vct_of_pair>(key, vct_of_pair()));
        hash_map[key].push_back(make_pair(field, value));
        return 1;
    }else if(!is_field_exists(key, field)){
        hash_map[key].push_back(make_pair(field, value));
        return 1;
    }else{
        for(int i=0 ; i < hash_map[key].size(); i++){
            if(hash_map[key][i].first == field){
                hash_map[key][i].second = value;
                return 0;
            }
        }
    }
}

string DB::h_get(string key, string field){
    if(is_key_exists_in_hmap(key)){
        for(int i=0; i < hash_map[key].size(); i++ ){
            if(hash_map[key][i].first == field){
                return hash_map[key][i].second;
            }
        }
    }
    return NIL_MSG;
}

string DB::h_get_all(string key){
    string result = "";
    if(is_key_exists_in_hmap(key)){
        for(int i = 0; i < hash_map[key].size(); i++ ){
            result =  result + hash_map[key][i].first + "\n" + hash_map[key][i].second + "\n";
        }
        if (!result.empty()) {
		    result.pop_back();
	    }
    }else{
        return EMPTY_LIST_MSG;
    }
    return result;
}

int DB::l_push(string key, string value){
    if(!is_key_exists_in_list(key)){
        list.insert(pair<string, deque<string>>(key, deque<string>()));
        list[key].push_front(value);
        return list[key].size();
    }else{
        list[key].push_front(value);
        return list[key].size();
    }
}

string DB::l_pop(string key){
    string value;
    if(is_key_exists_in_list(key) && list[key].size() > 0){
        value = list[key].front();
        list[key].pop_front();
    }else{
        return NIL_MSG;
    }
    return value;
}

int DB::l_len(string key){
    int length;
    if(is_key_exists_in_list(key) && list[key].size() > 0){
        length = list[key].size();
    }else{
        length = 0;
    }
    return length;
}

string DB::l_range(string key, int start_index, int stop_index){
    string result = "";
    if(is_key_exists_in_list(key)){
        if(stop_index < 0 && (list[key].size() + stop_index) <= 0 ){
            return EMPTY_LIST_MSG;
        }else if(stop_index < 0 && (list[key].size() + stop_index) > 0 ){
            stop_index = list[key].size() + stop_index;
        }else if(stop_index > 0 && (stop_index > list[key].size())){
            stop_index = list[key].size();
        }

        if(start_index < 0 && (list[key].size() + start_index) <= 0 ){
            return EMPTY_LIST_MSG;
        }else if(start_index < 0 && (list[key].size() + start_index) > 0){
            start_index = list[key].size() + start_index;
        }else if(start_index > list[key].size()){
            return EMPTY_LIST_MSG;
        }

        for(int i = start_index; i < stop_index; i++ ){
            result =  result + list[key].at(i) + "\n";
        }

        if (!result.empty()) {
		    result.pop_back();
	    }
    }else{
        return EMPTY_LIST_MSG;
    }
    return result;
}