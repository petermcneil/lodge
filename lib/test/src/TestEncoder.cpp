#include "catch.hpp"
#include <encoder.h>
#include <iostream>
#include <vector>

using namespace std;
using namespace lodge;

TEST_CASE("Bytes are flattened correctly") {
    unsigned long input;

    SECTION("1 at end") {
        input = 100001UL;

        lsb<unsigned long>::flatten_bit(input);
        REQUIRE(input == 100000UL);
    }

    SECTION("0 at end") {
        input = 100000UL;

        lsb<unsigned long>::flatten_bit(input);
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

        lsb<unsigned long>::write_lsb(input, replacement);
        lsb<int>::write_lsb(intInput, intReplacement);
        REQUIRE(input == 100001UL);
        REQUIRE(intInput == 100001);
    }

    SECTION("Replace 0 with 0") {
        input = 100000UL;
        replacement = 0UL;

        intInput = 100000;
        intReplacement = 0;

        lsb<unsigned long>::write_lsb(input, replacement);
        lsb<int>::write_lsb(intInput, intReplacement);
        REQUIRE(input == 100000UL);
        REQUIRE(intInput == 100000);
    }

    SECTION("Replace 1 with 1") {
        input = 100001UL;
        replacement = 1UL;

        intInput = 100001;
        intReplacement = 1;

        lsb<unsigned long>::write_lsb(input, replacement);
        lsb<int>::write_lsb(intInput, intReplacement);
        REQUIRE(input == 100001UL);
        REQUIRE(intInput == 100001);
    }

    SECTION("Replace 1 with 0") {
        input = 100001UL;
        replacement = 0UL;

        intInput = 100001;
        intReplacement = 0;

        lsb<unsigned long>::write_lsb(input, replacement);
        lsb<int>::write_lsb(intInput, intReplacement);
        REQUIRE(input == 100000UL);
        REQUIRE(intInput == 100000);
    }

}

TEST_CASE("Get Lsb correctly") {
    unsigned long input;
    unsigned long output;

    SECTION("Get 0 Lsb") {
        input = 100000UL;

        output = lsb<unsigned long>::read_lsb(input);
        REQUIRE(output == 0UL);
    }


    SECTION("Get 1 Lsb") {
        input = 100001UL;

        output = lsb<unsigned long>::read_lsb(input);
        REQUIRE(output == 1UL);
    }

}

TEST_CASE("Replace LSB in elements of an array") {
    int input[] = {111110, 100001};
    int replacement[] = {1, 1};

    lsb<int>::write_lsb_array(input, replacement);

    REQUIRE(input[0] == 111111);
    REQUIRE(input[1] == 100001);
}

TEST_CASE("Read LSB from an array") {
    int input[] = {111110, 111101, 100000, 100000, 100000, 100001, 100000, 100000};

    vector<int> output = lsb<int>::read_lsb_array(input);

    REQUIRE(output.size() == 8);
    REQUIRE(output[0] == 0);
    REQUIRE(output[1] == 1);
    REQUIRE(output[2] == 0);
    REQUIRE(output[3] == 0);
    REQUIRE(output[4] == 0);
    REQUIRE(output[5] == 1);
    REQUIRE(output[6] == 0);
    REQUIRE(output[7] == 0);
}