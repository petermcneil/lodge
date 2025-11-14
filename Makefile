.PHONY: clean deps build test run read install-conan help docker-build docker-image

help:
	@echo "Lodge Build System - Conan 2 Edition"
	@echo "====================================="
	@echo ""
	@echo "Local Development:"
	@echo "  make install-conan  - Install Conan package manager"
	@echo "  make deps          - Install dependencies via Conan"
	@echo "  make build         - Build the CLI application"
	@echo "  make test          - Run tests"
	@echo "  make run           - Build and run encode example"
	@echo "  make read          - Read encoded video"
	@echo "  make clean         - Clean build artifacts"
	@echo ""
	@echo "Docker Build:"
	@echo "  make docker-image  - Build Docker image"
	@echo "  make docker-build  - Build project in Docker"
	@echo ""

# Install Conan if not present
install-conan:
	@command -v conan >/dev/null 2>&1 || { \
		echo "Installing Conan..."; \
		pip3 install --user "conan>=2.0.0"; \
	}
	@echo "Conan version: $$(conan --version)"

# Install dependencies using Conan
deps: install-conan
	@echo "Installing dependencies with Conan..."
	conan profile detect --force || true
	conan install . \
		--output-folder=build \
		--build=missing \
		--settings=build_type=Release

# Build the project
build: deps
	@echo "Building Lodge..."
	cmake --preset conan-release
	cmake --build build/Release

# Run tests
test: build
	@echo "Running tests..."
	cd build/Release && ctest --output-on-failure

# Run encode example
run: build
	@mkdir -p output
	./build/Release/app/ldge write -i -d "resources/videos/Time Lapse Video Of Night Sky.mp4" \
		-s "resources/subtitles/proper_test.srt" -o "output/test.mp4"

# Read encoded video
read: run
	./build/Release/app/ldge read -i -d "output/test.mp4"

# Clean build artifacts
clean:
	rm -rf build/
	rm -rf output/
	rm -rf CMakeUserPresets.json

# Docker targets
docker-image:
	docker build -t lodge:latest .

docker-build: docker-image
	docker run --rm -v $(PWD):/workspace lodge:latest

# Legacy compatibility (for old workflows)
release: build
