#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <map>
#include <nlohmann/json.hpp>

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
	map<string, string> user_details;

	bool is_key_exists(string key){
		map<string, string>::iterator i;
		for (i = user_details.begin(); i != user_details.end(); ++i){
			if (i->first == key){
				return true;
			}
		}
		return false;
	}

	string save_data(string key, string value){
		if(!is_key_exists(key)){
			user_details.insert(pair<string, string>(key, value));
			return "OK";
		}else{
			return "key already exists";
		}
	}

	string get_data(string key){
		map<string, string>::iterator i;
		for (i = user_details.begin(); i != user_details.end(); ++i){
			if (i->first == key){
				return i->second;
			}
		}
		return "key does not exists";
	}

	string update_data(string key, string value){
		if(is_key_exists(key)){
			user_details.at(key) = value;
			return "OK";
		}else{
			return "key does not exists to update, first set the key";
		}
	}

	string delete_data(string key){
		if(is_key_exists(key)){
			user_details.erase(key);
			return "OK";
		}else{
			return "key does not exists to delete";
		}
	}

	string process_data(json data){
		string response;
		if (data["command"] == "GET"){
			response = get_data(data["key"]);
		}else if(data["command"] == "SET"){
			response = save_data(data["key"], data["value"]);
		}else if(data["command"] == "PUT"){
			response = update_data(data["key"], data["value"]);;
		}else if(data["command"] == "DELETE"){
			response = delete_data(data["key"]);;
		}else{
			response = "Invalid command";
		}
		return response;
	}

public:
	Server(){
		socket_address.sin_family = AF_INET; 
   		socket_address.sin_addr.s_addr = INADDR_ANY; 
    	socket_address.sin_port = htons( PORT ); 
	}

	void create_socket(){
		socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(socket_fd < 0){
			cout<<"Couldn't create socket \n";
			exit(0);
		}
		cout<<"Socket created successfully\n";
	}

	void bind_address(){
		if(bind(socket_fd, (struct sockaddr *)&socket_address, address_lenth)<0){
			cout<< "Couldn't bind the IP address \n";
			close_socket();
		}
		cout<<"Address binding completed \n";	
	}

	void listen_to_request(){
		if ((listen(socket_fd,0))<0){
			cout<<"Couldn't listen to incoming request \n";
			close_socket();
		}	
		cout<<"Listening to incoming request....... \n";	
	}

	void accept_connection(){
		socket_fd_client = accept(socket_fd, (struct sockaddr *)&socket_address,(socklen_t*)&address_lenth);
		if (socket_fd_client < 0) { 
			cout << "Couldn't accept the connection \n"; 
			close_socket();
		} 
		cout<<"Connection accepted \n";
	}

	void process_request(){
		json data; string response;
		while (true){
			memset(buffer,0,sizeof(buffer));
			if((recv(socket_fd_client, buffer, 1024, 0)) <= 0){
				close(socket_fd_client);
				break;
			}
			cout << "Request::" << buffer << "\n";
			data = json::parse(buffer);
			response = process_data(data);
			if ((send(socket_fd_client, response.c_str(), strlen(response.c_str()), 0)) < 0){
				cout<< "Error while sending response \n";
				close(socket_fd_client);
				break;
			}
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
		server.process_request();
	}
	server.close_socket();
	return 0;
}
