.PHONY: clean build run test

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
	build/app/lodge write -i -d "extras/samples/videos/Time Lapse Video Of Night Sky.mp4" -s "extras/samples/subtitles/proper_test.srt" -o "output/test.mp4"

read: run
	build/app/lodge read -i -d "output/test.mp4" -o "output/test.srt"

test:
	@mkdir -p ./build
	@cmake -B./build -H./
	@make -C ./build lodge_tests
	./build/lib/test/lodge_tests

tests: test

release:
	@mkdir -p ./release
	@cmake -B./release -H./
	@make -C ./release

gui: build
	open build/gui/Lodge.app

gdb: build
	gdb -ex=r --args build/app/lodge write -i -d extras/samples/videos/blah.mp4 -s extras/samples/subtitles/proper_test.srt -o output/test.mp4