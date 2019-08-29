%module kinect2

%{
#define SWIG_FILE_WITH_INIT
#include "kinect2.h"
%}

%include "std_string.i"
%include "numpy.i"
%init %{
import_array();
%}

%apply (uint8_t** ARGOUTVIEWM_FARRAY3, int* DIM1, int* DIM2, int* DIM3) {(uint8_t** data, int* dim1, int* dim2, int* dim3)}
%apply (float** ARGOUTVIEWM_FARRAY3, int* DIM1, int* DIM2, int* DIM3) {(float** data, int* dim1, int* dim2, int* dim3)}

%include "kinect2.h"
