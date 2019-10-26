//
// Created by xinyang on 2019/10/25.
//

#ifndef _MINDVISION_HPP_
#define _MINDVISION_HPP_

#include "../VideoStream.hpp"
#include "CameraAPI/CameraApi.h"
#include <Queue/Queue.hpp>

struct CameraControlInfo{
    enum {
        SET_EXPOSURE, SET_GAIN
    } type;
    int val;
};

class MindVision : public VideoStream {
    friend void cameraCallback(CameraHandle hCamera, BYTE *pFrameBuffer, tSdkFrameHead *pFrameHead, PVOID pContext);

private:
    const std::string &camera_name;
    CameraHandle h_camera;
    bool working;
    int exposure, gain;
    unsigned char *rgb_buffer;
    Queue<cv::Mat, 2> queue;
public:
    MindVision(int e, int g, const std::string &name);

    ~MindVision() override;

    bool init() override;

    void read(cv::Mat &src) noexcept(false) override;

    bool control(void *) override;
};


#endif /* _MINDVISION_HPP_ */
