#include <iostream>
#include <fstream> 
#include <map>
#include <vector>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#define EMPTY_LIST_MSG "empty list or set"
#define NIL_MSG "nil"

using namespace std;

typedef vector<pair<string,string>> vct_of_pair;

class Hashmap {
    private:
        map<string, vct_of_pair> hash_map;

        bool is_field_exists(string key, string field);
        bool is_key_exists_in_hmap(string key);
    
    public:
        Hashmap();
        int h_set(string key, string field, string value);
        string h_get(string key, string field);
        string h_get_all(string key); 
        void take_backup();
        void restore_backup();
};