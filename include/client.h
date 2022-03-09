#include <memory>
#include <string>
#include "server.h"
#ifndef CLIENT_H
#define CLIENT_H

class Client
{
public:
    Client(const std::string, const Server &);
    std::string get_id(); /*
     std::string get_publickey();
     double get_wallet();
     bool transfer_money(std::string receiver, double value);*/
    size_t generate_nonce();

private:
    Server const *const server;
    const std::string id;
    std::string public_key;
    std::string private_key;
};
#endif // CLIENT_H