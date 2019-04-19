# SFM,VO,SLAM,BA,Graph Optimization的区别 - KinboSong的博客 - CSDN博客
2017年05月20日 20:40:28[songjinbo3](https://me.csdn.net/KinboSong)阅读数：2029
1、SLAM  
 SLAM 分为很多种，目前主要有：PATM、MonoSLAM、ORB-SLAM、RGBD-SLAM、RTAB-SLAM、LSD-SLAM
SLAM这里主要讨论VSLAM，其中还可能包含其他传感器，如IMU、GPS、COMPASS等
VSLAM主要包含建图（包含回环检测）和后端优化两个部分
建图可以分为光流法、直接法和基于特征的方法；
直接法参考：[http://www.cnblogs.com/gaoxiang12/p/5689927.html](http://www.cnblogs.com/gaoxiang12/p/5689927.html)
基于特征的方法如下：
在已经接触到的RGBD-SLAM和MonoSLAM前端主要分为feature detect and descriptor、motion estimation等步骤，
feature detector可以分为point-feature detectors （比如：Moravec、Forstner、Harris、Shi-Tomasi、Fast）和 blob detectors（比如：SIFT、SURF、CENSURE）(《visual odometry part II》)
motion estimation 有3D-to-3D方法（如ICP算法）、3D-to-2D方法（D.nister 的《visual odometry》）、2D-to-2D（还没见过，应该有）方法 《visual odometry part I》
2、VO
Monocular VO 可以分为feature-based method、appearance-based method and hybrid method (《visual odometry part I》)
3、VO Versus SLAM
The goal of SLAM in general (and V-SLAM in particular) is to obtain a global ,consistent estimate of the robot path.This implies keeping a track of a map of the enviroment(even in the case where the map is not needed per se) because it is needed to realize
 when the robot returns to a previously visited area.(This is called loop closure.When a loop closure is detected ,this information is used to reduce the drift in both the map and camera path.Understanding when a loop closure occurs and efficiently integrating
 this new constraint into the current map are two of the main issues in SLAM.)Conversely,VO aims at recovering the path incrementally,pose after pose, and potentially optimizing only over the last n poses of the path(this is also called windowed bundle adjustment).This
 sliding window optimization can be considered equivalent to building a local map in SLAM;however,the philosophy is different:in VO,we only care about local consistency of the trajectory and the local map is used to obtain a more accurate estimate of the local
 trajectory (for example,in bundle adjustment),whereas SLAM is concerned with the global map consistency.  (《visual odometry part I》)
大意就是：VO专注于改善局部的轨迹，而SLAM专注于全局的轨迹c
4、SFM Versus SLAM
参考：[https://www.quora.com/What-is-the-difference-between-SfM-and-Visual-SLAM](https://www.quora.com/What-is-the-difference-between-SfM-and-Visual-SLAM)
（1）SFM 图像无序的、离线处理的，对处理时间要求不严格；SLAM图像有序的，在线处理的，对处理时间要求严格；
（2）SFM一般用在大的范围，如构建谷歌地图；SLAM中尺度较大也就达到几千米。
5、BA（bundle adjustment）
Definition of reference frames for the bundle adjustment problem.There is a stationary reference frame and a moving reference frame, attached to a vehicle.A collection of points,Pj,are observed by the moving vehicle (using a camera) and the goal is to determine
 the relative pose of the moving frame with respect to the stationary one(at all of the times of interest) as well as the positions of all the points in the stationary frame.(摘自：《state estimation for robotics》第9章)
简而言之，BA算法是估计运动相机的位姿并且估计三维场景点的位置。
6、Graph Optimization
只要将优化问题表示成图的形式，这个优化问题就叫做图优化。
7、BA Versus Graph Optimization
通过以上关于Graph Optimization和BA算法的定义可以知道，BA算法属于图优化。所以既可以说SLAM的后端处理用的是BA算法，也可以说用的是图优化。
