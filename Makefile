.PHONY: clean build run test

clean:
	rm -rf build/
build:
	mkdir -p ./build
	cmake -B./build -H./
	make -C ./build

run: build
	./build/src/lodge

test: build
	./build/test/tests
