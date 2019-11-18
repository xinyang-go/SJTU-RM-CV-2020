//
// Created by xinyang on 2019/10/26.
// CHANGED BY WANGJINGE ON 2019/11/18
//

#ifndef _ALGORITHM_ARMOR_TRT_HPP_
#define _ALGORITHM_ARMOR_TRT_HPP_

#include "Algorithm.hpp"
#include <iostream>

#include <string>
#include <vector>
#include <cstdlib>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include <cuda.h>
#include "buffers.h"
#include "common.h"
#include "logger.h"

#include "NvInfer.h"
#include "NvInferPlugin.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cuda_runtime_api.h>
#include <sstream>
class ArmorTRT : public Algorithm {
private:
    string filename;
    float* detectionOut;
    nvinfer1::ICudaEngine* engine;
    IExecutionContext *context;
    int kINPUT_C ;          // Input image channels
    int kINPUT_H ;        // Input image height
    int kINPUT_W ;        // Input image width
    float THRESHOLD;

    char* kINPUT_BLOB_NAME ;            // Input blob name
    char* kOUTPUT_BLOB_NAME; // Output blob name
    void doInference(IExecutionContext & context, float* inputData, float* detectionOut, int batchSize);
    void generateLabel(cv::Mat original,float* p);
public:
    ArmorTRT();

    ~ArmorTRT() override;

    bool run(const cv::Mat &src, GimbalControlData &data) override;

    void onEnter() override;

    void onExit() override;
};


#endif /* _ALGORITHM_ARMOR_TRT_HPP_ */
