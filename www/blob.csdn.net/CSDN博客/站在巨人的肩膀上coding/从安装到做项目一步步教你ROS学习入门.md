# 从安装到做项目一步步教你ROS学习入门 - 站在巨人的肩膀上coding - CSDN博客





2018年04月17日 10:05:18[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：842








**      雷锋网(公众号：雷锋网)新智造按：本文来源于知乎，作者卓求，雷锋网新智造已获得授权，也欢迎大家关注作者的知乎账号，将不定期更新ROS, Robotics,Python, Matlab相关内容总结。**

第一次接触ROS是在沈阳自动化所做项目的时候，那时候我才知道自己非常感兴趣的机器人领域有一个叫做ROS(Robot Operating System)的平台。仔细了解之后才知道ROS被机器人界看作是一套准标准平台，很多人将其比作是手机界的安卓，电脑界的微软。在我看来ROS最大的优点在于灵活、低耦合、分布式、开源以及功能强大而丰富的第三方功能包（类似于编程语言中的库）。于是我乘着毕设的机会申请了做ROS相关的项目--自主导航小车PlantBot。

ROS虽然叫操作系统，但是真正底层的任务调度、编译、寻址等任务还是由Linux操作系统完成，也就是ROS充其量是一个运行在Linux上的次级操作系统。关于ROS的介绍有一篇大牛写的很不错的文章：[机器人操作系统ROS | 简介篇](http://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA5MDE2MjQ0OQ%3D%3D%26mid%3D2652786427%26idx%3D1%26sn%3Dac4b9a890fec3d68773c6cb65ed86946%26mpshare%3D1%26scene%3D1%26srcid%3D0326mGy0nXBqFqBWxQCeNv6B%26pass_ticket%3D1eHBdZLk6YjR3YAN0wCTt5ZXh4HtyqvVaLglwMh4ZRmOyALdqcjLhuOqe%252BBWkmJm%23rd)。

**第一步，安装Linux和ROS**

**![从安装到做项目一步步教你ROS学习入门](https://static.leiphone.com/uploads/new/article/740_740/201703/58d8ef946a91e.png)**

学习ROS首先要安装Linux，为了不抛弃原Windows操作系统，有两个选择：一个是在VMvare等虚拟机上运行，另一个是安装Windows和Linux的双系统。我的建议是电脑有足够的空间的话，最好安装双系统，这样在Linux上使用ROS时运行速度更快。虚拟机跑ROS我没尝试过，但是网上也看到有人用虚拟机学习ROS，如果电脑不好装双系统或者内存不够的话可以尝试使用虚拟机。

Linux 有不同的发行版本，目前对ROS支持最好的版本是Ubuntu 14.04 LTS (Long Term Support)，对应的ROS的版本推荐为indigo，官网安装主页已更新为最新版本的Ubuntu。可以到[Old Ubuntu Releases](http://old-releases.ubuntu.com/releases/) 下载对应的14.04版本。Linux安装教程可以参照：[Win7上安装Ubuntu14.04双系统](https://pan.baidu.com/s/1o8HL4ym) 。

需要说明的是，随着电脑的配置和操作系统的不同，安装过程大同小异，但是 有的电脑就是装不上，谷歌之后仍然无法解决问题，这时候可以尝试换台电脑或者装虚拟机，我之前用联想ideapad，折腾了一周，试了各种方法就是不成功，最后换了台华硕一下就好了。

装好Linux后可以根据这篇官方指导安装ROS： [Ubuntu install of ROS Indigo](http://wiki.ros.org/indigo/Installation/Ubuntu)。如果没有任何Linux命令行基础，那么可以先学习Linux命令行，然后再装ROS。

**第二步，学习Linux命令行**

**![从安装到做项目一步步教你ROS学习入门](https://static.leiphone.com/uploads/new/article/740_740/201703/58d8f080b7980.jpg)**

对于熟悉Windows而没有接触过Linux系统的人来说，上一步安装的Ubuntu是一个很好的Linux过渡版本。在Linux中，一切任务都可以在终端（Terminal）通过命令行完成。因此非常有必要学习Linux命令行。学习Linux推荐在线编程学习环境实验楼的两门课程：[Linux 基础入门（新版）](https://www.shiyanlou.com/courses/1)和 [Linux命令实例练习](https://www.shiyanlou.com/courses/68) ，两门课程可以结合互补学习，也可以参见我的学习笔记：[Linux命令行](https://pan.baidu.com/s/1nviB7Gt) 有一个小建议是实验楼的在线编程环境就是Linux，因此没有必要在它们的网络环境学习，而是直接用自己的终端。

同时Ubuntu 作为非商业操作系统，里面或多或少有dug，遇到问题多搜索，对于常见的问题，网上都有解决办法。我在安装完Ubuntu后就遇到一个很窘的问题：网络连接不上，但是神奇的是盖上盖子再打开后就好了，之后碰到的带耳机的时候没声音也是这样，一种解释如下：[ubuntu14.04下外放有声音，耳机没声音](http://www.2cto.com/os/201410/346653.html)。对于同样的遇到网络无法连接的同学可以在终端输入：

echo "options asus_nb_wmi wapf=4"| sudo tee /etc/modprobe.d/asus_nb_wmi.conf

重启即可。

**第三步， 学习ROS基础**

**![从安装到做项目一步步教你ROS学习入门](https://static.leiphone.com/uploads/new/article/740_740/201703/58d8f0075dd55.jpg)**

对于ROS的学习，最好的办法还是直接学习官方的Tutorial: [ROS Tutorials](http://wiki.ros.org/ROS/Tutorials) 。英语不太好的话可以看中文版的 [核心 ROS 教程](http://wiki.ros.org/cn/ROS/Tutorials) 也可以参见我的学习笔记 [ROS基础笔记](https://pan.baidu.com/s/1jIHnEjs) ，前期只学初级20讲即可。学习完基础20讲后可以根据兴趣或用途来学习其他教程，如上图就是实时观测工具Rviz显示机器人的URDF(Unified Robot Description Format)模型，里面还涉及坐标变换的TF相关知识，都需要学习对应的Tutorial来入门。当然，无从着手的话，这时候选择一本参考书也是很不错的，如[ros by example](https://pan.baidu.com/s/1sli30Gd), [Learning Robotics Using Python , Learning ROS for Robotics Programming](https://pan.baidu.com/s/1c1FhMH6) 。

此外，ROS的编程语言主要是C++ 和 Python。C++是国内大多数高校的教学语言，更加深入的学习可以阅读C++ Prime 。但是我同时推荐的是学习Python，Python语言作为一种解释性语言/脚本语言，更加灵活、编程效率更高，同时有强大的库支持，因此也是现在人工智能的第一语言。如果有C/C++基础也可以很快入门，推荐廖雪峰的微博教程 [Python 2.7教程](http://www.liaoxuefeng.com/wiki/001374738125095c955c1e6d8bb493182103fac9270762a000) ，以及实验楼快速入门课程：Python快速教程 ，书的话我用的是《Python核心编程》（Core Python Programming）。

**第四步，做项目**

**![从安装到做项目一步步教你ROS学习入门](https://static.leiphone.com/uploads/new/article/740_740/201703/58d8f0333e539.jpg)**

只有不断的学习和实践才能学好一门知识，学习ROS最好是一开始就挑战一个项目。在完成前三步的基础上，这时候就要根据尝试写第一个功能包(package)、第一个节点(node)了。碰到问题可以谷歌，官方问答平台[ROS Q&A](http://answers.ros.org/questions/)也很有用。同时要在实践中不断学习，因为基础20讲还远远不够，用到相关知识可以继续参见相关Tutorial。

总之，ROS的学习曲线在开始阶段很陡，需要很多预备知识，比如编程语言、Linux基础、熟练的英文阅读功底，甚至在理解ros::spin()，ros::spinOnce() （C++），rospy.spin() （Python）的时候只有理解线程才能搞清楚这三者之间的区别。总之，有志于学习ROS的同学要有不断学习的心理准备，但是同时，ROS的熟练使用绝对是以后找工作或实现项目的一块很有力的敲门砖。



