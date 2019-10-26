//
// Created by xinyang on 2019/10/25.
//

#ifndef _VIDEO_HPP_
#define _VIDEO_HPP_

#include "VideoStream/VideoStream.hpp"
#include "VideoStream/Exceptions.hpp"
#include <string>
#include <opencv2/highgui/highgui.hpp>

class VideoFile : public VideoStream{
private:
    std::string name;
    cv::VideoCapture v;

public:
    explicit VideoFile(const std::string &name);

    ~VideoFile() override;

    bool init() override;

    void read(cv::Mat &src) noexcept(false) override;

    bool control(void*) override;
};


#endif /* _VIDEO_HPP_ */
