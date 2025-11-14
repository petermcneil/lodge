# Modern Lodge Build with Conan 2
FROM ubuntu:24.04 AS builder

# Set timezone
ENV TZ=Europe/London
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Install build essentials and dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    python3 \
    curl \
    ca-certificates \
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
    libxcb-res0-dev \
    && rm -rf /var/lib/apt/lists/*

# Create non-root user for builds
RUN useradd -m -u 1000 builder && \
    mkdir -p /workspace && \
    chown -R builder:builder /workspace

USER builder
WORKDIR /workspace

# Install uv and setup Python virtual environment
RUN curl -LsSf https://astral.sh/uv/install.sh | sh && \
    echo 'export PATH="$HOME/.local/bin:$PATH"' >> ~/.bashrc

# Copy pyproject.toml first for dependency installation
COPY --chown=builder:builder pyproject.toml .

# Create venv and install Python dependencies
RUN /home/builder/.local/bin/uv venv && \
    /home/builder/.local/bin/uv pip install -r pyproject.toml

# Configure Conan default profile
RUN .venv/bin/conan profile detect --force

# Copy remaining project files
COPY --chown=builder:builder . .

# Install dependencies via Conan
RUN .venv/bin/conan install . \
    --output-folder=build \
    --build=missing \
    --settings=build_type=Release

# Build the project
RUN cmake --preset conan-release && \
    cmake --build build/build/Release

# The built binary will be at: build/build/Release/app/ldge
CMD ["/bin/bash"]
