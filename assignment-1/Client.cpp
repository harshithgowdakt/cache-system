#include <iostream> 
#include <string.h>
#include <time.h>
#include <nlohmann/json.hpp>
#include <algorithm>
#include "connection/ClientConnection.h"

using namespace std;
using json = nlohmann::json;
   
class Client{
private:
    ClientConnection clientConnection;

    string to_upper(string str);
    string get_user_data();

public:
    void connect(const char *host);
    void send_request();
    void close_connection();
};

string Client::to_upper(string str){
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

string Client::get_user_data(){
    string command, key, value, field;
    time_t ttl;
    int start_index, stop_index;
    json json_data;

    cout << "Enter the command::";
    cin >> command;
    command = to_upper(command);
    json_data["command"] = command;
    if(command == "EXIT"){
        close_connection();
    }else if (command == "GET" || command == "DEL" || 
              command == "HGETALL" || command == "LPOP" ||
              command == "LLEN"){
        cin >> key;
        json_data["key"] = key;
    } else if (command == "SET"){
        cin >> key;
        cin >> value;
        cin >> ttl;
        json_data["key"] = key;
        json_data["value"] = value;
        json_data["ttl"] = ttl;
    } else if(command == "HSET"){
        cin >> key;
        cin >> field;
        cin >> value;
        json_data["key"] = key;
        json_data["field"] = field;
        json_data["value"] = value;
    }else if(command == "HGET"){
        cin >> key;
        cin >> field;
        json_data["key"] = key;
        json_data["field"] = field;
    }else if(command == "LPUSH"){
        cin >> key;
        cin >> value;
        json_data["key"] = key;
        json_data["value"] = value;
    }else if(command == "LRANGE"){
        cin >> key;
        cin >> start_index;
        cin >> stop_index;
        json_data["key"] = key;
        json_data["start_index"] = start_index;
        json_data["stop_index"] = stop_index;
    }else{
        cout << "ERR unknown command \n";
    }
    return json_data.dump();
}

void Client::close_connection(){
    clientConnection.close_socket();
}

void Client::connect(const char *host){
    clientConnection.create_socket();
    clientConnection.get_host_ip(host);
    clientConnection.connet_to_server();
}

void Client::send_request(){
    char *buffer;
    while (1){
        string request = get_user_data();
        clientConnection.send_req(request.c_str());
        buffer = clientConnection.recieve_res();
        cout << buffer << "\n";
    }
}

int main(int argc, char **argv) {
    if (argc < 2){
        cout << "Usage:: " << argv[0] << " hostname" << "\n";
        exit(-1);
    }
    Client client;
    client.connect(argv[1]);
    client.send_request();
    return 0; 
} 
