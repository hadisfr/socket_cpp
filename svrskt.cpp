#include <unistd.h>
#include <cerrno>
#include <iostream>
#include "svrskt.h"
#include "utils.h"

#define BACKLOG 10

#ifdef __sun
    const char yes = '1';
#else
    const int yes = 1;
#endif

using namespace std;

SvrSocketWrapper::SvrSocketWrapper(string svr_ip, int svr_port)
    : SocketWrapper(svr_ip, svr_port), backlog(BACKLOG) {
        create_socket();
    }

SvrSocketWrapper::~SvrSocketWrapper() {
    while(clients.size()) {
        ::close(*clients.end());
        clients.pop_back();
    }
}

bool SvrSocketWrapper::bind_or_connect(sockaddr *ai_addr, int ai_addrlen) {
    if(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        log("Net Err:\tsetsockopt");
        return false;
    }
    if(bind(socket_fd, ai_addr, ai_addrlen) == -1) {
        log("Net Err:\tbind");
        ::close(socket_fd);
        return false;
    }
    return true;
}

void SvrSocketWrapper::listen() {
    if(::listen(socket_fd, backlog) == -1) {
        log("Net Err:\tlisten" + string(strerror(errno)));
        throw NetworkException("listen failure");
    }
}

int SvrSocketWrapper::send(FileDescriptor fd, string msg) {
    if(!is_client(fd))
        throw WrongNetworkPartnerAddr("send");
    return SocketWrapper::send(fd, msg);
}

vector<string> SvrSocketWrapper::recv(FileDescriptor fd) {
    if(!is_client(fd))
        throw WrongNetworkPartnerAddr("recv");
    return SocketWrapper::recv(fd);
}

bool SvrSocketWrapper::is_client(FileDescriptor fd) {
    for(size_t i = 0; i < clients.size(); i++)
        if(clients[i] == fd)
            return true;
    return false;
}

int SvrSocketWrapper::accecpt() {
    sockaddr_storage client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    int client_fd = accept(socket_fd, (struct sockaddr *)(&client_addr), &client_addr_size);
    clients.push_back(client_fd);
    return client_fd;
}

void SvrSocketWrapper::terminate_client(FileDescriptor fd) {
    for(vector<FileDescriptor>::iterator i = clients.begin(); i != clients.end(); ++i)
        if(*i == fd) {
            log("terminate client FD " + itos(*i));
            ::close(*i);
            clients.erase(i);
            break;
        }
}

void SvrSocketWrapper::run() {
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    FD_SET(socket_fd, &fds);
    for(vector<FileDescriptor>::iterator i = clients.begin(); i != clients.end(); i++)
        FD_SET(*i, &fds);
    FileDescriptor max_fd = socket_fd;
    log("Svr is listening...");

    bool continue_flag = true;
    while(continue_flag) {
        fd_set read_fds = fds;
        select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        for(FileDescriptor i = 0; i <= max_fd; i++)
            if(FD_ISSET(i, &read_fds)) {
                if(i == STDOUT_FILENO || i == STDERR_FILENO)
                    continue;
                else if(i == STDIN_FILENO) {
                    string str;
                    cin >> str;
                    if(cin.eof())
                        continue_flag = false;
                    on_stdin(str);
                }
                else if(i == socket_fd) {
                    FileDescriptor client_fd = accecpt();
                    FD_SET(client_fd, &fds);
                    if(max_fd < client_fd)
                        max_fd = client_fd;
                } else {
                    vector<string> msgs = recv(i);
                    for(vector<string>::iterator msg = msgs.begin(); msg != msgs.end(); msg++)
                        if(!on_recv(i, *msg)) {
                            terminate_client(i);
                            FD_CLR(i, &fds);
                            break;
                        }
                }
            }
    }
    log("Svr is shutting down...");
}

vector<FileDescriptor> SvrSocketWrapper::get_clients() {
    return clients;
}
