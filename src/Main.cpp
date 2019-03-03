#include <stdio.h>
#include "VideoFile.h"
#include "SubtitleFile.h"
#include <string>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <spdlog/spdlog.h>

using namespace std;
using namespace lodge;
using namespace boost;
namespace po = boost::program_options;

int main(int ac, char *av[]) {
    try {
        po::variables_map vm;
        int ret;
        filesystem::path input;
        filesystem::path output;
        filesystem::path subtitle;
        po::options_description io("Input/Output options");
        po::options_description utility("Utility options");
        po::positional_options_description encode;
        po::positional_options_description decode;

        utility.add_options()
                ("help,h", "Prints help message")
                ("debug,d", "Debug output")
                ("remove,r", "Remove saved files after making them");

        io.add_options()
                ("combine", po::value<std::string>(), "Combine subtitle and video file.")
                ("extract", po::value<std::string>(), "Extract subtitle from a video file.")
                ("input,i", po::value<string>()->required(), "Path to the input video")
                ("output,o", po::value<string>(), "Output path")
                ("subtitle,s", po::value<string>()->required(), "Output path");

        encode.add("combine", 1);
        encode.add("extract", 1);

        po::options_description cmdline_options;
        cmdline_options.add(io).add(utility);

        auto parsed = po::command_line_parser(ac, av)
                .options(cmdline_options)
                .positional(encode)
                .run();
        po::store(parsed, vm);

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
        }

        if (vm.count("output")) {
            output = filesystem::path(vm["output"].as<string>());
        } else {
            output = filesystem::current_path();
        }

        if (vm.count("subtitle")) {
            subtitle = filesystem::path(vm["subtitle"].as<string>());
        }

        po::notify(vm);

        SubtitleFile *subtitleFile = new SubtitleFile(subtitle, true);
        VideoFile *video = new VideoFile(input, output, subtitleFile);
        ret = video->bob();

        if (ret != 0) {
            spdlog::error("Failed to save video frames");
            return EXIT_FAILURE;
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
