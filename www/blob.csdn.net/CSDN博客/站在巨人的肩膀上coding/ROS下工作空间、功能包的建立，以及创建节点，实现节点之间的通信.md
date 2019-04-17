# ROS下工作空间、功能包的建立，以及创建节点，实现节点之间的通信 - 站在巨人的肩膀上coding - CSDN博客





2018年04月24日 15:41:53[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：716








                我是ROS初学者，完全根据《ROS机器人高效编程》第三版这本书学习的，学习第2.4节时，经常会出现一些莫名其妙的问题，所以通过各种上网查资料，最后终于完成了书上的编程！以下是我对ROS工作空间的建立、创建节点、实现通讯进行总结

#    第一步：创建ROS工作空间

   我们通过mkdir -p直接创建多层目录。

   $ mkdir -p  ~/dev/catkin_ws/src             //catkin_ws是工作空间目录，src工作空间下的原文件目录

   $ cd  ~/dev/catkin_ws/src

   $ catkin_init_workspace                       //在工作空间中创建CMakeList.txt文件，这个文件是建立在～/dev/catkin_ws/src目录下的

   $ cd  ~/dev/catkin_ws                          //移动到工作空间目录是为了编译工作空间

   $ catkin_make                                     //编译工作空间，只能在工作空间目录进行

   此时，如果输入ll命令，就可以看见工作空间目录下自动创建了新文件，分别是build和devel文件夹

   $ source devel/setup.bash                  //重新加载setup.bash文件，你也可以通过关闭终端重新打开来重新加载

   最后还要在~/.bashrc文件的末尾添加命令行，来配置我们的环境 $ echo "source /opt/ros/kinetic/setup.bash" >> ~/.bashrc  如果在安装ros时已经配置了，那就不需要配置了，我们使用的ROS版本是ROS kinetic，如果要使用其他版本的，请自行更改该配置语句！


#    第二步：创建ROS功能包和元功能包

   $ cd ~/dev/catkin_ws/src

   $ catkin_create_pkg chapter2_tutorials std_msgs roscpp     //chapter2_tutorials是功能包名字，std_msgs和roscpp是依赖项，std_msgs包含了常见道消息类型，表示基本数据类型和其他基本道消息构造，如多维数组。roscpp 使用C++ 实现ROS的各种功能。如果你还要使用Python库，还需要在最后边添加 rospy

#    第三步：编译ROS功能包

    $ cd  ~/dev/catkin_ws

    $ catkin_make                         //编译工作空间下的所有包，依然是要到工作空间下编译

#    第四步：创建节点

    $ roscd chapter2_tutorials/src              //节点是要创建到功能包下边的源文件下边，所以要先移动到该目录，如果该语句执行失败，可以用 $ cd ~/dev/catkin_ws/src/chapter2_tutorials/src替代。

   然后创建两个文件，分别命名为example1_a.cpp和example1_b.cpp，第一个文件会发送带有节点名称的数据，第二个文件接受这些数据并把他显示在命令行中。

    $ vim example1_a.cpp        //创建并打开example1_a.cpp文件

    把以下内容复制到该文件中：

 #include "ros/ros.h"
#include "std_msgs/String.h"//  
#include <sstream>///////////包含要使用的文件类型

int main(int argc, char **argv)
{
  ros::init(argc, argv, "example1_a");//启动该节点并设置其名称，名称必须唯一  
  ros::NodeHandle n;//设置节点进程的句柄  
  ros::Publisher pub = n.advertise<std_msgs::String>("message", 1000);//将节点设置成发布者，并将所发布>主题道类型和名称告知节点管理器。第一个参数是消息的名称：message，第二个是缓冲区道大小。如果主题发布数据速度较快
，那么将缓冲区设置为1000个消息。  
  ros::Rate loop_rate(10);//设置发送数据道频率为10Hz  
  while (ros::ok())//当收到Ctrl+C的按键消息或ROS停止当前节点运行时，ros::ok()库会执行停止节点运行道命令  
  {
//////创建一个消息变量，变量类型必须符合发送数据的要求  
    std_msgs::String msg;
    std::stringstream ss;
    ss << " I am the example1_a node ";
    msg.data = ss.str();
    //ROS_INFO("%s", msg.data.c_str());  
    pub.publish(msg);//消息被发布  
    ros::spinOnce();//如果一个订阅者出现，ROS就会更新和读取所有主题  
    loop_rate.sleep();//按照10Hz的频率将程序挂起  
  }
  return 0;
}
     保存之后创建打开第二个文件

     $ vim example1_b.cpp

     复制以下内容：

#include "ros/ros.h"
#include "std_msgs/String.h"

void messageCallback(const std_msgs::String::ConstPtr& msg)//每次该节点收到一条消息时都调用此函数，就可以使用或
处理数据。本例中，将收到道数据在命令窗口中输出来  
{
  ROS_INFO("I heard: [%s]", msg->data.c_str());//用于在命令行中输出数据  
}

int main(int argc, char **argv)
{
   ros::init(argc, argv, "example1_b");
   ros::NodeHandle n;
    /////创建一个订阅者，并从主题获取以message为名称的消息数据。设置缓冲区为1000个消息，处理消息句柄的回调函数>为messageCallback  
   ros::Subscriber sub = n.subscribe("message", 1000, messageCallback);
     //// ros::spin()库是节点读取数据道消息响应循环，当消息到达的时候，回调函数就会被调用。当按下Ctrl+C时，节点
会退出消息循环，于是循环结束。  
   ros::spin();
   return 0;
}  


#     第五步：编译节点，其实前边的步，基本不会出现问题，后边可能就会有问题出现了

    接下来需要自行编辑CMakeList.txt文件，使用如下语句编辑他

    $ rosed chapter2_tutorials CMakeList.txt                 //如果你这步没有出现错误，恭喜你，但是我在执行这步时出现问题：不能找见chapter2_tutorials这个包，我找了好久才发现在编译之前，需要修改CMakeLists.txt文件和manifest.xml文件，需要通过rosmake编译一下功能包

    $ rosmake chapter2_tutorials                      //编译完这步后，应该就不会出现上述问题了，接下来再执行上一步，就能成功打开CMakeList.txt文件了

    打开CMakeList.txt文件后，把以下内容添加到最后边：

include_directories(
include
  ${catkin_INCLUDE_DIRS}
)

add_executable(example1_a src/example1_a.cpp)
add_executable(example1_b src/example1_b.cpp)

add_dependencies(example1_a
chapter2_tutorials_generate_messages_cpp)
add_dependencies(example1_b
chapter2_tutorials_generate_messages_cpp)

target_link_libraries(example1_a ${catkin_LIBRARIES})
target_link_libraries(example1_b ${catkin_LIBRARIES})



   这部分内容一定不能写错，我第一次也是把最后一行中的LIBRARIES的最后一个s写成了小写，导致第二个文件一直不能接收信息，找了好久才找见！

   接下来就是编译CMakeList.txt文件，还是要先到工作空间才能进行编译

   $ cd ~/dev/catkin_ws

   $ catkin_make --pkg chapter2_tutorials 


   接下来在电脑中启动ROS，需要首先调用：

   $ roscore

   启动ros后，需要再打开一个终端，我们称为终端2；

#    第六步：运行节点

   首先要检查ROS是否在运行，在终端2中使用rosnode list命令查询

   $ rosnode list

   接下来在终端2中运行第一个节点：

   $ rosrun chapter2_tutorials example1_a        //在这里可能会出现问题依然是：不能发现chapter2_tutorials包，需要在该终端中继续编辑chapter2_tutorials包，加载setup.bash文件,执行以下两步

   $ catkin_make --pkg chapter2_tutorials     


   $ source devel/setup.bash

   然后在重新运行上边哪一步，即 $ rosrun chapter2_tutorials example1_a ,运行成功后出现空白行，处于等待状态，

   接着继续打开另一个终端，即终端3，在该终端中运行第二个节点：

   $ rosrun chapter2_tutorials example1_b      //如果出现找不到chapter2_tutorials包的问题，请参考上一步。

   如果运行成功，你会发现命令行会不断重复 [ INFO] [*********.**********]: I heard : [I am the example1_a node]
   则建立链接成功！            


