#ifndef SERVER_H
#define SERVER_H
#include <memory>
#include <string>
#include <map>
class Client;
class Server
{
public:
    Server();
    std::shared_ptr<Client> add_client(std::string); /*
    double get_wallet(std::string);
    std::shared_ptr<Client> get_client(std::string);
    bool parse_trx(std::string trx, std::string sender, std::string receiver, double value);
    bool add_pending_trx(std::string trx, std::string signature);
    size_t mine();
*/
private:
    std::map<std::shared_ptr<Client>, double> clients;
};
#endif // SERVER_H