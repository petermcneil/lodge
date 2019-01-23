#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <VideoFile.h>
#include "catch.hpp"
#include "fact.cpp"
#include "boost/filesystem.hpp"

using namespace boost::filesystem;

path current_dir(current_path());

int framesInDirectory() {
    int count = 0;

    directory_iterator end_itr; // default construction yields past-the-end

    for (directory_iterator itr(current_dir);
         itr != end_itr;
         ++itr) {

        string ext(itr->path().extension().c_str());
        if (ext.find(".ppm") != std::string::npos) {
            count++;
        }
    }

    return count;
}

TEST_CASE("Factorials are computed", "[factorial]") {
    REQUIRE(Factorial(1) == 1);
    REQUIRE(Factorial(2) == 2);
    REQUIRE(Factorial(3) == 6);
    REQUIRE(Factorial(10) == 3628800);
}

TEST_CASE("Save frames outputs five") {
    path file_path = current_dir;
    file_path.append("samples/night/Time Lapse Video Of Night Sky.avi");

    VideoFile *video = new VideoFile(file_path);
    int result = video->saveFrames();
    REQUIRE(result == 0);
    REQUIRE(framesInDirectory() == 5);
}