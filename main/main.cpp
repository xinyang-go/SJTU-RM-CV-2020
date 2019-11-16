#include <iostream>
#include <utils>
#include <Algorithm>
#include <log.h>
#include <opencv2/imgproc.hpp>
#include "MainFSM.hpp"

int main(int argc, char *argv[]) {
    // 处理命令行参数，仅在Debug下生效
    processOptions(argc, argv);

    // 创建串口对象
    Serial serial(wait_serial);


    // 创建云台控制数据
    GimbalControlData data{};

    // 创建状态机，并注册算法对象
    char mode = 'a';
    MainFSM fsm(mode, {
#ifdef USE_ARMOR
            {'a', std::shared_ptr<Algorithm>{new Armor()}},
#endif
#ifdef USE_ENERGY
            {'e', std::shared_ptr<Algorithm>{new Energy()}},
#endif
#ifdef USE_BOX
            {'b', std::shared_ptr<Algorithm>{new Box()}},
#endif
    });

    // 选择视频流源
    bool from_camera = true;
    const_if (!run_with_camera) {
        LOGM(STR_CTR(WORD_LIGHT_WHITE, "Input 1 for camera, 0 for video files"));
        std::cin >> from_camera;
    }
    // 创建视频流对象
    std::unique_ptr<VideoStream> video;
    if (from_camera) {
        video = std::make_unique<MindVision>(10, 64);
    } else {
        std::string name;
        LOGM(STR_CTR(WORD_LIGHT_WHITE, "Input the video to run:"));
        std::cin >> name;
        video = std::make_unique<VideoFile>(name);
    }
    if (!video->init()) LOGE("Video init fail!");

    cv::Mat src;

    // 进入主循环
    while (true) {
        CNT_FPS("main_loop");
        try {
            video->read(src);
            const_if (show_origin) {
                cv::imshow("origin", src);
                cv::waitKey(1);
            }
            if (fsm.step(std::ref(src), std::ref(data)))
                serial.WriteData((u_char *) &data, sizeof(data));
        } catch (FrameReadError &error) {
            LOGW("video source reload!");
            video->init();
        } catch (SerialOfflineError &error) {
            const_if (wait_serial) {
                LOGW("serial reload!");
                serial.InitPort();
            }
        } catch (cv::Exception &e) {
            LOGW("An opencv exception occurs: %s", e.what());
        } catch (...) {
            LOGW("something unexpected happened! restart!");
        }
    }
    return 0;
}