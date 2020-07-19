#include <iostream>
#include <fstream> 
#include <map>
#include <deque>
#include <boost/serialization/map.hpp>
#include <boost/serialization/deque.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#define EMPTY_LIST_MSG "empty list or set"
#define NIL_MSG "nil"

using namespace std;

class List {
    private:
        map<string, deque<string>> list;

        bool is_key_exists_in_list(string key);
    
    public:
        List();
        int l_push(string key, string value);
        string l_pop(string key);
        int l_len(string key);
        string l_range(string key, int start, int stop);
        void take_backup();
        void restore_backup();
};