#!/bin/bash
UPGRADE=('cmake')
INSTALL=('ffmpeg', 'boost')
CUR_DIR=$(pwd)
PARENT_DIR=${CUR_DIR%'/scripts'}

printList(){
    for item in $1 ; do
        printf '%s\n' ${item}
    done
}

printLn() {
    printf '%s\n' $1
}

printSystem() {
    printLn '-----------------------------------------------'
    printLn "OS:           $1"
    printLn "Build System: $2"
    printLn '-----------------------------------------------'
}

if [[ $1 == 'darwin' ]]; then
    printSystem "Mac" "Homebrew"
    printLn 'Updating brew'
    brew update

    printLn 'Upgrading the packages:'
    printList ${UPGRADE}
    for item in ${UPGRADE} ; do
        brew upgrade ${item}
    done

    printLn 'Installing the packages:'
    printList ${INSTALL}
    for item in ${INSTALL} ; do
        brew install ${item}
    done
fi

printLn '-----------------------------------------------'
printLn 'Installing/Updating platform independent code'
printLn '-----------------------------------------------'
printLn 'Updating catch2'
curl -o "$PARENT_DIR/include/catch/catch.hpp" https://raw.githubusercontent.com/catchorg/Catch2/master/single_include/catch2/catch.hpp