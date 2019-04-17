# ros::spin() 和 ros::spinOnce() 区别及详解 - 站在巨人的肩膀上coding - CSDN博客





2018年03月14日 14:29:18[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：107
个人分类：[ros::spin()与ros::spinOnce()区别联](https://blog.csdn.net/LOVE1055259415/article/category/7506877)









# [ros::spin() 和 ros::spinOnce() 区别及详解](http://www.cnblogs.com/liu-fa/p/5925381.html)

### 版权声明：本文为博主原创文章，转载请标明出处: [http://www.cnblogs.com/liu-fa/p/5925381.html](http://www.cnblogs.com/liu-fa/p/5925381.html)

**博主提示**：本文基于ROS Kinetic Kame，如有更(gèng)新版本，可能存在细微差别，请大兄弟以官方资料为准。

博主向来愚钝，若大兄弟发现该文章有不妥之处，还请速速告知。

# 1 函数意义

首先要知道，这俩兄弟学名叫**ROS消息回调处理函数**。它俩通常会出现在ROS的主循环中，程序需要不断调用ros::spin() 或 ros::spinOnce()，**两者区别在于前者调用后不会再返回，也就是你的主程序到这儿就不往下执行了，而后者在调用后还可以继续执行之后的程序。**

其实消息回调处理函数的原理非常简单。我们都知道，ROS存在消息发布订阅机制，什么？不知道？不知道还不快去：[http://wiki.ros.org/ROS/Tutorials](http://wiki.ros.org/ROS/Tutorials) （ROS官方基础教程) 瞅瞅。

好，我们继续，如果你的程序写了相关的消息订阅函数，那么程序在执行过程中，除了主程序以外，ROS还会自动在后台按照你规定的格式，接受订阅的消息，但是所接到的消息并不是立刻就被处理，而是**必须**要等到ros::spin()或ros::spinOnce()执行的时候才被调用，这就是消息回到函数的原理，怎么样，简单吧，至于为什么这么设计？咳咳，嗯，肯定有他的道理。。。

# 2 区别

就像上面说的，ros::**spin()** 在调用后不会再返回，也就是你的主程序到这儿就不往下执行了，而** ros::spinOnce()** 后者在调用后还可以继续执行之后的程序。

其实看函数名也能理解个差不多，一个是一直调用；另一个是只调用一次，如果还想再调用，就需要加上循环了。

这里一定要记住，ros::spin()函数一般不会出现在循环中，因为程序执行到spin()后就不调用其他语句了，也就是说该循环没有任何意义，还有就是spin()函数后面一定不能有其他语句(return 0 除外)，有也是白搭，不会执行的。ros::spinOnce()的用法相对来说很灵活，但往往需要考虑调用消息的时机，调用频率，以及消息池的大小，这些都要根据现实情况协调好，不然会造成数据丢包或者延迟的错误。

# 3 常见使用方法

这里需要**特别强调**一下，如果大兄弟你的程序写了相关的**消息订阅函数**，那**千万千万千万**不要忘了在相应位置**加上ros::spin()或者ros::spinOnce()**函数，不然你是永远都得不到另一边发出的数据或消息的，博主血的教训，万望紧记。。。

## 3.1 ros::spin()

ros::spin()函数用起来比较简单，一般都在主程序的最后，加入该语句就可。例子如下：

#### 发送端：




```
```cpp
#include "ros/ros.h"
```



```cpp
#include "std_msgs/String.h"
```



```cpp
#include <sstream>
```





```cpp
int
```

```cpp
main(
```

```cpp
int
```

```cpp
argc,
```

```cpp
char
```

```cpp
**argv)
```



```cpp
{
```



```cpp

```

```cpp
ros::init(argc, argv,
```

```cpp
"talker"
```

```cpp
);
```



```cpp

```

```cpp
ros::NodeHandle n;
```



```cpp

```

```cpp
ros::Publisher chatter_pub = n.advertise<std_msgs::String>(
```

```cpp
"chatter"
```

```cpp
, 1000);
```



```cpp

```

```cpp
ros::Rate loop_rate(10);
```





```cpp

```

```cpp
int
```

```cpp
count = 0;
```



```cpp

```

```cpp
while
```

```cpp
(ros::ok())
```



```cpp

```

```cpp
{
```



```cpp

```

```cpp
std_msgs::String msg;
```



```cpp

```

```cpp
std::stringstream ss;
```



```cpp

```

```cpp
ss <<
```

```cpp
"hello world "
```

```cpp
<< count;
```



```cpp

```

```cpp
msg.data = ss.str();
```



```cpp

```

```cpp
ROS_INFO(
```

```cpp
"%s"
```

```cpp
, msg.data.c_str());
```





```cpp

```

```cpp
/**
```



```cpp

```

```cpp
* 向 Topic: chatter 发送消息, 发送频率为10Hz（1秒发10次）；消息池最大容量1000。
```



```cpp

```

```cpp
*/
```



```cpp

```

```cpp
chatter_pub.publish(msg);
```





```cpp

```

```cpp
loop_rate.sleep();
```



```cpp

```

```cpp
++count;
```



```cpp

```

```cpp
}
```



```cpp

```

```cpp
return
```

```cpp
0;
```



```cpp
}
```
```




#### 接收端：




```
```cpp
#include "ros/ros.h"
```



```cpp
#include "std_msgs/String.h"
```





```cpp
void
```

```cpp
chatterCallback(
```

```cpp
const
```

```cpp
std_msgs::String::ConstPtr& msg)
```



```cpp
{
```



```cpp

```

```cpp
ROS_INFO(
```

```cpp
"I heard: [%s]"
```

```cpp
, msg->data.c_str());
```



```cpp
}
```





```cpp
int
```

```cpp
main(
```

```cpp
int
```

```cpp
argc,
```

```cpp
char
```

```cpp
**argv)
```



```cpp
{
```



```cpp

```

```cpp
ros::init(argc, argv,
```

```cpp
"listener"
```

```cpp
);
```



```cpp

```

```cpp
ros::NodeHandle n;
```



```cpp

```

```cpp
ros::Subscriber sub = n.subscribe(
```

```cpp
"chatter"
```

```cpp
, 1000, chatterCallback);
```





```cpp

```

```cpp
/**
```



```cpp

```

```cpp
* ros::spin() 将会进入循环， 一直调用回调函数chatterCallback(),每次调用1000个数据。
```



```cpp

```

```cpp
* 当用户输入Ctrl+C或者ROS主进程关闭时退出，
```



```cpp

```

```cpp
*/
```



```cpp

```

```cpp
ros::spin();
```



```cpp

```

```cpp
return
```

```cpp
0;
```



```cpp
}
```
```




## 3.2 ros::spinOnce()

对于ros::spinOnce()的使用，虽说比ros::spin()更自由，可以出现在程序的各个部位，但是需要注意的因素也更多。比如：

#### 1 对于有些传输特别快的消息，尤其需要注意合理控制消息池大小和ros::spinOnce()执行频率; 比如消息送达频率为10Hz, ros::spinOnce()的调用频率为5Hz，那么消息池的大小就一定要大于2，才能保证数据不丢失，无延迟。






```
```cpp
/**接收端**/
```

```cpp
<br>#include
```

```cpp
"ros/ros.h"
```



```cpp
#include "std_msgs/String.h"
```



```cpp

```



```cpp
void
```

```cpp
chatterCallback(
```

```cpp
const
```

```cpp
std_msgs::String::ConstPtr& msg)
```



```cpp
{
```



```cpp

```

```cpp
/*...TODO...*/
```



```cpp
}
```



```cpp

```



```cpp
int
```

```cpp
main(
```

```cpp
int
```

```cpp
argc,
```

```cpp
char
```

```cpp
**argv)
```



```cpp
{
```



```cpp

```

```cpp
ros::init(argc, argv,
```

```cpp
"listener"
```

```cpp
);
```



```cpp

```

```cpp
ros::NodeHandle n;
```



```cpp

```

```cpp
ros::Subscriber sub = n.subscribe(
```

```cpp
"chatter"
```

```cpp
, 2, chatterCallback);
```



```cpp

```



```cpp

```

```cpp
ros::Rate loop_rate(5);
```



```cpp

```

```cpp
while
```

```cpp
(ros::ok())
```



```cpp

```

```cpp
{
```



```cpp

```

```cpp
/*...TODO...*/
```





```cpp

```

```cpp
ros::spinOnce();
```



```cpp

```

```cpp
loop_rate.sleep();
```



```cpp

```

```cpp
}
```



```cpp

```

```cpp
return
```

```cpp
0;
```



```cpp
}
```
```






#### 2 ros::spinOnce()用法很灵活，也很广泛，具体情况需要具体分析。但是对于用户自定义的周期性的函数，最好和ros::spinOnce并列执行，不太建议放在回调函数中；



```
```cpp
/*...TODO...*/
```



```cpp
ros::Rate loop_rate(100);
```



```cpp

```



```cpp
while
```

```cpp
(ros::ok())
```



```cpp
{
```



```cpp

```

```cpp
/*...TODO...*/
```



```cpp

```

```cpp
user_handle_events_timeout(...);
```





```cpp

```

```cpp
ros::spinOnce();
```



```cpp

```

```cpp
loop_rate.sleep();
```



```cpp
}
```
```



