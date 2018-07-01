cmake_minimum_required(VERSION 3.0)
project(pykinect2)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(BUILD_SHARED_LIBS ON)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif ()

# freenect2
find_library(FREENECT2 freenect2)
if (NOT FREENECT2)
  message(FATAL_ERROR "Fail to find freenect2 in the system")
endif ()

## SWIG3
find_package(SWIG REQUIRED)
find_package(PythonLibs 2.7 REQUIRED)

if (NOT EXISTS ${CMAKE_BINARY_DIR}/numpy.i)
  message(STATUS "Downloading numpy.i")
  file(DOWNLOAD
    https://raw.githubusercontent.com/numpy/numpy/master/tools/swig/numpy.i
    ${CMAKE_BINARY_DIR}/numpy.i)
else ()
  message(STATUS "Found numpy.i")
endif ()

message(STATUS "Using PythonLibs ${PYTHONLIBS_VERSION_STRING}")
include(${SWIG_USE_FILE})
include_directories(${PYTHON_INCLUDE_DIR}
  ${CMAKE_CURRNET_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/src)

set(CMAKE_SWIG_FLAGS "")
set_source_files_properties(kinect2.i PROPERTIES CPLUSPLUS ON)
file(GLOB SOURCE_FILES src/*.cc src/*.h)
swig_add_module(kinect2 python kinect2.i ${SOURCE_FILES})
swig_link_libraries(kinect2
  ${PYTHON_LIBRARY}
  ${FREENECT2})

find_package(CUDA)
if (CUDA_FOUND)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D HAS_CUDA")
endif ()

# test scripts
file(COPY test/test_kinect2.py DESTINATION ${CMAKE_BINARY_DIR})
file(GLOB EXAMPLES examples/*.py)
file(COPY ${EXAMPLES} DESTINATION ${CMAKE_BINARY_DIR})
