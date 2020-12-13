cmake_minimum_required(VERSION 3.2)

if ($ENV{TRAVIS_BUILD_NUMBER})
    project(print_ip VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(print_ip VERSION 0.0.123)
endif()


find_package(Boost COMPONENTS unit_test_framework REQUIRED)

configure_file(version.h.in version.h)

add_executable(print_ip_cli src/main.cpp)
add_executable(test_print_ip tests/test_print_ip.cpp)

set_target_properties(print_ip_cli test_print_ip PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

set_target_properties(test_print_ip PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

target_link_libraries(test_print_ip
    ${Boost_LIBRARIES}
)

if (MSVC)
    target_compile_options(print_ip_cli PRIVATE
        /W4
    )
    target_compile_options(print_ip PRIVATE
        /W4
    )
    target_compile_options(test_print_ip PRIVATE
        /W4
    )
else ()
    target_compile_options(print_ip_cli PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(test_print_ip PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()



install(TARGETS print_ip_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT semen@sam002.net)

include(CPack)

enable_testing()

add_test(test_print_ip test_print_ip)
