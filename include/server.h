#ifndef SERVER_H
#define SERVER_H
#include <memory>
#include <string>
#include <map>
#include <vector>
class Client;
class Server
{
public:
  Server();
  std::shared_ptr<Client> add_client(const std::string &);
  std::shared_ptr<Client> get_client(std::string) const;
  double get_wallet(std::string) const;
  bool parse_trx(std::string &, std::string &, std::string &, double &) const;
  bool add_pending_trx(std::string, std::string) const;
  size_t mine();

private:
  mutable std::map<std::shared_ptr<Client>, double> clients;
};
#endif // SERVER_H