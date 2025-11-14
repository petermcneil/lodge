.PHONY: clean deps build test run read venv install-conan help docker-build docker-image

help:
	@echo "Lodge Build System - Conan 2 Edition"
	@echo "====================================="
	@echo ""
	@echo "Local Development:"
	@echo "  make venv          - Setup Python virtual environment with uv"
	@echo "  make install-conan - Install Conan package manager (via venv)"
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

# Setup Python virtual environment
venv:
	@if [ ! -d ".venv" ]; then \
		echo "Setting up Python virtual environment with uv..."; \
		if ! command -v uv >/dev/null 2>&1; then \
			echo "Installing uv..."; \
			curl -LsSf https://astral.sh/uv/install.sh | sh; \
			export PATH="$$HOME/.local/bin:$$PATH"; \
		fi; \
		uv venv; \
		uv pip install -r pyproject.toml; \
		echo "✅ Virtual environment ready!"; \
		echo "To activate: source .venv/bin/activate"; \
	else \
		echo "Virtual environment already exists"; \
	fi

# Install Conan if not present
install-conan: venv
	@if [ -f ".venv/bin/conan" ]; then \
		echo "Conan version: $$(.venv/bin/conan --version)"; \
	else \
		echo "ERROR: Conan not found in venv. Run 'make venv' first."; \
		exit 1; \
	fi

# Install dependencies using Conan
deps: install-conan
	@echo "Installing dependencies with Conan..."
	.venv/bin/conan profile detect --force || true
	.venv/bin/conan install . \
		--output-folder=build \
		--build=missing \
		--settings=build_type=Release \
		-c tools.build:jobs=$$(nproc)

# Build the project
build: deps
	@echo "Building Lodge..."
	cmake --preset conan-release
	cmake --build build/build/Release -j $$(nproc)

# Run tests
test: build
	@echo "Running tests..."
	cd build/build/Release && ctest --output-on-failure

# Run encode example
run: build
	@mkdir -p output
	./build/build/Release/app/ldge write -i -d "resources/videos/Time Lapse Video Of Night Sky.mp4" \
		-s "resources/subtitles/proper_test.srt" -o "output/test.mp4"

# Read encoded video
read: run
	./build/build/Release/app/ldge read -i -d "output/test.mp4"

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
