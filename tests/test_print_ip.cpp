#define BOOST_TEST_MODULE test_ipfilter

#include "../src/print_ip.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_print_ip)

    BOOST_AUTO_TEST_CASE(test_main_integral)
    {
        BOOST_CHECK_EQUAL(print_ip(char(-1)), std::string("255"));
        BOOST_CHECK_EQUAL(print_ip(short(0)), std::string("00"));
        BOOST_CHECK_EQUAL(print_ip(int(2130706433)), std::string("127.0.0.1"));
        BOOST_CHECK_EQUAL(print_ip(long(8875824491850138409)), std::string("123.45.67.89.101.112.131.41"));
    }


    BOOST_AUTO_TEST_CASE(test_main_string)
    {
        BOOST_CHECK_EQUAL(print_ip(std::string{"1.12.123.234"}), std::string("1.12.123.234"));
    }

    BOOST_AUTO_TEST_CASE(test_main_container)
    {
        BOOST_CHECK_EQUAL(print_ip(std::vector<int>{1,2,3,4}), std::string("1.2.3.4"));
        BOOST_CHECK_EQUAL(print_ip(std::list<int>{2,3,4,5}), std::string("2.3.4.5"));
    }
}
