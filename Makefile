.PHONY: clean build run test

clean:
	rm -rf build/

build:
	@mkdir -p ./build
	@cmake -B./build -H./
	@make -C ./build

run: build
	./build/src/lodge -i "samples/night/Time Lapse Video Of Night Sky.avi" -s "samples/night/subtitle.srt" -d

test: build
	./build/test/lodge_tests

tests: test

clean_test:
	rm -rf *.pgm