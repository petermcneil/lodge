#!/bin/bash

brew upgrade cmake
curl -o "$1/include/catch/catch.hpp" https://raw.githubusercontent.com/catchorg/Catch2/master/single_include/catch2/catch.hpp