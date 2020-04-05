#include <iostream>
#include <map>
#include <tuple>
#include <time.h>
#include <nlohmann/json.hpp>

using namespace std;

class DB {
    private:
        map<string, string> data_base;
        map<string, string>::iterator i;
        using callback_func = string (DB::*)(string);
        bool is_key_exists(string key);
        void set_timer(int ttl, string key);
        void setTimeout(callback_func callback, int ttl, string key);

    public:
        DB();
        string save_data(string key, string value,  int ttl);
        string get_data(string key);
        string update_data(string key, string value);
        string delete_data(string key);
        void join_db_threads();  
};