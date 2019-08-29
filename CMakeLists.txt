cmake_minimum_required(VERSION 3.0)
project(pykinect2)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
# set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_DEBUG "")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(BUILD_SHARED_LIBS ON)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif ()

include(ExternalProject)

ExternalProject_Add(
  libfreenect2
  GIT_REPOSITORY https://github.com/OpenKinect/libfreenect2
  GIT_TAG v0.2.0
  UPDATE_COMMAND ""
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/third_party
    -DBUILD_SHARED_LIBS=OFF
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON)

ExternalProject_Add(
  gtest
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG release-1.8.1
  UPDATE_COMMAND ""
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/third_party)

add_subdirectory(src)

# # test scripts
# file(COPY test/test_kinect2.py DESTINATION ${CMAKE_BINARY_DIR})
# file(GLOB EXAMPLES examples/*.py)
# file(COPY ${EXAMPLES} DESTINATION ${CMAKE_BINARY_DIR})
