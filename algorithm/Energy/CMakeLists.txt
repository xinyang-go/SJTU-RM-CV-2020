MESSAGE("use energy")

ADD_DEFINITIONS("-DUSE_ENERGY")

FIND_PACKAGE(OpenCV 4 REQUIRED)
FIND_PACKAGE(Boost COMPONENTS timer REQUIRED)

FILE(GLOB_RECURSE energyfiles "*.cpp")
ADD_LIBRARY(energy SHARED ${energyfiles})
TARGET_LINK_LIBRARIES(energy ${OpenCV_LIBS})
TARGET_LINK_LIBRARIES(energy ${Boost_LIBRARIES})

SET(ENERGY_LIBS energy PARENT_SCOPE)
SET(ENERGY_DEFINES "-DUSE_ENERGY" PARENT_SCOPE)