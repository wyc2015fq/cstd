# Ubuntu16.04安装ROS - 心纯净，行致远 - CSDN博客





2018年09月18日 22:02:29[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：1366标签：[ROS																[ubuntu](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)](https://so.csdn.net/so/search/s.do?q=ROS&t=blog)
个人分类：[【ROS】](https://blog.csdn.net/zhanshen112/article/category/8068262)








环境：Ubuntu16.04

官方链接：http://wiki.ros.org/kinetic/Installation/Ubuntu

ROS每年更新一个版本，其中偶数年份的为LTS版本，为长期支持版本，不同ROS版本对应不同Ubuntu，这里的安装环境为Ubuntu16.04，因此安装ROS Kinetic版本。

如何查看电脑Ubuntu版本呢，可以直接开一个终端输入：lsb_release -a

显示如下

Distributor ID: Ubuntu                           //类别是ubuntu

Description:  Ubuntu 16.04.3 LTS          //16年3月发布的稳定版本，LTS是Long Term Support：长时间支持版本，支持周期长达三至五年

Release:    16.04                                    //发行日期或者是发行版本号

Codename:   xenial                               //ubuntu的代号名称

![](https://img-blog.csdn.net/20180607201856269?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0thbGVuZWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

1. 安装软件源，建议采用国内软件源，下面的为exbot的软件源。
`sudo sh -c '. /etc/lsb-release && echo "deb http://ros.exbot.net/rospackage/ros/ubuntu/ $DISTRIB_CODENAME main" > /etc/apt/sources.list.d/ros-latest.list'`
2. 配置key。
`sudo apt-key adv --keyserver hkp://ha.pool.sks-keyservers.net:80 --recv-key 421C365BD9FF1F717815A3895523BAEEB01FA116`
3. 更新并安装，这里为方便之后学习安装完全版。
`sudo apt-get update``sudo apt-get install ros-kinetic-desktop-full`
```
sudo rosdep init
rosdep update
```

4. 初始化环境，否则每次启动都需要source一次

```
echo "source /opt/ros/kinetic/setup.zsh" >> ~/.zshrc
source ~/.zshrc
```

5. 安装额外依赖包
`sudo apt-get install python-rosinstall python-rosinstall-generator python-wstool build-essential`
6. 测试，若可显示下图乌龟则配置成功。
`roscore``rosrun turtlesim turtlesim_node`
rosrun turtlesim turtle_teleop_key

再每个终端开始之前都要先运行一遍source  /opt/ros/indigo/setup.bash 不然的话，终端会显示命令不存在，第二个指令执行后，会打开一个窗口，这个窗口会显示一个模拟龟型机器人，第三个指令执行后，按键盘上的上下左右按键，乌龟就会按照你的指令开始移动，并显示轨迹。

相关链接：


[https://blog.csdn.net/kalenee/article/details/80614606](https://blog.csdn.net/kalenee/article/details/80614606)

[https://blog.csdn.net/qq_35239859/article/details/77938646](https://blog.csdn.net/qq_35239859/article/details/77938646)




