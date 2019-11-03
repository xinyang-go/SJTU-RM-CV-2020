//
// Created by xinyang on 2019/10/25.
//

#include "options.hpp"
#include <map>
#include <log.h>

#ifndef FIXED_OPTIONS
bool show_origin = false;
bool show_armor_box = false;
bool wait_serial = false;
bool run_with_camera = false;

std::map<std::string, std::pair<std::string, void(*)(void)>> options = {
        {"--help", {
                "show the help information.", []() {
                    LOG(LOG_MSG, "<HELP>: "
                            STR_CTR(WORD_BLUE, "All options below are for debug use."));
                    for (const auto &[str, info] : options) {
                        LOG(LOG_MSG, "<HELP>: "
                                STR_CTR(WORD_GREEN, "%s: %s"), str.data(), info.first.data());
                    }
                }
        }},
        {"--show-origin", {
                "show the origin video.", []() {
                    show_origin = true;
                    LOGM("Enable show origin video");
                }
        }},
        {"--show-armor-box", {
                "show the aim box.", []() {
                    show_armor_box = true;
                    LOGM("Enable show armor box");
                }
        }},
        {"--wait-uart", {
                "wait serial until ready before running.", [](){
                    wait_serial = true;
                    LOGM("Enable wait uart!");
                }
        }},
        {"--run-with-camera", {
                "run the program with camera without asking.", []() {
                    run_with_camera = true;
                    LOGM("Enable running with camera");
                }
        }},
};

void processOptions(int argc, char **argv){
    if (argc >= 2) {
        for (int i = 1; i < argc; i++) {
            auto key = options.find(std::string(argv[i])); // 寻找对应选项。
            if(key != options.end()){
                key->second.second();
            }else{
                LOGW("Unknown option: %s. Use --help to see options.", argv[i]);
            }
        }
    }
}

#else

void processOptions(int argc, char **argv){

}

#endif
