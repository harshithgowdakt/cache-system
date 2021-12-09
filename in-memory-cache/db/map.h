#include <iostream>
#include <map>
#include <time.h>
#include <fstream> 
#include <boost/serialization/map.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#define EMPTY_LIST_MSG "empty list or set"
#define NIL_MSG "nil"

using namespace std;

class Map {
    private:
        map<string, string> data_base;

        using callback_func = string (Map::*)(string);
        bool is_key_exists(string key);
        bool is_field_exists(string key, string field);
        void set_timer(int ttl, string key);
        void setTimeout(callback_func callback, int ttl, string key);
    
    public:
        Map();
        string save_data(string key, string value,  int ttl);
        string get_data(string key);
        int delete_data(string key);
        void join_db_threads(); 
        void take_backup();
        void restore_backup();
};