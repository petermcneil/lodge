.PHONY: clean build run test

clean:
	rm -rf build/
	rm -rf test.*

build:
	@mkdir -p ./build
	@cmake -B./build -H./
	@make -C ./build

run: build
	build/src/lodge write -i "extras/samples/videos/Time Lapse Video Of Night Sky.mp4" -s "extras/samples/subtitles/actual_subtitle_file.srt" -o "test.mp4"

read: run
	build/src/lodge read -i "test.mp4" -o "test.srt"

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