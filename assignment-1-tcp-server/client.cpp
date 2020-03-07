#include <iostream> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include <nlohmann/json.hpp>

#define _XOPEN_SOURCE_EXTENDED 1 
#define PORT 8080

using namespace std;
using json = nlohmann::json;
   
class Client{
private:
    int socket_fd; 
    struct sockaddr_in server_address; 
    char data[1024] = R"({"key":"id"})"; 

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
        if(inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr)<=0) { 
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

    void send_and_recieve_data(){
        char buffer[1024];
        cout<<"Sending request to server \n";
        if ((send(socket_fd, data, strlen(data), 0)) > 0){
            if ((recv(socket_fd, buffer, 10240, 0)) > 0){
                cout << "Response:: " << buffer << "\n";
            }else{
                cout << "Couldn't receive response \n";
                close_socket();
            }
        }else{
            cout << "Couldn't send the request \n";
            close_socket();
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
    client.send_and_recieve_data();

    return 0; 
} 
