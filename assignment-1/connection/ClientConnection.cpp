#include <iostream> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h>
#include <netdb.h> 
#include <string.h>
#include "ClientConnection.h"

#define _XOPEN_SOURCE_EXTENDED 1 
#define PORT 8080

using namespace std;

 ClientConnection::ClientConnection(){
    server_address.sin_family = AF_INET; 
    server_address.sin_port = htons(PORT);
}

void ClientConnection::create_socket(){
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0){ 
        cout<<"Failed to create socket \n";
        exit(0);
    }
    cout<<"Socket created successfully \n";
}

void ClientConnection::get_host_ip(const char *name){
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

void ClientConnection::connet_to_server(){
    if (connect(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) { 
        cout<< "Couldn't connect\n";
        close_socket();
    } 
    cout<<"Connected successfully \n";
}

int ClientConnection::send_req(const char *data){
	if ((send(socket_fd, data, strlen(data), 0)) < 0){
        cout<< "Error while sending response \n";
		close_socket();
	}
}

char * ClientConnection::recieve_res(){
	memset(buffer,0,sizeof(buffer));
	if((recv(socket_fd, buffer, 1024, 0)) <= 0){
        cout << "Couldn't receive response \n";
		close_socket();
	}
	return buffer;
}

void ClientConnection::close_socket(){       
    close(socket_fd);
    exit(0);
}