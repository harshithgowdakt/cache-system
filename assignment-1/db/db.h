#include <iostream>
#include <map>
#include <tuple>
#include <vector>
#include <time.h>
#include <nlohmann/json.hpp>

using namespace std;

typedef vector<pair<string,string>> vct_of_pair;

class DB {
    private:
        map<string, string> data_base;
        map<string, string>::iterator db_itr;
        map<string, vct_of_pair> hash_map;
        map<string, vct_of_pair>::iterator hmap_itr;
        vct_of_pair::iterator vct_itr;
        using callback_func = string (DB::*)(string);
        bool is_key_exists(string key);
        bool is_field_exists(string key, string field);
        bool is_key_exists_in_hmap(string key);
        void set_timer(int ttl, string key);
        void setTimeout(callback_func callback, int ttl, string key);
    
    public:
        DB();
        string save_data(string key, string value,  int ttl);
        string get_data(string key);
        string delete_data(string key);
        string h_set(string key, string field, string value);
        string h_get(string key, string field);
        string h_get_all(string key);
        void join_db_threads();  
};