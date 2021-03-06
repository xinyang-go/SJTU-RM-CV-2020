MESSAGE("use armor-trt")


FIND_PACKAGE(OpenCV 4 REQUIRED)
FIND_PACKAGE(Boost COMPONENTS timer REQUIRED)

SET(CUDA_HOST_COMPILER ${CMAKE_CXX_COMPILER})
FIND_PACKAGE(CUDA)
# 在这里修改我们显卡的计算能力 这里我是sm_61
#set(
#        CUDA_NVCC_FLAGS
#        ${CUDA_NVCC_FLAGS};
#        -O3
#        -gencode arch=compute_61,code=sm_61
#)


set(TENSORRT_ROOT /home/k/RM/TensorRT-6.0.1.5)
find_path(TENSORRT_INCLUDE_DIR NvInfer.h
        HINTS ${TENSORRT_ROOT} ${CUDA_TOOLKIT_ROOT_DIR}
        PATH_SUFFIXES include)
set(TENSORRT_INCLUDE_DIR2 ${TENSORRT_ROOT}/samples/common)
MESSAGE(STATUS "Found TensorRT headers at ${TENSORRT_INCLUDE_DIR} ${TENSORRT_INCLUDE_DIR2}")
find_library(TENSORRT_LIBRARY_INFER nvinfer
        HINTS ${TENSORRT_ROOT} ${TENSORRT_BUILD} ${CUDA_TOOLKIT_ROOT_DIR}
        PATH_SUFFIXES lib lib64 lib/x64)
find_library(TENSORRT_LIBRARY_INFER_PLUGIN nvinfer_plugin
        HINTS  ${TENSORRT_ROOT} ${TENSORRT_BUILD} ${CUDA_TOOLKIT_ROOT_DIR}
        PATH_SUFFIXES lib lib64 lib/x64)
set(TENSORRT_LIBRARY ${TENSORRT_LIBRARY_INFER} ${TENSORRT_LIBRARY_INFER_PLUGIN})
MESSAGE(STATUS "Find TensorRT libs at ${TENSORRT_LIBRARY}")
find_package_handle_standard_args(
        TENSORRT DEFAULT_MSG TENSORRT_INCLUDE_DIR TENSORRT_LIBRARY)
if(NOT TENSORRT_FOUND)
    message(ERROR
            "Cannot find TensorRT library.")
endif()

AUX_SOURCE_DIRECTORY(${TENSORRT_INCLUDE_DIR2} DIR_SRCS)
FILE(GLOB_RECURSE armorcvfiles "*.cpp")
ADD_LIBRARY(armor-trt SHARED ${armorcvfiles} ${TENSORRT_INCLUDE_DIR2}/logger.cpp)
TARGET_LINK_LIBRARIES(armor-trt ${OpenCV_LIBS})
TARGET_LINK_LIBRARIES(armor-trt ${Boost_LIBRARIES})
target_include_directories(armor-trt PUBLIC ${CUDA_INCLUDE_DIRS} ${TENSORRT_INCLUDE_DIR} ${TENSORRT_INCLUDE_DIR2})
TARGET_LINK_LIBRARIES(armor-trt ${CUDA_LIBRARIES} ${TENSORRT_LIBRARY} ${CUDA_CUBLAS_LIBRARIES} ${CUDA_cudart_static_LIBRARY})
SET(ARMOR_TRT_LIBS armor-trt PARENT_SCOPE)
SET(ARMOR_TRT_DEFINES "-DUSE_ARMOR_TRT" PARENT_SCOPE)