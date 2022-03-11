#include "server.h"
#include "client.h"
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

bool Server::parse_trx(const std::string &trx, std::string &sender, std::string &receiver, double &value)
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
        return false;
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
    bool state{t && authentic && value <= get_wallet(sender) && get_client(receiver)};
    if (state)
        pending_trxs.push_back(trx);
    return state;
}
size_t Server::mine()
{
    std::string mempool{""};
    bool hash_rew{false};
    std::string hash_reward{""};
    size_t nonce_reward{};
    for (size_t i{}; i < pending_trxs.size(); i++)
        mempool += pending_trxs[i];
    do
    {
        for (std::map<std::shared_ptr<Client>, double>::iterator it{clients.begin()}; it != clients.end(); it++)
        {
            size_t nonce{(*(it->first)).generate_nonce()};
            std::string test{mempool + std::to_string(nonce)};
            std::string hash{crypto::sha256(test)};
            if (hash.substr(0, 10).find("000") != std::string::npos)
            {
                nonce_reward = nonce;
                hash_reward = hash;
                hash_rew = true;
                (it->second) += 6.25;
                for (int i; i < pending_trxs.size(); i++)
                {
                    double value{};
                    std::string sender{""};
                    std::string receiver{""};
                    parse_trx(pending_trxs[i], sender, receiver, value);
                    clients[get_client(sender)] -= value;
                    clients[get_client(receiver)] += value;
                }
                pending_trxs.clear();
                return nonce_reward;
            }
        }
    } while (hash_reward.substr(0, 10).find("000") == std::string::npos);
    return nonce_reward;
}
void show_wallets(const Server &server)
{
    std::cout << std::string(20, '*') << std::endl;
    for (const auto &client : server.clients)
        std::cout << client.first->get_id() << " : " << client.second << std::endl;
    std::cout << std::string(20, '*') << std::endl;
}
void show_pending_transactions(std::vector<std::string> pending_trxs)
{
    std::cout << std::string(20, '*') << std::endl;
    for (const auto &trx : pending_trxs)
        std::cout << trx << std::endl;
    std::cout << std::string(20, '*') << std::endl;
}
