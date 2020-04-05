#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include "connection/ServerConnection.h"
#include "db/db.h"

using namespace std;
using json = nlohmann::json;

class Server{

private:
	ServerConnection serverConnection;
	DB db;
	
	string process_data(json data);
	void process_req();

public:
	void start();
	void stop();
};

string Server::process_data(json data){
	string response;
	if (data["command"] == "GET"){
		response = db.get_data(data["key"]);
	}else if(data["command"] == "SET"){
		response = db.save_data(data["key"], data["value"], data["ttl"]);
	}else if(data["command"] == "PUT"){
		response = db.update_data(data["key"], data["value"]);;
	}else if(data["command"] == "DELETE"){
		response = db.delete_data(data["key"]);;
	}else{
		response = "Invalid command";
	}
	return response;
}

void Server::process_req(){
	char *buffer;
	json data; string response;
	while (true){
		buffer = serverConnection.recieve_req();
		if(buffer[0] == 0) break;
		cout << "Request::" << buffer << "\n";
		data = json::parse(buffer);
		response = process_data(data);	
		if ((serverConnection.send_res(response.c_str()) < 0)){
			cout<< "Error while sending response \n";
			break;
		}
	};
}

void Server::start(){
	serverConnection.create_socket();
	serverConnection.bind_address();
	serverConnection.listen_to_request();
	while(true){
		serverConnection.accept_connection();
		process_req();
	}
}

void Server::stop(){
	db.join_db_threads();
	serverConnection.close_socket();
}

int main(){
	Server server;
	server.start();
	server.stop();
	return 0;
}
