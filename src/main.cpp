#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "lib.h"

int main()
{
    try
    {
        std::vector<IpAddr> ip_pool;

        for(std::string line; std::getline(std::cin, line);)
        {
            std::vector<std::string> v = split(line, '\t');

            ip_pool.emplace_back(v.at(0));
        }

        std::sort(ip_pool.begin(), ip_pool.end(), std::greater<>());

        for(auto ip : ip_pool)
        {
            std::cout << ip.to_string() << std::endl;
        }

        // ip = filter(1)
        for(auto ip : ip_pool)
        {
            if (ip.filter(1)) {
                std::cout << ip.to_string() << std::endl;
            }
        }

        // ip = filter(46, 70)
        for(auto ip : ip_pool)
        {
            if (ip.filter(46, 70)) {
                std::cout << ip.to_string() << std::endl;
            }
        }


        // ip = filter_any(46)
        for(auto ip : ip_pool)
        {
            if (ip.filter_any(46)) {
                std::cout << ip.to_string() << std::endl;
            }
        }

    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
