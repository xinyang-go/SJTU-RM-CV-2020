//
// Created by xinyang on 19-2-19.
//
// 该文件提供一个更加方便的调试信息输出方式
// 所有输出信息分为三个LEVEL:MSG,WARNING,ERROR
// 可使用宏LOG_LEVEL定义当前文件使用的输出LEVEL
// 高于该LEVEL的输出讲不会被显示
// ============================================================
// 输出API：
// LOG(level, format, ...)
// arguments: level:当前输出的level
//            format:标准printf格式化字符串
// LOGM(format, ...) 使用MSG level进行输出
// LOGW(format, ...) 使用WARNING level进行输出
// LOGE(format, ...) 使用ERROR level进行输出
// ============================================================
// 输出颜色API：(仅对部分终端生效)
// STR_CTR(ctrs, str)
// arguments: ctrs:该字符串对应的颜色(所有以WORD开头的宏)
//　　　　　　　str:需要上色的字符串
// ============================================================
// 时间计算API：(需要配合systime.h使用)
// CNT_TIME(tag, codes, ...)
// arguments: tag:显示代码块执行时间前的用户信息，支持printf格式化字符串
//            codes:需要被统计时间的代码块
// attention: 代码块内定义的局部变量作用域仅限于该代码块
//            代码块内不支持使用break,continue语句,将无法达到预想效果
//            支持多出口离开代码块都能显示代码块执行时间
//
#ifndef _LOG_H_
#define _LOG_H_

#include <cstdio>

/************** Define the control code *************/
#define START_CTR           "\033[0"
#define END_CTR             "m"
#define CLEAR_CODE          ";0"
#define LIGHT_CODE          ";1"
#define LINE_CODE           ";4"
#define BLINK_CODE          ";5"
#define REVERSE_CODE        ";7"
#define VANISH_CODE         ";8"
#define WORD_WHITE_CODE     ";30"
#define WORD_RED_CODE       ";31"
#define WORD_GREEN_CODE     ";32"
#define WORD_YELLOW_CODE    ";33"
#define WORD_BLUE_CODE      ";34"
#define WORD_PURPLE_CODE    ";35"
#define WORD_CYAN_CODE      ";36"
#define WORD_GRAY_CODE      ";37"
#define BACK_WHITE_CODE     ";40"
#define BACK_RED_CODE       ";41"
#define BACK_GREEN_CODE     ";42"
#define BACK_YELLOW_CODE    ";43"
#define BACK_BLUE_CODE      ";44"
#define BACK_PURPLE_CODE    ";45"
#define BACK_CYAN_CODE      ";46"
#define BACK_GRAY_CODE      ";47"

#define CTRS(ctrs)          START_CTR ctrs END_CTR
#define STR_CTR(ctrs, str)          START_CTR ctrs END_CTR str CLEAR_ALL

#define WORD_WHITE          WORD_WHITE_CODE
#define WORD_RED            WORD_RED_CODE
#define WORD_GREEN          WORD_GREEN_CODE
#define WORD_YELLOW         WORD_YELLOW_CODE
#define WORD_BLUE           WORD_BLUE_CODE
#define WORD_PURPLE         WORD_PURPLE_CODE
#define WORD_CYAN           WORD_CYAN_CODE
#define WORD_GRAY           WORD_GRAY_CODE
#define WORD_LIGHT_WHITE    LIGHT_CODE WORD_WHITE
#define WORD_LIGHT_RED      LIGHT_CODE WORD_RED
#define WORD_LIGHT_GREEN    LIGHT_CODE WORD_GREEN
#define WORD_LIGHT_YELLOW   LIGHT_CODE WORD_YELLOW
#define WORD_LIGHT_BLUE     LIGHT_CODE WORD_BLUE
#define WORD_LIGHT_PURPLE   LIGHT_CODE WORD_PURPLE
#define WORD_LIGHT_CYAN     LIGHT_CODE WORD_CYAN
#define WORD_LIGHT_GRAY     LIGHT_CODE WORD_GRAY
#define CLEAR_ALL           CTRS(CLEAR_CODE)
/*************** Define the log level value ***************/
#define LOG_NONE    0
#define LOG_ERROR   1
#define LOG_WARNING 2
#define LOG_MSG     3
#define LOG_NOTHING 4
/************** Ensure the current log level **************/
#ifndef LOG_LEVEL
    #define LOG_LEVEL LOG_MSG
#endif
#if LOG_LEVEL < LOG_NONE
    #define LOG_LEVEL LOG_NONE
#elif LOG_LEVEL > LOG_MSG
    #define  LOG_LEVEL LOG_MSG
#endif
/******* Ensure the color corresponding to the level ******/
#ifndef LOG_ERROR_COLOR
    #define LOG_ERROR_COLOR         WORD_RED
#endif
#ifndef LOG_WARNING_COLOR
    #define LOG_WARNING_COLOR       WORD_YELLOW
#endif
#ifndef LOG_MSG_COLOR
    #define LOG_MSG_COLOR           WORD_GRAY
#endif
#ifndef LOG_LINK_COLOR
    #define LOG_LINK_COLOR          LINE_CODE WORD_BLUE
#endif

/********************** log place *************************/
#ifndef LOG_OUT
    #define LOG_OUT stdout
#endif
/******************** The log API *************************/
#define LOG_0(format, ...)          fprintf(LOG_OUT, format, ##__VA_ARGS__)
#if LOG_LEVEL >= LOG_ERROR
    #define LOG_1(format, ...)      fprintf(LOG_OUT, format, ##__VA_ARGS__)
#else
    #define LOG_1(format, ...)      ((void)0)
#endif
#if LOG_LEVEL >= LOG_WARNING
    #define LOG_2(format, ...)      fprintf(LOG_OUT, format, ##__VA_ARGS__)
#else
    #define LOG_2(format, ...)      ((void)0)
#endif
#if LOG_LEVEL >= LOG_MSG
    #define LOG_3(format, ...)      fprintf(LOG_OUT, format, ##__VA_ARGS__)
#else
    #define LOG_3(format, ...)      ((void)0)
#endif
#define LOG_4(format, ...)          ((void)0)
#define LOG_(level, format, ...)    LOG_##level (format, ##__VA_ARGS__)
#define LOG(level, format, ...)     LOG_(level, format"\n", ##__VA_ARGS__)

#define LOGA(format, ...)           LOG(LOG_NONE, format, ##__VA_ARGS__)
#define LOGA_INFO(format, ...)      LOG(LOG_NONE, "<%s:%d>: " format, ##__VA_ARGS__)
#define LOGE(format, ...)           LOG(LOG_ERROR,   STR_CTR(LOG_ERROR_COLOR,  "<ERROR>: "   format), ## __VA_ARGS__)
#define LOGW(format, ...)           LOG(LOG_WARNING, STR_CTR(LOG_WARNING_COLOR,"<WARNING>: " format), ## __VA_ARGS__)
#define LOGM(format, ...)           LOG(LOG_MSG,     STR_CTR(LOG_MSG_COLOR,    "<MSG>: "     format), ## __VA_ARGS__)
#define LOGE_INFO(format, ...)      LOG(LOG_ERROR, \
                                            STR_CTR(LOG_ERROR_COLOR,  "<") \
                                            STR_CTR(LOG_LINK_COLOR,  "%s:%d") \
                                            STR_CTR(LOG_ERROR_COLOR, " ERROR>: "   format), \
                                            __FILE__, __LINE__, ##__VA_ARGS__)
#define LOGW_INFO(format, ...)      LOG(LOG_WARNING, \
                                            STR_CTR(LOG_WARNING_COLOR,"<") \
                                            STR_CTR(LOG_LINK_COLOR,"%s:%d") \
                                            STR_CTR(LOG_WARNING_COLOR, " WARNING>: " format), \
                                            __FILE__, __LINE__, ##__VA_ARGS__)
#define LOGM_INFO(format, ...)      LOG(LOG_MSG, \
                                            STR_CTR(LOG_MSG_COLOR,    "<") \
                                            STR_CTR(LOG_LINK_COLOR,    "%s:%d") \
                                            STR_CTR(LOG_MSG_COLOR, " MSG>: "     format), \
                                            __FILE__, __LINE__, ##__VA_ARGS__)

/******************** the time counter API ************************/
#include <boost/timer/timer.hpp>
#if !defined(DO_NOT_CNT_TIME) && LOG_LEVEL > LOG_NONE
    #define CNT_TIME(tag, codes) do{ \
        boost::timer::auto_cpu_timer t1(tag": %w\n"); \
        codes; \
    }while (0)
#else
	#define CNT_TIME(tag, codes, ...) codes
#endif
#else /* _LOG_H_ */
    #warning "Multiple include of log.h, some settings may not work."
#endif /* _LOG_H_ */ 
