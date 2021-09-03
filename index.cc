#include <cstring>
#include <string>
#include <iostream>

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <unistd.h>

int main() {
    const unsigned int backlog = 8;
    addrinfo hints, *res, *p;
    memset(&hints, 0, sizeof(hints));

    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE;

    getaddrinfo("0.0.0.0", "3000", &hints, &res);

    int socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if (socket_fd == -1) {
        std::cerr << "Error while creating of socket" << std::endl;
        return -4;
    }

    int bind_r = bind(socket_fd, res->ai_addr, res->ai_addrlen);
    int listen_r = listen(socket_fd, backlog);

    sockaddr_storage client_addr;
    socklen_t client_addr_size = sizeof(client_addr);

    const std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\nConnection: close\r\n\r\nHello, world!";

    while (1) {
        int new_fd = accept(socket_fd, (sockaddr *) &client_addr, &client_addr_size);

        if (new_fd == -1) {
            std::cerr << "Error while accept\n";
            continue;
        }

        std::cout << "Connection accepted\n";

        auto bytes_sent = send(new_fd, response.data(), response.length(), 0);
        close(new_fd);
    }

    close(socket_fd);
    freeaddrinfo(res);

    return 0;
}
