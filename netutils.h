#ifndef __NETUTILS_H__
#define __NETUTILS_H__

#include <stdexcept>
#include <string>

#define NET_MSG_DELIMITER ";"

typedef int FileDescriptor;

class NetworkException : public std::runtime_error {
public:
    NetworkException(std::string what = "");
};

class WrongNetworkPartnerAddr : NetworkException
{
public:
    WrongNetworkPartnerAddr(std::string what = "");
};

#endif