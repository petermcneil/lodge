#include "catch.hpp"
#include <Encoder.h>

TEST_CASE("Bytes are flattened correctly") {
    unsigned long output;

    SECTION("1 at end") {
        unsigned long input = 100001UL;

        output = Encoder::flatten_bit(input);
        REQUIRE(output == 100000UL);

    }

    SECTION("0 at end") {
        unsigned long input = 100000UL;

        output = Encoder::flatten_bit(input);
        REQUIRE(output == 100000UL);
    }
}