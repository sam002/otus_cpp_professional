#define BOOST_TEST_MODULE test_ipfilter

#include "../src/container.h"
#include "../src/allocator.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_allocator)

    BOOST_AUTO_TEST_CASE(test_const_memory)
    {
        auto tempContainer = container<int, CustomAllocator<element<int>, 2>>{};
        for (int i = 0; i < 10; i++) {
            tempContainer.push((i));
        }
    }
}
