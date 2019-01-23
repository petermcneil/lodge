.PHONY: clean build run test

clean:
	rm -rf build/
	rm -rf *.ppm

build:
	@mkdir -p ./build
	@cmake -B./build -H./
	@make -C ./build

run: build
	./build/src/lodge

test: build
	./build/test/tests

tests: test