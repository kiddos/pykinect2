from kinect2 import Kinect2
import cv2
import numpy as np


def main():
  kinect = Kinect2('', Kinect2.GL, False, True)
  while True:
    kinect.NextFrame()
    ir = kinect.IR()
    depth = kinect.Depth()
    ir = np.reshape(np.ravel(ir, 'F'), ir.shape) / 1000.0
    depth = np.reshape(np.ravel(depth, 'F'), depth.shape)

    cv2.imshow('IR', ir)
    cv2.imshow('Depth', depth)
    key = cv2.waitKey(30)
    if key in [ord('q'), ord('Q')]:
      break
  kinect.Stop()


if __name__ == '__main__':
  main()
