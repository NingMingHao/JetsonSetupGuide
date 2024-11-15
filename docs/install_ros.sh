#!/usr/bin/env bash

# Note: you need to type 'sudo -s' in terminal before running the scripts

ROS_VERSION='noetic' 
HOME_USER="/home/admin"

echo "------------------------------------"
echo "** Install ROS (1/2)"
echo "------------------------------------"

sh -c 'echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list'
apt install curl # if you haven't already installed curl
curl -s https://raw.githubusercontent.com/ros/rosdistro/master/ros.asc | sudo apt-key add -
apt update && apt-get install -y --no-install-recommends \
	build-essential \
    ros-${ROS_VERSION}-desktop-full

source /opt/ros/noetic/setup.bash
echo "source /opt/ros/noetic/setup.bash" >> ${HOME_USER}/.bashrc

apt-get install -y --no-install-recommends python3-rosdep python3-rosinstall python3-rosinstall-generator python3-wstool build-essential

rosdep init
rosdep update

echo "------------------------------------"
echo "** Install ROS dependencies (2/2)"
echo "------------------------------------"
# install ROS related dependencies
apt-get install -y python3-pip python3-yaml

pip3 install --upgrade pip

pip3 install rospkg catkin_pkg    

apt-get install -y python3-dev python3-numpy python3-catkin-tools

apt-get install -y --no-install-recommends ros-${ROS_VERSION}-jsk-common ros-${ROS_VERSION}-jsk-recognition-msgs ros-${ROS_VERSION}-gps-common ros-${ROS_VERSION}-jsk-rviz-plugins libgeographic-dev ros-${ROS_VERSION}-geographic-msgs libpugixml-dev ros-${ROS_VERSION}-nmea-msgs libeigen3-dev ros-${ROS_VERSION}-grid-map ros-${ROS_VERSION}-cmake-modules libglew-dev libpcap-dev  ros-${ROS_VERSION}-lanelet2 ros-${ROS_VERSION}-unique-id ros-${ROS_VERSION}-pacmod-msgs  ros-${ROS_VERSION}-usb-cam libcanberra-gtk-module libcanberra-gtk3-module ros-${ROS_VERSION}-tf2-sensor-msgs ros-${ROS_VERSION}-lanelet2-core ros-${ROS_VERSION}-lanelet2-io ros-${ROS_VERSION}-lanelet2-maps ros-${ROS_VERSION}-lanelet2-projection  ros-${ROS_VERSION}-unique-id ros-${ROS_VERSION}-can-msgs ros-${ROS_VERSION}-socketcan-bridge ros-${ROS_VERSION}-roswww ros-${ROS_VERSION}-joint-state-publisher-gui ros-${ROS_VERSION}-nmea-navsat-driver ros-${ROS_VERSION}-mavros ros-${ROS_VERSION}-mavros-extras ros-${ROS_VERSION}-joy ros-${ROS_VERSION}-rviz-imu-plugin 

apt-get install -y libturbojpeg libturbojpeg0-dev

pip3 install -U jetson-stats

echo "------------------------------------"
echo "** Install ROS "${ROS_VERSION}" successfully"
echo "------------------------------------"

echo "** Bye :)"