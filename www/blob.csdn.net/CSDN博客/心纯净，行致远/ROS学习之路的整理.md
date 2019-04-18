# ROS学习之路的整理 - 心纯净，行致远 - CSDN博客





2018年09月20日 20:18:39[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：1563








        ROS是基于linux系统的一个次级操作系统，目前被看做是机器人界的一套标准平台，可以类比手机的安卓操作系统或者是电脑的windows操作系统。ROS最大的优点在于灵活、低耦合、分布式、开源以及功能强大而丰富的第三方功能包（类似于编程语言中的库）。

       ROS虽然叫操作系统，但是真正底层的任务调度、编译、寻址等任务还是由Linux操作系统完成，也就是ROS充其量是一个运行在Linux上的次级操作系统。

       学习ROS的步骤如下：

1、首先要在电脑上安装Ubuntu系统和ROS系统，这一步可以参照[Ubuntu16.04安装ROS](https://blog.csdn.net/zhanshen112/article/details/82764990)

2、学习基本的linux命令行。

      由于ROS要在Ubuntu上运行，而Ubuntu是linux系统的变种，因此有必要学习一下linux的操作和命令行。在Linux中，一切任务都可以在终端（Terminal）通过命令行完成。

      学习Linux推荐在线编程学习环境实验楼的两门课程：[Linux 基础入门（新版）](http://link.zhihu.com/?target=https%3A//www.shiyanlou.com/courses/1)和 [Linux命令实例练习](http://link.zhihu.com/?target=https%3A//www.shiyanlou.com/courses/68) ，两门课程可以结合互补学习，也可以参见我的学习笔记：[Linux命令行](http://link.zhihu.com/?target=https%3A//github.com/marooncn/learning_note/blob/master/Linux%25E5%2591%25BD%25E4%25BB%25A4%25E8%25A1%258C.pdf)有一个小建议是实验楼的在线编程环境就是Linux，因此没有必要在它们的网络环境学习，而是直接用自己的终端。这样可以有操作感，获得的体验更深刻。

     这一部分可以参照[机器人操作系统（ROS）在线实训平台学习实验指南](https://blog.csdn.net/zhangrelay/article/details/74615051)

3、学习ROS基础

    这一部分也可以参照[机器人操作系统（ROS）在线实训平台学习实验指南](https://blog.csdn.net/zhangrelay/article/details/74615051)

        对于ROS的学习，最好的办法还是直接学习官方的Tutorial: [ROS Tutorials](http://link.zhihu.com/?target=http%3A//wiki.ros.org/ROS/Tutorials) 。英语不太好的话可以看中文版的 [核心 ROS 教程](http://link.zhihu.com/?target=http%3A//wiki.ros.org/cn/ROS/Tutorials) 也可以参见我的学习笔记[ROS基础笔记](http://link.zhihu.com/?target=https%3A//github.com/marooncn/learning_note/blob/master/ROS%25E5%259F%25BA%25E7%25A1%2580%25E7%25AC%2594%25E8%25AE%25B0.pdf) ，前期只学初级20讲即可。学习完基础20讲后可以根据兴趣或用途来学习其他教程，如上图就是实时观测工具Rviz显示机器人的URDF(Unified Robot Description Format)模型，里面还涉及坐标变换的TF相关知识，都需要学习对应的Tutorial来入门。当然，无从着手的话，这时候选择一本参考书也是很不错的，如ros by example, Robot Operating System (ROS) The Complete Reference。最后推荐Roi Yehoshua教授授课所用的ppt：[Introduction to Robotics](http://link.zhihu.com/?target=https%3A//u.cs.biu.ac.il/~yehoshr1/89-685/)。

        此外，ROS的编程语言主要是C++ 和 Python。C++是国内大多数高校的教学语言，更加深入的学习可以阅读*C++ Primer*。但是我同时推荐的是学习Python，Python语言作为一种解释性语言/脚本语言，更加灵活、编程效率更高，同时有强大的库支持，因此也是现在人工智能的第一语言。如果有C/C++基础也可以很快入门，推荐廖雪峰的微博教程 [Python 2.7教程](http://link.zhihu.com/?target=https%3A//www.baidu.com/link%3Furl%3Dm2XBxZY-Md3fjMsK18PKuvW7X3bw4PkDOvsWQrGMhZL2mHKxyYTZYwn-9MDeL-dAT8iSgxGls7iLkKrEU9v7Qbgdz46yfBY15zFu4P2dg2R8lL6odoEPpkIO_5hZvlqk%26wd%3D%26eqid%3Dcd59247b00085cfc0000000458d7bf7b) ，以及实验楼快速入门课程：[Python快速教程](http://link.zhihu.com/?target=https%3A//www.shiyanlou.com/courses/214/) ，书的话我用的是《Python核心编程》（Core Python Programming）。

4、做项目

      学习ROS最好是一开始就挑战一个项目。在完成前三步的基础上，这时候就要根据尝试写第一个功能包(package)、第一个节点(node)了。碰到问题可以谷歌，官方问答平台[ROS Q&A](http://link.zhihu.com/?target=http%3A//answers.ros.org/questions/)也很有用。同时要在实践中不断学习，因为基础20讲还远远不够，用到相关知识可以继续参见相关Tutorial。

      总之，ROS的学习曲线在开始阶段很陡，需要很多预备知识，比如编程语言、Linux基础、熟练的英文阅读功底，甚至在理解ros::spin()，ros::spinOnce() （C++），rospy.spin() （Python）的时候只有理解线程才能搞清楚这三者之间的区别。总之，有志于学习ROS的同学要有不断学习的心理准备，但是同时，ROS的熟练使用绝对是以后找工作或实现项目的一块很有力的敲门砖。

相关链接：
[1、ROS学习入门（抛砖引玉篇）](https://zhuanlan.zhihu.com/p/26007106)

2、[ubuntu14.04下外放有声音，耳机没声音](https://www.2cto.com/os/201410/346653.html)

3、[机器人操作系统ROS | 简介篇](https://mp.weixin.qq.com/s?__biz=MzA5MDE2MjQ0OQ==&mid=2652786427&idx=1&sn=ac4b9a890fec3d68773c6cb65ed86946&mpshare=1&scene=1&srcid=0326mGy0nXBqFqBWxQCeNv6B&pass_ticket=1eHBdZLk6YjR3YAN0wCTt5ZXh4HtyqvVaLglwMh4ZRmOyALdqcjLhuOqe%2BBWkmJm#rd)

4、[https://www.zhihu.com/question/49903412/answer/120374605?from=profile_answer_card](https://www.zhihu.com/question/49903412/answer/120374605?from=profile_answer_card)

5、[ROS学习总结（1）--入门、学习路线](https://blog.csdn.net/kunyxu/article/details/69658696)



