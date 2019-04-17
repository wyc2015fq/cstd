# windows time-wait 问题处理记录 - DoubleLi - 博客园






问题描述：有一段时间，服务器启动了好多程序，做的是 obd监听服务，连接好多个服务器，由于程序的本身的问题造成大量的wait-time，一番百度后找到找到方案1

设置一
由于wait-time 需要经过2MSL 之后才会释放，所以可以通过设置这个时间来解决,方法如下 
修改Time_Wait参数的方法 (在服务端修改)Windows下在 
HKEY_LOCAL_MACHINE/SYSTEM/CurrentControlSet/Services/Tcpip/Parameters 
,添加名为TcpTimedWaitDelay的 DWORD键,设置为30,以缩短TIME_WAIT的等待时间

设置以后问题得到缓解，但还是会报端口不够用。

设置二
经过一段时间的摸索，发现，win server 2008 给程序可用的端口有一个范围，参照 
此文： https://blog.csdn.net/susubuhui/article/details/51012622 
查询到给程序用的 动态端口数量为 16384 个，而计算机的端口 总共有65535



可以参照上面的连接对该值进行设置。 
最后，个人认为以上的设置，虽然能解决一点问题，但是最主要的还是从程序入手，利用各种池，对程序进行改进，才是最理想的。
--------------------- 









