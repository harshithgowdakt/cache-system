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
	    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	    if (socket_fd < 0){ 
            cout<<"Failed to create a socket"<<endl;
            exit(0);
        }
        else{
            cout<<"Socket created successfully"<<endl;
        }
    }

    void convert_stringIP_to_numericIP(){
        if(inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr)<=0) { 
            cout<<"Invalid address"<<endl;
            exit(0);
        } 
    }

    void connet_to_server(){
        if (connect(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) { 
            cout<<"Connection Failed"<<endl; 
            exit(0); 
        } 
        else{
            cout<<"Connected to the server"<<endl;
        }
    }

    void send_and_recieve_data(){
	    while(1){
	    	cout<<"Enter the data :";
		    cin.getline(message,50);
		    send(socket_fd , message, strlen(message), 0);
    	    recv(socket_fd , buffer, 10240,0); 
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
