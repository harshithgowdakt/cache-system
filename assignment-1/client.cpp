#include <iostream> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include <nlohmann/json.hpp>
#include <algorithm>

#define _XOPEN_SOURCE_EXTENDED 1 
#define PORT 8080
#define IP_ADDRESS "127.0.0.1"

using namespace std;
using json = nlohmann::json;
   
class Client{
private:
    int socket_fd; 
    struct sockaddr_in server_address;

    string to_upper(string str){
        transform(str.begin(), str.end(), str.begin(), ::toupper);
        return str;
    }
    
    string get_string_from_stdin(const char* type){
        string input;
        cout << "Enter the " << type <<"::";
        cin >> input;
        return input;
    }

    string get_user_data(){
        string command, key, value;
        json json_data;

        command = get_string_from_stdin("command");
        command = to_upper(command);
        json_data["command"] = command;
        if(command == "EXIT"){
            close_socket();
        }else if (command == "GET" || command == "DELETE"){
            key = get_string_from_stdin("key");
            json_data["key"] = key;
        } else if (command == "SET" || command == "PUT" ){
            key = get_string_from_stdin("key");
            value = get_string_from_stdin("value");
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

    void convert_stringIP_to_numericIP(){
        if(inet_pton(AF_INET, IP_ADDRESS, &server_address.sin_addr)<=0) { 
            cout<<"Invalid address \n";
            close_socket();
        }
    }

    void connet_to_server(){
        if (connect(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) { 
            cout<< "Couldn't connect server \n";
            close_socket();
        } 
        cout<<"Connected to the server \n";
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

int main() { 
    Client client;

    client.create_socket();
    client.convert_stringIP_to_numericIP();
    client.connet_to_server();
    client.send_request();
    return 0; 
} 
