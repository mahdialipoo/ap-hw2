#include "server.h"
#include <string>
#include <memory>
#include "client.h"
#include <random>
#include "crypto.h"
std::vector<std::string> pending_trxs;
Server::Server() = default;
std::shared_ptr<Client> Server::add_client(const std::string &id)
{
    std::string id2{id};
    std::shared_ptr<Client> p{get_client(id2)};
    std::random_device rd;
    std::string rnum{};
    while (p != nullptr)
    {
        size_t num{static_cast<size_t>(9000 * (static_cast<double>(rd()) / rd.max()) + 1000)};
        rnum = std::to_string(num);
        id2 += rnum;
        p = get_client(id2);
    }
    std::shared_ptr<Client> p_c{new Client{id2, *this}};
    clients.insert(std::pair<std::shared_ptr<Client>, double>(p_c, 5.0));
    return get_client(id2);
}
std::shared_ptr<Client> Server::get_client(std::string id) const
{
    std::shared_ptr<Client> p{nullptr};
    for (std::map<std::shared_ptr<Client>, double>::iterator it{clients.begin()}; it != clients.end(); it++)

        if (id.compare((*(it->first)).get_id()) == 0)
            p = it->first;

    return p;
}

double Server::get_wallet(std::string id) const
{
    return clients[get_client(id)];
}

bool Server::parse_trx(std::string &trx, std::string &sender, std::string &receiver, double &value) const
{
    std::string tr1{trx.substr(0, trx.find_first_of('-'))};
    std::string tr2h{trx.substr(trx.find_first_of('-') + 1, trx.length() - tr1.length() - 1)};
    std::string tr2{tr2h.substr(0, tr2h.find_first_of('-'))};
    std::string tr3{tr2h.substr(tr2h.find_first_of('-') + 1, tr2h.length() - tr2.length() - 1)};
    bool is_double{true};
    bool cnt_dot{false};
    for (int i{}; i < tr3.length() && is_double; i++)
    {
        if (!isdigit(tr3[i]) && (tr3[i] != '.' || cnt_dot))
            is_double = false;
        if (tr3[i] == '.' && !cnt_dot)
            cnt_dot = true;
    };
    if (tr1.compare(tr2) == 0 || tr2.compare(tr3) == 0 || tr1.compare(tr3) == 0 || !is_double)
    {
        throw std::runtime_error("invalid pattern");
    }
    receiver = tr2;
    sender = tr1;
    value = std::stod(tr3);
    return true;
}
bool Server::add_pending_trx(std::string trx, std::string signature) const
{
    std::string sender{""};
    std::string receiver{""};
    double value{0.0};
    bool t{parse_trx(trx, sender, receiver, value)};
    bool authentic{crypto::verifySignature((*get_client(sender)).get_publickey(), trx, signature)};
    if (t && authentic && value <= get_wallet(sender))
        pending_trxs.push_back(trx);
    return t;
}
size_t Server::mine() //******************هدزخئحمثفث
{
    std::string mempool{""};
    for (size_t i{}; i < pending_trxs.size() - 1; i++)
        mempool += mempool[i];
    for (std::map<std::shared_ptr<Client>, double>::iterator it{clients.begin()}; it != clients.end(); it++)
    {
        size_t nonce{(*(it->first)).generate_nonce()};
        std::string test{mempool + std::to_string(nonce)};
        std::string hash{crypto::sha256(test)};
    }
    return 1;
} /*
 void show_wallets(const Server &server)
 {
     std::cout << std::string(20, '*') << std::endl;
     for (const auto &client : server.clients)
         std::cout << client.first->get_id() << " : " << client.second << std::endl;
     std::cout << std::string(20, '*') << std::endl;
 }*/