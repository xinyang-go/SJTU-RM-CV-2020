//
// Created by xinyang on 2019/10/25.
//

#include "ArmorCV.hpp"
#include <log.h>

ArmorCV::ArmorCV() {

}

ArmorCV::~ArmorCV() {

}

bool ArmorCV::run(const cv::Mat &src, GimbalControlData &data) {
    LOGM("armor running");
}

void ArmorCV::onEnter() {
    LOGM("armor enter");
}

void ArmorCV::onExit() {
    LOGM("armor exit");
}
