# 多ROS设备之间通信设置【NVIDIA TX2和自己的笔记本】 - 站在巨人的肩膀上coding - CSDN博客





2018年04月23日 21:19:16[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：785








编辑 .bashrc文件和/etc/hosts文件

1)        同步时钟,tx2和工作站上都执行,[sudoapt-0get install chrony]

2)        在tx2和笔记本工作站上都安装ssh[sudo apt-get install ssh ],同时,启动ssh服务,[sudo /etc/init.d/ssh start].

3)        笔记本ssh登录[ssh -X ubutnu@turtlebot_ip].注意:笔记本和TX2小车一定一定一定要连接在同一个局域网中,否则ssh无法登录.

4)        剩下的按照”创客智造”上面完成,连接如下:http://www.ncnynl.com/archives/201609/793.html

5)        至此,两台ROS设备之间的连接测试任务已完成.



