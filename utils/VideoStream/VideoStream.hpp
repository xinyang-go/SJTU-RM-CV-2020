//
// Created by xinyang on 2019/10/25.
//

class VideoStream;

#ifndef _VIDEO_STREAM_HPP_
#define _VIDEO_STREAM_HPP_

#include <opencv2/core.hpp>

class VideoStream{
public:
    virtual ~VideoStream() = default;

    virtual bool init() = 0;

    virtual void read(cv::Mat &) noexcept(false) = 0;

    virtual bool control(void*) = 0;
};

#endif /* _VIDEO_STREAM_HPP_ */
