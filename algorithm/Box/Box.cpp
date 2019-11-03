//
// Created by xinyang on 2019/11/3.
//

#include "Box.hpp"
#include <log.h>

Box::Box() {

}

Box::~Box() {

}

bool Box::run(const cv::Mat &src, GimbalControlData &data) {
    LOGM("box running");
}

void Box::onEnter() {
    LOGM("box enter");
}

void Box::onExit() {
    LOGM("box exit");
}
