# download numpy
if (NOT EXISTS ${CMAKE_BINARY_DIR}/src/numpy.i)
  message(STATUS "Downloading numpy.i")
  file(DOWNLOAD
    https://raw.githubusercontent.com/numpy/numpy/master/tools/swig/numpy.i
    ${CMAKE_BINARY_DIR}/src/numpy.i SHOW_PROGRESS)
else ()
  message(STATUS "Found numpy.i")
endif ()
include_directories(${CMAKE_BINARY_DIR})

# SWIG3
find_package(SWIG REQUIRED)
find_package(PythonLibs 3 REQUIRED)

# cmake modules from libfreenect
find_package(OpenCL)
find_package(OpenGL)
find_library(TURBOJPEG turbojpeg)
find_library(TEGRAJPEG tegrajpeg)
find_library(OPENNI2 OpenNI2)
find_library(LIBUSB libusb-1.0)
find_library(VA va)
find_library(VA_DRM  va-drm)
find_library(GLFW glfw)
find_library(LIBUSB usb-1.0)
find_library(JPEG jpeg)

set(LIBRARIES "${CMAKE_BINARY_DIR}/third_party/lib/libfreenect2.a")
if (VA)
  list(APPEND LIBRARIES ${VA})
endif ()
if (VA_DRM)
  list(APPEND LIBRARIES ${VA_DRM})
endif ()
if (GLFW)
  list(APPEND LIBRARIES ${GLFW})
endif ()
if (LIBUSB)
  list(APPEND LIBRARIES ${LIBUSB})
endif ()
if (OPENGL_FOUND)
  list(APPEND LIBRARIES ${OPENGL_LIBRARIES})
endif ()
if (OPENGL_FOUND AND GLFW)
  message(STATUS "found OpenGL")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D HAS_OPENGL")
endif ()
if (OPENCL_FOUND)
  message(STATUS "found OpenCL")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D HAS_OPENCL")
  list(APPEND LIBRARIES ${OPENCL_LIBRARIES})
endif ()
if (TURBOJPEG)
  list(APPEND LIBRARIES ${TURBOJPEG})
endif ()
if (TEGRAJPEG)
  list(APPEND LIBRARIES ${TEGRAJPEG})
endif ()
if (JPEG)
  list(APPEND LIBRARIES ${JPEG})
endif ()
if (OPENNI2)
  list(APPEND LIBRARIES ${OPENNI2})
endif ()

message(STATUS "using libraries: ${LIBRARIES}")

# enable fPIC
set(CMAKE_POSITION_INDEPENDENT_CODE)

message(STATUS "Using PythonLibs ${PYTHONLIBS_VERSION_STRING}")
include_directories(
  ${PYTHON_INCLUDE_DIR}
  ${CMAKE_BINARY_DIR}/third_party/include)
link_directories(${CMAKE_BINARY_DIR}/third_party/lib)
include(${SWIG_USE_FILE})

include_directories(
  ${CMAKE_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR})

# find cuda
find_package(CUDA)
if (CUDA_FOUND)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D HAS_CUDA")
  list(APPEND LIBRARIES ${CUDA_LIBRARIES})
endif ()

list(APPEND LIBRARIES ${PYTHON_LIBRARY})

set(CMAKE_SWIG_FLAGS "")
set_source_files_properties(kinect2.i PROPERTIES CPLUSPLUS ON)
file(GLOB SOURCE_FILES kinect2.cc kinect2.h)

SWIG_ADD_LIBRARY(kinect2 LANGUAGE python SOURCES kinect2.i ${SOURCE_FILES})
SWIG_LINK_LIBRARIES(kinect2 ${LIBRARIES})
add_dependencies(_kinect2 libfreenect2)
