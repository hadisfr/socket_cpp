#include <unistd.h>
#include <netdb.h>
#include "iostream"
#include "skt.h"
#include "utils.h"

#define BUFFER_SIZE 100

using namespace std;

SocketWrapper::SocketWrapper(string _ip, int _port) : ip(_ip), port(itos(_port)) {}

SocketWrapper::~SocketWrapper() {
    close();
}

void SocketWrapper::create_socket() {
    addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; //IPv4
    hints.ai_socktype = SOCK_STREAM; //TCP
    if(ip == "localhost")
        hints.ai_flags = AI_PASSIVE; // use default ip

    addrinfo *addr_info, *addr;
    int getaddrinfo_ret;
    if((getaddrinfo_ret = getaddrinfo(ip.c_str(), port.c_str(), &hints, &addr_info)) != 0) {
        string err_msg = gai_strerror(getaddrinfo_ret);
        freeaddrinfo(addr_info);
        throw NetworkException(err_msg);
    }

    for(addr = addr_info; addr; addr = addr->ai_next) {
        socket_fd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
        if(socket_fd == -1){
            log("Net Err:\tsocket");
            continue;
        }
        if(!bind_or_connect(addr->ai_addr, addr->ai_addrlen))
            continue;
        break;
    }
    freeaddrinfo(addr_info);
    if(!addr) {
        log("Net Err:\tbind failure");
        throw NetworkException("bind failure");
    }
    listen();

    log(itos(socket_fd) + " : socket created");
}

void SocketWrapper::listen() {}

int SocketWrapper::close() {
    return ::close(socket_fd);
}

int SocketWrapper::send(FileDescriptor fd, string msg) {
    msg += NET_MSG_DELIMITER;
    return ::send(fd, msg.c_str(), msg.size() + 1, 0);
}

vector<string> SocketWrapper::recv(FileDescriptor fd) {
    char buffer[BUFFER_SIZE];
    ssize_t len = ::recv(fd, buffer, BUFFER_SIZE - 1, 0);
    if(len < 0)
        throw NetworkException("recv error");
    buffer[len] = 0;
    return split(string(buffer), NET_MSG_DELIMITER);
}

