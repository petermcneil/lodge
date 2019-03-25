.PHONY: clean build run test

clean:
	rm -rf build/
	rm -rf test.*

build:
	@mkdir -p ./build
	@cmake -B./build -H./
	@make -C ./build

run: build
	build/src/lodge write -i -d "extras/samples/videos/Time Lapse Video Of Night Sky.mp4" -s "extras/samples/subtitles/got_s01e01.srt" -o "test.mp4"

read: run
	build/src/lodge read -i -d "test.mp4" -o "test.srt"

test:
	@mkdir -p ./build
	@cmake -B./build -H./
	@make -C ./build lodge_tests
	./build/test/lodge_tests

tests: test

release:
	@mkdir -p ./release
	@cmake -B./release -H./
	@make -C ./release