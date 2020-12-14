#include <iostream>
#include <map>
#include <list>
#include "print_ip.h"

int main() {
    try {
        std::cout << print_ip(char(-1)) << std::endl;
        std::cout << print_ip(short(0)) << std::endl;
        std::cout << print_ip(int(2130706433)) << std::endl;
        std::cout << print_ip(long(8875824491850138409)) << std::endl  << std::endl;

        std::cout << print_ip(std::string{"1.12.123.234"}) << std::endl;
        std::cout << print_ip(std::vector<int>{1,2,3,4}) << std::endl;
        std::cout << print_ip(std::list<int>{2,3,4,5}) << std::endl;
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
