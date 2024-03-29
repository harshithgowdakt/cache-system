#include <iostream> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>

#define _XOPEN_SOURCE_EXTENDED 1 
#define PORT 8000

using namespace std;
   
class Client{
private:
    int socket_fd; 
    struct sockaddr_in server_address; 
    char message[50]; 
    char buffer[10240] = {0}; 

public:
    Client(){
        server_address.sin_family = AF_INET; 
        server_address.sin_port = htons(PORT);
    }

    void create_socket(){
        try{
            socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	        if (socket_fd < 0){ 
                throw "Failed to create a socket";
            }
        }catch(const char* message){
            cout << message << endl;
            exit(0);
        }

        cout<<"Socket created successfully"<<endl;
    }

    void convert_stringIP_to_numericIP(){
        try{   
            if(inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr)<=0) { 
                throw "Invalid address";
              
            } 
        }catch(const std::exception& e){
            cout<< message << endl;
            close(socket_fd);
            exit(0);
        }
    }

    void connet_to_server(){
        try{
            if (connect(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) { 
                throw "Connection Failed";
            } 
        }catch(const char* message){
            cout<< message << endl;
            close(socket_fd);
            exit(0); 
        }

        cout<<"Connected to the server"<<endl;
    }

    void send_and_recieve_data(){
	    while(1){
            memset(buffer,0,sizeof(buffer));
	    	cout<<"Enter the data :";
		    cin.getline(message,50);

            try{
                if((send(socket_fd , message, strlen(message), 0)) < 0){
                    throw "Error while sending the data";
                }

                if((recv(socket_fd , buffer, 10240,0)) < 0){
                    throw "Error while receiving the data";
                }
            }catch(const char* message){
               cout<< message << endl;
               close(socket_fd);
               exit(0);
            }

	        cout << "Data from the server::" << endl;
            cout << buffer << endl;
        }	
    }
};


int main() { 

    Client client;

    client.create_socket();
    client.convert_stringIP_to_numericIP();
    client.connet_to_server();

    cout<<"Sending data to server"<<endl;
    client.send_and_recieve_data();

    return 0; 
} 
