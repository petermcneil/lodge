#!/bin/bash
UPGRADE=('cmake')
INSTALL=('ffmpeg')
CUR_DIR=$(pwd)
PARENT_DIR=${CUR_DIR%'/scripts'}

printList(){
    for item in $1 ; do
        printf '%s\n' ${item}
    done
}

if [[ $1 == 'darwin' ]]; then
    printf '%s\n' '-----------------------------------------------'
    printf '%s\n' 'OS:           Mac'
    printf '%s\n' 'Build System: Homebrew'
    printf '%s\n' '-----------------------------------------------'
    
    printf '%s\n' 'Updating brew'
    brew update

    printf '%s\n' 'Upgrading the packages:'
    printList ${UPGRADE}
    for item in ${UPGRADE} ; do
        brew upgrade ${item}
    done

    printf 'Installing the packages:'
    printList ${INSTALL}
    for item in ${INSTALL} ; do
        brew install ${item}
    done
fi

printf '%s\n' '-----------------------------------------------'
printf '%s\n' 'Installing/Updating platform independent code'
printf '%s\n' '-----------------------------------------------'
printf '%s\n' 'Updating catch2'
curl -o '$PARENT_DIR/include/catch/catch.hpp' https://raw.githubusercontent.com/catchorg/Catch2/master/single_include/catch2/catch.hpp