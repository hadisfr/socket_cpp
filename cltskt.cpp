#include <unistd.h>
#include "cltskt.h"
#include "utils.h"

using namespace std;

CltSocketWrapper::CltSocketWrapper(string ip, int port)
    : SocketWrapper(ip, port) {
        create_socket();
    }

bool CltSocketWrapper::bind_or_connect(sockaddr *ai_addr, int ai_addrlen) {
    if(connect(socket_fd, ai_addr, ai_addrlen) == -1) {
        ::close(socket_fd);
        log("Net Err:\tconnect");
        return false;
    }
    return true;
}

int CltSocketWrapper::send(string msg) {
    return SocketWrapper::send(socket_fd, msg);
}

vector<string> CltSocketWrapper::recv() {
    return SocketWrapper::recv(socket_fd);
}

