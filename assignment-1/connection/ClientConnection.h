#include <netdb.h> 

class ClientConnection{
private:
    int socket_fd; 
    struct sockaddr_in server_address;
    struct hostent *host;
    char buffer[1024];

public:
    ClientConnection();
    void create_socket();
    void get_host_ip(const char *name);
    void connet_to_server();
    int send_req(const char *data);
    char *recieve_res();
    void close_socket();
};
