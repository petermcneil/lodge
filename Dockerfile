# Modern Lodge Build with Conan 2
FROM ubuntu:24.04 AS builder

# Set timezone
ENV TZ=Europe/London
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# ============================================
# Layer 1: Core build tools (rarely changes)
# ============================================
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    python3 \
    curl \
    ca-certificates \
    pkg-config \
    nasm \
    && rm -rf /var/lib/apt/lists/*

# ============================================
# Layer 2: Video/FFmpeg dependencies (rarely changes)
# ============================================
RUN apt-get update && apt-get install -y \
    # Hardware acceleration
    libva-dev \
    libvdpau-dev \
    # Video codecs
    libx264-dev \
    libx265-dev \
    # X11 core libraries
    libx11-dev \
    libx11-xcb-dev \
    libfontenc-dev \
    libice-dev \
    libsm-dev \
    # X11 utilities
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
    # XCB libraries
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
    libxcb-res0-dev \
    libxcb-xkb-dev \
    # Other dependencies
    uuid-dev \
    && rm -rf /var/lib/apt/lists/*

# ============================================
# Layer 3: Install uv (rarely changes)
# ============================================
WORKDIR /workspace
RUN curl -LsSf https://astral.sh/uv/install.sh | sh

# ============================================
# Layer 4: Create Python venv (rarely changes)
# ============================================
COPY pyproject.toml .
RUN ~/.local/bin/uv venv

# ============================================
# Layer 5: Install Python dependencies (changes when pyproject.toml changes)
# ============================================
RUN ~/.local/bin/uv pip install -r pyproject.toml

# ============================================
# Layer 6: Configure Conan profile (one-time setup)
# ============================================
RUN .venv/bin/conan profile detect --force

# ============================================
# Layer 7: Install Conan dependencies (changes when conanfile.txt changes)
# ============================================
COPY conanfile.txt .
RUN .venv/bin/conan install . \
    --output-folder=build \
    --build=missing \
    --settings=build_type=Release

# ============================================
# Layer 8: Copy build configuration (changes moderately)
# ============================================
COPY CMakeLists.txt .

# ============================================
# Layer 9: Copy source code (changes frequently)
# ============================================
COPY lib ./lib
COPY app ./app

# ============================================
# Layer 10: Build the project (changes frequently)
# ============================================
# cmake_layout puts generators at build/build/Release/generators/
RUN cmake -B build/build/Release -S . \
    -DCMAKE_TOOLCHAIN_FILE=/workspace/build/build/Release/generators/conan_toolchain.cmake \
    -DCMAKE_BUILD_TYPE=Release && \
    cmake --build build/build/Release

# The built binary will be at: build/build/Release/app/ldge
CMD ["/bin/bash"]
