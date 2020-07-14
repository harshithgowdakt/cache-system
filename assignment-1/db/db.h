#include <iostream>
#include <map>
#include <tuple>
#include <vector>
#include <deque>
#include <time.h>
#include <nlohmann/json.hpp>
#include <fstream> 
#include <boost/serialization/map.hpp>
#include <boost/serialization/deque.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#define EMPTY_LIST_MSG "empty list or set"
#define NIL_MSG "nil"

using namespace std;

typedef vector<pair<string,string>> vct_of_pair;

class DB {
    private:
        map<string, string> data_base;
        map<string, string>::iterator db_itr;
        map<string, vct_of_pair> hash_map;
        map<string, vct_of_pair>::iterator hmap_itr;
        vct_of_pair::iterator vct_itr;
        map<string, deque<string>> list;
        map<string, deque<string>>::iterator list_itr;
        using callback_func = string (DB::*)(string);
        bool is_key_exists(string key);
        bool is_field_exists(string key, string field);
        bool is_key_exists_in_hmap(string key);
        bool is_key_exists_in_list(string key);
        void set_timer(int ttl, string key);
        void setTimeout(callback_func callback, int ttl, string key);
    
    public:
        DB();
        string save_data(string key, string value,  int ttl);
        string get_data(string key);
        int delete_data(string key);
        int h_set(string key, string field, string value);
        string h_get(string key, string field);
        string h_get_all(string key);
        int l_push(string key, string value);
        string l_pop(string key);
        int l_len(string key);
        string l_range(string key, int start, int stop);
        void join_db_threads(); 
        void take_backup();
        void restore_backup();
};