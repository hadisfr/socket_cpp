#ifndef __SKT_H__
#define __SKT_H__

#include "skt.h"

class CltSocketWrapper : public SocketWrapper
{
protected:
    bool bind_or_connect(sockaddr *ai_addr, int ai_addrlen);
public:
    CltSocketWrapper(std::string ip, int port);
    int send(std::string);
    std::vector<std::string> recv();
};

#endif