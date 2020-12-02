cmake_minimum_required(VERSION 3.2)

if ($ENV{TRAVIS_BUILD_NUMBER})
    project(allocator VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(allocator VERSION 0.0.123)
endif()


find_package(Boost COMPONENTS unit_test_framework REQUIRED)

configure_file(version.h.in version.h)

add_executable(allocator_cli src/main.cpp)
add_executable(test_allocator tests/test_allocator.cpp)

set_target_properties(allocator_cli test_allocator PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

set_target_properties(test_allocator PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

target_link_libraries(test_allocator
    ${Boost_LIBRARIES}
)

if (MSVC)
    target_compile_options(allocator_cli PRIVATE
        /W4
    )
    target_compile_options(allocator PRIVATE
        /W4
    )
    target_compile_options(test_allocator PRIVATE
        /W4
    )
else ()
    target_compile_options(allocator_cli PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(test_allocator PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()



install(TARGETS allocator_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT semen@sam002.net)

include(CPack)

enable_testing()

add_test(test_allocator test_allocator)
