cmake_minimum_required(VERSION 3.22.1)
project(PyMatrixAnalysis)
set(CMAKE_CXX_STANDARD 20)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/libs)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    /usr/include/python3.11
    $ENV{HOME}/.local/lib/python3.11/site-packages/numpy/core/include/numpy
)
add_library("matrix" STATIC
    srcs/MatrixObject.cpp
    srcs/MatrixObjectCal.cpp
    srcs/Matrix_Equivalence.cpp
)
add_library("var" STATIC
    srcs/ComplexVar.cpp
    srcs/ComplexVarObject.cpp
)
add_library("utility" STATIC
    srcs/utilities.cpp
    srcs/unsure.cpp
    srcs/custom_exceptions.cpp)
add_library("errordouble" STATIC
    srcs/errordouble.cpp
)