cmake_minimum_required(VERSION 3.21)
project(mp_os_arthmtc_frctn_tests)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip)

# For Windows users: prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(
        googletest)

add_executable(
        mp_os_arthmtc_frctn_tests
        fraction_test.cpp)
target_link_libraries(
        mp_os_arthmtc_frctn_tests
        PRIVATE
        gtest_main)
target_link_libraries(
        mp_os_arthmtc_frctn_tests
        PUBLIC
        mp_os_arthmtc_frctn)

set_target_properties(
        mp_os_arthmtc_frctn_tests PROPERTIES
        LANGUAGES CXX
        LINKER_LANGUAGE CXX
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
        VERSION 1.0
        DESCRIPTION "fraction implementation library tests")