#include "server.h"
#include <string>
#include <cstring>
#include <memory>
#include "client.h"
#include <random>
Server::Server() = default;
std::shared_ptr<Client> Server::add_client(std::string id) // incomplete
{
    Client c{id, *this};
    std::shared_ptr<Client> p{&c};
    clients[p] = 5.0;
    return p;
}
std::shared_ptr<Client> Server::get_client(std::string id)
{
    std::shared_ptr<Client> p{};
    for (std::map<std::shared_ptr<Client>, double>::iterator it{clients.begin()}; it != clients.end(); it++)

        if (!(id < (*(it->first)).get_id() || (*(it->first)).get_id() < id))
            p = it->first;

    return p;
}
double Server::get_wallet(std::string id)
{
    return clients[get_client(id)];
}