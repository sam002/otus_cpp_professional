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


    BOOST_AUTO_TEST_CASE(test_complex_container)
    {
        auto firstContainer = CustomContainer<std::vector<int>, CustomAllocator<element<std::vector<int>>, 2>>{};
        for (int i = 0; i < 100; i++) {
            auto elem = std::vector<int>();
            for (int j = i; j > 0; j--) {
                elem.emplace_back(j);
            }
            BOOST_CHECK_NO_THROW(firstContainer.push_back(elem));
        }

        for (int i = 5; i < 50; i++) {
            BOOST_CHECK_NO_THROW(firstContainer.pop());
        }
    }
}
