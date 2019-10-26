#include <iostream>
#include <utils>
#include <Algorithm>
#include <thread>
#include <log.h>

int main(int argc, char *argv[]) {
    // 处理命令行参数，仅在Debug下生效
    processOptions(argc, argv);

    // 创建串口对象
    Serial serial(wait_serial);

    // 选择视频流源
    bool from_camera = true;
    const_if(!run_with_camera){
        LOGM(STR_CTR(WORD_LIGHT_WHITE, "Input 1 for camera, 0 for video files"));
        std::cin >> from_camera;
    }
    // 创建视频流对象
    VideoStream *v;
    if(from_camera){
        v = new MindVision(5, 32, "gimbal");
    }else{
        std::string name;
        LOGM(STR_CTR(WORD_LIGHT_WHITE, "Input the video to run:"));
        std::cin >> name;
        v = new VideoFile(name);
    }
    std::unique_ptr<VideoStream> video(v);
    if(!video->init()) LOGE("Video init fail!");

    // 创建云台控制数据
    GimbalControlData data{};

    // 创建算法对象
    std::unique_ptr<Algorithm> armor(new Armor());
    std::unique_ptr<Algorithm> energy(new Energy());

    // 创建状态机，并注册函数
    int mode = 0;
    FSM<int, std::function<bool(cv::Mat &)>> fsm(mode);
    fsm.registerOnStateFunc(0, [&](cv::Mat &src) -> bool { return armor->run(src, data); });
    fsm.registerOnStateFunc(1, [&](cv::Mat &src) -> bool { return energy->run(src, data); });
//    fsm.registerOnEnterFunc(0, [&]() { armor->onEnter(); });
//    fsm.registerOnEnterFunc(1, [&]() { energy->onEnter(); });
//    fsm.registerOnExitFunc(0, [&]() { armor->onExit(); });
//    fsm.registerOnExitFunc(1, [&]() { energy->onExit(); });

    cv::Mat src;

    // 进入主循环
    while (true) {
        try {
            video->read(src);
            const_if(show_origin){
                cv::imshow("origin", src);
                cv::waitKey(1);
            }
            if(fsm.step(std::ref(src)))
                serial.WriteData((u_char*)&data, sizeof(data));
        } catch (FrameReadError &error) {
            LOGW("video source reload!");
            video->init();
        } catch (SerialOfflineError &error) {
            const_if (wait_serial){
                LOGW("serial reload!");
                serial.InitPort();
            }
        } catch (...){
            LOGW("something unexpected happened! restart!");
        }
    }

    return 0;
}