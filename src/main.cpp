#include <iostream>
#include <gtest/gtest.h>
#include "client.h"
#include "server.h"
int main(int argc, char **argv)
{
    if (true) // make false to run unit-tests
    {
        Server s{};
        std::string o{"sjh"};
        Client c{o, s};
        std::cout << c.get_publickey() << std::endl;
        std::cout << c.sign("a") << std::endl;
        // debug section
    }
    else
    {
        ::testing::InitGoogleTest(&argc, argv);
        std::cout << "RUNNING TESTS ..." << std::endl;
        int ret{RUN_ALL_TESTS()};
        if (!ret)
            std::cout << "<<<SUCCESS>>>" << std::endl;
        else
            std::cout << "FAILED" << std::endl;
    }
    return 0;
}