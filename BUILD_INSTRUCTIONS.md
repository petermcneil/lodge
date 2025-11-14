# Lodge Build Instructions

Modern build system using **Conan 2** for cross-platform C++ dependency management.

---

## 🚀 Quick Start

### Prerequisites
- **macOS**: Xcode Command Line Tools, Homebrew
- **Ubuntu 24.04+**: build-essential, cmake, python3

### One-Command Setup
```bash
# Install system dependencies and Conan
./scripts/dependencies.sh

# Build the project
make build
```

---

## 📋 Detailed Instructions

### 1. Install System Dependencies

#### macOS
```bash
# Install Homebrew if not present
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Run dependency script
./scripts/dependencies.sh
```

#### Ubuntu 24.04+
```bash
# Run dependency script (installs build-essential, cmake, python3, pip)
./scripts/dependencies.sh
```

### 2. Install C++ Dependencies via Conan

```bash
# This will download and build all C++ libraries
make deps
```

**What gets installed:**
- Boost 1.84.0 (filesystem, program_options, regex)
- FFmpeg 6.1 (video processing)
- spdlog 1.13.0 (logging)
- Catch2 3.5.2 (testing)

**First time:** ~5-15 minutes (downloads pre-compiled binaries when available)
**Subsequent builds:** Instant (cached)

### 3. Build the Project

```bash
make build
```

This will:
1. Configure CMake with Conan toolchain
2. Build the `ldge` CLI application
3. Build the static library
4. Build tests

**Output:** `build/Release/app/ldge`

---

## 🔧 Development Workflow

### Build Commands

```bash
make help              # Show all available commands
make clean             # Clean build artifacts
make build             # Full build
make test              # Run tests
make run               # Encode test video with subtitles
make read              # Read subtitles from encoded video
```

### Manual CMake (Advanced)

```bash
# Install dependencies
conan install . --output-folder=build --build=missing --settings=build_type=Release

# Configure
cmake --preset conan-release

# Build
cmake --build build/Release

# Run
./build/Release/app/ldge --help
```

---

## 🐳 Docker Build

### Build and run in Docker:

```bash
# Build Docker image (Ubuntu 24.04 + Conan)
make docker-image

# Build project inside Docker
make docker-build
```

### Manual Docker:

```bash
docker build -t lodge:latest .
docker run --rm -v $(PWD):/workspace lodge:latest
```

---

## 🧪 Testing

```bash
# Run all tests
make test

# Run tests manually
cd build/Release
ctest --output-on-failure
```

---

## 🔍 Troubleshooting

### "conan: command not found"
```bash
# Ensure pip user bin is in PATH
export PATH="$HOME/.local/bin:$PATH"

# Or reinstall Conan
pip3 install --user "conan>=2.0.0"
```

### CMake can't find packages
```bash
# Re-run conan install
make clean
make deps
```

### Build fails with missing symbols
```bash
# Clean and rebuild everything
make clean
rm -rf ~/.conan2/p  # Clear Conan cache
make build
```

### macOS: "xcrun: error: invalid active developer path"
```bash
# Install Xcode Command Line Tools
xcode-select --install
```

---

## 📦 Package Versions

| Package | Version | Purpose |
|---------|---------|---------|
| Boost | 1.84.0 | Utilities (filesystem, program_options, regex) |
| FFmpeg | 6.1 | Video encoding/decoding |
| spdlog | 1.13.0 | Fast logging |
| Catch2 | 3.5.2 | Unit testing framework |

---

## 🏗️ Project Structure

```
lodge/
├── app/              # CLI application source
├── lib/              # Core library (video processing, encoding)
├── gui/              # Qt5 GUI (optional, not built by default)
├── conanfile.txt     # Conan dependency specification
├── CMakeLists.txt    # Root CMake configuration
├── Makefile          # High-level build commands
├── Dockerfile        # Containerized build
└── scripts/
    └── dependencies.sh  # System dependency installer
```

---

## 🌍 Cross-Platform Notes

### macOS Specific
- Uses Clang compiler by default
- Conan will download macOS ARM64 binaries (Apple Silicon) or x86_64 (Intel)

### Ubuntu Specific
- Uses GCC compiler by default
- Requires `pkg-config` for FFmpeg detection

### Windows
- Not currently supported (contributions welcome!)
- Would require MSVC toolchain and additional CMake configuration

---

## 🚀 Performance

**Build Times (approximate):**

| Stage | First Build | Subsequent |
|-------|-------------|------------|
| `make deps` | 5-15 min | Instant |
| `make build` | 1-2 min | 10-30 sec |
| **Total** | **6-17 min** | **< 1 min** |

*Compare to vcpkg: ~30-60 minutes first build*

---

## 📚 Learn More

- **Conan Documentation**: https://docs.conan.io/2/
- **CMake Documentation**: https://cmake.org/documentation/
- **Project README**: [README.md](README.md)

---

## ❓ Getting Help

1. Run `make help` for available commands
2. Check [GitHub Issues](../../issues) for known problems
3. Review Conan logs in `build/conan.log`

---

**Built with ❤️ using Conan 2 for modern C++ package management**
