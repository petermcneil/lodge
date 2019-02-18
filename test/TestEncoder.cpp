#include "catch.hpp"
#include <Encoder.h>
#include <iostream>
using namespace std;

TEST_CASE("Bytes are flattened correctly") {
    unsigned long input;

    SECTION("1 at end") {
        input = 100001UL;

        lodge::lsb<unsigned long>::flatten_bit(input);
        REQUIRE(input == 100000UL);
    }

    SECTION("0 at end") {
        input = 100000UL;

        lodge::lsb<unsigned long>::flatten_bit(input);
        REQUIRE(input == 100000UL);
    }
}

TEST_CASE("Lsb is set correctly") {
    unsigned long input;
    unsigned long replacement;

    int intInput;
    int intReplacement;

    SECTION("Replace 0 with 1") {
        input = 100000UL;
        replacement = 1UL;

        intInput = 100000;
        intReplacement = 1;

        lodge::lsb<unsigned long>::write_lsb(input, replacement);
        lodge::lsb<int>::write_lsb(intInput, intReplacement);
        REQUIRE(input == 100001UL);
        REQUIRE(intInput == 100001);
    }

    SECTION("Replace 0 with 0") {
        input = 100000UL;
        replacement = 0UL;

        intInput = 100000;
        intReplacement = 0;

        lodge::lsb<unsigned long>::write_lsb(input, replacement);
        lodge::lsb<int>::write_lsb(intInput, intReplacement);
        REQUIRE(input == 100000UL);
        REQUIRE(intInput == 100000);
    }

    SECTION("Replace 1 with 1") {
        input = 100001UL;
        replacement = 1UL;

        intInput = 100001;
        intReplacement = 1;

        lodge::lsb<unsigned long>::write_lsb(input, replacement);
        lodge::lsb<int>::write_lsb(intInput, intReplacement);
        REQUIRE(input == 100001UL);
        REQUIRE(intInput == 100001);
    }

    SECTION("Replace 1 with 0") {
        input = 100001UL;
        replacement = 0UL;

        intInput = 100001;
        intReplacement = 0;

        lodge::lsb<unsigned long>::write_lsb(input, replacement);
        lodge::lsb<int>::write_lsb(intInput, intReplacement);
        REQUIRE(input == 100000UL);
        REQUIRE(intInput == 100000);
    }

}

TEST_CASE("Get Lsb correctly") {
    unsigned long input;
    unsigned long output;

    SECTION("Get 0 Lsb") {
        input = 100000UL;

        output = lodge::lsb<unsigned long>::read_lsb(input);
        REQUIRE(output == 0UL);
    }


    SECTION("Get 1 Lsb") {
        input = 100001UL;

        output = lodge::lsb<unsigned long>::read_lsb(input);
        REQUIRE(output == 1UL);
    }

}

TEST_CASE("Perform LSB replacement on an element of arrays") {
    int input[] = {100000};
    int replacement[] = {1};

    lodge::lsb<int>::write_lsb_array(input, replacement);

    REQUIRE(input[0] == 100001);
}