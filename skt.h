#ifndef __CLTSKT_H__
#define __CLTSKT_H__

#include "netutils.h"
#include <sys/socket.h>
#include <vector>

class SocketWrapper
{
protected:
    void create_socket();
    virtual bool bind_or_connect(sockaddr *ai_addr, int ai_addrlen) = 0;
    virtual void listen();
    int send(FileDescriptor fd, std::string);
    std::vector<std::string> recv(FileDescriptor fd);
public:
    SocketWrapper(std::string ip, int port);
    ~SocketWrapper();
    int close();
    FileDescriptor socket_fd;
protected:
    std::string ip, port;
    std::vector<std::string> recv_box;
};

#endif