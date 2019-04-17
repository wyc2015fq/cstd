# TODO：gazebo仿真环境的使用，gmapping建图、cartographer建图实验等 - 站在巨人的肩膀上coding - CSDN博客





2018年06月25日 20:46:43[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：728








1、gazebo环境搭建

参考上一篇文章[https://blog.csdn.net/love1055259415/article/details/80805156](https://blog.csdn.net/love1055259415/article/details/80805156)


2、gazebo下gmapping的使用

顺序执行以下命令，先后roslaunch打开 turtlebot_world、gmapping_demo、view_navigation、keyboard_telep.launch

```
roslaunch turtlebot_gazebo turtlebot_world.launch  
roslaunch turtlebot_gazebo gmapping_demo.launch   
roslaunch turtlebot_rviz_launchers view_navigation.launch   
roslaunch turtlebot_teleop keyboard_teleop.launch
```

3、




