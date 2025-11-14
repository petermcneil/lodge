.PHONY: clean build run test gui release

install:
	 git clone https://github.com/Microsoft/vcpkg.git
	 ./vcpkg/bootstrap-vcpkg.sh
	./vcpkg/vcpkg install boost spdlog ffmpeg catch2 qt5
clean:
	rm -rf build/
	rm -rf release/
	rm -rf output/
	rm -rf Testing/
	rm -rf .flatpak-builder/

build:
	docker-compose run builder

image:
	docker build . -t lodge_build

build_docker:
	ls -alG ${VCPKG}/
	${VCPKG}/vcpkg list
	mkdir -p ./build
	cmake -B./build -H./ -DCMAKE_TOOLCHAIN_FILE=${VCPKG}/scripts/buildsystems/vcpkg.cmake
	make -C ./build
run: build
	build/app/lodge write -i -d "resources/videos/Time Lapse Video Of Night Sky.mp4" -s "resources/subtitles/proper_test.srt" -o "output/test.mp4"

read: run
	build/app/lodge read -i -d "output/test.mp4"

test: build
	build/lib/test/lodge_tests

tests: test

release:
	@mkdir -p ./release
	@cmake -B./release -H./
	@make -C ./release

gui: build
	docker-compose run --entrypoint "make gui_docker" builder
	flatpak-builder --ccache --repo=repo --force-clean --subject="Build of AWESOMEAPP `date`" build/flatpak ./gui/mcneil.peter.lodge.yml
gui_docker:
	@mkdir -p build/gui
	mkdir -p ./build
	mkdir -p ./build/flatpak
	ls -al gui
	cmake -B./build/gui -H./gui -DCMAKE_TOOLCHAIN_FILE=${VCPKG}/scripts/buildsystems/vcpkg.cmake
	make -C build/gui -j8
