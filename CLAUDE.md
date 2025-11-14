# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Lodge is a video steganography system that embeds subtitle files directly into video frames using LSB (Least Significant Bit) encoding. The project consists of:

- **lodge library** (`lib/`): Core steganography implementation using FFmpeg for video processing
- **ldge CLI** (`app/`): Command-line tool for encoding/decoding subtitles
- **lodge GUI** (`gui/`): Qt5-based graphical interface (optional, not built by default)

The system works by writing a custom frame header to each frame followed by subtitle data encoded in the least significant bits of pixel values.

## Build System

### Modern Stack (Current)
- **Package Manager**: Conan 2 (replacing deprecated vcpkg)
- **Python Environment**: uv + virtual environment at `.venv/`
- **Build System**: CMake with Conan-generated toolchain
- **FFmpeg Version**: 7.1.2 (modern API, see video.cpp for migration from deprecated API)

### Essential Build Commands

```bash
# Setup (first time only)
./scripts/dependencies.sh  # Install system deps and create .venv
source .venv/bin/activate  # Activate Python venv (or let Makefile handle it)

# Development workflow
make build                 # Full build (runs deps + cmake + build)
make test                  # Run Catch2 tests
make clean                 # Clean build artifacts

# Testing encode/decode
make run                   # Encode test video with subtitles
make read                  # Read subtitles from encoded video

# Manual workflow (if needed)
make deps                  # Install C++ dependencies via Conan
cmake --preset conan-release
cmake --build build/build/Release
```

**Important**: The Makefile automatically uses `.venv/bin/conan`, so you don't need to activate the venv for make commands. If running conan directly, activate with `source .venv/bin/activate`.

### Build Output Locations
- Binary: `build/build/Release/app/ldge`
- Tests: `build/build/Release/lib/test/lodge_tests`
- Generated files: `build/generators/` (Conan CMake files)

## Architecture

### Core Components

**video.h/cpp** - Main steganography engine
- Manages FFmpeg contexts (format, codec, filter)
- Implements frame-level encoding/decoding
- **Key methods**:
  - `write_subtitle_file()`: Encodes subtitle into video frames
  - `read_subtitle_file()`: Extracts subtitle from video frames
  - `has_steg_file()`: Detects if video contains embedded subtitle
  - `write_steg_header()` / `read_steg_header()`: Frame header I/O

**encoder.h** - LSB steganography implementation
- Template-based LSB manipulation: `lsb<T>`
- `write_lsb()` / `read_lsb()`: Single bit operations
- `flatten_bit()`: Zero out LSB before writing

**subtitle.h/cpp** - Subtitle file handling
- Reads/writes .srt files line-by-line
- Converts between char and bitset representation
- Tracks file size and maintains read/write state

**frame_header.h/cpp** - Metadata format
- Format: `|L|(char count)|(filename)|(total frames)|(frame number)|L|`
- Enables multi-frame subtitle distribution
- Regex-based parsing for robustness

### FFmpeg Integration

**Recent Migration** (commit ed2338f): Upgraded from FFmpeg 4.x to 7.1.2
- Replaced deprecated `av_init_packet()` with direct allocation
- Updated `avcodec_send_frame()` / `avcodec_receive_packet()` API
- Migrated to `AVChannelLayout` from legacy channel APIs
- Fixed pointer ownership (avoid double-free in `avcodec_free_context()`)

**Recent Fixes** (FFmpeg 7.1.2 compatibility):
- **Filter initialization**: Use encoder's `time_base` (not decoder's) for filter buffer source (video.cpp:523)
- **PTS generation**: Generate sequential PTS values for videos without timestamps using per-stream `frame_count` (video.cpp:914)
- **Encoder time_base validation**: Fallback to stream `time_base` if framerate is invalid (video.cpp:416-421)
- **EOF handling**: Treat `AVERROR_EOF` as normal during encoder flush, not as error (video.cpp:712)
- **Steganography pict_type preservation**: Don't reset `filt_frame->pict_type` to NONE after filtering - preserves I-frame type for LSB encoding (video.cpp:790)
- **I-frame detection**: Check `filt_frame->pict_type` instead of stale `frame->pict_type` for steganography (video.cpp:687)

**Critical FFmpeg Patterns**:
- Always check return codes from avcodec/avformat functions
- Use RAII-style cleanup with `avformat_close_input()`, `avcodec_free_context()`
- Video stream found with `av_find_best_stream()`
- Filtering pipeline: `buffersrc` → custom filters → `buffersink`
- **Filter time_base**: Always use encoder's `time_base` for filter initialization, never decoder's (often 0/1)
- **PTS handling**: If `best_effort_timestamp` is `AV_NOPTS_VALUE`, generate PTS from frame count

### Data Flow

**Encoding (Write)**:
1. Open input video with `open_input_file()`
2. Initialize output video with `open_output_file()`
3. Generate frame headers with subtitle metadata
4. For each frame:
   - Write frame header to top-left pixels (LSB encoding)
   - Write subtitle line bits across remaining pixels
   - Encode and mux to output video
5. Flush encoder and close contexts

**Decoding (Read)**:
1. Open video file and find video stream
2. Detect steganography with `has_steg_file()`
3. Read frame header to get metadata
4. Extract LSB bits from pixels
5. Reconstruct subtitle characters
6. Write to output .srt file

## Dependencies

Managed via `conanfile.txt`:
- **Boost 1.84.0**: filesystem, program_options, regex (static)
- **FFmpeg 7.1.2**: Video codec/format/filter APIs (static, ~160MB in binary)
- **spdlog 1.13.0**: Fast logging (static)
- **Catch2 3.5.2**: Unit testing framework

**Platform-specific**:
- macOS: Requires Xcode Command Line Tools
- Ubuntu 24.04+: Requires build-essential, cmake, pkg-config

## Testing

Uses Catch2 v3 framework. Tests in `lib/test/src/`:
- `TestEncoder.cpp`: LSB encoding tests (all passing)
- `TestSubtitleFile.cpp`: Subtitle parsing tests (all passing)
- `TestVideoFile.cpp`: Video I/O integration tests (some segfaults in video encoding)

**Test resource paths**: The test CMakeLists.txt defines `TEST_RESOURCES_DIR` pointing to the source directory, so tests can find subtitle/video files regardless of build directory location.

**Run tests**:
```bash
make test
# Or manually:
cd build/build/Release
ctest --output-on-failure

# Run specific test:
./lib/test/lodge_tests "test name"
```

**Current status**: All tests pass (10/10 test cases, 47/47 assertions). FFmpeg filter and steganography issues resolved.

## Common Development Tasks

### Running Single Tests
```bash
./build/build/Release/lib/test/lodge_tests "[test name]"
```

### Debugging FFmpeg Issues
Enable debug logging in code:
```cpp
spdlog::set_level(spdlog::level::debug);
av_log_set_level(AV_LOG_DEBUG);  // FFmpeg logging
```

Or use CLI flag:
```bash
./build/build/Release/app/ldge write -d -i video.mp4 -s subs.srt -o out.mp4
```

### Changing FFmpeg Version
Edit `conanfile.txt`:
```
[requires]
ffmpeg/7.1.2  # Change version here
```
Then: `make clean && make deps && make build`

**Note**: FFmpeg is statically linked (`ffmpeg/*:shared=False` in conanfile.txt) to create a portable CLI binary. This results in a ~160MB binary but eliminates runtime dependencies.

### Adding New Dependencies
1. Add to `conanfile.txt` under `[requires]`
2. Update `[options]` if needed (shared/static linking)
3. Update `lib/CMakeLists.txt` with `find_package()` and `target_link_libraries()`
4. Run `make deps`

## File Structure

```
lodge/
├── app/src/lodge.cpp         # CLI entry point with boost::program_options
├── lib/
│   ├── include/              # Public headers
│   │   ├── video.h           # Main steganography engine
│   │   ├── encoder.h         # LSB template implementation
│   │   ├── subtitle.h        # Subtitle file I/O
│   │   └── frame_header.h    # Frame metadata format
│   ├── src/                  # Implementation files
│   └── test/                 # Catch2 tests
├── conanfile.txt             # C++ dependencies (Conan 2 format)
├── pyproject.toml            # Python deps (just Conan)
├── CMakeLists.txt            # Root CMake config
├── Makefile                  # High-level build orchestration
└── Dockerfile                # Ubuntu 24.04 + Conan build environment
```

## Docker Development

Build in containerized environment (Ubuntu 24.04):
```bash
make docker-image   # Build image with dependencies
make docker-build   # Build project inside container
```

Image includes all system dependencies and pre-installed Conan cache.

## Platform Notes

**macOS**:
- Uses Clang by default
- ARM64 (Apple Silicon) or x86_64 (Intel) binaries from Conan
- FFmpeg may need Homebrew for system codecs

**Ubuntu 24.04+**:
- Uses GCC by default
- Requires pkg-config for FFmpeg detection
- May need `libva-dev`, `libvdpau-dev` for hardware acceleration

**Cross-compilation**: Not currently supported

## Code Style

- **Namespace**: All library code in `namespace lodge`
- **Naming**: snake_case for functions/variables, PascalCase for classes
- **Memory**: Manual management with new/delete (legacy codebase)
- **Logging**: Use spdlog for all debug output, not cout/cerr
- **Error handling**: Return int error codes (FFmpeg convention)

## Known Constraints

- **Video formats**: Currently optimized for H.264/H.265 (tested with MP4)
- **Uncompressed intermediates**: May decompress for processing
- **No GUI build by default**: Qt5 GUI exists but requires separate qmake build
- **Single subtitle track**: One .srt file per video

## Build Troubleshooting

**"conan: command not found"**:
```bash
source .venv/bin/activate
# Or recreate venv:
make venv
```

**CMake can't find packages**:
```bash
make clean && make deps
```

**FFmpeg API errors after upgrade**:
Check `lib/src/video.cpp` for current API patterns. FFmpeg 7.x removed many deprecated functions from 4.x/6.x.

## Recent Migrations

The project recently underwent several modernization efforts:

1. **vcpkg → Conan 2** (commit 10a5fc4): Faster builds, better caching
2. **pip → uv** (commit 6e81c34): Modern Python package management
3. **FFmpeg 4.4 → 7.1.2** (commit ed2338f): Latest codec APIs, removed deprecations
4. **Ubuntu 20.04 → 24.04** (commit 0f98fa9): Modern build toolchain

When working on FFmpeg code, reference commit ed2338f for API migration patterns.
