#!/bin/bash

echo "Updating brew"
brew update
echo "Upgrading cmake"
brew upgrade cmake
echo "Installing ffmpeg"
brew install ffmpeg
echo "Updating catch2"
curl -o "$1/include/catch/catch.hpp" https://raw.githubusercontent.com/catchorg/Catch2/master/single_include/catch2/catch.hpp