# ros消息发布与订阅 - 站在巨人的肩膀上coding - CSDN博客





2018年03月14日 13:59:31[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：131








节点是一个可执行程序，它连接到了ROS的网络系统中。我们将会创建一个发布者，也就是说话者节点，它将会持续的广播一个信息。

     改变目录到之前所建立的那个包下：



`cd ~/catkin_ws/src/beginner_tutorials  `




     在beginner_tutorials包下面建立一个src文件夹：

`mkdir -p ~/catkin_ws/src/beginner_tutorials/src  `


     创建文件src/talker.cpp：
`gedit src/talker.cpp   `



     将下面的内容复制进去：

```
#include "ros/ros.h"  
    #include "std_msgs/String.h"  
      
    #include <sstream>  
      
    /**  
     * This tutorial demonstrates simple sending of messages over the ROS system.  
     */  
    int main(int argc, char **argv)  
    {  
      /**  
       * The ros::init() function needs to see argc and argv so that it can perform  
       * any ROS arguments and name remapping that were provided at the command line. For programmatic  
       * remappings you can use a different version of init() which takes remappings  
       * directly, but for most command-line programs, passing argc and argv is the easiest  
       * way to do it.  The third argument to init() is the name of the node.  
       *  
       * You must call one of the versions of ros::init() before using any other  
       * part of the ROS system.  
       */  
      ros::init(argc, argv, "talker");  
      
      /**  
       * NodeHandle is the main access point to communications with the ROS system.  
       * The first NodeHandle constructed will fully initialize this node, and the last  
       * NodeHandle destructed will close down the node.  
       */  
      ros::NodeHandle n;  
      
      /**  
       * The advertise() function is how you tell ROS that you want to  
       * publish on a given topic name. This invokes a call to the ROS  
       * master node, which keeps a registry of who is publishing and who  
       * is subscribing. After this advertise() call is made, the master  
       * node will notify anyone who is trying to subscribe to this topic name,  
       * and they will in turn negotiate a peer-to-peer connection with this  
       * node.  advertise() returns a Publisher object which allows you to  
       * publish messages on that topic through a call to publish().  Once  
       * all copies of the returned Publisher object are destroyed, the topic  
       * will be automatically unadvertised.  
       *  
       * The second parameter to advertise() is the size of the message queue  
       * used for publishing messages.  If messages are published more quickly  
       * than we can send them, the number here specifies how many messages to  
       * buffer up before throwing some away.  
       */  
      ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);  
      
      ros::Rate loop_rate(10);  
      
      /**  
       * A count of how many messages we have sent. This is used to create  
       * a unique string for each message.  
       */  
      int count = 0;  
      while (ros::ok())  
      {  
        /**  
         * This is a message object. You stuff it with data, and then publish it.  
         */  
        std_msgs::String msg;  
      
        std::stringstream ss;  
        ss << "hello world " << count;  
        msg.data = ss.str();  
      
        ROS_INFO("%s", msg.data.c_str());  
      
        /**  
         * The publish() function is how you send messages. The parameter  
         * is the message object. The type of this object must agree with the type  
         * given as a template parameter to the advertise<>() call, as was done  
         * in the constructor above.  
         */  
        chatter_pub.publish(msg);  
      
        ros::spinOnce();  
      
        loop_rate.sleep();  
        ++count;  
      }  
      
      
      return 0;  
    }
```





     解释一下代码：

`#include "ros/ros.h"  `




ros/ros.h包括了使用ROS系统最基本的头文件。




`#include "std_msgs/String.h"`


这条代码包括了std_msgs/String消息，它存在于std_msgs包中。这是有std_msgs中的String.msg文件自动产生的。

`ros::init(argc, argv, "talker");  `




初始化ROS，它允许ROS通过命令行重新命名，现在还不太重要。这里也是我们确切说明节点名字的地方，在运行的系统中，节点的名字必须唯一




`ros::NodeHandle n;  `



为处理的节点创建了一个句柄，第一个创建的节点句柄将会初始化这个节点，最后一个销毁的节点将会释放节点所使用的所有资源。




`ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);  `





告诉主机，我们将会在一个名字为chatter的话题上发布一个std_msgs/String类型的消息，这就使得主机告诉了所有订阅了chatter话题的节点，我们将在这个话题上发布数据。第二个参数是发布队列的大小，它的作用是缓冲。当我们发布消息很快的时候，它将能缓冲1000条信息。如果慢了的话就会覆盖前面的信息。

NodeHandle::advertise()将会返回ros::Publisher对象，该对象有两个作用，首先是它包括一个publish()方法可以在制定的话题上发布消息，其次，当超出范围之外的时候就会自动的处理。



`ros::Rate loop_rate(10);  `




一个ros::Rate对象允许你制定循环的频率。它将会记录从上次调用Rate::sleep()到现在为止的时间，并且休眠正确的时间。在这个例子中，设置的频率为10hz。




```
int count = 0;  
    while (ros::ok())  
    {
```





默认情况下，roscpp将会安装一个SIGINT监听，它使当Ctrl-C按下时，ros::ok()将会返回false。

ros::ok()在以下几种情况下也会返回false：（1）按下Ctrl-C时（2）我们被一个同名同姓的节点从网络中踢出（3）ros::shutdown()被应用程序的另一部分调用（4）所有的ros::NodeHandles都被销毁了。一旦ros::ok()返回false，所有的ROS调用都会失败。




```
std_msgs::String msg;  
      
    std::stringstream ss;  
    ss << "hello world " << count;  
    msg.data = ss.str();
```





我们使用message-adapted类在ROS中广播信息，这个类一般是从msg文件中产生的。我们现在使用的是标准的字符串消息，它只有一个data数据成员，当然更复杂的消息也是可以的。



- `chatter_pub.publish(msg);  `



现在我们向话题chatter发布消息。

`ROS_INFO("%s", msg.data.c_str());  `




ROS_INFO是cout和printf的替代品。

`ros::spinOnce();  `




在这个简单的程序中调用ros::spinOnce();是不必要的，因为我们没有收到任何的回调信息。然而如果你为这个应用程序添加一个订阅者，并且在这里没有调用ros::spinOnce()，你的回调函数将不会被调用。所以这是一个良好的风格。



`loop_rate.sleep();  `




休眠一下，使程序满足前面所设置的10hz的要求。     下面总结一下创建一个发布者节点的步骤：（1）初始化ROS系统（2）告诉主机我们将要在chatter话题上发布std_msgs/String类型的消息（3）循环每秒发送10次消息。





 打开一个终端，进入到beginner_tutorials包下面：

`cd ~/catkin_ws/src/beginner_tutorials  `




     编辑文件src/listener.cpp：


`gedit src/listener.cpp  `



     将下面的代码复制到文件中：



```
#include "ros/ros.h"  
    #include "std_msgs/String.h"  
      
    /**  
     * This tutorial demonstrates simple receipt of messages over the ROS system.  
     */  
    void chatterCallback(const std_msgs::String::ConstPtr& msg)  
    {  
      ROS_INFO("I heard: [%s]", msg->data.c_str());  
    }  
      
    int main(int argc, char **argv)  
    {  
      /**  
       * The ros::init() function needs to see argc and argv so that it can perform  
       * any ROS arguments and name remapping that were provided at the command line. For programmatic  
       * remappings you can use a different version of init() which takes remappings  
       * directly, but for most command-line programs, passing argc and argv is the easiest  
       * way to do it.  The third argument to init() is the name of the node.  
       *  
       * You must call one of the versions of ros::init() before using any other  
       * part of the ROS system.  
       */  
      ros::init(argc, argv, "listener");  
      
      /**  
       * NodeHandle is the main access point to communications with the ROS system.  
       * The first NodeHandle constructed will fully initialize this node, and the last  
       * NodeHandle destructed will close down the node.  
       */  
      ros::NodeHandle n;  
      
      /**  
       * The subscribe() call is how you tell ROS that you want to receive messages  
       * on a given topic.  This invokes a call to the ROS  
       * master node, which keeps a registry of who is publishing and who  
       * is subscribing.  Messages are passed to a callback function, here  
       * called chatterCallback.  subscribe() returns a Subscriber object that you  
       * must hold on to until you want to unsubscribe.  When all copies of the Subscriber  
       * object go out of scope, this callback will automatically be unsubscribed from  
       * this topic.  
       *  
       * The second parameter to the subscribe() function is the size of the message  
       * queue.  If messages are arriving faster than they are being processed, this  
       * is the number of messages that will be buffered up before beginning to throw  
       * away the oldest ones.  
       */  
      ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);  
      
      /**  
       * ros::spin() will enter a loop, pumping callbacks.  With this version, all  
       * callbacks will be called from within this thread (the main one).  ros::spin()  
       * will exit when Ctrl-C is pressed, or the node is shutdown by the master.  
       */  
      ros::spin();  
      
      return 0;  
    }
```



