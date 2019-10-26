//
// Created by xinyang on 2019/10/25.
//

#ifndef _TYPE_HPP_
#define _TYPE_HPP_

#ifdef FIXED_OPTIONS
#define const_if if constexpr
#else
#define const_if if
#endif

struct GimbalControlData{
    float pitch, yaw, distance;
};

struct McuData{

};

#endif /* _TYPE_HPP_ */
