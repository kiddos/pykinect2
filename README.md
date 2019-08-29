PyKinect2
=========

[![Build Status](https://travis-ci.org/kiddos/pykinect2.svg?branch=master)](https://travis-ci.org/kiddos/pykinect2)

python wrapper for kinect2 allow easier access of kinect2

## build

install dependencies from libfreenect2

```shell
sudo apt-get install build-essential cmake pkg-config libturbojpeg libjpeg-turbo8-dev mesa-common-dev freeglut3-dev libxrandr-dev libxi-dev
sudo apt-get install libturbojpeg0-dev
sudo apt-get install libusb-1.0-0-dev
```

cmake build

```shell
mkdir build
cd build
cmake ..
make -j
cd ..
```

pip install


```shell
pip install . --user
```

or

```shell
sudo pip install .
```


## Examples

examples on how to fetch rgb images and depth map from kinect2 and the
registered frame

```python
from kinect2 import Kinect2
import cv2
import numpy as np


def main():
  kinect = Kinect2()
  if not kinect.IsOpened():  # check if kinect2 device is connected
    print('no kinect2 device found')
    return

  while True:
    kinect.NextFrame()  # grab the data from libfreenect2 frame
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
