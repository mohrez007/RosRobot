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




