CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
PROJECT(SJTU-RM-CV-ML)

SET(CMAKE_CXX_STANDARD 17)
ADD_COMPILE_OPTIONS("-O3")
ADD_DEFINITIONS("-DPROJECT_DIR=\"\\\"${PROJECT_SOURCE_DIR}\\\"\"")
ADD_DEFINITIONS("-D${CMAKE_SYSTEM_NAME}")
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(BIN_NAME "run")

# 尝试使用ccache加速编译
FIND_PROGRAM(CCACHE_FOUND ccache)
IF(CCACHE_FOUND)
    SET_PROPERTY(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    SET_PROPERTY(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
    MESSAGE("< Use ccache for compiler >")
ENDIF()

# 寻找必要依赖库
FIND_PACKAGE(OpenCV 4 REQUIRED)
FIND_PACKAGE(Boost COMPONENTS timer REQUIRED)
FIND_PACKAGE(Threads)

# 寻找必要源文件
FILE(GLOB_RECURSE sourcefiles "utils/*.cpp" "algorithm/Energy/*.cpp" "main/*.cpp" "algorithm/Box/*.cpp")

# 选择自瞄算法，并添加到源文件编译列表中
SET(ARMOR_TARGET "TARGET_CV" CACHE STRING INTERNAL) # candidates: TARGET_CV, TARGET_TRT, TARGET_TORCH
ADD_DEFINITIONS("-D${ARMOR_TARGET}")
if("${ARMOR_TARGET}" STREQUAL "TARGET_TRT")
    SET(BIN_NAME "${BIN_NAME}-trt")
    FILE(GLOB_RECURSE armorfiles "algorithm/Armor/ArmorTRT/*.cpp")
elseif("${ARMOR_TARGET}" STREQUAL "TARGET_TORCH")
    SET(BIN_NAME "${BIN_NAME}-torch")
    FILE(GLOB_RECURSE armorfiles "algorithm/Armor/ArmorTorch/*.cpp")
else()
    SET(BIN_NAME "${BIN_NAME}-cv")
    FILE(GLOB_RECURSE armorfiles "algorithm/Armor/ArmorCV/*.cpp")
endif()
SET(sourcefiles ${sourcefiles} ${armorfiles})

# 根据编译方式不同生成不同名称的可执行文件
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    SET(BIN_NAME "${BIN_NAME}-debug")
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DFIXED_OPTIONS")
    SET(BIN_NAME "${BIN_NAME}-release")
endif()

# 设置头文件目录
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/utils)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/algorithm)

# 添加可执行文件以及链接
ADD_EXECUTABLE("${BIN_NAME}" ${sourcefiles})
TARGET_LINK_LIBRARIES(${BIN_NAME} "${PROJECT_SOURCE_DIR}/lib/libMVSDK.so")
TARGET_LINK_LIBRARIES(${BIN_NAME} ${OpenCV_LIBS})
target_link_libraries(${BIN_NAME} ${Boost_LIBRARIES})
