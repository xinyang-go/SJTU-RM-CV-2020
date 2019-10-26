//
// Created by xinyang on 2019/10/25.
//

#include "VideoFile.hpp"

VideoFile::VideoFile(const std::string &name): name(name){
}

VideoFile::~VideoFile() = default;

bool VideoFile::init() {
    if(v.isOpened())
        v.release();
    v.open(name);
    return v.isOpened();
}

void VideoFile::read(cv::Mat &src) {
    if(!v.read(src)){
        throw FrameReadError();
    }
}

bool VideoFile::control(void *) {
    return true;
}




