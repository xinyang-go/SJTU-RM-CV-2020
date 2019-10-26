//
// Created by xinyang on 2019/10/26.
//

#ifndef _ALGORITHM_ARMOR_TRT_HPP_
#define _ALGORITHM_ARMOR_TRT_HPP_

#include "Algorithm.hpp"

class ArmorTRT : public Algorithm {
private:

public:
    ArmorTRT();

    ~ArmorTRT() override;

    bool run(const cv::Mat &src, GimbalControlData &data) override;

    void onEnter() override;

    void onExit() override;
};


#endif /* _ALGORITHM_ARMOR_TRT_HPP_ */
