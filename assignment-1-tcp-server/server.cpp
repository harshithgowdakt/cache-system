#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <nlohmann/json.hpp>
#include <map>

#define _XOPEN_SOURCE_EXTENDED 1
#define PORT 8080

using namespace std;
using json = nlohmann::json;

class Server{

private:
	int socket_fd,socket_fd_client;
	struct sockaddr_in socket_address;
	int address_lenth = sizeof(socket_address);
	char buffer[1024];
	map<string, string> list;

public:
	Server(){
		socket_address.sin_family = AF_INET; 
   		socket_address.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    	socket_address.sin_port = htons( PORT ); 
	}

	void create_socket(){
		socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(socket_fd < 0){
			cout<<"Couldn't create socket \n";
			close(socket_fd);
		}
		cout<<"Socket created successfully\n";
	}

	void bind_address(){
		if(bind(socket_fd, (struct sockaddr *)&socket_address, address_lenth)<0){
			cout<< "Couldn't bind the IP address \n";
			close(socket_fd);
		}
		cout<<"Address binding completed \n";	
	}

	void listen_to_request(){
		if ((listen(socket_fd,0))<0){
			cout<<"Couldn't listen to incoming request";
			close(socket_fd);
		}	
		cout<<"Listening to incoming request....... \n";	
	}

	void accept_connection(){
		socket_fd_client = accept(socket_fd, (struct sockaddr *)&socket_address,(socklen_t*)&address_lenth);
		if (socket_fd_client < 0) { 
			cout << "Couldn't accept the connection \n"; 
			close(socket_fd);
		} 
		cout<<"Connection accepted \n";
	}

	bool is_key_exists(string key){
		map<string, string>::iterator i;
		for (i = list.begin(); i != list.end(); ++i){
			if (i->first == key){
				return true;
			}
		}
		return false;
	}

	string save_data(string key, string value){
		if(!is_key_exists(key)){
			list.insert(pair<string, string>(key, value));
			return "OK";
		}else{
			return "key already exists";
		}
	}

	string get_data(string key){
		map<string, string>::iterator i;
		for (i = list.begin(); i != list.end(); ++i){
			if (i->first == key){
				return i->second;
			}
		}
		return "key does not exists";
	}

	string process_request(json data){
		string response;
		if (data["command"] == "GET"){
			response =	get_data(data["key"]);
		}else if(data["command"] == "SET"){
			response = save_data(data["key"], data["value"]);
		}
		return response;
	}

	void recieve_and_send(){
		json data; string response;
		memset(buffer,0,sizeof(buffer));
		if(recv(socket_fd_client, buffer, 1024, 0) > 0){
			cout << "Request ::" << buffer << "\n";
			data = json::parse(buffer);
		 	response = process_request(data);
			 if ((send(socket_fd_client, response.c_str(), strlen(response.c_str()), 0)) < 0){
				close_socket();
				cout<< "Error while sending response \n";
			}	
		}else{
			close_socket();
			cout<< "Error while receiving request \n";
		}
	}

	void close_socket(){       
		close(socket_fd);
		exit(0);
	}
};


int main(){
	Server server;

	server.create_socket();
	server.bind_address();
	server.listen_to_request();
	while (true){
		server.accept_connection();
		server.recieve_and_send();
	}
	server.close_socket();
	return 0;
}
