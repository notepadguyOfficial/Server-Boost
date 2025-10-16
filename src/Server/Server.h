#include "Http.h"
#include <iostream>

class Server {
private:
    short port;
public:
    Server(short port);
    ~Server() = default;
};
