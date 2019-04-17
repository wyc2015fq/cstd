# Indigo版本Gazebo2.2.3的启动修复 - 站在巨人的肩膀上coding - CSDN博客





2018年06月25日 17:41:48[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：335










indigo版本的Gazebo是2.2.3, 针对首次运行Gazebo，会出现非常缓慢，原因可能是不能正确下载模型。
`Warning [ModelDatabase.cc:356] Getting models from[http://gazebosim.org/models/]. This may take a few seconds.`
[http://gazebosim.org/models/](http://gazebosim.org/models/)地址已经变换[http://models.gazebosim.org](http://models.gazebosim.org)， 可能导致在访问时候无法自动转换地址。 
解决方法： 通过直接下载所有模型到用户的根目录下的.gazebo/models/下。 
运行：

```
cd ~/.gazebo/
ls
mkdir -p models
ls
cd ~/.gazebo/models/
wget http://file.ncnynl.com/ros/gazebo_models.txt
ls
```

~~新模型~~
~~`wget http://gazebosim.org/models.txt`~~
输出：
`gazebo_models.txt`
运行：

```
wget -i gazebo_models.txt
ls model.tar.g* | xargs -n1 tar xzvf
```

或者直接在：http://models.gazebosim.org/ 下载。[https://pan.baidu.com/s/1pKaeg0F](https://pan.baidu.com/s/1pKaeg0F)



Turtlebot包安装 
安装Turtlebot，打开终端，运行如下命令:

```
sudo apt-get update
sudo apt-get install ros-indigo-turtlebot ros-indigo-turtlebot-apps ros-indigo-turtlebot-interactions ros-indigo-turtlebot-simulator ros-indigo-kobuki-ftdi ros-indigo-rocon-remocon ros-indigo-rocon-qt-library ros-indigo-ar-track-alvar-msgs
```

新窗口运行：
`roscore`
新窗口, 打开turtlebot_gazebo：
`roslaunch turtlebot_gazebo turtlebot_world.launch`
启动Gazebo： 
![这里写图片描述](https://img-blog.csdn.net/20161009162853818)
![这里写图片描述](https://img-blog.csdn.net/20161009163711734)
![这里写图片描述](https://img-blog.csdn.net/20161009163724242)






