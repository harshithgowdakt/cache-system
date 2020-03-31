#include <iostream> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h>
#include <netdb.h> 
#include <string.h>
#include <nlohmann/json.hpp>
#include <algorithm>

#define _XOPEN_SOURCE_EXTENDED 1 
#define PORT 8080

using namespace std;
using json = nlohmann::json;
   
class Client{
private:
    int socket_fd; 
    struct sockaddr_in server_address;
    struct hostent *host;

    string to_upper(string str){
        transform(str.begin(), str.end(), str.begin(), ::toupper);
        return str;
    }
    
    string get_user_data(){
        string command, key, value;
        json json_data;

        cout << "Enter the command::";
        cin >> command;
        command = to_upper(command);
        json_data["command"] = command;
        if(command == "EXIT"){
            close_socket();
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

public:
    Client(){
        server_address.sin_family = AF_INET; 
        server_address.sin_port = htons(PORT);
    }

    void create_socket(){
        socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	    if (socket_fd < 0){ 
            cout<<"Failed to create socket \n";
            exit(0);
        }
        cout<<"Socket created successfully \n";
    }

    void get_host_ip(const char *name){
        host = gethostbyname(name);

        if (host == NULL){
            cout << "Couldn't get the host\n";
        }else if(host -> h_addr_list[0] != NULL){
            cout << "Connecting to "<< host->h_name << "\n";
            server_address.sin_addr = *(struct in_addr *)(host->h_addr_list[0]);
        }else{
            cout << "Couldn't get the IP\n";
        }
    }

    void connet_to_server(){
        if (connect(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) { 
            cout<< "Couldn't connect\n";
            close_socket();
        } 
        cout<<"Connected successfully \n";
    }

    void send_request(){
        while (1){
            string request = get_user_data();
            char buffer[1024];
            memset(buffer,0,sizeof(buffer));
            if ((send(socket_fd, request.c_str(), strlen(request.c_str()), 0)) < 0){
                cout << "Couldn't send the request \n";
                close_socket();
            }
            if ((recv(socket_fd, buffer, 10240, 0)) <= 0){
                cout << "Couldn't receive response \n";
                close_socket();
            }
            cout << "Response::" << buffer << "\n";
        }
    }

    void close_socket(){       
		close(socket_fd);
		exit(0);
	}
};

int main(int argc, char **argv) {
    if (argc < 2){
        cout << "Usage:: " << argv[0] << " hostname" << "\n";
        exit(-1);
    }

    Client client;
    client.create_socket();
    client.get_host_ip(argv[1]);
    client.connet_to_server();
    client.send_request();
    return 0; 
} 
