#ifndef __SVRSKT_H__
#define __SVRSKT_H__

#include "skt.h"
#include <sys/socket.h>

class SvrSocketWrapper : public SocketWrapper
{
protected:
    bool on_stdin(std::string); // should be defined
    bool on_recv(FileDescriptor, std::string); // should be defined; return false to terminate connection
    bool bind_or_connect(sockaddr *ai_addr, int ai_addrlen);
    void listen();
public:
    SvrSocketWrapper(std::string svr_ip, int svr_port);
    ~SvrSocketWrapper();
    int send(int fd, std::string);
    std::vector<std::string> recv(FileDescriptor fd);
    bool is_client(FileDescriptor);
    int accecpt();
    void terminate_client(FileDescriptor fd);
    void run();
    std::vector<FileDescriptor> get_clients();
protected:
    std::vector<FileDescriptor> clients;
    int backlog;
};

#endif