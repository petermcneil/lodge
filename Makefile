.PHONY: clean build run test

clean:
	rm -rf build/
	rm -rf test.*

build:
	@mkdir -p ./build
	@cmake -B./build -H./
	@make -C ./build

run: build
	./build/src/lodge -i "extras/samples/videos/Time Lapse Video Of Night Sky.mp4" -o "test.mp4" -s "extras/samples/subtitles/actual_subtitle_file.srt" -d

read: run
	./build/src/lodge -i "extras/samples/videos/Time Lapse Video Of Night Sky.mp4" -o "test.mp4" -s "test.srt" --read -d

test:
	@mkdir -p ./build
	@cmake -B./build -H./
	@make -C ./build lodge_tests
	./build/test/lodge_tests

tests: test