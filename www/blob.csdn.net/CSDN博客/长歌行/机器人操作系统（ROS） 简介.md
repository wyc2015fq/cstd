# 机器人操作系统（ROS） 简介 - 长歌行 - CSDN博客





2017年08月01日 17:18:32[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：550








关于[机器人](http://lib.csdn.net/base/robot)[操作系统](http://lib.csdn.net/base/operatingsystem)（ROS）的一点介绍，更多请见[GitHub](https://github.com/knowledge-warehousing/ROS-introduction)






# 常用命令

## 配置



配置账户需要输入的

`rosdep update`

将ros包的环境加入到当前控制台

`source ./devel/setup.bash`

将ros环境加入到当前控制台的环境变量

`source /opt/ros/indigo/setup.bash`

获取所有已安装ROS软件包列表清单

`rospack list`

编译

`catkin_makecatkin_make install`

编译工程

`catkin_make`

看依赖

`rospack depends1 beginner_tutorials`

切到包中

`roscd beginner_tutorials`

输出包的安装目录

`pwd`

创建功能包

`catkin_create_pkg package-name`

在roslaunch中请求输出详细信息

`roslaunch -v package-name launch-file-name`

在控制台中设置所有节点的输出

`roslaunch -screen package-name launch-file-name`

新窗口中显示数据

`加入前缀， launch-prefix=“xetrm-e”`
`xterm -e rosrun package node`

更换节点名

`rosrun turtlesim turtlesim_node turtle1/pos:=tim<remap from="org-name"to "new-name">`

## 操作

运行核心程序

`roscore`

运行某一个节点

`rosrun turtlesim turtlesim_node`

## 调试

查看当前系统运行情况的动态图形

`rosrun rqt_graph rqt_graph`

问题检查

`roswtf`

查看/rosout消息

`rqt_console`

查看参数列表

`rosparam list`

查看参数值

`rosparam get param_name`

设置参数

`rosparam set param_name`

`

## ROS node

完全删除已经运行完毕节点的记录

`rosnode cleanup`

获取特定节点的信息

`rosnode info node-name`

查看发布到某个话题上面的图形

`$ rosrun rqt_plot rqt_plot`

## ROS topic

查看消息类型

`$ rostopic type /turtle1/cmd_vel`

`你应该会看到:geometry_msgs/Twist`

将数据发布到某个正在广播的话题上

`rostopic pub topic [args]`

查看在某个话题上发布的数据

`rostopic echo [topic]`

示例代码：

`rostopic echo /turtle1/cmd_vel`

查看tf tree 
`rosrun rqt_tf_tree rqt_tf_tree`

列出当前发布的话题

`rostopic list -v`

查看frame id

`rostopic echo /first -n 1 --noarr 找到frame id`

查看更多信息

`rostopic info topic-name`

`rostopic echo topic-name -n 1 --noarr`

使用命令行发布数据

`rostopic pub -r rate-in-hz topic-name message-type message content`

查找使用某消息的话题

`$ rostopic find /message_type:`

## ROS messages

查看消息的详细情况

`$ rosmsg show geometry_msgs/Twist`

```
geometry_msgs/Vector3 linear

      float64 x

      float64 y

      float64 z

    geometry_msgs/Vector3 angular

      float64 x

      float64 y

      float64 z
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15

`$ rosmsg list: This lists all message`

`$ rosmsg package [package_name]: This lists messages in a package`

## ROS services

```
rosservice list         输出可用服务的信息

    rosservice call         调用带参数的服务

    rosservice type         输出服务类型

    rosservice find         依据类型寻找服务find services by service type

    rosservice uri          输出服务的ROSRPC uri
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10

## ROS bags

录制

`$ rosbag record topic_name -o bag_name`

We can also record all topics using the -a argument.`

播放 
`$ rosbag play [bag_name]: This will playback the existing bag file.`

查看信息

`rosbag info NAME`

`rosbag play NAME`

# 结构

## Structure
![](https://github.com/knowledge-warehousing/ROS-introduction/blob/master/img/ROS-Graph-layer-structure.jpg?raw=true)






The following are abstracts of each graph’s concepts:
- Nodes: Nodes are the process that perform computation. Each ROS node is written

using ROS client libraries such as roscpp and rospy. Using client library APIs, we 
can implement different types of communication methods in ROS nodes. In a robot, 
there will be many nodes to perform different kinds of tasks. Using the ROS 
communication methods, it can communicate with each other and exchange data. 
One of the aims of ROS nodes is to build simple processes rather than a large process 
with all functionality. Being a simple structure, ROS nodes are easy to debug too. 
Master: The ROS Master provides name registration and lookup to the rest of the 
nodes. Nodes will not be able to find each other, exchange messages, or invoke 
services without a ROS Master. In a distributed system, we should run the master on 
one computer, and other remote nodes can find each other by communicating with 
this master.
- Parameter Server: The parameter server allows you to keep the data to be stored in

a central location. All nodes can access and modify these values. Parameter server is 
a part of ROS Master
- Messages: Nodes communicate with each other using messages. Messages are

simply a data structure containing the typed field, which can hold a set of data and 
that can be sent to another node. There are standard primitive types (integer, floating 
point, Boolean, and so on) and these are supported by ROS messages. We can also 
build our own message types using these standard types.
- Topics: Each message in ROS is transported using named buses called topics. When

a node sends a message through a topic, then we can say the node is publishing a 
topic. When a node receives a message through a topic, then we can say that the node 
is subscribing to a topic. The publishing node and subscribing node are not aware of 
each other’s existence. We can even subscribe a topic that might not have any 
publisher. In short, the production of information and consumption of it are 
decoupled. Each topic has a unique name, and any node can access this topic and 
send data through it as long as they have the right message type.
- Services: In some robot applications, a publish/subscribe model will not be enough if

it needs a request/response interaction. The publish/subscribe model is a kind of oneway 
transport system and when we work with a distributed system, we might need a 
request/response kind of interaction. ROS Services are used in these case. We can 
define a service definition that contains two parts; one is for requests and the other is 
for responses. Using ROS Services, we can write a server node and client node. The 
server node provides the service under a name, and when the client node sends a 
request message to this server, it will respond and send the result to the client. The 
client might need to wait until the server responds. The ROS service interaction is 
like a remote procedure call.
- Bags: Bags are a format for saving and playing back ROS message data. Bags are an

important mechanism for storing data, such as sensor data, which can be difficult to 
collect but is necessary for developing and testing robot algorithms. Bags are very 
useful features when we work with complex robot mechanisms.

# 文件系统

### Ros source list

/ect/apt/sources.list/ros-latest.lis

### Source Space

The source space contains the source code of catkin packages. This is where you can extract/checkout/clone source code for the packages you want to build. Each folder within the source space contains one or more catkin packages. This space should remain unchanged by configuring, building, or installing. The root of the source space contains a symbolic link to catkin’s boiler-plate ‘toplevel’ CMakeLists.txt file. This file is invoked by CMake during the configuration of the catkin projects in the workspace. It can be created by calling catkin_init_workspace in the source space directory.

### Build Space

The build space is where CMake is invoked to build the catkin packages in the source space. CMake and catkin keep their cache information and other intermediate files here. The build space does not have to be contained within the workspace nor does it have to be outside of the source space, but this is recommended.

build 目录是build space的默认所在位置，同时cmake 和 make也是在这里被调用来配置并编译你的程序包。

### Development (Devel) Space

The development space (or devel space) is where built targets are placed prior to being installed. The way targets are organized in the devel space is the same as their layout when they are installed. This provides a useful testing and development environment which does not require invoking the installation step. The location of the devel space is controlled by a catkin specific CMake variable called CATKIN_DEVEL_PREFIX, and it defaults to/develspace. This is the default behavior because it might be confusing to CMake users if they invokedcmake .. in a build folder and that modified things outside of the current directory. It is recommended, however, to set thedevel space directory to be a peer of the build space directory.

devel 目录是devel space的默认所在位置, 同时也是在你安装程序包之前存放可执行文件和库文件的地方。

### Install Space

Once targets are built, they can be installed into the install space by invoking the install target, usually with make install. The install space does not have to be contained within the workspace. Since the install space is set by theCMAKE_INSTALL_PREFIX, it defaults to /usr/local, which you should not use (because uninstall is near-impossible, and using multiple ROS distributions does not work either).

### Result space

When ever referring to a folder which can either be a development space or an install space the generic term result spaceis used.

### ros包的位置

/opt/ros/kinetic/share/turtlesim

### 自己编写的节点的生成位置

这会生成两个可执行文件, talker 和 listener, 默认存储到devel space目录,具体是在~/catkin_ws/devel/lib/中.

### 环境变量

`ROS_ROOT` /opt/ros/kinetic/share/ros 
`ROS_PACKAGE_PATH` /opt/ros/kinetic/share 
`PYTHONPATH` /opt/ros/kinetic/lib/python2.7/dist-packages

### msg生成的编程语言版本的文件的路径

所有在msg路径下的.msg文件都将转换为ROS所支持语言的源代码。生成的C++头文件将会放置在~/catkin_ws/devel/include/beginner_tutorials/。 [Python](http://lib.csdn.net/base/python)脚本语言会在~/catkin_ws/devel/lib/python2.7/dist-packages/beginner_tutorials/msg 目录下创建。 lisp文件会出现在~/catkin_ws/devel/share/common-lisp/ros/beginner_tutorials/msg/ 路径下.

### The use of each folder in ROS package
- 
config: All configuration files that are used in this ROS package are kept in this 
folder. This folder is created by the user and is a common practice to name the folder 
config to keep the configuration files in it.

- 
include/package_name: This folder consists of headers and libraries that we need to 
use inside the package.

- 
scripts: This folder keeps executable [python](http://lib.csdn.net/base/python) scripts. In the block diagram, we can 
see two example scripts.

- 
src: This folder stores the C++ source codes. We can see two examples of the source 
code in the block diagram.

- 
launch: This folder keeps the launch files that are used to launch one or more ROS 
nodes.

- 
msg: This folder contains custom message definitions.

- 
srv: This folder contains the service definitions.

- 
action: This folder contains the action definition. We will see more about actionlib 
in the upcoming sections.

- 
package.xml: This is the package manifest file of this package.

- 
CMakeLists.txt: This is the CMake build file of this package.


## REF
- 
[http://wiki.ros.org/cn/ROS/](http://wiki.ros.org/cn/ROS/)

- 
“Mastering ROS for Robotics Programming(PACKT, 2015).pdf”


# 如何自己写一个包

## package
- 快速创建程序包

```
$ catkin_create_pkg beginner_tutorials std_msgs rospy roscpp
# catkin_create_pkg <package_name> [depend1] [depend2] [depend3]
```
- 1
- 2
- 1
- 2
- 查看依赖
`$ rospack depends1 beginner_tutorials `- 1
- 1
- 依赖

> 
Build Tool Dependencies specify build system tools which this package needs to build itself. Typically the only build tool needed is catkin. In a cross-compilation scenario build tool dependencies are for the architecture on which the compilation is performed. 
Build Dependencies specify which packages are needed to build this package. This is the case when any file from these packages is required at build time. This can be including headers from these packages at compilation time, linking against libraries from these packages or requiring any other resource at build time (especially when these packages are find_package()-ed in CMake). In a cross-compilation scenario build dependencies are for the targeted architecture. 
Run Dependencies specify which packages are needed to run code in this package, or build libraries against this package. This is the case when you depend on shared libraries or transitively include their headers in public headers in this package (especially when these packages are declared as (CATKIN_)DEPENDS in catkin_package() in CMake). 
Test Dependencies specify only additional dependencies for unit tests. They should never duplicate any dependencies already mentioned as build or run dependencies.


```
- <buildtool_depend>
- <build_depend>
- <run_depend>
- <test_depend>
```
- 1
- 2
- 3
- 4
- 1
- 2
- 3
- 4

## CMakeLists
- Required CMake Version (cmake_minimum_required) 
- Package Name (project())
- Find other CMake/Catkin packages needed for build (find_package())
- Enable Python module support (catkin_python_setup())
- Message/Service/Action Generators (add_message_files(), add_service_files(), add_action_files())
- Invoke message/service/action generation (generate_messages())
- Specify package build info export (catkin_package())
- Libraries/Executables to build (add_library()/add_executable()/target_link_libraries())
- Tests to build (catkin_add_gtest())
- Install rules (install())





