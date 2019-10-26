//
// Created by xinyang on 2019/10/25.
//

#ifndef _ENERGY_HPP_
#define _ENERGY_HPP_

#include "../Algorithm.hpp"

class Energy : public Algorithm {
private:

public:
    Energy();

    ~Energy() override;

    bool run(const cv::Mat &src, GimbalControlData &data) override;

    void onEnter() override;

    void onExit() override;
};

#endif /* _ENERGY_HPP_ */
