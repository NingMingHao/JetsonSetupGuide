#!/usr/bin/env bash

# Note: you need to type 'sudo -s' in terminal before running the scripts

HOME_USER="/home/admin"
OPENCV_VERSION="4.5.5"
OPENCV_DIR="${HOME_USER}/workspace"
CUDA_ARCH_BIN='8.7'

# Install OpenCV
echo "------------------------------------"
echo "** Opencv Install Process"
echo "------------------------------------"

echo "------------------------------------"
echo "** Install opencv requirement (1/4)"
echo "------------------------------------"
sudo apt-get update
sudo apt-get install -y build-essential cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev checkinstall libavresample-dev libfaac-dev libfaac0 libffms2-4 libgpac4 libmp3lame-dev libopencore-amrnb-dev libopencore-amrwb-dev libvorbis-dev libx264-dev libxvidcore-dev x264 yasm
sudo apt-get install -y libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev
sudo apt-get install -y python3.8-dev python-dev python-numpy python3-numpy
sudo apt-get install -y libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libdc1394-22-dev libdc1394-22 libxine2-dev libv4l-dev
sudo apt-get install -y libv4l-dev v4l-utils qv4l2 
sudo apt-get install -y opencl-headers

cd /usr/include/linux
ln -s -f ../libv4l1-videodev.h videodev.h
cd ~

sudo apt-get install -y libgtk-3-dev libtbb-dev libatlas-base-dev gfortran libopenblas-dev libblas-dev libprotobuf-dev protobuf-compiler libgoogle-glog-dev libgflags-dev libgphoto2-dev libeigen3-dev libhdf5-dev doxygen libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev ocl-icd-opencl-dev libgtkglext1-dev libgtkglext1 libgtk3.0-cil-dev



echo "------------------------------------"
echo "** Download opencv "${OPENCV_VERSION}" (2/4)"
echo "------------------------------------"
mkdir ${OPENCV_DIR}
cd ${OPENCV_DIR}
curl -L https://github.com/opencv/opencv/archive/${OPENCV_VERSION}.zip -o opencv-${OPENCV_VERSION}.zip
curl -L https://github.com/opencv/opencv_contrib/archive/${OPENCV_VERSION}.zip -o opencv_contrib-${OPENCV_VERSION}.zip
unzip opencv-${OPENCV_VERSION}.zip
unzip opencv_contrib-${OPENCV_VERSION}.zip
rm opencv-${OPENCV_VERSION}.zip opencv_contrib-${OPENCV_VERSION}.zip
cd opencv-${OPENCV_VERSION}/


echo "------------------------------------"
echo "** Build opencv "${OPENCV_VERSION}" (3/4)"
echo "------------------------------------"
mkdir release
cd release/
cmake -D CMAKE_BUILD_TYPE=RELEASE \
        -D CMAKE_CXX_FLAGS_RELEASE="-O3" \
        -D CMAKE_INSTALL_PREFIX=/usr/local \
        -D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib-${OPENCV_VERSION}/modules  \
        -D OPENCL_INCLUDE_DIR=/usr/include \
        -D OPENCL_LIBRARY=/usr/lib/aarch64-linux-gnu/libOpenCL.so.1 \
        -D BUILD_TIFF=ON \
        -D WITH_GSTREAMER=ON \
        -D WITH_TBB=ON \
        -D BUILD_TBB=ON \
        -D OpenBLAS_INCLUDE_DIR=/usr/include/openblas \
        -D OpenBLAS_LIB=/usr/lib/aarch62-linux-gnu/libopenblas.so \
        -D Atlas_CLAPACK_INCLUDE_DIR=/usr/include/aarch62-linux-gnu/atlas \
        -D Atlas_INCLUDE_DIR=/usr/include/aarch62-linux-gnu/atlas \
        -D Atlas_LIB_DIR=/usr/lib/aarch62-linux-gnu \
        -D Atlas_CBLAS_LIBRARY=/usr/lib/aarch62-linux-gnu/libcblas.so \
        -D Atlas_BLAS_LIBRARY=/usr/lib/aarch62-linux-gnu/libatlas.so \
        -D WITH_EIGEN=ON \
        -D WITH_V4L=ON \
        -D WITH_LIBV4L=ON \
        -D WITH_VTK=OFF \
        -D WITH_OPENGL=OFF \
        -D WITH_OPENCL=ON \
        -D WITH_LAPACK=ON \
        -D BUILD_WEBP=OFF \
        -D OPENCV_ENABLE_NONFREE=ON \
        -D INSTALL_C_EXAMPLES=OFF \
        -D PYTHON3_EXECUTABLE=/usr/bin/python3 \
        -D PYTHON_INCLUDE_DIR=/usr/include/python3.8 \
        -D PYTHON_LIBRARY=/usr/lib/libpython3.8.so \
        -D PYTHON3_NUMPY_INCLUDE_DIRS=/usr/lib/python3/dist-packages/numpy/core/include \
        -D BUILD_OPENCV_PYTHON3=ON \
        -D INSTALL_PYTHON_EXAMPLES=ON \
        -D BUILD_NEW_PYTHON_SUPPORT=ON \
        -D OPENCV_GENERATE_PKGCONFIG=ON \
        -D BUILD_TESTS=OFF \
        -D BUILD_EXAMPLES=OFF \
        -D BUILD_PERF_TESTS=OFF \
        -D WITH_CUDA=ON \
        -D ENABLE_FAST_MATH=ON \
        -D CUDA_FAST_MATH=ON \
        -D WITH_CUDNN=ON \
        -D OPENCV_DNN_CUDA=ON \
        -D CUDA_ARCH_BIN=${CUDA_ARCH_BIN} \
        -D CUDA_ARCH_PTX=${CUDA_ARCH_BIN} \
        -D WITH_CUBLAS=ON ..
make -j$(nproc)


echo "------------------------------------"
echo "** Install opencv "${OPENCV_VERSION}" (4/4)"
echo "------------------------------------"
sudo make install
echo 'export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH' >> ${HOME_USER}/.bashrc
echo 'export PYTHONPATH=/usr/local/lib/python3.8/site-packages/:$PYTHONPATH' >> ${HOME_USER}/.bashrc
source ${HOME_USER}/.bashrc


echo "** Install opencv "${OPENCV_VERSION}" successfully"


echo "** Bye :)"