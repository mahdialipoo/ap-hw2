#include "client.h"
#include <string>
#include <iostream>
#include "server.h"
Client::Client(const std::string _id, const Server &_server) : server{&_server}, id{_id}
{
    std::cout << "construct or " << std::endl;
}
std::string Client::get_id()
{
    return id;
}