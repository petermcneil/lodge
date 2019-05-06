.PHONY: clean build run test gui release

clean:
	rm -rf build/
	rm -rf release/
	rm -rf output/
	rm -rf Testing/

build:
	@mkdir -p ./build
	@cmake -B./build -H./
	@make -C ./build

run: build
	build/app/lodge write -i -d "resources/videos/Time Lapse Video Of Night Sky.mp4" -s "resources/subtitles/proper_test.srt" -o "output/test.mp4"

read: run
	build/app/lodge read -i -d "output/test.mp4"

test:
	@mkdir -p ./build
	@cmake -B./build -H./
	@make -C ./build lodge_tests
	build/lib/test/lodge_tests

tests: test

release:
	@mkdir -p ./release
	@cmake -B./release -H./
	@make -C ./release

gui: build
	@mkdir -p build/gui
	@qmake -o build/gui
	make -C build/gui -j8
	open build/gui/Lodge.app
