//
// Created by xinyang on 2019/10/25.
//

#ifndef _OPTIONS_HPP_
#define _OPTIONS_HPP_

#ifdef FIXED_OPTIONS
constexpr bool show_origin = false;
constexpr bool show_armor_box = false;
constexpr bool wait_serial = true;
constexpr bool run_with_camera = true;
#else /* FIXED_OPTIONS */
extern bool show_origin;
extern bool show_armor_box;
extern bool wait_serial;
extern bool run_with_camera;
#endif /* FIXED_OPTIONS */

void processOptions(int argc, char **argv);

#endif /* _OPTIONS_HPP */
