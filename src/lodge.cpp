#include <stdio.h>
#include <video.h>
#include <subtitle.h>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <iostream>

using namespace std;
using namespace lodge;
using namespace boost;
namespace po = boost::program_options;
namespace log = spdlog;

int main(int ac, char *av[]) {
    cout << "=======================================\n"
            " | |    / __ \\|  __ \\ / ____|  ____|\n"
            " | |   | |  | | |  | | |  __| |__   \n"
            " | |   | |  | | |  | | | |_ |  __|  \n"
            " | |___| |__| | |__| | |__| | |____ \n"
            " |______\\____/|_____/ \\_____|______|\n"
            "======================================="
            "" << endl;
    cout << "A tool to read and write stegonagraphic information in video files." << endl;
    cout << "Version: 0.1" << endl;
    cout << endl;
    try {
        int ret = 0;
        po::variables_map vm;
        po::options_description general("General options");
        po::options_description read_desc("Read options");
        po::options_description write_desc("Write options");
        po::positional_options_description p;

        read_desc.add_options()
                ("input,i", po::value<string>()->required(), "Input video file")
                ("output,o", po::value<string>(), "Output subtitle file");

        write_desc.add_options()
                ("input_video,i", po::value<string>()->required(), "Input video file")
                ("input_subtitle,s", po::value<string>()->required(), "Input subtitle file")
                ("output_video,o", po::value<string>(), "Output video file");

        general.add_options()
                ("help,h", "Prints help message")
                ("debug,d", "Debug output")
                ("method,m", po::value<string>()->required(), "Method: read or write")
                ("subargs", po::value<std::vector<std::string> >(), "Arguments for read/write");

        p.add("method", 1).add("subargs", -1);

        auto parsed = po::command_line_parser(ac, av)
                .options(general)
                .positional(p)
                .allow_unregistered()
                .run();

        po::store(parsed, vm);

        if (vm.count("help")) {
            cout << general << read_desc << write_desc << "\n";
            return 1;
        }

        log::set_pattern("| %H:%M:%S | %l | %v");

        if (vm.count("debug")) {
            log::set_level(log::level::debug);
            log::debug("Logging level set to DEBUG");
        } else {
            log::set_level(log::level::off);
        }

        po::notify(vm);

        vector<string> opts = po::collect_unrecognized(parsed.options, po::include_positional);
        opts.erase(opts.begin());

        string method = vm["method"].as<string>();

        if (method == "read") {
            cout << "Reading " << endl;
            log::debug("Read option selected");
            po::store(po::command_line_parser(opts).options(read_desc).run(), vm);

            log::debug("Setting input video path");
            string input = vm["input"].as<string>();;
            cout << "Reading from video file: " << input << endl;
            string output;

            log::debug("Checking for output subtitle path");
            if (vm.count("output")) {
                log::debug("Found output subtitle path and setting it");
                output = vm["output"].as<string>();
            } else {
                cout << "Output file is going to be generated from video file";
            }

            log::debug("Building subtitle object");
            subtitle *sub = new subtitle(output, false);
            log::debug("Building video object");
            video *vid = new video(input, sub);

            if (vid->has_steg_file()) {
                cout << "Writing to subtitle file: " << sub->get_path() << endl;
                log::debug("Starting to read from the video file");
                ret = vid->read_subtitle_file();
                if (ret == 0) {
                    cout << "\e[32mSuccessfully saved the subtitle file: " << sub->get_path() << "\e[0m" << endl;
                    return EXIT_SUCCESS;
                } else {
                    cout << "\e[91mFailed to extract a subtitle file from: " << input << "\e[0m" << endl;
                    return EXIT_FAILURE;
                }
            } else {
                cout << "\e[91mFile: " << input << " does not contain a steg file" << "\e[0m" << endl;
                return EXIT_FAILURE;
            }

        } else if (method == "write") {
            cout << "Writing" << endl;
            log::debug("Write option selected");
            po::store(po::command_line_parser(opts).options(write_desc).run(), vm);

            log::debug("Setting input video path");
            string input_video = vm["input_video"].as<string>();
            log::debug("Setting input subtitle path");
            string input_subtitle = vm["input_subtitle"].as<string>();

            string output_video;
            log::debug("Checking for an output video path");
            if (vm.count("output_video")) {
                output_video = vm["output_video"].as<string>();
            }

            log::debug("Building subtitle object");
            subtitle *sub = new subtitle(input_subtitle, true);
            log::debug("Building video object");
            video *vid = new video(input_video, output_video, sub);

            log::debug("Starting to write to the video file");
            ret = vid->write_subtitle_file();

            if (ret == 0) {
                cout << "\e[32mSuccessfully written subtitle file to video" << "\e[0m" << endl;
                return EXIT_SUCCESS;
            } else {
                cout << "\e[91mFailed to write the subtitle file to the video" << "\e[0m" << endl;
            }
        } else {
            throw po::invalid_option_value(method);
        }

        return EXIT_SUCCESS;

    } catch (std::exception &e) {
        log::error(e.what());
        return EXIT_FAILURE;
    }
    catch (...) {
        log::error("Unknown exception");
        return EXIT_FAILURE;
    }
}
