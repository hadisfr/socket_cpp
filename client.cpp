#include <iostream>
#include "utils.h"
#include "cltskt.h"

using namespace std;

int main(int argc, char const *argv[])
{
    CltSocketWrapper skt("localhost", 4000);
    string msg = "Hi!";
    cout << "server << " << msg << endl;
    skt.send(msg);
    cout << "server >> " << skt.recv()[0] << endl;
    string end = "end";
    cout << "server << " << end << endl;
    skt.send(end);
    return 0;
}
