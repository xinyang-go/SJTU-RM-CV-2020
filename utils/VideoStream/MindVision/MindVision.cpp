//
// Created by xinyang on 2019/10/25.
//

#include "MindVision.hpp"
#include "../Exceptions.hpp"
#include <opencv2/core.hpp>
#include <log.h>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace boost::posix_time;

MindVision::MindVision(int e, int g, const std::string &name) :
        camera_name(name), h_camera(-1), working(false), exposure(e), gain(g), rgb_buffer(nullptr) {

}

MindVision::~MindVision() {
    if (working) {
        CameraUnInit(h_camera);
        delete[] rgb_buffer;
    }
}

void cameraCallback(CameraHandle hCamera, BYTE *pFrameBuffer, tSdkFrameHead *pFrameHead, PVOID pContext) {
    auto c = (MindVision *) pContext;
    CameraImageProcess(hCamera, pFrameBuffer, c->rgb_buffer, pFrameHead);
    cv::Mat mat(pFrameHead->iWidth, pFrameHead->iHeight, CV_8UC3, c->rgb_buffer);
    c->queue.push(cv::Mat(pFrameHead->iWidth, pFrameHead->iHeight, CV_8UC3, c->rgb_buffer).clone());
}

bool MindVision::init() {
    if (working) {
        CameraUnInit(h_camera);
        working = false;
    }

    CameraSdkInit(1);
    CameraSdkStatus camera_status;
    tSdkCameraDevInfo camera_enum_list[2];
    int camera_cnts;
    if ((camera_status = CameraEnumerateDevice(camera_enum_list, &camera_cnts)) != CAMERA_STATUS_SUCCESS) {
        LOGE("CameraEnumerateDevice fail with %d!", camera_status);
    }
    if (camera_cnts == 0) {
        LOGE("No camera device detected!");
        return false;
    } else {
        LOGM("%d camera device detected!", camera_cnts);
    }


    // 根据相机名称查找对应相机
    int i;
    char name[16];
    for (i = 0; i < camera_cnts; i++) {
        camera_status = CameraInit(&camera_enum_list[i], -1, -1, &h_camera);
        if (camera_status != CAMERA_STATUS_SUCCESS) {
            CameraUnInit(h_camera);
            continue;
        }
        CameraGetFriendlyName(h_camera, name);
        if (camera_name == "NULL" || strcmp(camera_name.data(), name) == 0) {
            break;
        }
        CameraUnInit(h_camera);
    }
    if (i >= camera_cnts) {
        LOGE("No device name %s or device open error!!", camera_name.data());
        return false;
    }

    // 获取相机特性描述结构体
    tSdkCameraCapbility tCapability;
    camera_status = CameraGetCapability(h_camera, &tCapability);
    if (camera_status != CAMERA_STATUS_SUCCESS) {
        LOGE("CameraGetCapability return error code %d", camera_status);
        return false;
    }
    rgb_buffer = new u_char(tCapability.sResolutionRange.iHeightMax * tCapability.sResolutionRange.iWidthMax * 3);

    CameraReadParameterFromFile(h_camera, PROJECT_DIR"/others/MV-UB31-Group0.config");
    CameraLoadParameter(h_camera, PARAMETER_TEAM_A);
    CameraSetAeState(h_camera, false);
    CameraSetExposureTime(h_camera, exposure * 1000);
    CameraSetAnalogGain(h_camera, gain);

    CameraPlay(h_camera);
    if (tCapability.sIspCapacity.bMonoSensor) {
        LOGE("Camera with only mono mode is not supported!");
        return false;
    } else {
        CameraSetIspOutFormat(h_camera, CAMERA_MEDIA_TYPE_BGR8);
        LOGM("camera %s color ", camera_name.data());
    }
//    CameraSetCallbackFunction(h_camera, cameraCallback, this, nullptr);
    return working = true;
}

void MindVision::read(cv::Mat &src) {
    ptime ts = microsec_clock::local_time();
    while (queue.empty()) {
        ptime te = microsec_clock::local_time();
        auto interval = te - ts;
        if (interval.total_milliseconds() > 100){
            throw FrameReadError();
        }
    }
}

bool MindVision::control(void *p) {
    auto info = (CameraControlInfo*)p;
    CameraSdkStatus status;
    switch(info->type){
        case CameraControlInfo::SET_EXPOSURE:
            status = CameraSetExposureTime(h_camera, info->val * 1000);
            if(status != CAMERA_STATUS_SUCCESS){
                LOGW("camera set exposure time failed with %d!", status);
            }else{
                exposure = info->val;
            }
            return status == CAMERA_STATUS_SUCCESS;
        case CameraControlInfo::SET_GAIN:
            status = CameraSetAnalogGain(h_camera, info->val);
            if(status != CAMERA_STATUS_SUCCESS){
                LOGW("camera set analog gain failed with %d!", status);
            }else{
                gain = info->val;
            }
            return status == CAMERA_STATUS_SUCCESS;
    }
    LOGW("unknown control type %d!", info->type);
    return false;
}

