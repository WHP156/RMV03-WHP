cmake_minimum_required(VERSION 3.1.0)
project(task)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenCV REQUIRED)

aux_source_directory(./src ALL_SRCS)
include_directories(include
    ${OpenCV_INCLUDE_DIRS})

    set( CMAKE_BUILD_TYPE "Release" )
    set( CMAKE_CXX_FLAGS "-std=c++11 -O3" )
    
    # 添加cmake模块以使用ceres库
    list( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules )
    
    # 寻找Ceres库并添加它的头文件
    find_package( Ceres REQUIRED )
    include_directories( ${CERES_INCLUDE_DIRS} )
    
set(SRC ./cc)

add_executable(task main.cpp ${ALL_SRCS})
target_link_libraries(task ${OpenCV_LIBS})
target_link_libraries( task ${CERES_LIBRARIES} )
