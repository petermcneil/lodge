#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include <spdlog/spdlog.h>


int main( int argc, char* argv[] ) {
//    spdlog::set_pattern("[%H:%M:%S:%e]%L|%@| %v");

    int result = Catch::Session().run( argc, argv );

    // global clean-up...

    return result;
}