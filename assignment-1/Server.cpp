#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <signal.h>
#include <nlohmann/json.hpp>
#include "connection/ServerConnection.h"
#include "db/map.h"
#include "db/hashmap.h"
#include "db/list.h"

using namespace std;
using json = nlohmann::json;

class Server{
	private:
		ServerConnection serverConnection;
		Map mapObj;
		List listObj;
		Hashmap hashmapObj;
		
		string process_data(json data);
		void process_req();

	public:
		void start();
		void stop(int signum);	
};

string Server::process_data(json data){
	string response;
	if (data["command"] == "GET"){
		response = mapObj.get_data(data["key"]);
	}else if(data["command"] == "SET"){
		response = mapObj.save_data(data["key"], data["value"], data["ttl"]);
	}else if(data["command"] == "DEL"){
		int res = mapObj.delete_data(data["key"]);
		response = to_string(res);
	}else if(data["command"] == "HSET"){
		int res = hashmapObj.h_set(data["key"], data["field"], data["value"]);
		response = to_string(res);
	}else if(data["command"] == "HGET"){
		response = hashmapObj.h_get(data["key"], data["field"]);
	}else if(data["command"] == "HGETALL"){
		response = hashmapObj.h_get_all(data["key"]);
	}else if(data["command"] == "LPUSH"){
		int length = listObj.l_push(data["key"], data["value"]);
		response = to_string(length);
	}else if(data["command"] == "LPOP"){
		response = listObj.l_pop(data["key"]);
	}else if(data["command"] == "LLEN"){
		int length = listObj.l_len(data["key"]);
		response = to_string(length);
	}else if(data["command"] == "LRANGE"){
		response = listObj.l_range(data["key"], data["start_index"], data["stop_index"]);
	}else{
		response = "ERR unknown command";
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
	mapObj.restore_backup();
	hashmapObj.restore_backup();
	listObj.restore_backup();
	while(true){
		serverConnection.accept_connection();
		process_req();
	}
}

Server server;

void Server::stop(int signum){
	mapObj.take_backup();
	hashmapObj.take_backup();
	listObj.take_backup();
	serverConnection.close_socket();
	exit(signum);
}

void signal_callback_handler(int signum){
	server.stop(signum);
}

int main(){
	signal(SIGINT, signal_callback_handler);
	server.start();
	return 0;
}
