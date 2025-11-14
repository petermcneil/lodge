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

install_uv() {
    echo "Checking for uv..."
    if command -v uv &> /dev/null; then
        echo "uv already installed: $(uv --version)"
        return 0
    fi

    echo "Installing uv..."
    curl -LsSf https://astral.sh/uv/install.sh | sh

    # Source uv env to make it available in current shell
    export PATH="$HOME/.local/bin:$PATH"

    if ! command -v uv &> /dev/null; then
        echo "ERROR: uv installation failed"
        exit 1
    fi

    echo "uv installed: $(uv --version)"
}

setup_venv() {
    echo "Setting up Python virtual environment..."

    if [ -d ".venv" ]; then
        echo "Virtual environment already exists"
    else
        echo "Creating virtual environment..."
        uv venv
    fi

    echo "Installing Python dependencies..."
    uv pip install -r pyproject.toml

    echo ""
    echo "✅ Python environment ready!"
    echo "To activate: source .venv/bin/activate"
}

install_conan() {
    install_uv
    setup_venv

    echo "Checking for Conan..."
    if [ -f ".venv/bin/conan" ]; then
        echo "Conan installed: $(.venv/bin/conan --version)"
    else
        echo "ERROR: Conan installation failed"
        exit 1
    fi
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

    printDash
    echo "Installing Qt 6 for GUI (optional)..."
    brew install qt@6

    install_conan

    echo ""
    printDash
    echo "✅ macOS dependencies installed successfully!"
    echo ""
    echo "Next steps:"
    echo "  1. Activate venv:    source .venv/bin/activate"
    echo "  2. Run: make deps    (to install C++ libraries via Conan)"
    echo "  3. Run: make build   (to build the project)"
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
            pkg-config \
            libva-dev \
            libvdpau-dev \
            libx264-dev \
            libx265-dev \
            nasm \
            libx11-dev \
            libx11-xcb-dev \
            libfontenc-dev \
            libice-dev \
            libsm-dev \
            libxau-dev \
            libxaw7-dev \
            libxcomposite-dev \
            libxcursor-dev \
            libxdamage-dev \
            libxext-dev \
            libxfixes-dev \
            libxi-dev \
            libxinerama-dev \
            libxkbfile-dev \
            libxmu-dev \
            libxmuu-dev \
            libxpm-dev \
            libxrandr-dev \
            libxrender-dev \
            libxres-dev \
            libxss-dev \
            libxt-dev \
            libxtst-dev \
            libxv-dev \
            libxvmc-dev \
            libxxf86vm-dev \
            libxcb1-dev \
            libxcb-glx0-dev \
            libxcb-render0-dev \
            libxcb-render-util0-dev \
            libxcb-shape0-dev \
            libxcb-randr0-dev \
            libxcb-image0-dev \
            libxcb-keysyms1-dev \
            libxcb-icccm4-dev \
            libxcb-sync-dev \
            libxcb-xfixes0-dev \
            libxcb-shm0-dev \
            libxcb-util-dev \
            libxcb-xinerama0-dev \
            libxcb-dri3-dev \
            libxcb-cursor-dev \
            libxcb-dri2-0-dev \
            libxcb-present-dev \
            libxcb-composite0-dev \
            libxcb-ewmh-dev \
            libxcb-res0-dev

        printDash
        echo "Installing Qt 6 for GUI (optional)..."
        sudo apt install -y \
            qt6-base-dev \
            qt6-declarative-dev \
            qt6-5compat-dev \
            qml6-module-qtcore \
            qml6-module-qtquick \
            qml6-module-qtquick-controls \
            qml6-module-qtquick-dialogs \
            qml6-module-qtquick-layouts \
            qml6-module-qtquick-window \
            qml6-module-qtquick-templates \
            qml6-module-qt-labs-folderlistmodel \
            qml6-module-qt-labs-qmlmodels \
            qml6-module-qtqml-workerscript

        install_conan

        echo ""
        printDash
        echo "✅ Ubuntu dependencies installed successfully!"
        echo ""
        echo "Next steps:"
        echo "  1. Activate venv:    source .venv/bin/activate"
        echo "  2. Run: make deps    (to install C++ libraries via Conan)"
        echo "  3. Run: make build   (to build the project)"
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
