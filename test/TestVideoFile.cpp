#include <VideoFile.h>
#include "boost/filesystem.hpp"
#include "catch.hpp"
#include <vector>

using namespace boost::filesystem;
using namespace lodge;

path night_video("extras/samples/night/Time Lapse Video Of Night Sky.avi");
path subtitle_file("extras/samples/night/subtitle.srt");
path output_file("build/test");
