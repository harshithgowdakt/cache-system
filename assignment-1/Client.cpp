#include <iostream> 
#include <string.h>
#include <time.h>
#include <algorithm>
#include "connection/ClientConnection.h"

using namespace std;
   
class Client{
private:
    ClientConnection clientConnection;

    string get_user_data();

public:
    void connect(const char *host);
    void send_request();
    void close_connection();
};

string Client::get_user_data(){
    string command;
    cout << "Enter the command::";
    getline(cin, command);
    return command;
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
