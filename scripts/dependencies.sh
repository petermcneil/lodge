#!/bin/bash
UPGRADE=("cmake")
INSTALL=("ffmpeg")

printList(){
    for item in $1 ; do
        echo ${item}
    done
}

echo "Updating brew"
brew update

echo "Upgrading the packages:"
printList ${UPGRADE}
for item in ${UPGRADE} ; do
    brew upgrade ${item}
done

echo "Installing the packages:"
printList ${INSTALL}
for item in ${INSTALL} ; do
    brew install ${item}
done

echo "Updating catch2"
curl -o "$1/include/catch/catch.hpp" https://raw.githubusercontent.com/catchorg/Catch2/master/single_include/catch2/catch.hpp