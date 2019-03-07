.PHONY: clean build run test

clean:
	rm -rf build/

build:
	@mkdir -p ./build
	@cmake -B./build -H./
	@make -C ./build

run: build
	./build/src/lodge -i "extras/samples/night/Time Lapse Video Of Night Sky.mp4" -o "test.mp4" -s "extras/samples/night/subtitle.srt"

read: run
	./build/src/lodge -i "extras/samples/night/Time Lapse Video Of Night Sky.mp4" -o "test.mp4" -s "extras/samples/night/subtitle.srt" --read

test:
	@mkdir -p ./build
	@cmake -B./build -H./
	@make -C ./build lodge_tests
	./build/test/lodge_tests

tests: test