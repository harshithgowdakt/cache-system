#include "list.h"

List::List(){}

bool List::is_key_exists_in_list(string key){
    map<string, deque<string>>::iterator list_itr;
    for (list_itr = list.begin(); list_itr != list.end(); ++list_itr){
        if (list_itr->first == key){
            return true;
        }
    }
    return false;
}

int List::l_push(string key, string value){
    if(!is_key_exists_in_list(key)){
        list.insert(pair<string, deque<string>>(key, deque<string>()));
        list[key].push_front(value);
        return list[key].size();
    }else{
        list[key].push_front(value);
        return list[key].size();
    }
}

string List::l_pop(string key){
    string value;
    if(is_key_exists_in_list(key) && list[key].size() > 0){
        value = list[key].front();
        list[key].pop_front();
        return value;
    }else{
        return NIL_MSG;
    }
}

int List::l_len(string key){
    int length;
    if(is_key_exists_in_list(key) && list[key].size() > 0){
        return list[key].size();
    }else{
        return 0;
    }
}

string List::l_range(string key, int start_index, int stop_index){
    string result = "";
    if(is_key_exists_in_list(key)){
        if(stop_index < 0 && (list[key].size() + stop_index) <= 0 ){
            return EMPTY_LIST_MSG;
        }else if(stop_index < 0 && (list[key].size() + stop_index) > 0 ){
            stop_index = list[key].size() + stop_index;
        }else if(stop_index > 0 && (stop_index >= list[key].size())){
            stop_index = list[key].size() - 1;
        }

        if(start_index < 0 && (list[key].size() + start_index) <= 0 ){
            return EMPTY_LIST_MSG;
        }else if(start_index < 0 && (list[key].size() + start_index) > 0){
            start_index = (list[key].size() + start_index) - 1;
        }else if(start_index > list[key].size()){
            return EMPTY_LIST_MSG;
        }

        for(int i = start_index; i <= stop_index; i++ ){
            result =  result + list[key].at(i) + "\n";
        }

        if (!result.empty()) {
		    result.pop_back();
            return result;
	    }
        return EMPTY_LIST_MSG;
    }else{
        return EMPTY_LIST_MSG;
    }
}

void List::take_backup(){
    fstream list_backup_file("list_backup");
    boost::archive::text_oarchive list_oarch(list_backup_file);
    list_oarch << list;

    cout <<"Took List data backup successfully \n";
}

void List::restore_backup(){
   std::ifstream list_backup_file("list_backup");
   boost::archive::text_iarchive list_iarch(list_backup_file);
   list_iarch >> list;

   std::cout << "Restored List data successfully \n";
}