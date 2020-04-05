#include <iostream>
#include <map>
#include <nlohmann/json.hpp>

using namespace std;

class DB {
    private:
        map<string, string> user_data;
        bool is_key_exists(string key);

    public:
        DB();
        string save_data(string key, string value);
        string get_data(string key);
        string update_data(string key, string value);
        string delete_data(string key);
};