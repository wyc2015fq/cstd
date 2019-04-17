# ROS与navigation教程-设置机器人使用TF - 站在巨人的肩膀上coding - CSDN博客





2018年03月14日 14:00:22[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：186








**ROS与navigation教程-设置机器人使用TF**

说明：
- 介绍如何配置机器人让其使用TF
- 注意：本教程假设您已完成[ROS教程](http://www.ncnynl.com/category/ros-junior-tutorial/)和[tf基础教程](http://wiki.ros.org/tf/Tutorials#Learning_tf)。
- 本教程的代码在[robot_setup_tf_tutorial包](http://wiki.ros.org/robot_setup_tf_tutorial)中可用

**步骤：**

**(1)变换配置**

许多ROS包需要使用tf软件库发布机器人的变换树。在抽象层，变换树根据不同坐标系之间的平移和旋转来定义偏移量。为了使这更具体，考虑一个简单的机器人的例子，它具有安装在其顶部的单个激光器的移动基座。在提及机器人时，我们定义两个坐标系：一个对应于机器人底座的中心点，一个坐标系安装在基座顶部的激光中心点。我们还给他们名字，以供参考。我们将调用附加到移动基础“base_link”的坐标系（对于导航，它重要的是将其放置在机器人的旋转中心），我们也会调用附加到激光“base_laser”的坐标系。[有关框架命名约定，请参见REP 105](http://www.ros.org/reps/rep-0105.html)

在这一点上，我们假设激光器的一些数据以与激光中心点的距离的形式来表达。换句话说，我们在“base_laser”坐标系中有一些数据。现在假设我们想要获取这些数据并使用它来帮助移动基地避免活动空间的障碍。要做到这一点，我们需要一种将我们从“base_laser”坐标系转换成“base_link”坐标系的激光扫描的方法。实质上，我们需要定义“base_laser”和“base_link”坐标系之间的关系。

![请输入图片描述](http://images.ncnynl.com/ros/2017/simple_robot.png)

在定义这种关系时，假设我们知道激光器在移动基座的中心点之上10厘米和向前20厘米处安装。这给了我们一个将“base_link”坐标系与“base_laser”坐标系相关联的平移值和偏移值。具体来说，我们知道要从“base_link”坐标系到“base_laser”坐标系的数据，我们必须应用（x：0.1m，y：0.0m，z：0.2m）的转换，并从“ base_laser“坐标系到”base_link“坐标系，我们必须应用相反的变换（x：-0.1m，y：0.0m，z：-0.20m）。

我们可以选择自己管理这种关系，这意味着在必要时存储和应用帧之间的适当变换，但随着坐标系的数量增加，这成为一个真正的痛苦。幸运的是，我们不用自己做这个工作。相反，我们将使用tf定义“base_link”和“base_laser”之间的关系，并让它管理两个坐标系之间的变换。

要使用tf定义和存储“base_link”和“base_laser”坐标系之间的关系，我们需要将它们添加到一个变换树中。在概念上，变换树中的每个节点对应于坐标系，每个方向对应于需要应用从当前节点移动到其子节点的变换。Tf使用树结构来确保只有一个遍历将任何两个坐标系连接在一起，并假设树中的所有方向都是从父节点引导到子节点。

![请输入图片描述](http://images.ncnynl.com/ros/2017/tf_robot.png)

要为我们的简单示例创建一个变换树，我们将创建两个节点，一个用于“base_link”坐标系，一个用于“base_laser”坐标系。为了创建它们之间的方向，我们首先需要决定哪个节点是父节点，哪些节点是子节点。记住，这种区别很重要，因为tf假定所有转换都从父对象移动到子对象。让我们选择“base_link”坐标系作为父节点，因为随着其他部分/传感器被添加到机器人，通过遍历“base_link”框架，它们将最有意义地与“base_laser”坐标系相关。这意味着连接“base_link”和“base_laser”的变换应为（x：0.1m，y：0.0m，z：0.2m）。使用这个变换树设置，将在“base_laser”坐标系中接收到的激光扫描转换为“base_link”坐标系就像调用tf库一样简单。我们的机器人可以使用这些信息来理解“base_link”框架中的激光扫描，并安全地计划避开环境中的障碍物。

**(2)编写代码**

希望上面的例子有助于在概念层面了解tf。现在，我们必须使用变换树并用代码创建它。在这个例子中，假设你已熟悉ROS，因此如果任何术语或概念不熟悉，请确保查看[ROS文档](http://www.ncnynl.com/category/ros-junior-tutorial/)。

假设我们有一个高级别的任务描述在“base_laser”坐标系中获取点并将其转换为“base_link”坐标系。我们需要做的第一件事是创建一个负责在系统中发布转换的节点。接下来，我们必须创建一个节点来监听通过ROS发布的变换数据，并应用它来转换一个点。我们将首先为源代码创建一个包，然后我们给它一个简单的名字，如“robot_setup_tf”我们将依赖于roscpp，tf和geometry_msgs。

```
$ cd %TOP_DIR_YOUR_CATKIN_WS%/src
$ catkin_create_pkg robot_setup_tf roscpp tf geometry_msgs
```

请注意，您必须运行上面的命令（例如~/ros，您可能已为之前的教程创建）。

在fuerte，groovy和hydro中的替代方法：在navigation_tutorials包中有一个标准的robot_setup_tf_tutorial包。您可能希望通过以下方式安装 (%YOUR_ROS_DISTRO% can be { fuerte, groovy } .)：

```
$ sudo apt-get install ros-%YOUR_ROS_DISTRO%-navigation-tutorials
```

**(3)广播变换**

现在我们有了我们的软件包，我们需要创建一个节点来完成在ROS 上广播base_laser → base_link变换的工作。在刚刚创建的robot_setup_tf包中，启动您喜欢的编辑器，并将以下代码粘贴到src/tf_broadcaster.cpp文件中。

```
#include <ros/ros.h>
#include <tf/transform_broadcaster.h>

int main(int argc, char** argv){
  ros::init(argc, argv, "robot_tf_publisher");
  ros::NodeHandle n;

  ros::Rate r(100);

  tf::TransformBroadcaster broadcaster;

  while(n.ok()){
    broadcaster.sendTransform(
      tf::StampedTransform(
        tf::Transform(tf::Quaternion(0, 0, 0, 1), tf::Vector3(0.1, 0.0, 0.2)),
        ros::Time::now(),"base_link", "base_laser"));
    r.sleep();
  }
}
```

现在，我们来更详细看看发布base_link → base_laser变换相关的代码

```
#include <tf/transform_broadcaster.h>
```

tf包提供了一个 tf::TransformBroadcaster的实现，以帮助使发布变换的任务更容易。要使用TransformBroadcaster，我们需要包含tf/transform_broadcaster.h头文件。

```
tf::TransformBroadcaster broadcaster;
```

在这里，我们创建一个TransformBroadcaster对象，我们稍后将通过网络发送base_link → base_laser变换。

```
broadcaster.sendTransform(
  tf::StampedTransform(
    tf::Transform(tf::Quaternion(0, 0, 0, 1), tf::Vector3(0.1, 0.0, 0.2)),
    ros::Time::now(),"base_link", "base_laser"));
```

这是实际工作完成的地方。使用TransformBroadcaster发送变换需要五个参数。首先，我们传递旋转变换，它由btQuaternion指定，需要在两个坐标系之间进行任何旋转。在这种情况下，我们不要施加旋转，所以我们发送一个由俯仰，滚动和偏航值等于零的btQuaternion。第二，btVector3可以应用到任何变换的，所以我们创建一个btVector3对应的激光的x偏移10厘米和Z偏移距离机器人基座20厘米。第三，我们需要给出发布的时间戳，我们将用ros::Time::now()。第四，我们需要传递我们创建的链接的父节点的名称，在这种情况下是“base_link”。第五，我们需要传递我们创建的链接的子节点的名称，在这种情况下是“base_laser”。

**(4)使用变换**

以上，我们创建了一个通过ROS发布base_laser → base_link变换的节点。现在，我们要编写一个节点，将使用该变换在“base_laser”坐标系中取一点，并将其转换为“base_link”坐标系中的一个点。我们将首先将下面的代码粘贴到一个文件中，并进行更详细的解释。在robot_setup_tf包中，创建一个名为src/tf_listener.cpp的文件，并粘贴以下内容：

```
#include <ros/ros.h>
#include <geometry_msgs/PointStamped.h>
#include <tf/transform_listener.h>

void transformPoint(const tf::TransformListener& listener){
  //we'll create a point in the base_laser frame that we'd like to transform to the base_link frame
  geometry_msgs::PointStamped laser_point;
  laser_point.header.frame_id = "base_laser";

  //we'll just use the most recent transform available for our simple example
  laser_point.header.stamp = ros::Time();

  //just an arbitrary point in space
  laser_point.point.x = 1.0;
  laser_point.point.y = 0.2;
  laser_point.point.z = 0.0;

  try{
    geometry_msgs::PointStamped base_point;
    listener.transformPoint("base_link", laser_point, base_point);

    ROS_INFO("base_laser: (%.2f, %.2f. %.2f) -----> base_link: (%.2f, %.2f, %.2f) at time %.2f",
        laser_point.point.x, laser_point.point.y, laser_point.point.z,
        base_point.point.x, base_point.point.y, base_point.point.z, base_point.header.stamp.toSec());
  }
  catch(tf::TransformException& ex){
    ROS_ERROR("Received an exception trying to transform a point from \"base_laser\" to \"base_link\": %s", ex.what());
  }
}

int main(int argc, char** argv){
  ros::init(argc, argv, "robot_tf_listener");
  ros::NodeHandle n;

  tf::TransformListener listener(ros::Duration(10));

  //we'll transform a point once every second
  ros::Timer timer = n.createTimer(ros::Duration(1.0), boost::bind(&transformPoint, boost::ref(listener)));

  ros::spin();

}
```

现在我们将一步一步解释

```
#include <tf/transform_listener.h>
```

在这里，我们包含tf/transform_listener.h 头文件，我们需要创建一个tf::TransformListener。一个TransformListener对象自动订阅了ROS变换消息主题和管理所有将在网络中广播的变换数据。

```
void transformPoint(const tf::TransformListener& listener){
```

我们将创建一个函数，给定一个TransformListener，在“base_laser”坐标系中取一点，并将其转换为“base_link”坐标系。这个函数将作为在我们程序的main()中创建的ros::Timer的回调，并且每秒都会触发。

```
//we'll create a point in the base_laser frame that we'd like to transform to the base_link frame
  geometry_msgs::PointStamped laser_point;
  laser_point.header.frame_id = "base_laser";

  //we'll just use the most recent transform available for our simple example
  laser_point.header.stamp = ros::Time();

  //just an arbitrary point in space
  laser_point.point.x = 1.0;
  laser_point.point.y = 0.2;
  laser_point.point.z = 0.0;
```

在这里，我们将创建一个geometry_msgs::PointStamped的点。消息名称结尾处的“Stamped”只是意味着它包含一个头，允许我们将时间戳和frame_id与消息相关联。我们将laser_point消息的Stamp字段设置为ros::Time()，它是一个特殊的时间值，允许我们向TransformListener询问最新的可用变换。对于标题的frame_id字段，我们将把它设置为“base_laser”，因为我们在“base_laser”坐标系中创建一个点。最后，我们将设置一些数据，例如x：1.0，y：0.2和z：0.0的点。

```
try{
    geometry_msgs::PointStamped base_point;
    listener.transformPoint("base_link", laser_point, base_point);

    ROS_INFO("base_laser: (%.2f, %.2f. %.2f) -----> base_link: (%.2f, %.2f, %.2f) at time %.2f",
        laser_point.point.x, laser_point.point.y, laser_point.point.z,
        base_point.point.x, base_point.point.y, base_point.point.z, base_point.header.stamp.toSec());
  }
```

现在我们将“base_laser”坐标系中的点转换成“base_link”坐标系的点。为此，我们将使用TransformListener对象，调用transformPoint()，并使用三个参数：我们要将点转换为（“base_link”在我们的例子中）坐标系的名称，我们正在转换的点，以及存储变换点。所以在调用transformPoint()之后，base_point保存与laser_point相同的信息，包含变换后的信息。

catch(tf::TransformException& ex){
ROS_ERROR("Received an exception trying to transform a point from "base_laser" to "base_link": %s", ex.what());
}

如果由于某种原因base_laser → base_link变换不可用（也许tf_broadcaster没有运行），当我们调用transformPoint()时，TransformListener可能会引发异常。为了确保我们优雅地处理这个问题，我们将捕获异常，并为用户打印一个错误。

**(5)编译代码**

现在我们已经写了一个例子，我们需要构建它。打开由roscreate-pkg自动生成的CMakeLists.txt文件，并将以下行添加到文件的底部。

```
add_executable(tf_broadcaster src/tf_broadcaster.cpp)
add_executable(tf_listener src/tf_listener.cpp)
target_link_libraries(tf_broadcaster ${catkin_LIBRARIES})
target_link_libraries(tf_listener ${catkin_LIBRARIES})
```

接下来，确保保存文件并构建包。

```
$ cd %TOP_DIR_YOUR_CATKIN_WS%
$ catkin_make
```

**(6)运行代码**

让我们来看看ROS实际发生了什么。对于本节，您将需要打开三个终端。
- 新开终端，运行roscore。

```
roscore
```
- 新开终端，运行tf_broadcaster

```
rosrun robot_setup_tf tf_broadcaster
```
- 新开终端，运行tf_listener，将我们的模拟点从“base_laser”坐标系变换为“base_link”坐标系一个点

```
rosrun robot_setup_tf tf_listener
```
- 如果一切正常，您应该看到以下输出显示一个点从“base_laser”坐标系转换到“base_link”坐标系。

```
[ INFO] 1248138528.200823000: base_laser: (1.00, 0.20. 0.00) -----> base_link: (1.10, 0.20, 0.20) at time 1248138528.19
[ INFO] 1248138529.200820000: base_laser: (1.00, 0.20. 0.00) -----> base_link: (1.10, 0.20, 0.20) at time 1248138529.19
[ INFO] 1248138530.200821000: base_laser: (1.00, 0.20. 0.00) -----> base_link: (1.10, 0.20, 0.20) at time 1248138530.19
[ INFO] 1248138531.200835000: base_laser: (1.00, 0.20. 0.00) -----> base_link: (1.10, 0.20, 0.20) at time 1248138531.19
[ INFO] 1248138532.200849000: base_laser: (1.00, 0.20. 0.00) -----> base_link: (1.10, 0.20, 0.20) at time 1248138532.19
```



