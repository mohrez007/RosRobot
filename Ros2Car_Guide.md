Here is a summary of the terminal commands to run SLAM Toolbox, RPLiDAR C1, and RViz teleoperation for your robot car.

Ensure that `ROS_DOMAIN_ID=42` (or your chosen ID) is exported in `~/.bashrc` on both machines before running these commands.

---


### **Raspberry Pi Terminals**

**Build after any change on YAML files**
```
cd ~/robot_ws && colcon build --symlink-install && source install/setup.bash

```

**Terminal 1: Robot Base & ROS2 Control**

```bash
cd ~/robot_ws
source install/setup.bash
ros2 launch articubot_one launch_robot.launch.py

```

**Terminal 2: RPLiDAR C1 Node**

```bash
cd ~/robot_ws
source install/setup.bash
ros2 launch articubot_one rplidar.launch.py

```

**Terminal 3: SLAM Toolbox (Mapping)**

```bash
cd ~/robot_ws
source install/setup.bash
ros2 launch slam_toolbox online_async_launch.py slam_params_file:=./src/articubot_one/config/mapper_params_online_async.yaml use_sim_time:=false

```

** For Nav2**
```bash
ros2 launch nav2_bringup bringup_launch.py   map:=/home/pi/robot_ws/maps/my_room_map.yaml   use_sim_time:=false   params_file:=./src/articubot_one/config/nav2_params.yaml
```
---

### **PC Terminals**

**Terminal 1: RViz2 Visualization**

```bash
cd ~/robot_ws
source install/setup.bash
rviz2 -d src/articubot_one/config/main.rviz

```

**Terminal 2: Keyboard Teleoperation**

```bash
cd ~/robot_ws
source install/setup.bash
ros2 run teleop_twist_keyboard teleop_twist_keyboard --ros-args --remap cmd_vel:=/diff_cont/cmd_vel_unstamped

```

---

### **Optional: Save the Generated Map (PC or Raspberry Pi)**

Once you have driven around and built the map, open an extra terminal on either machine and run:

```bash
cd ~/robot_ws
ros2 run nav2_map_server map_saver_cli -f my_room_map

```

**Run Full Navigation with automatic opening terminals**
```
ros2 launch articubot_one navigation_full.launch.py

```
