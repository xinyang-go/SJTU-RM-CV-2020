//
// Created by xinyang on 2019/11/3.
//

#ifndef _BOX_HPP_
#define _BOX_HPP_

#include "../Algorithm.hpp"

class Box : public Algorithm{
private:

public:
    Box();

    ~Box() override;

    bool run(const cv::Mat &src, GimbalControlData &data) override;

    void onEnter() override;

    void onExit() override;

};


#endif /* _BOX_HPP_ */
