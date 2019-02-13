#include "catch.hpp"
#include <Encoder.h>

TEST_CASE("Bytes are flattened correctly") {
    unsigned long input;

    SECTION("1 at end") {
        input = 100001UL;

        Encoder::flatten_bit(input);
        REQUIRE(input == 100000UL);
    }

    SECTION("0 at end") {
        input = 100000UL;

        Encoder::flatten_bit(input);
        REQUIRE(input == 100000UL);
    }
}

TEST_CASE("LSB is set correctly") {
    unsigned long input;
    unsigned long replacement;


    SECTION("Replace 0 with 1") {
        input = 100000UL;
        replacement = 1UL;

        Encoder::write_lsb(input, replacement);
        REQUIRE(input == 100001UL);
    }

    SECTION("Replace 0 with 0") {
        input = 100000UL;
        replacement = 0UL;

        Encoder::write_lsb(input, replacement);
        REQUIRE(input == 100000UL);
    }

    SECTION("Replace 1 with 1") {
        input = 100001UL;
        replacement = 1UL;

        Encoder::write_lsb(input, replacement);
        REQUIRE(input == 100001UL);
    }

    SECTION("Replace 1 with 0") {
        input = 100001UL;
        replacement = 0UL;

        Encoder::write_lsb(input, replacement);
        REQUIRE(input == 100000UL);
    }

}

TEST_CASE("Get LSB correctly") {
    unsigned long input;
    unsigned long output;


    SECTION("Get 0 LSB") {
        input = 100000UL;

        output = Encoder::read_lsb(input);
        REQUIRE(output == 0UL);
    }


    SECTION("Get 1 LSB") {
        input = 100001UL;

        output = Encoder::read_lsb(input);
        REQUIRE(output == 1UL);
    }

}
