#include <fstream>

#include <libfreenect2/logger.h>

class Kinect2FileLogger : public libfreenect2::Logger {
 public:
  Kinect2FileLogger(const std::string& filename) {
    logfile_.open(filename.c_str());
    level_ = Debug;
  }

  bool good() { return logfile_.is_open() && logfile_.good(); }

  virtual void log(Level level, const std::string& message) {
    logfile_ << "[" << libfreenect2::Logger::level2str(level) << "] "
             << message << std::endl;
  }

 private:
  std::ofstream logfile_;
};
