PyKinect2
=========

python wrapper for kinect2 allow easier access of kinect2

## setup

* install [libfreenect](https://github.com/OpenKinect/libfreenect2)

```shell
git clone https://github.com/OpenKinect/libfreenect2
cd libfreenect2
mkdir build
cd build
cmake -D CMAKE_INSTALL_PREFIX=/usr/local ..
make
[sudo] make install
```

* install numpy

```shell
[sudo] pip install numpy
```

* install package

```shell
[sudo] ./setup.py install
```

## examples

examples on how to fetch rgb images and depth map from kinect2 and the
registered frame

```python
from kinect2 import Kinect2
import cv2
import numpy as np


def main():
  kinect = Kinect2()
  while True:
    kinect.NextFrame()
    rgb = kinect.RGB()
    ir = kinect.IR()
    depth = kinect.Depth()
    undistorted = kinect.Undistorted()
    registered = kinect.Registered()

    rgb = np.reshape(np.ravel(rgb, 'F'), rgb.shape)
    ir = np.reshape(np.ravel(ir, 'F'), ir.shape) / 1000.0
    depth = np.reshape(np.ravel(depth, 'F'), depth.shape)
    undistorted = np.reshape(np.ravel(undistorted, 'F'), undistorted.shape)
    registered = np.reshape(np.ravel(registered, 'F'), registered.shape)

    cv2.imshow('RGB', rgb)
    cv2.imshow('IR', ir)
    cv2.imshow('Depth', depth)
    cv2.imshow('Undistorted', undistorted)
    cv2.imshow('Registered', registered)
    key = cv2.waitKey(30)
    if key in [ord('q'), ord('Q')]:
      break
  kinect.Stop()


if __name__ == '__main__':
  main()
```
