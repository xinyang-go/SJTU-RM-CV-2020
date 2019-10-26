//
// Created by xinyang on 2019/10/25.
//

#include "Energy.hpp"
#include <log.h>

Energy::Energy() {

}

Energy::~Energy() {

}

bool Energy::run(const cv::Mat &src, GimbalControlData &data) {
    LOGM("energy running");
}

void Energy::onEnter() {
    LOGM("energy enter");
}

void Energy::onExit() {
    LOGM("energy exit");
}
