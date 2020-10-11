#define BOOST_TEST_MODULE test_ipfilter

#include "../src/lib.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_ipfilter)

    BOOST_AUTO_TEST_CASE(test_constructor)
    {
        BOOST_REQUIRE_THROW(IpAddr("1.2.3.500"), std::string);
        BOOST_REQUIRE_THROW(IpAddr("1.2.-3.4"), std::string);
        BOOST_CHECK_NO_THROW(IpAddr("1.2.3.5"));
    }

    BOOST_AUTO_TEST_CASE(test_compare)
    {
        auto ip1 = IpAddr("1.2.3.5");
        auto ip2 = IpAddr("1.2.3.4");
        auto ip3 = IpAddr("1.3.3.4");
        BOOST_CHECK(ip1 > ip2);
        BOOST_CHECK(ip3 > ip2);
        BOOST_CHECK(ip3 > ip1);
    }


    BOOST_AUTO_TEST_CASE(test_filters)
    {
        auto ip1 = IpAddr("1.2.3.4");

        BOOST_CHECK_EQUAL(ip1.filter(0), false);
        BOOST_CHECK_EQUAL(ip1.filter(255), false);
        BOOST_CHECK_EQUAL(ip1.filter(1), true);
        BOOST_CHECK_EQUAL(ip1.filter(1, 0), false);
        BOOST_CHECK_EQUAL(ip1.filter(1, 255), false);
        BOOST_CHECK_EQUAL(ip1.filter(1, 2), true);
        BOOST_CHECK_EQUAL(ip1.filter(1, 2, 0), false);
        BOOST_CHECK_EQUAL(ip1.filter(1, 2, 255), false);
        BOOST_CHECK_EQUAL(ip1.filter(1, 2, 3), true);
        BOOST_CHECK_EQUAL(ip1.filter(1, 2, 3, 0), false);
        BOOST_CHECK_EQUAL(ip1.filter(1, 2, 3, 255), false);
        BOOST_CHECK_EQUAL(ip1.filter(1, 2, 3, 4), true);

        BOOST_CHECK_EQUAL(ip1.filter_any(1), true);
        BOOST_CHECK_EQUAL(ip1.filter_any(2), true);
        BOOST_CHECK_EQUAL(ip1.filter_any(3), true);
        BOOST_CHECK_EQUAL(ip1.filter_any(4), true);
        BOOST_CHECK_EQUAL(ip1.filter_any(0), false);
        BOOST_CHECK_EQUAL(ip1.filter_any(255), false);
    }
}
