#include <iostream> 
#include <string.h>
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
    string command, key, value;
    json json_data;

    cout << "Enter the command::";
    cin >> command;
    command = to_upper(command);
    json_data["command"] = command;
    if(command == "EXIT"){
        close_connection();
    }else if (command == "GET" || command == "DELETE"){
        cin >> key;
        json_data["key"] = key;
    } else if (command == "SET" || command == "PUT" ){
        cin >> key;
        cin >> value;
        json_data["key"] = key;
        json_data["value"] = value;
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
        cout << "Response::" << buffer << "\n";
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
