.PHONY: clean build run test

clean:
	rm -rf build/
	rm -rf gen_subs.srt

build:
	@mkdir -p ./build
	@cmake -B./build -H./
	@make -C ./build

run: build
	./build/src/lodge -i "samples/night/Time Lapse Video Of Night Sky.avi" -s "samples/night/subtitle.srt" -d

test:
	@mkdir -p ./build
	@cmake -B./build -H./
	@make -C ./build lodge_tests
	./build/test/lodge_tests

tests: test