#include <iostream>
#include <netinet/in.h>

class ServerConnection {
    private:
        int socket_fd, socket_fd_client, address_lenth;
        struct sockaddr_in socket_address;
        char buffer[1024];

    public:
        ServerConnection();
        void create_socket();
        void bind_address();
        void listen_to_request();
        void accept_connection();
        void close_socket();
        char *recieve_req();
        int send_res(const char *data);
};
