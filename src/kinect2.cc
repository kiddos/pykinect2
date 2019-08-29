#include "kinect2.h"

namespace kinect2 {

Kinect2::Kinect2(const std::string& serial, Pipeline pipeline, bool enable_rgb,
                 bool enable_depth)
    : device_(nullptr),
      pipeline_(nullptr),
      listener_(nullptr),
      registration_(nullptr),
      undistorted_frame_(512, 424, 4),
      registered_frame_(512, 424, 4),
      enable_rgb_(enable_rgb),
      enable_depth_(enable_depth) {
  // setup logger
  libfreenect2::setGlobalLogger(
      libfreenect2::createConsoleLogger(libfreenect2::Logger::Info));

  // setup device
  if (GetDeviceCount() > 0) {
    if (serial.length() == 0) {
      GetDefaultDeviceSerial();
    }
    SetupPipeline(pipeline);

    OpenDevice();

    SetupListener(enable_rgb, enable_depth);
    started_ = Start();

    if (enable_rgb && enable_depth) {
      SetupRegistration();
    }
  }
}

Kinect2::~Kinect2() {
  if (device_) {
    device_->stop();
    device_->close();
  }

  if (registration_) delete registration_;
  registration_ = nullptr;
}

int Kinect2::GetDeviceCount() { return freenect2_.enumerateDevices(); }

std::string Kinect2::GetDefaultDeviceSerial() {
  serial_ = freenect2_.getDefaultDeviceSerialNumber();
  return serial_;
}

bool Kinect2::SetupPipeline(Pipeline pipeline, int device_id) {
  switch (pipeline) {
#ifdef HAS_CUDA
    case CUDA:
      pipeline_ = new libfreenect2::CudaPacketPipeline(device_id);
      break;
#endif
#ifdef HAS_OPENCL
    case CL:
      pipeline_ = new libfreenect2::OpenCLPacketPipeline(device_id);
      break;
#endif
#ifdef HAS_OPENGL
    case GL:
      pipeline_ = new libfreenect2::OpenGLPacketPipeline();
      break;
#endif
    case CPU:
      pipeline_ = new libfreenect2::CpuPacketPipeline();
      break;
    default:
      break;
  }
  return pipeline_ != nullptr;
}

bool Kinect2::OpenDevice() {
  if (pipeline_) {
    device_ = freenect2_.openDevice(serial_, pipeline_);
  } else {
    device_ = freenect2_.openDevice(serial_);
  }
  device_serial_ = device_->getSerialNumber();
  device_firmware_version_ = device_->getFirmwareVersion();

  if (device_) return true;
  return false;
}

bool Kinect2::IsOpened() {
  if (device_) return true;
  return false;
}

bool Kinect2::EnableFileLogger(const std::string& filepath) {
  Kinect2FileLogger* file_logger = new Kinect2FileLogger(filepath);
  if (file_logger->good()) {
    libfreenect2::setGlobalLogger(file_logger);
    return true;
  } else {
    delete file_logger;
  }
  return false;
}

bool Kinect2::SetupListener(bool enable_rgb, bool enable_depth) {
  int types = 0;
  if (enable_rgb) types |= libfreenect2::Frame::Color;
  if (enable_depth)
    types |= libfreenect2::Frame::Ir | libfreenect2::Frame::Depth;

  listener_ = new libfreenect2::SyncMultiFrameListener(types);
  if (device_ && listener_) {
    device_->setColorFrameListener(listener_);
    device_->setIrAndDepthFrameListener(listener_);
    return true;
  }
  return false;
}

bool Kinect2::SetupRegistration() {
  if (device_) {
    registration_ = new libfreenect2::Registration(
        device_->getIrCameraParams(), device_->getColorCameraParams());
  }
  if (registration_) return true;
  return false;
}

bool Kinect2::Start() {
  if (device_) {
    return device_->start();
  }
  return false;
}

template <typename T>
void PrepareBuffer(libfreenect2::Frame* frame, int channel, FrameSize& size,
                   std::vector<T>& data) {
  if (data.size() != frame->width * frame->height * channel) {
    // setting frame size param
    size.width = frame->width;
    size.height = frame->height;
    size.channel = channel;
    size.byte_size = sizeof(T);
    size.size = size.width * size.height * size.byte_size * size.channel;
    data = std::vector<T>(size.size, 0);
  }
}

bool Kinect2::NextFrame(int wait) {
  if (!IsOpened()) return false;

  if (listener_->waitForNewFrame(frames_, wait)) {
    libfreenect2::Frame* rgb = nullptr;
    libfreenect2::Frame* ir = nullptr;
    libfreenect2::Frame* depth = nullptr;

    if (enable_rgb_) {
      rgb = frames_[libfreenect2::Frame::Color];
      PrepareBuffer(rgb, 4, rgb_size_, rgb_);
      std::memcpy(&rgb_[0], rgb->data, rgb_.size());
    }

    if (enable_depth_) {
      ir = frames_[libfreenect2::Frame::Ir];
      depth = frames_[libfreenect2::Frame::Depth];
      PrepareBuffer(ir, 1, ir_size_, ir_);
      PrepareBuffer(depth, 1, depth_size_, depth_);

      std::memcpy(&ir_[0], ir->data, ir_.size());
      std::memcpy(&depth_[0], depth->data, depth_.size());
    }

    // registration
    if (registration_ && enable_rgb_ && enable_depth_) {
      registration_->apply(rgb, depth, &undistorted_frame_,
                           &registered_frame_);
      PrepareBuffer(&undistorted_frame_, 4, undistorted_size_, undistorted_);
      PrepareBuffer(&registered_frame_, 4, registered_size_, registered_);

      std::memcpy(&undistorted_[0], undistorted_frame_.data,
                  undistorted_.size());
      std::memcpy(&registered_[0], registered_frame_.data, registered_.size());
    }

    listener_->release(frames_);
    return true;
  }
  return false;
}

bool Kinect2::Stop() {
  if (device_) {
    device_->stop();
  }
  return true;
}

template <typename T>
T* CopyData(T** data, int* dim1, int* dim2, int* dim3,
            const std::vector<T>& buffer, const FrameSize& size) {
  *dim1 = size.height;
  *dim2 = size.width;
  *dim3 = size.channel;

  *data = new T[buffer.size()];
  std::memset(*data, 0, buffer.size());
  std::memcpy(*data, &buffer[0], buffer.size());
  return *data;
}

void Kinect2::RGB(uint8_t** data, int* dim1, int* dim2, int* dim3) {
  CopyData(data, dim1, dim2, dim3, rgb_, rgb_size_);
}

void Kinect2::IR(float** data, int* dim1, int* dim2, int* dim3) {
  CopyData(data, dim1, dim2, dim3, ir_, ir_size_);
}

void Kinect2::Depth(float** data, int* dim1, int* dim2, int* dim3) {
  CopyData(data, dim1, dim2, dim3, depth_, depth_size_);
}

void Kinect2::Undistorted(uint8_t** data, int* dim1, int* dim2, int* dim3) {
  CopyData(data, dim1, dim2, dim3, undistorted_, undistorted_size_);
}

void Kinect2::Registered(uint8_t** data, int* dim1, int* dim2, int* dim3) {
  CopyData(data, dim1, dim2, dim3, registered_, registered_size_);
}

}  // namespace kinect2
