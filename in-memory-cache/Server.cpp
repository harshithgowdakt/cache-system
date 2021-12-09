#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <signal.h>
#include <vector>
#include "connection/ServerConnection.h"
#include "db/map.h"
#include "db/hashmap.h"
#include "db/list.h"

using namespace std;

class Server{
	private:
		ServerConnection serverConnection;
		Map mapObj;
		List listObj;
		Hashmap hashmapObj;
		
		vector<string> split_string(char *command);
		string to_upper(string str);
		string process_data(char *command);
		void process_req();

	public:
		void start();
		void stop(int signum);	
};

vector<string> Server::split_string(char *command){
	const char *delim = " ";
    vector<string> command_data;
    char *token = strtok(command, delim);
    while (token){
        command_data.push_back(string(token));
        token = strtok(NULL, delim);
    }
	return command_data;
}

string Server::to_upper(string str){
    transform(str.begin(), str.end(), str.begin(), ::toupper); 
    return str;
}


string Server::process_data(char *command){
	vector<string> command_data;
    command_data = split_string(command);

	if (to_upper(command_data.at(0))== "GET"){
		if(command_data.size() < 2){
			return "worng number of arguments";
		}
		return mapObj.get_data(command_data.at(1));
	}else if(to_upper(command_data.at(0)) == "SET"){
		if(command_data.size() < 3){
			return "worng number of arguments";
		}
		if(command_data.size() == 3){
			return  mapObj.save_data(command_data.at(1), command_data.at(2), -1);
		}
		return  mapObj.save_data(command_data.at(1), command_data.at(2), stoi(command_data.at(3)));
	}else if(to_upper(command_data.at(0)) == "DEL"){
		if(command_data.size() < 2){
			return "worng number of arguments";
		}
		int res = mapObj.delete_data(command_data.at(1));
		return to_string(res);
	}else if(to_upper(command_data.at(0)) == "HSET"){
		if(command_data.size() < 4){
			return "worng number of arguments";
		}
		int res = hashmapObj.h_set(command_data.at(1), command_data.at(2), command_data.at(3));
		return to_string(res);
	}else if(to_upper(command_data.at(0)) == "HGET"){
		if(command_data.size() < 3){
			return "worng number of arguments";
		}
		return hashmapObj.h_get(command_data.at(1), command_data.at(2));
	}else if(to_upper(command_data.at(0)) == "HGETALL"){
		if(command_data.size() < 2){
			return "worng number of arguments";
		}
		return hashmapObj.h_get_all(command_data.at(1));
	}else if(to_upper(command_data.at(0)) == "LPUSH"){
		if(command_data.size() < 3){
			return "worng number of arguments";
		}
		int length = listObj.l_push(command_data.at(1), command_data.at(2));
		return to_string(length);
	}else if(to_upper(command_data.at(0)) == "LPOP"){
		if(command_data.size() < 2){
			return "worng number of arguments";
		}
		return listObj.l_pop(command_data.at(1));
	}else if(to_upper(command_data.at(0)) == "LLEN"){
		if(command_data.size() < 2){
			return "worng number of arguments";
		}
		int length = listObj.l_len(command_data.at(1));
		return to_string(length);
	}else if(to_upper(command_data.at(0)) == "LRANGE"){
		if(command_data.size() < 4){
			return "worng number of arguments";
		}
		return listObj.l_range(command_data.at(1), stoi(command_data.at(2)) , stoi(command_data.at(3)));
	}else{
		return "ERR unknown command";
	}
}

void Server::process_req(){
	char *buffer;
	vector<string> data; string response;
	while (true){
		buffer = serverConnection.recieve_req();
		if(buffer[0] == 0) break;
		cout << "Request::" << buffer << "\n";
		response = process_data(buffer);	
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
