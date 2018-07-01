import unittest
from kinect2 import Kinect2
import numpy as np


class TestKinect2(unittest.TestCase):
  @classmethod
  def setUpClass(cls):
    cls.kinect = Kinect2()

  def test_setup(self):
    print(self.kinect.serial_number())
    print(self.kinect.firmware_version())
    for i in range(100):
      if self.kinect.NextFrame():
        rgb = self.kinect.RGB()
        ir = self.kinect.IR()
        depth = self.kinect.Depth()
        undistorted = self.kinect.Undistorted()
        registered = self.kinect.Registered()

        rgb = np.reshape(np.ravel(rgb, 'F'), rgb.shape)
        ir = np.reshape(np.ravel(ir, 'F'), ir.shape) / 1000.0
        depth = np.reshape(np.ravel(depth, 'F'), depth.shape)
        undistorted = np.reshape(np.ravel(undistorted, 'F'), undistorted.shape)
        registered = np.reshape(np.ravel(registered, 'F'), registered.shape)
    self.kinect.Stop()


if __name__ == '__main__':
  unittest.main()
