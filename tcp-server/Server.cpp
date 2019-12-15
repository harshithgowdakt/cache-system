#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h> 
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <fstream>

#define _XOPEN_SOURCE_EXTENDED 1
#define PORT 8000

using namespace std;

class Server{

private:
	int socket_fd,socket_fd_client;
	struct sockaddr_in socket_address;
	int address_lenth = sizeof(socket_address);
	char buffer[1024];
	char *message = "Hello Cient, I recived your data";
	char data[10240];

public:
	Server(){
		socket_address.sin_family = AF_INET; 
   		socket_address.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    	socket_address.sin_port = htons( PORT ); 
	}
	void create_socket(){
		
		try{
			socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

			if(socket_fd<0){
				throw "Failed to create socket";
			}
		}catch(const std::exception& e){
			cout<< message <<endl;
			exit(0);
		}

		cout<<"Socket is created"<<endl;
	}

	void bind_address(){

		try{
			if(bind(socket_fd, (struct sockaddr *)&socket_address, address_lenth)<0){
				throw "Failed to bind the address";
			}
		}catch(const char* message){
			cout<< message <<endl;
			close(socket_fd);
			exit(0);
		}
			
		cout<<"Address binding completed"<<endl;	
	}

	void listen_to_request(){
		try{
			if ((listen(socket_fd,0))<0){
   			throw "Failed to listen";
			}
		
		}catch(const char* message){
			cout << message << endl;
			exit(0);
		}

		cout<<"Listening to incoming connection......."<<endl;	
	}

	void accept_connection(){
		
    	try{
			socket_fd_client = accept(socket_fd, (struct sockaddr *)&socket_address,(socklen_t*)&address_lenth);
			if (socket_fd_client < 0) { 
    		throw "Failed to accept the connection";  
		}
		}catch(const char* message){
			cout << message << endl;
			exit(0);
		}
		cout<<"Connection accepted"<<endl;
	}
	
	void get_process_statics(){
    	int result;
    	ifstream infile; 
    
    	result = system("ps -o pid,user,%cpu,%mem ax | sort -b -k3 -r > process_statics.txt");
		try {
			if(result >= 0){
	    		infile.open("process_statics.txt");

        		for (int i = 0; i <= 10240; i++){
            		while (!infile.eof()) {  
						data[i] = infile.get();
                    	break;
					}
        		}
        		infile.close();
			}else{
				throw "Something went wrong";
			}
		} catch(const char* message) {
			cout<<message<<endl;
			strcpy(data, "Something went wrong, Can't get process statics"); 
		}
	}

	void recieve_and_send(){
		while(1){
			memset(buffer,0,sizeof(buffer));

			try {
				if(recv(socket_fd_client, buffer, 1024, 0) > 0) {
   					cout<<"Data from the client :: "<< buffer << endl;

					if(strcmp(buffer,"GetProcessStatistics") == 0){
						get_process_statics();
    					if( (send(socket_fd_client, data , strlen(data), 0)) < 0){
							throw "Error while sending the data";
						}
					}else{
						if( (send(socket_fd_client, message , strlen(message), 0)) < 0){
							throw "Error while sending the data";
						}
					}
				}else{
					throw "Problem receiving the message";
				}
			} catch(const char* msg) {
				cout<<msg<<endl;
				close(socket_fd_client);				
				break;
			}
		}
	}

	void close_socket(){       
		close(socket_fd);
	}
};


int main(){

	Server server;

	server.create_socket();
	server.bind_address();
	server.listen_to_request();

	while(1){
		server.accept_connection();
		server.recieve_and_send();
	}
	
	server.close_socket();
	return 0;
}
