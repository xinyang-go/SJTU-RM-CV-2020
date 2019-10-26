//
// Created by xinyang on 2019/10/25.
//

#ifndef _ALGORITHM_HPP_
#define _ALGORITHM_HPP_

#include <utils>

class Algorithm {
public:
    virtual ~Algorithm() = default;

    virtual bool run(const cv::Mat &, GimbalControlData &) = 0;

    virtual void onEnter() = 0;

    virtual void onExit() = 0;
};

#endif /* _ALGORITHM_HPP_ */
