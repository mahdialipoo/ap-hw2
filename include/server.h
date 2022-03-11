#ifndef SERVER_H
#define SERVER_H
#include <memory>
#include <string>
#include <map>
#include "crypto.h"
#include <vector>
extern std::vector<std::string> pending_trxs;
class Client;
class Server
{
public:
  Server();
  std::shared_ptr<Client> add_client(const std::string &);
  std::shared_ptr<Client> get_client(std::string) const;
  double get_wallet(std::string) const;
  static bool parse_trx(const std::string &, std::string &, std::string &, double &);
  bool add_pending_trx(std::string, std::string) const;
  size_t mine();
  friend void show_wallets(const Server &);

private:
  mutable std::map<std::shared_ptr<Client>, double> clients;
};
#endif // SERVER_H