add_executable(prime_scan primes_scan.cpp)
target_link_libraries(prime_scan gtest_main NTL HElib)