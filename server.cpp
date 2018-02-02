#include <iostream>
#include "utils.h"
#include "svrskt.h"

using namespace std;

int main(int argc, char const *argv[])
{
    SvrSocketWrapper skt("localhost", 4000);
    skt.run();
    return 0;
}

bool SvrSocketWrapper::on_stdin(string str) {
    cout << str << endl;
    return true;
}

bool SvrSocketWrapper::on_recv(FileDescriptor fd, string str) {
    cout << fd << " >> " << str << endl;
    string msg = "got it!";
    cout << fd << " << " << msg << endl;
    send(fd, msg);
    return str != "end";
}