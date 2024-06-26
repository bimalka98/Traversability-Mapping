## Special Notice:

This package also depends on the package [bimalka98/data_base](https://github.com/bimalka98/data_base), when loading and saving the PRM graph. Therefore clone that first to the `catkin_ws/src` folder.

## Git rebase notice on April 19th, 2023

* This branch was rebased to the main branch for further development. 
* Saving the PRM Graph, and loading it to the program in different runtime were successful.
* Add `const bool debugMode = false;` parameter in `void loadPRMGraph()` and `void savePRMGraph()` fucntions of `traversability_prm.cpp` file to surpress the unnecessay debug messages.
* Created separate launch files for mapping and localization.
  - `offline_createMap.launch` = runs lego-loam-bor-create-map node, save global 3D map, and save PRM graph
  - `offline_localize.launch` = runs lego-loam-bor-localize, and load PRM graph

### How to Run

#### 1. Mapping Stage 

* 3D Global map building and saving
* PRM Graph building and saving

```shell
# -----Shell 1-----
# change dir to catkin workspace
$ catkin build
$ source devel/setup.bash 
$ roslaunch traversability_mapping offline_createMap.launch

# -----Shell 2-----
$ rosbag play *.bag --clock
```

#### 2. Localization Stage: Mapping stage must have done before entering this stage

* 3D Global map loading
* PRM Graph loading

```shell
# -----Shell 1-----
# change dir to catkin workspace
$ catkin build
$ source devel/setup.bash 
$ roslaunch traversability_mapping offline_localize.launch


# -----Shell 2-----
$ rosbag play *.bag --clock
```

**Note:** This repository initialized from the https://github.com/TixiaoShan/traversability_mapping repository.

1. [Traversability Mapping and Autonomous Navigation in Urban Area and Rough Terrain](https://youtu.be/B6lrbAEhEnE)
2. [Method: Bayesian Generalized Kernel Inference for Terrain Traversability Mapping](https://youtu.be/4pdBpeRGXmw)

## An image of the used robot is shown below:
<figure>
  <img src="figures/robot.jpg" alt="Robot" style="width: 75%;"/>
  <figcaption>Robot</figcaption>
</figure>


## System Architecture is shown below:

<figure>
  <img src="figures/system_architecture.png" alt="System Architecture" style="width: 75%;"/>
  <figcaption>System Architecture</figcaption>
</figure>

--- 

Original README starts here:

# Traversability Mapping and Motion Planning

This repository contains code for a traversability mapping and motion plannign system for ROS compatible UGVs. The system takes in point cloud from a Velodyne VLP-16 Lidar and outputs a traversability map for autonomous navigation in real-time. A demonstration of the system can be found here -> https://www.youtube.com/watch?v=4pdBpeRGXmw


## Get Started

- Install [ROS](http://www.ros.org/install/).

- Install [LeGO-LOAM](https://github.com/RobustFieldAutonomyLab/LeGO-LOAM).

- Install [ROS Navigation stack](http://wiki.ros.org/navigation). You can install it by running ```sudo apt-get install ros-indigo-navigation```. If you are using other versions of ROS, replace indigo in the command with your ROS version.


## Compile

You can use the following commands to download and compile the package.

```
cd ~/catkin_ws/src
git clone https://github.com/TixiaoShan/traversability_mapping.git
cd ..
catkin_make -j1
```
When you compile the code for the first time, you need to add "-j1" behind "catkin_make" for generating some message types. "-j1" is not needed for future compiling.

## Run the System (in simulation)

1. Run the launch file:
```
roslaunch traversability_mapping offline.launch
```

2. Play existing bag files:
```
rosbag play *.bag --clock --topic /velodyne_points /imu/data
```
Notes: our system only needs /velodyne_points for input from bag files. However, a 3D SLAM method usually needs /imu/data.

## Run the System (with real robot)

Run the launch file:
```
roslaunch traversability_mapping online.launch
```

## Cite *Traversability_Mapping*

Thank you for citing our paper if you use any of this code: 
```
@inproceedings{bayesian2018shan,
  title={Bayesian Generalized Kernel Inference for Terrain Traversability Mapping},
  author={Shan, Tixiao and Wang, Jinkun and Englot, Brendan and Doherty, Kevin},
  booktitle={In Proceedings of the 2nd Annual Conference on Robot Learning},
  year={2018}
}
```
