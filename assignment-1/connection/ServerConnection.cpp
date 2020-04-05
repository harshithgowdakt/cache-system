#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include "ServerConnection.h"

#define _XOPEN_SOURCE_EXTENDED 1
#define PORT 8080

using namespace std;

ServerConnection::ServerConnection(){
	address_lenth = sizeof(socket_address);
	socket_address.sin_family = AF_INET;
	socket_address.sin_addr.s_addr = INADDR_ANY;
	socket_address.sin_port = htons(PORT);
}

void ServerConnection::create_socket(){
	socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(socket_fd < 0){
		cout<<"Couldn't create socket \n";
		exit(0);
	}
	cout<<"Socket created successfully\n";
}

void  ServerConnection::bind_address(){
	if(bind(socket_fd, (struct sockaddr *)&socket_address, address_lenth)<0){
		cout<< "Couldn't bind the IP address \n";
		close_socket();
	}
	cout<<"Address binding completed \n";	
}

void ServerConnection::listen_to_request(){
	if ((listen(socket_fd,0))<0){
		cout<<"Couldn't listen to incoming request \n";
		close_socket();
	}	
	cout<<"Listening to incoming request....... \n";	
}

void ServerConnection::accept_connection(){
	socket_fd_client = accept(socket_fd, (struct sockaddr *)&socket_address,(socklen_t*)&address_lenth);
	if (socket_fd_client < 0) { 
		cout << "Couldn't accept the connection \n"; 
		close_socket();
	} 
	cout<<"Connection accepted \n";
}

char * ServerConnection::recieve_req(){
	memset(buffer,0,sizeof(buffer));
	if((recv(socket_fd_client, buffer, 1024, 0)) <= 0){
		close(socket_fd_client);
	}
	return buffer;
}

int ServerConnection::send_res(const char *data){
	if ((send(socket_fd_client, data, strlen(data), 0)) < 0){
		close(socket_fd_client);
		return -1;
	}
}

void ServerConnection::close_socket(){       
	close(socket_fd);
	exit(0);
}