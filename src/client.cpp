#include "client.h"
#include <string>
#include <iostream>
#include "server.h"
#include "crypto.h"
#include <random>
Client::Client(std::string _id, const Server &_server) : server{&_server}, id{_id}
{
    crypto::generate_key(public_key, private_key);
    std::cout << "construct or " << std::endl;
}
std::string Client::get_id() const
{
    return id;
}
std::string Client::get_publickey() const { return public_key; }
size_t Client::generate_nonce()
{
    std::random_device rd;
    return rd();
}
double Client::get_wallet() const
{
    return (*server).get_wallet(id);
}
std::string Client::sign(std::string txt)
{
    return crypto::signMessage(private_key, txt);
}
bool Client::transfer_money(std::string receiver, double value)
{
    std::string trx{id + "-" + receiver + "-" + std::to_string(value)};
    return (*server).add_pending_trx(trx, sign(trx));
}