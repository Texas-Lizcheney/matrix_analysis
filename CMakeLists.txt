cmake_minimum_required(VERSION 3.22.1)
project(PyMatrixAnalysis)
set(CMAKE_CXX_STANDARD 20)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/libs)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

include_directories(${PROJECT_SOURCE_DIR}/include /usr/include/python3.11)
add_library("matrix" STATIC
    srcs/MatrixObject.cpp
)
add_library("var" STATIC
    srcs/ComplexVarObject.cpp
)