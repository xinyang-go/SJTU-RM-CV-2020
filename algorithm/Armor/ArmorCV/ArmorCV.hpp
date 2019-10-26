//
// Created by xinyang on 2019/10/25.
//

#ifndef _ALGORITHM_ARMOR_CV_HPP_
#define _ALGORITHM_ARMOR_CV_HPP_

#include "Algorithm.hpp"

class ArmorCV : public Algorithm {
private:

public:
    ArmorCV();

    ~ArmorCV() override;

    bool run(const cv::Mat &src, GimbalControlData &data) override;

    void onEnter() override;

    void onExit() override;
};

#endif /* _ALGORITHM_ARMOR_CV_HPP_ */
