#ifndef CLIENT_H
#define CLIENT_H
#include <memory>
#include <string>
#include "crypto.h"
class Server;
class Client
{
public:
  Client(std::string, const Server &);
  std::string get_id() const;
  std::string get_publickey() const;
  std::string sign(std::string);
  double get_wallet() const;
  bool transfer_money(std::string, double);
  size_t generate_nonce();

private:
  Server const *const server;
  const std::string id;
  std::string public_key;
  std::string private_key;
};
#endif // CLIENT_H