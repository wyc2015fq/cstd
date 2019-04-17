# 使用ROS下标定脚本标定笔记本摄像头或其他单目摄像头 - 站在巨人的肩膀上coding - CSDN博客





2018年04月10日 16:48:52[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：221







   在[上一个ROS教程视觉文章](http://blog.csdn.net/heyijia0327/article/details/41623419)中，我们使用usb_cam包读入并发布了图像消息，但是图像没有被标定，因此存在畸变。ROS官方提供了用于单目或者双目标定的[camera_calibration包](http://wiki.ros.org/camera_calibration)。这个包是使用opencv里的张正友标定法，所以如果你有使用opencv标定的经验，可以直接标定，而不使用官方的程序。


        官方也给出了[单目的标定教程](http://wiki.ros.org/camera_calibration/Tutorials/MonocularCalibration)和[双目视觉的标定教程](http://wiki.ros.org/camera_calibration/Tutorials/StereoCalibration)。本教程基于usb_cam
  package 读取图像，然后使用官方例程进行标定。

1.使用usb_cam 读取图像，launch文件如下：





**[plain]**[view plain](https://blog.csdn.net/heyijia0327/article/details/43538695#)[copy](https://blog.csdn.net/heyijia0327/article/details/43538695#)

- <launch>  
-     <arg name="device" default="/dev/video0" />  
-     <node name="usb_cam_node" pkg="usb_cam" type="usb_cam_node" output="screen">  
-         <param name="video_device" value="$(arg device)" />  
-         <param name="image_width" value="640" />  
-         <param name="image_height" value="480" />  
-         <param name="framerate" value="30" />  
-         <param name="brightness" value="128" />  
-         <param name="contrast" value="128" />  
-         <param name="saturation " value="70" />  
-     </node>  
- </launch>  


2.检查usb_cam package发布的消息：





**[plain]**[view plain](https://blog.csdn.net/heyijia0327/article/details/43538695#)[copy](https://blog.csdn.net/heyijia0327/article/details/43538695#)

- $ rostopic list  

看是否有如下消息，并记住名称
- /usb_cam_node/camera_info
/usb_cam_node/image_raw
3.启动标定程序：




**[plain]**[view plain](https://blog.csdn.net/heyijia0327/article/details/43538695#)[copy](https://blog.csdn.net/heyijia0327/article/details/43538695#)

- $ rosrun camera_calibration cameracalibrator.py --size 11x8 --square 0.03 image:=/usb_cam_node/image_raw camera:=/usb_cam_node  


其中参数size 11x8 注意是**字母x**，他是棋盘**内部角点**个数，如下图所示。square为棋盘正方形边长，注意刚刚开始标定的时候，CALIBRATE按钮是灰色的。

![](https://img-blog.csdn.net/20150206151527139)


4.标定程序使用方法：

       为了得到一个好的标定结果，应该使得标定板尽量出现在摄像头视野的各个位置里：

       如标定板出现在视野中的左边，右边，上边和下边，标定板既有倾斜的，也有水平的，。

       界面中的x：表示标定板在视野中的左右位置。

                     y：表示标定板在视野中的上下位置。

                size：标定板在占视野的尺寸大小，也可以理解为标定板离摄像头的远近。

                skew：标定板在视野中的倾斜位置。

![](https://img-blog.csdn.net/20150206091733735)

并**还要有棋盘占住大部分视野的图片**，如下图所示：


![](https://img-blog.csdn.net/20150206151924041)

       按着上述方式不断移动标定板，直到**CALIBRATE**按钮变亮，点击该按钮就会进行标定。标定过程将持续一两分钟，并且标定界面会变成灰色，无法进行操作，耐心等待即可。

       标定完成以后，你将看到如下图所示的窗口，窗口中的图像为**标定后的结果，纠正了畸变**。

![](https://img-blog.csdn.net/20150206151721225)


        并且，摄像机的各个参数会出现在标定程序的终端窗口中，如下:

![](https://img-blog.csdn.net/20150206152058125)


如果对标定结果满意，点击**COMMIT**按钮将结果保存到默认文件夹，在usb_cam窗口中可以看到如下信息，说明标定结果已经保存在相应文件夹下下次启动usb_cam节点时，会自动调用。

![](https://img-blog.csdn.net/20150206152214459)




## **参数解释**

对于这些参数你可能不太熟悉，没关系，我们一起来看：

![https://images2015.cnblogs.com/blog/1096433/201702/1096433-20170210111214541-236214331.png](https://images2015.cnblogs.com/blog/1096433/201702/1096433-20170210111214541-236214331.png)










