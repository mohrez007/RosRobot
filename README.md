# RosRobot
This is my repository for Ros2 Robot car
Required Hardware: Raspberry Pi 4, Desktop PC
Required OS: 
For the Desktop PC:
Go to the Ubuntu MATE website's download page, select "Desktop", and download the Ubuntu MATE 22.04.3 LTS ISO file. Flash this to a USB drive (by RUFUS) to install on your PC.

For the Raspberry Pi:
Open the Raspberry Pi Imager tool on your PC.
Go to Choose OS -> Other specific-purpose OS -> Ubuntu -> scroll down and select Ubuntu MATE 22.04 LTS (Make sure to pick the 64-bit version for your specific Raspberry Pi model).
Ubuntu Mate server edition is good for Rpi.


# Install ROS 2 Humble on Raspberry Pi:
```
sudo apt update && sudo apt install locales software-properties-common curl -y
sudo add-apt-repository universe -y
sudo curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /usr/share/keyrings/ros-archive-keyring.gpg
echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu jammy main" | sudo tee /etc/apt/sources.list.d/ros2.list > /dev/null

sudo apt update
sudo apt install ros-humble-ros-base ros-humble-slam-toolbox ros-humble-navigation2 ros-humble-nav2-bringup -y

echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc
source ~/.bashrc

sudo apt update
sudo apt install ros-humble-demo-nodes-cpp -y
```

# install ROS2 Humble on PC:
```
sudo apt update && sudo apt install locales -y
sudo locale-gen en_US en_US.UTF-8
sudo update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8
export LANG=en_US.UTF-8

sudo apt install software-properties-common -y
sudo add-apt-repository universe -y
sudo apt update && sudo apt install curl -y
sudo curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /usr/share/keyrings/ros-archive-keyring.gpg
echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu jammy main" | sudo tee /etc/apt/sources.list.d/ros2.list > /dev/null

sudo apt update
sudo apt upgrade -y
sudo apt install ros-humble-desktop -y

echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc
source ~/.bashrc
```

# Test the connection
On PC:
```
ros2 run demo_nodes_cpp talker
```

on Rpi:
```
ros2 run demo_nodes_cpp listener
```

# Camera setup
Hardware Privileges (Raspberry Pi Only)
```
sudo usermod -aG video $USER

```

1. Raspberry Pi (The Publisher)

Terminal 1: Start the Camera Capture
This command uses libcamerify to translate the raw camera data, and cam2image to reliably read the format without distorting it. It uses best_effort networking to drop late frames and prevent lag.

```
libcamerify ros2 run image_tools cam2image --ros-args -p width:=640 -p height:=480 -p reliability:=best_effort

```

Terminal 2: Compress the Stream
Open a second SSH session to your Pi. This command listens to the raw image data from the first terminal, compresses it into tiny JPEG files to save Wi-Fi bandwidth, and broadcasts it to the network.

```

ros2 run image_transport republish --ros-args -p in_transport:=raw -p out_transport:=compressed --remap in:=/image --remap out:=/image_compressed

```

2. Desktop PC (The Viewer)
1.Run the decompressor (PC Terminal 1):This node catches the tiny, compressed Wi-Fi stream (/image_compressed/compressed) and unpacks it into a full raw image (/image_viewable) right inside your PC.
```
source /opt/ros/lyrical/setup.bash
ros2 run image_transport republish --ros-args -p in_transport:=compressed -p out_transport:=raw --remap in:=/image_compressed --remap out:=/image_viewable
```

2.Clear cache and launch viewer (PC Terminal 2):We will launch the viewer with the --clear-config flag. This deletes its memory, breaking the crash loop and opening a fresh window.

```
source /opt/ros/lyrical/setup.bash
ros2 run rqt_image_view rqt_image_view --clear-config
```

Once the fresh window opens:

    Click the dropdown menu at the top left.

    Select /image_viewable (do not select the compressed one!).



# Check Lidar and Arduino ports

```
ls /dev/ttyACM*
ls /dev/ttyUSB*
```

# Ros Robot 
Rpi
Terminal1:
sudo chmod 666 /dev/ttyACM0
ros2 run my_robot_bridge serial_node

Lidar:
ros2 run rplidar_ros rplidar_node --ros-args -p serial_port:=/dev/ttyUSB0 -p serial_baudrate:=460800 -p frame_id:=laser_frame -p angle_compensate:=true


PC:
ros2 run teleop_twist_keyboard teleop_twist_keyboard






