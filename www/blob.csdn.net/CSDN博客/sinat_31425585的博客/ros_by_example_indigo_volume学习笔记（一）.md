# ros_by_example_indigo_volume学习笔记（一） - sinat_31425585的博客 - CSDN博客
2016年10月19日 10:43:12[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：3206
记忆力差的孩子得勤做笔记！
# 关于仿真器的安装可以参考《ros_by_example_indigo_volume》这本书的第五章和第六章，或者直接参考博客[rbx1 package 下载安装过程。](http://www.cnblogs.com/yoyo-sincerely/p/5774622.html)
一、简单节点仿真
1. 运行fake_turtlebot节点：
`$ roslaunch rbx1_bringup fake_turtlebot.launch`
2. 运行rbx1_nav仿真：
`$ rosrun rviz rviz -d  `rospack find rbx1_nav` /sim.rviz`
3. 发布一个topic，让机器人动起来：
`$ rostopic pub -r 10 /cmd_vel  geometry_msgs/Twist  '{ linear:{x: 0.2, y: 0, z: 0 }, angular:{x: 0, y: 0, z: 0.5} }' `
这里特别要注意，命令中冒号（：）后面要有一个空格，不然会报错。运行效果如下：
![](https://img-blog.csdn.net/20161019101424926?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
4. 让机器人停止运动：
首先，用ctrl+c结束掉上条命令的执行，然后运行：
`$  rostopic pub -r 10 /cmd_vel  geometry_msgs/Twist  '{ }' `仿真器中机器人就会停止运动。然后再使用ctrl+c结束掉当前命令即可。
二、导航仿真
1. 运行fake_turtlebot节点
首先得用ctrl+c关闭先前运行的fake_turtlebot节点，然后运行：
`$ roslaunch rbx1_bringup fake_turtlebot.launch`
2. 运行蒙特卡洛粒子滤波算法节点，同时加载地图：
`$ roslaunch rbx1_nav fake_amcl.launch map:=test_map.yaml`
3. 运行rviz仿真器
`$ rosrun rviz rviz -d `rospack find rbx1_nav`/nav_test.rviz`可以看到：
![](https://img-blog.csdn.net/20161019104040209?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
点击左上角2D Nav Goal，选择目标点，即可看到导航的仿真效果。到此，总结完毕！
