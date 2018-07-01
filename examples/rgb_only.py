from kinect2 import Kinect2
import cv2
import numpy as np


def main():
  kinect = Kinect2('', Kinect2.GL, True, False)
  while True:
    kinect.NextFrame()
    rgb = kinect.RGB()
    rgb = np.reshape(np.ravel(rgb, 'F'), rgb.shape)

    cv2.imshow('RGB', rgb)
    key = cv2.waitKey(30)
    if key in [ord('q'), ord('Q')]:
      break
  kinect.Stop()


if __name__ == '__main__':
  main()
