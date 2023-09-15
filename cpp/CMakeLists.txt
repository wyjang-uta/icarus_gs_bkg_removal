cmake_minimum_required(VERSION 3.0)
project(ICARUSEventVisualAnalyzer)

find_package(OpenCV REQUIRED)

add_executable(detectSignal detectSignal.cpp)
target_link_libraries(detectSignal PRIVATE ${OpenCV_LIBS})
