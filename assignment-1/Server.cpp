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
	void *process_req();

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
	}else if(data["command"] == "DEL"){
		int res = db.delete_data(data["key"]);
		response = to_string(res);
	}else if(data["command"] == "HSET"){
		int res = db.h_set(data["key"], data["field"], data["value"]);
		response = to_string(res);
	}else if(data["command"] == "HGET"){
		response = db.h_get(data["key"], data["field"]);
	}else if(data["command"] == "HGETALL"){
		response = db.h_get_all(data["key"]);
	}else if(data["command"] == "LPUSH"){
		int length = db.l_push(data["key"], data["value"]);
		response = to_string(length);
	}else if(data["command"] == "LPOP"){
		response = db.l_pop(data["key"]);
	}else if(data["command"] == "LLEN"){
		int length = db.l_len(data["key"]);
		response = to_string(length);
	}else if(data["command"] == "LRANGE"){
		response = db.l_range(data["key"], data["start_index"], data["stop_index"]);
	}else{
		response = "ERR unknown command";
	}
	return response;
}

void *Server::process_req(){
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
		pthread_t thread_id;pthread_t thread_id;pthread_t thread_id;
		int  socket_fd = serverConnection.accept_connection();
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
