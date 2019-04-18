# ROS5—理解ROS节点 - 心纯净，行致远 - CSDN博客





2018年10月04日 17:30:56[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：224








这篇教程会介绍ROS图概念和并讨论roscore、rosnode和rosrun命令的使用。

1.预备条件

这里会使用轻量级的模拟器，先安装

$ sudo apt-get install ros--ros-tutorials

1

将替换成你的ROS版本名称。

2.图概念的快速概览

节点（nodes）：节点是一个利用ROS来与其他节点通信的可执行文件。

消息（messages）：在顶阅或推送一个主题（topic）时使用的ROS数据类型。

主题（topics）：节点可以向主题公布（publish）消息或向主题订阅（subscribe）来接受其消息。

主控（master）：ROS的名称服务。

rosout：stdout/stderr的ROS等价物。

roscore：master+rosout+参数（parameter）服务器。
3.节点

节点仅是ROS包中的一个可执行文件。ROS节点利用一个ROS客户端库（client library）来与其他节点通信。节点可以向一个主题（Topic）公布或订阅消息；也能利用或提供一个服务（Service）。

4.客户端库

ROS客户端库允许节点被写入不同的编程语言来通信：
- rospy = python client library
- roscpp = c++ client library

5.roscore

在使用ROS时首先运行roscore。

roscore

1

你会看到类似这样的消息：

… logging to ~/.ros/log/9cf88ce4-b14d-11df-8a75-00251148e8cf/roslaunch-machine_name-13039.log

Checking log directory for disk usage. This may take awhile.

Press Ctrl-C to interrupt

Done checking log file disk usage. Usage is <1GB.
started roslaunch server http://machine_name:33919/

ros_comm version 1.4.7

# SUMMARY

PARAMETERS
- /rosversion
- /rosdistro

NODES

auto-starting new master

process[master]: started with pid [13054]

ROS_MASTER_URI=http://machine_name:11311/
setting /run_id to 9cf88ce4-b14d-11df-8a75-00251148e8cf

process[rosout-1]: started with pid [13067]

started core service [/rosout]

123456789101112131415161718192021222324
若roscore未初始化，则需进行网络配置。

若roscore没有初始化并发送了缺少权限的消息，则~/.ros文件夹被root所有，递归地改变那个文件夹的所属：

$ sudo chown -R <your_username> ~/.ros

1

6.使用rosnode

新打开一个终端，使用rosnode来看运行的roscore做的。。。

rosnode显示关于现在运行的ROS节点的信息。rosnode list命令累出这些活动的节点。

$ rosnode list

1

你会看到

/rosout

1

rosnode info命令返回一个特定的节点的信息。

$ rosnode info /rosout

1

这会给我们更多关于rosout的信息：

Node [/rosout]

Publications:
- /rosout_agg [rosgraph_msgs/Log]

Subscriptions:
- /rosout [unknown type]

Services:
- /rosout/set_logger_level
- /rosout/get_loggers

contacting node http://machine_name:54614/ …

Pid: 5092

1234567891011121314
接下来用rosrun来创建另一个节点。

7使用rosrun

rosrun使用包的名称来直接在一个包内运行一个节点（而不必知道包的路径）。

$ rosrun [package_name] [node_name]

1

现在运行turtlesim包中的turtlesim_node。打开一个新的终端，并运行：

$ rosrun turtlesim turtlesim_node

1

就可以看到一下窗口：

你看到的turtlesim可能会不同。

在新终端中输入rosnode list就会看到多了turtlesim节点。

可以在命令行给节点重新命名。关闭turtlesim窗口一结束该节点并使用一个重映射参数重新运行此命令来改变节点的名称：
$ rosrun turtlesim turtlesim_node --name:=my_turtle

1

也可以使用新的rosnode命令，ping，来检测节点是否在线：

$ rosnode ping my_turtle

1

这会看到：

rosnode: node is [/my_turtle]

pinging /my_turtle with a timeout of 3.0s

xmlrpc reply from [http://aqy:42235/](http://aqy:42235/)     time=1.152992ms

xmlrpc reply from [http://aqy:42235/](http://aqy:42235/)     time=1.120090ms

xmlrpc reply from [http://aqy:42235/](http://aqy:42235/)     time=1.700878ms

xmlrpc reply from [http://aqy:42235/](http://aqy:42235/)     time=1.127958ms

123456
8.回顾

这篇教程包含了：
- roscore = ros + core:：master(提供ROS的名称服务) + rosout(stdout/stderr) + 参数服务(后面介绍)
- rosnode = ros + node： 获得节点消息的ROS工具。
- rosrun = ros + run： 从给定的包中运行节点。

本文来自 InfinityForever 的CSDN 博客 ，全文地址请点击：[https://blog.csdn.net/InfinityForever/article/details/50492473?utm_source=copy](https://blog.csdn.net/InfinityForever/article/details/50492473?utm_source=copy)










