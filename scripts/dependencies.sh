#!/usr/bin/env bash

set -e

printDash() {
    printf "%$(tput cols)s" | tr ' ' '-'
}

printSystem() {
    printDash
    printf '%s%s\n' "OS:           " "$1"
    printf "%s%s\n" "Package Manager: " "$2"
    printDash
}

install_conan() {
    echo "Checking for Conan..."
    if command -v conan &> /dev/null; then
        echo "Conan already installed: $(conan --version)"
        return 0
    fi

    echo "Installing Conan 2..."
    if command -v pip3 &> /dev/null; then
        pip3 install --user "conan>=2.0.0"
    elif command -v pip &> /dev/null; then
        pip install --user "conan>=2.0.0"
    else
        echo "ERROR: pip3 or pip not found. Please install Python 3 first."
        exit 1
    fi

    echo "Conan installed: $(conan --version)"
}

# Detect OS
if [[ "$OSTYPE" == "darwin"* ]]; then
    printSystem "macOS" "Homebrew"

    # Check if Homebrew is installed
    if ! command -v brew &> /dev/null; then
        echo "ERROR: Homebrew not found. Please install from https://brew.sh"
        exit 1
    fi

    echo "Updating Homebrew..."
    brew update

    printDash
    echo "Installing build dependencies..."
    brew install cmake python3

    install_conan

    echo ""
    printDash
    echo "✅ macOS dependencies installed successfully!"
    echo ""
    echo "Next steps:"
    echo "  1. Run: make deps    (to install C++ libraries via Conan)"
    echo "  2. Run: make build   (to build the project)"
    printDash

elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    if grep -q "Ubuntu\|Debian" /etc/os-release 2>/dev/null; then
        printSystem "Ubuntu/Debian" "apt"

        echo "Updating package lists..."
        sudo apt update

        printDash
        echo "Installing build dependencies..."
        sudo apt install -y \
            build-essential \
            cmake \
            git \
            python3 \
            python3-pip \
            pkg-config

        install_conan

        echo ""
        printDash
        echo "✅ Ubuntu dependencies installed successfully!"
        echo ""
        echo "Next steps:"
        echo "  1. Run: make deps    (to install C++ libraries via Conan)"
        echo "  2. Run: make build   (to build the project)"
        printDash
    else
        echo "ERROR: Unsupported Linux distribution"
        echo "This script supports Ubuntu/Debian only"
        exit 1
    fi
else
    echo "ERROR: Unsupported operating system: $OSTYPE"
    echo "Supported: macOS, Ubuntu/Debian Linux"
    exit 1
fi
