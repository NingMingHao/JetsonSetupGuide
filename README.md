# JetsonSetupGuide
**What does this repository do?**
- Install ROS on Jetson
- Install OpenCV with CUDA support on Jetson
- Install some ROS packages relying on OpenCV
- Clone the Jetson NVME SSD (Optional)

**Tested on**
- Jetson AGX Orin
- Jetson AGX Orin NX

**Tested software version**
- JetPack 5.1.2, JetPack 5.1.3, JetPack 5.1.4
- ROS Noetic
- OpenCV 4.5.5

## First, you need to install the JetPack
Follow the [instructions](https://docs.isarsoft.com/administration/install-jetpack/) to install the JetPack using the SDK Manager if you have not done it yet.

## Things to do after installing JetPack
1. Clone this repository.
2. Modify the `HOME_USER`, `OPENCV_VERSION`, `CUDA_ARCH_BIN` and `ROS_VERSION` in the `install_ros.sh` and `install_opencv.sh` and `install_packages.sh` scripts. You may go to [cuda-arch-bin](https://developer.nvidia.com/cuda-gpus) to find the `CUDA_ARCH_BIN` for your GPU.
3. Run the scripts in the `docs` folder.

### For a new Jetson, you need to install the ros
#### Note: you need to type 'sudo -s' in terminal before running the scripts
```
cd docs
sudo -s
bash install_ros.sh
```

### Then compile the OpenCV with CUDA support
#### Note: you need to type 'sudo -s' in terminal before running the scripts
```
cd docs
sudo -s
bash install_opencv.sh
```

### After Reinstall OpenCV, we need to compile some ROS packages relying on OpenCV
#### Note: you need to type 'sudo -s' in terminal before running the scripts
```
cd docs
sudo -s
bash install_packages.sh
```


## Clone the Jetson NVME SSD
For the case where you have multiple Jetson devices, you can clone the NVME SSD to save time. 
* Before cloning, you may also install other necessary softwares (like VS code, camera driver, etc.) on the Jetson to avoid the installation process.

I have tried to use the tools in `Linux_for_Tegra` to clone the NVME SSD, but it did not work. The whole process will be stuck at the 'Flashing the device' step. So I switched other tools to clone the NVME SSD. The `dd` command can work, however, it is not recommended because it will clone the whole disk, including the empty space. Finally, I found the `Clonezilla` tool can work well.

**What you need**
- A NVME SSD
- A NVME SSD enclosure adapter
- A USB drive
- A Host PC to save the clone image

**Step 1: Prepare the USB drive**
1. Download the Clonezilla live **iso** file from the [official website](https://clonezilla.org/downloads/download.php?branch=stable).
2. Use the 'Startup Disk Creator' to create a bootable USB drive with the Clonezilla live **iso** file.

**Step 2: Clone the NVME SSD**
1. Boot the Host PC with the USB drive.
2. When Clonezilla boots, select `Clonezilla live` and press `Enter`.
3. Choose your language and select Keep the default keyboard layout.
4. Select `Start Clonezilla`.
5. Choose `device-device` mode if you want to clone directly from one drive to another or `device-image` mode if you want to create an image file of the drive.
6. For the first time, you need to select `local_dev` to specify the destination path to save the image file.
7. For Device-Image Clone (Save an Image):
	1.	Select Beginner mode.
	2.	Choose `savedisk` to save the entire disk as an image.
	3.	Select the source drive.
	4.	Confirm the operation, and Clonezilla will create a compressed image of the drive.

**Step 3: Restore the NVME SSD**
1. Choose `device-image` mode.
2. For Device-Image Clone (Restore an Image):
    1.	Select Beginner mode.
    2.	Choose `restoredisk` to restore the entire disk from an image.
    3.	Select the image file.
    4.	Select the destination drive.
    5.	Confirm the operation, and Clonezilla will restore the image to the drive.
