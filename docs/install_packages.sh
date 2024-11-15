#!/usr/bin/env bash

# Note: you need to type 'sudo -s' in terminal before running the scripts

HOME_USER="/home/admin"
ROS_VERSION='noetic' 

OPENCV_DIR="${HOME_USER}/workspace"
OPENCV_ROS_WS="${OPENCV_DIR}/FOR_OPENCV_SETUP"

# Install OpenCV
echo "------------------------------------"
echo "** Copying Folders into workspace"
echo "------------------------------------"

# Copying Folders in Packages into workspace
cp -r ../Packages/* ${OPENCV_DIR}

echo "------------------------------------"
echo "** Install ros-opencv requirement (1/3)"
echo "------------------------------------"
sudo apt-get update
sudo apt-get install -y libturbojpeg0-dev
sudo apt-get install -y ros-${ROS_VERSION}-people-msgs ros-${ROS_VERSION}-view-controller-msgs ros-${ROS_VERSION}-dynamic-tf-publisher ros-${ROS_VERSION}-dynamic-reconfigure ros-${ROS_VERSION}-nodelet ros-${ROS_VERSION}-nodelet-topic-tools ros-${ROS_VERSION}-camera-calibration-parsers ros-${ROS_VERSION}-camera-info-manager ros-${ROS_VERSION}-stereo-msgs ros-${ROS_VERSION}-roslint ros-${ROS_VERSION}-pcl-ros ros-${ROS_VERSION}-tf-conversions 


echo "------------------------------------"
echo "** Compiling the ros-opencv packages (2/3)"
echo "------------------------------------"
cd ${OPENCV_ROS_WS}
source /opt/ros/${ROS_VERSION}/setup.bash
catkin_make

echo "------------------------------------"
echo "** Install opencv requirement (3/3)"
echo "------------------------------------"
echo "source ${OPENCV_ROS_WS}/devel/setup.bash" >> ${HOME_USER}/.bashrc

echo "** Install opencv packages successfully"

echo "** Bye :)"