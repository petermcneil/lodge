#!/bin/bash
declare -a UPGRADE=("cmake" "boost")
declare -a INSTALL=("ffmpeg")
CUR_DIR=$(pwd)
PARENT_DIR=${CUR_DIR%'/scripts'}

printDash() {
    printLn '-----------------------------------------------'
}

printList(){
    for item in $1 ; do
        printf '%s\n' ${item}
    done
}

printLn() {
    printf '%s\n' $1
}

printSystem() {
    printDash
    printf '%s%s\n' "OS:           " $1
    printf "%s%s\n" "Build System: " $2
    printDash
}


if [[ $1 == 'darwin' ]]; then
    printSystem "Mac" "Homebrew"
    printLn 'Updating brew'
    brew update

    printDash
    printLn 'Upgrading the packages:'
    printDash
    printList ${UPGRADE[@]}
    for item in ${UPGRADE[@]} ; do
        brew upgrade ${item}
    done

    printLn 'Installing the packages:'
    printList ${INSTALL[@]}
    for item in ${INSTALL[@]} ; do
        brew install ${item}
    done
fi

printDash
printLn 'Installing/Updating platform independent code'
printDash
printLn 'Updating catch2'
curl -o "$PARENT_DIR/include/catch/catch.hpp" https://raw.githubusercontent.com/catchorg/Catch2/master/single_include/catch2/catch.hpp