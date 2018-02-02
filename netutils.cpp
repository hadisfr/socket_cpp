#include <iostream>
#include "netutils.h"
#include "utils.h"

using namespace std;

NetworkException::NetworkException(string what) : runtime_error("Net Exc:\t" + what) {
    log("Net Exc thrown:\t" + what);
}

WrongNetworkPartnerAddr::WrongNetworkPartnerAddr(string what)
    : NetworkException("Wrong Partner " + what) {}
