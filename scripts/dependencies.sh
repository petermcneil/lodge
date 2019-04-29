#!/bin/bash
declare -a UPGRADE=("cmake" "boost")
declare -a INSTALL=("ffmpeg" "spdlog" "qt5")
CUR_DIR=$(pwd)
PARENT_DIR=${CUR_DIR%'/scripts'}

printDash() {
    printf "%`tput cols`s" | tr ' ' '-'
}

printList(){
    for item in $1 ; do
        printf '%s\n' ${item}
    done
}

printSystem() {
    printDash
    printf '%s%s\n' "OS:           " $1
    printf "%s%s\n" "Build System: " $2
    printDash
}


if [[ $1 == 'darwin' ]]; then
    printSystem "Mac" "Homebrew"
    printf "%s\n" 'Updating brew'
    brew update

    printDash
    printf "%s\n" 'Upgrading the packages:'
    printDash
    printList ${UPGRADE[@]}
    for item in ${UPGRADE[@]} ; do
        brew upgrade ${item}
    done

    printf "%s\n" 'Installing the packages:'
    printList ${INSTALL[@]}
    for item in ${INSTALL[@]} ; do
        brew install ${item}
    done
fi

printDash
printf "%s\n" 'Installing/Updating platform independent code'
printDash
printf "%s\n" 'Updating Catch2'
curl -o "$PARENT_DIR/include/catch/catch.hpp" https://raw.githubusercontent.com/catchorg/Catch2/master/single_include/catch2/catch.hpp
