#include "client.h"
#include <string>
#include <iostream>
#include "server.h"
#include "crypto.h"
#include <random>
Client::Client(const std::string _id, const Server &_server) : server{&_server}, id{_id}
{
    crypto::generate_key(public_key, private_key);
    std::cout << "construct or " << std::endl;
}
std::string Client::get_id()
{
    return id;
}
std::string Client::get_publickey() { return public_key; }
size_t Client::generate_nonce()
{
    std::random_device rd;
    return rd();
} /*
 double Client::get_wallet()
 {
     return (*server).get_wallet(id);
 }*/