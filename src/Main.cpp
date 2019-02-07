#include <stdio.h>
#include "VideoFile.h"
#include <string>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <spdlog/spdlog.h>

using namespace std;
using namespace boost;
namespace po = boost::program_options;



int main(int ac, char *av[]) {
    try {
        po::variables_map vm;
        int ret;
        filesystem::path input;
        filesystem::path output;

        string currentPathS = filesystem::current_path().generic_string();
        std::size_t lodge = currentPathS.find("lodge");

        currentPathS = currentPathS.substr(0, lodge + 5);

        po::options_description debug("Debug options");

        debug.add_options()
                ("help,h", "Prints help message")
                ("debug,d", "Debug output")
                ("remove,r", "Remove saved files after making them");

        po::options_description io("Input/Output options");

        io.add_options()
                ("input,i", po::value<string>(), "Path to the input video")
                ("output,o", po::value<string>(), "Output path");


        po::options_description cmdline_options;
        cmdline_options.add(io).add(debug);

        po::store(po::command_line_parser(ac, av).options(cmdline_options).run(), vm);
        po::notify(vm);

        if (vm.count("help")) {
            cout << cmdline_options << "\n";
            return 1;
        }

        if (vm.count("debug")) {
            spdlog::set_level(spdlog::level::debug);
            spdlog::debug("Logging level set to DEBUG");
        } else {
            spdlog::info("Logging level set to INFO");
        }

        if (vm.count("input")) {
            input = filesystem::path(vm["input"].as<string>());
        } else {
            input = filesystem::path(currentPathS + "/samples/night/Time Lapse Video Of Night Sky.avi");
        }

        if (vm.count("output")) {
            output = filesystem::path(vm["output"].as<string>());
        } else {
            output = filesystem::path(currentPathS);
        }

        VideoFile *video = new VideoFile(input, output);
        ret = video->saveFrames(7);

        if (ret != 0) {
            spdlog::error("Failed to save video frames");
            exit(ret);
        } else {
            spdlog::info("Successfully saved video frames");
        }

        if (vm.count("remove")) {
            video->delete_saved_frames();
        }

        return EXIT_SUCCESS;

    } catch (std::exception &e) {
        spdlog::error(e.what());
        return EXIT_FAILURE;
    }
    catch (...) {
        spdlog::error("Unknown exception");
        return EXIT_FAILURE;
    }
}
