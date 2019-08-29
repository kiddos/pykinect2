#ifndef KINECT2_H
#define KINECT2_H

#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/logger.h>
#include <libfreenect2/packet_pipeline.h>
#include <libfreenect2/registration.h>
#include <libfreenect2/libfreenect2.hpp>

#include "kinect2_file_logger.h"

namespace kinect2 {

struct FrameSize {
  FrameSize() : width(0), height(0), channel(0), byte_size(0), size(0) {}

  int width, height, channel;
  int byte_size, size;
};

class Kinect2 {
 public:
  enum Pipeline {
#ifdef HAS_CUDA
    CUDA,
#endif
#ifdef HAS_OPENCL
    CL,
#endif
#ifdef HAS_OPENGL
    GL,
#endif
    CPU
  };

  Kinect2(const std::string& serial = "", Pipeline pipeline = GL,
          bool enable_rgb = true, bool enable_depth = true);
  ~Kinect2();

  int GetDeviceCount();
  std::string GetDefaultDeviceSerial();
  bool SetupPipeline(Pipeline pipeline, int device_id = 0);
  bool OpenDevice();
  bool IsOpened();
  bool EnableFileLogger(const std::string& filepath);
  bool SetupListener(bool enable_rgb, bool enable_depth);
  bool SetupRegistration();
  bool Start();
  bool NextFrame(int wait = 10000);
  bool Stop();

  std::string serial_number() const { return device_serial_; }
  std::string firmware_version() const { return device_firmware_version_; }
  bool started() const { return started_; }

  void RGB(uint8_t** data, int* dim1, int* dim2, int* dim3);
  void IR(float** data, int* dim1, int* dim2, int* dim3);
  void Depth(float** data, int* dim1, int* dim2, int* dim3);
  void Undistorted(uint8_t** data, int* dim1, int* dim2, int* dim3);
  void Registered(uint8_t** data, int* dim1, int* dim2, int* dim3);

 private:
  libfreenect2::Freenect2 freenect2_;
  libfreenect2::Freenect2Device* device_;
  libfreenect2::PacketPipeline* pipeline_;
  libfreenect2::SyncMultiFrameListener* listener_;
  libfreenect2::FrameMap frames_;
  // registration
  libfreenect2::Registration* registration_;
  libfreenect2::Frame undistorted_frame_, registered_frame_;

  std::string serial_;
  std::string device_serial_, device_firmware_version_;

  std::vector<uint8_t> rgb_, undistorted_, registered_;
  std::vector<float> ir_, depth_;

  FrameSize rgb_size_, ir_size_, depth_size_;
  FrameSize undistorted_size_, registered_size_;
  bool enable_rgb_, enable_depth_, started_;
};

}  // namespace kinect2

#endif /* end of include guard: KINECT2_H */
