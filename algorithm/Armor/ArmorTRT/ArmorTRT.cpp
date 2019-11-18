//
// Created by xinyang on 2019/10/26.
// CHANGED BY WANGJINGE ON 2019/11/18
//

#include "ArmorTRT.hpp"
using namespace cv;
ArmorTRT::ArmorTRT(){
//    net = init();
    filename= "/home/k/RM/RMDet/model_trt_of_mobilenetv2.engine";
    kINPUT_C = 3;          // Input image channels
    kINPUT_H = 640;        // Input image height
    kINPUT_W = 320;        // Input image width
    THRESHOLD = 0.8f;

    kINPUT_BLOB_NAME = "input_0";            // Input blob name
    kOUTPUT_BLOB_NAME = "output_0"; // Output blob name
    std::stringstream gieModelStream;
    gieModelStream.seekg(0, gieModelStream.beg);
    std::ifstream cache( filename,ios::binary);//该文件路径必须是绝对路径，否则会报错

    gieModelStream << cache.rdbuf();
    cache.close();
    nvinfer1::IRuntime* runtime = nvinfer1::createInferRuntime(gLogger);
    initLibNvInferPlugins(&gLogger.getTRTLogger(), "");
    assert(runtime != nullptr);
    gieModelStream.seekg(0, std::ios::end);
    const int modelSize = gieModelStream.tellg();
    gieModelStream.seekg(0, std::ios::beg);
    cout<<modelSize<<endl;//test
    void* modelMem = malloc(modelSize);
    gieModelStream.read((char*)modelMem, modelSize);
    engine = runtime->deserializeCudaEngine(modelMem, modelSize, nullptr);
    assert(engine != nullptr);
    free(modelMem);

    context = engine->createExecutionContext();
    assert(context != nullptr);

    // Host memory for outputs
    detectionOut = new float[15* 12600];

}

ArmorTRT::~ArmorTRT() {

}
void ArmorTRT::doInference(IExecutionContext & context, float* inputData, float* detectionOut, int batchSize)
{
    const ICudaEngine& engine = context.getEngine();
    assert(engine.getNbBindings() == 2);
    void* buffers[2];

    int inputIndex = engine.getBindingIndex(kINPUT_BLOB_NAME),
            outputIndex = engine.getBindingIndex(kOUTPUT_BLOB_NAME);


    // Create GPU buffers and a stream
    CHECK(cudaMalloc(&buffers[inputIndex], batchSize * kINPUT_C * kINPUT_H * kINPUT_W * sizeof(float))); // Data
    CHECK(cudaMalloc(&buffers[outputIndex], batchSize *15* 12600* sizeof(float)));               // Detection_out

    cudaStream_t stream;
    CHECK(cudaStreamCreate(&stream));

    // DMA the input to the GPU,  execute the batch asynchronously, and DMA it back:
    CHECK(cudaMemcpyAsync(buffers[inputIndex], inputData, batchSize * kINPUT_C * kINPUT_H * kINPUT_W * sizeof(float), cudaMemcpyHostToDevice, stream));
    context.enqueue(batchSize, buffers, stream, nullptr);
    CHECK(cudaMemcpyAsync(detectionOut, buffers[outputIndex], batchSize *15* 12600* sizeof(float), cudaMemcpyDeviceToHost, stream));
    cudaStreamSynchronize(stream);

    // Release the stream and the buffers
    cudaStreamDestroy(stream);
    CHECK(cudaFree(buffers[inputIndex]));
    CHECK(cudaFree(buffers[outputIndex]));

}
void ArmorTRT::generateLabel(Mat original,float* p){//这里的p是15*i的位置
    float x0=p[0];
    float y0=p[1];
    float width=p[2];
    float height=p[3];
    int maxindex=0;
    float max=p[5];
    Scalar color;
    for(int i=1;i<3;i++){
        if(p[5+i]>max){
            maxindex=i;
            max=p[5+i];
        }
    }
    switch(maxindex){
        case 0:
            color=Scalar(0,0,255);
            break;
        case 1:
            color=Scalar(255,0,0);
            break;
        default:
            color=Scalar(0,255,0);
    }
    maxindex=1;
    max=p[8];
    for(int i=2;i<8;i++){
        if(p[7+i]>max){
            maxindex=i;
            max=p[7+i];
        }
    }
    if(maxindex>5)
        maxindex++;
    Rect det_rect = Rect(x0-width/2, y0-height/2+80, width, height);
    rectangle(original, det_rect, color, 1);
    putText(original, to_string(maxindex), Point(x0-width/2, y0-height/2 - 15+80), FONT_HERSHEY_SIMPLEX, 2, color, 2, LINE_AA);
}
bool ArmorTRT::run(const cv::Mat &src, GimbalControlData &data) {
//    src -> src_tensor;
//    out = net(src_tensor);
//    out[..., 4] > 0.5
//    NMS(out);
//    sort(target);
//    data = target
    Mat roi = src(Rect(0,80,640,320));
    //cout<<roi.rows<<endl;


    // Create Input data
    Mat imgFloat;
    roi.convertTo(imgFloat, CV_32FC3);//本来是char型，需要事先转换为浮点型，否则结果会不正确
    // Run inference
    doInference(*context, reinterpret_cast<float*>(imgFloat.data), detectionOut,1);

    //doInference2(*engine, imgRow, detectionOut);
    //cout<<"detect over"<<endl;
    int k=1;
    float maxconf=detectionOut[4];
    for(int i=0;i<12600;i++)
    {

        if(detectionOut[15*i+4]>THRESHOLD)
        {
            //k=0;//这一句方便发现误识别的点，调试时可加
            generateLabel(src,&detectionOut[15*i]);

        }
        if(detectionOut[15*i+4]>maxconf)
        {
            maxconf=detectionOut[15*i+4];
        }
    }

    imshow("detect",src);
    waitKey(k);
    return false;
}

void ArmorTRT::onEnter() {

}

void ArmorTRT::onExit() {

}

