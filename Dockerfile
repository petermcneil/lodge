# Modern Lodge Build with Conan 2
FROM ubuntu:24.04 AS builder

# Set timezone
ENV TZ=Europe/London
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Install build essentials and Conan dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    python3 \
    python3-pip \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*

# Install Conan 2
RUN pip3 install --no-cache-dir "conan>=2.0.0" --break-system-packages

# Create non-root user for builds
RUN useradd -m -u 1000 builder && \
    mkdir -p /workspace && \
    chown -R builder:builder /workspace

USER builder
WORKDIR /workspace

# Configure Conan default profile
RUN conan profile detect --force

# Copy project files
COPY --chown=builder:builder . .

# Install dependencies via Conan
RUN conan install . \
    --output-folder=build \
    --build=missing \
    --settings=build_type=Release

# Build the project
RUN cmake --preset conan-release && \
    cmake --build build/Release

# The built binary will be at: build/Release/app/ldge
CMD ["/bin/bash"]
