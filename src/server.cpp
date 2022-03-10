#include "server.h"
#include <string>
#include <memory>
#include "client.h"
#include <random>
Server::Server() = default;
std::shared_ptr<Client> Server::add_client(std::string id) // incomplete
{
    std::shared_ptr<Client> pc{get_client(id)};

    if (pc == nullptr)
    {
        std::shared_ptr<Client> p{new Client{id, *this}};
        clients.insert(std::pair<std::shared_ptr<Client>, double>(p, 5.0));
        return p;
    }
    else
    {
        ;
    }
    return pc; // mjhgj
}
std::shared_ptr<Client> Server::get_client(std::string id) const
{
    std::shared_ptr<Client> p{nullptr};
    for (std::map<std::shared_ptr<Client>, double>::iterator it{clients.begin()}; it != clients.end(); it++)

        if (id.compare((*(it->first)).get_id()) != 0)
            p = it->first;

    return p;
}

double Server::get_wallet(std::string id) const
{
    return clients[get_client(id)];
}

bool Server::parse_trx(std::string trx, std::string sender, std::string receiver, double value)
{
    std::string s{sender + receiver + std::to_string(value)};
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
    return !(s.compare(trx));
}