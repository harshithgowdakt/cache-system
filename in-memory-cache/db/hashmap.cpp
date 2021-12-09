#include "hashmap.h"

Hashmap::Hashmap(){}

bool Hashmap::is_key_exists_in_hmap(string key){
    map<string, vct_of_pair>::iterator hmap_itr;
    for (hmap_itr = hash_map.begin(); hmap_itr != hash_map.end(); ++hmap_itr){
        if (hmap_itr->first == key){
            return true;
        }
    }
    return false;
}


bool Hashmap::is_field_exists(string key, string field){
    for(int i=0; i< hash_map[key].size(); i++ ){
        if(hash_map[key][i].first == field){
            return true;
        }
    }
    return false;
}


int Hashmap::h_set(string key, string field, string value){
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

string Hashmap::h_get(string key, string field){
    if(is_key_exists_in_hmap(key)){
        for(int i=0; i < hash_map[key].size(); i++ ){
            if(hash_map[key][i].first == field){
                return hash_map[key][i].second;
            }
        }
    }
    return NIL_MSG;
}

string Hashmap::h_get_all(string key){
    string result = "";
    if(is_key_exists_in_hmap(key)){
        for(int i = 0; i < hash_map[key].size(); i++ ){
            result =  result + hash_map[key][i].first + "\n" + hash_map[key][i].second + "\n";
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

void Hashmap::take_backup(){
    fstream hashmap_backup_file("hashmap_backup");
    boost::archive::text_oarchive hashmap_oarch(hashmap_backup_file);
    hashmap_oarch << hash_map;
    cout <<"Took hashmap data backup successfully \n";
}

void Hashmap::restore_backup(){
   ifstream hashmap_backup_file("hashmap_backup");
   boost::archive::text_iarchive hashmap_iarch(hashmap_backup_file);
   hashmap_iarch >> hash_map;

   cout<<"Restored hashmap data successfully \n";
}