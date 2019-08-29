#!/usr/bin/env python

from distutils.core import setup

setup(name='kinect2',
  version='0.1.0',
  description='Python Kinect2 Wrapper',
  author='Joseph Yu',
  author_email='kiddo831007@gmail.com',
  url='https://github.com/kiddos/pykinect2',
  packages=[''],
  package_dir={'': 'build/src'},
  package_data={'': ['kinect2.py', '_kinect2.so']})
