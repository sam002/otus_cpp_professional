#define BOOST_TEST_MODULE test_ipfilter

#include "../src/container.h"
#include "../src/allocator.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_allocator)

    BOOST_AUTO_TEST_CASE(test_const_memory)
    {
        auto firstContainer = CustomContainer<int, CustomAllocator<element<int>, 2>>{};
        auto secondContainer = CustomContainer<int, CustomAllocator<element<int>, 2>>{};
        for (int i = 0; i < 10; i++) {
            firstContainer.push(i);
            secondContainer.push_back(i);
        }

        for (int i = 10; i > 5; i--) {
            firstContainer.pop();
        }
        for (int i = 5; i < 10; i++) {
            firstContainer.push(i);
        }

        for (int i = 0; i < 10; i++) {
            BOOST_CHECK_EQUAL(firstContainer.size(), secondContainer.size());
        }
    }
}
