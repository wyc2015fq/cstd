# ROS学习笔记（九）- 理解ROS服务和参数 - weixin_33985507的博客 - CSDN博客
2017年03月03日 15:17:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
*这一节介绍ROS服务和参数，以及rosservice和rosparam命令的使用*
我们的小乌龟仍然在运行，可以看一下它提供了什么服务：
# 1 ROS服务
服务是node跟其他node交流的另一种方式。Service允许node发送一个请求然后接受一个回复。
# 2 使用rosservice
它可以轻松地通过Service连接到ROS客户端或服务框架。它有很多命令可以用在topic上。
用法：
```
rosservice list         print information about active services
rosservice call         call the service with the provided args
rosservice type         print service type
rosservice find         find services by service type
rosservice uri          print service ROSRPC uri
```
# 2.1 rosservice list
`$ rosservice list`
运行后显示turtlesim 提供了9个服务，还有与rosout 相关的服务。因为之前用Ctrl+c结束了一些东西，也遗留下了别的服务：
```
/clear
/kill
/reset
/rosout/get_loggers
/rosout/set_logger_level
/rostopic_4364_1488456319134/get_loggers
/rostopic_4364_1488456319134/set_logger_level
/rostopic_7446_1488504811853/get_loggers
/rostopic_7446_1488504811853/set_logger_level
/rqt_gui_py_node_3638/get_loggers
/rqt_gui_py_node_3638/set_logger_level
/spawn
/teleop_turtle/get_loggers
/teleop_turtle/set_logger_level
/turtle1/set_pen
/turtle1/teleport_absolute
/turtle1/teleport_relative
/turtlesim/get_loggers
/turtlesim/set_logger_level
```
## 2.2 rosservice type
用法：
`rosservice type [service]`
我们来看看clear服务是什么类型:
`$ rosservice type /clear`
显示：
```
std_srvs/Empty
```
这个意思就是它发送的请求和回复都没有任何参数。
## 2.3 rosservice call
用法：
`rosservice call [service] [args]`
我们呼叫这个服务的时候不需要参数：
`$ rosservice call /clear`
然后我们发现小海龟的背景清除干净了。
再看个需要参数的：
`$ rosservice type /spawn| rossrv show`
```
float32 x
float32 y
float32 theta
string name
---
string name
```
这个服务可以让我们创建一只新海龟：
`$ rosservice call /spawn 2 2 0.2 ""`
它会返回小海龟名称。
# 3 使用rosparam
这个命令可以用来在ROS参数服务器存储和操作参数。
- rosparam uses the YAML markup language for syntax. In simple cases, YAML looks very natural: 1 is an integer, 1.0 is a float, one is a string, true is a boolean, [1, 2, 3] is a list of integers, and {a: b, c: d} is a dictionary. rosparam has many commands that can be used on parameters, as shown below:*
用法：
```
rosparam set            set parameter
rosparam get            get parameter
rosparam load           load parameters from file
rosparam dump           dump parameters to file
rosparam delete         delete parameter
rosparam list           list parameter names
```
## 3.1 rosparam list
`$ rosparam list`
显示这个：
```
/background_b
/background_g
/background_r
/rosdistro
/roslaunch/uris/host_ubuntu__35711
/rosversion
/run_id
```
能看出来有三个参数决定背景颜色，我们试着改一下：
## 3.2 rosparam set and rosparam get
用法：
```
rosparam set [param_name]
rosparam get [param_name]
```
现在改变红色通道：
`$ rosparam set /background_r 150`
现在我们需要让clear服务使参数生效：
`$ rosservice call /clear`
获取绿色通道参数：
`$ rosparam get /background_g`
还可以让它显示整个service的内容：
`$ rosparam get /`
返回结果如下：
```
background_b: 255
background_g: 86
background_r: 200
rosdistro: 'kinetic
  '
roslaunch:
  uris: {host_ubuntu__35711: 'http://ubuntu:35711/'}
rosversion: '1.12.6
  '
run_id: f56bed9c-ff18-11e6-b6a3-000c29724ec8
```
你可能想存下来参数下次再读进来，可以的：
## 3.3 rosparam dump and rosparam load
用法：
```
rosparam dump [file_name] [namespace]
rosparam load [file_name] [namespace]
```
我们把所有参数写入 params.yaml：
`$ rosparam dump params.yaml`
可以在新的命名空间重新加载：
```
$ rosparam load params.yaml copy
$ rosparam get /copy/background_b
```
