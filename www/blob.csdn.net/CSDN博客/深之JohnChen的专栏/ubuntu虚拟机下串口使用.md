# ubuntu虚拟机下串口使用 - 深之JohnChen的专栏 - CSDN博客

2019年03月29日 15:45:29[byxdaz](https://me.csdn.net/byxdaz)阅读数：74


  一、虚拟串口配置

虚拟串口完成主机与vmware下虚拟机进行串口通信，配置方法：

1、使用vspd软件，创建虚拟串口COM1,COM2。

2、打开vmware在设置中添加物理串口，将创建的虚拟串口COM1映射到虚拟机上。

![](https://img-blog.csdn.net/20170210090509295)

![](https://img-blog.csdn.net/20170210090554467)

3、在windows主机上打开串口工具，打开COM2，数据接收与发送。

4、在虚拟机中使用自己的程序或者minicom调试助手和主机进行串口通信。注意打开的串口如果是COM1，一般虚拟机对应的为ttyS1。不能通信就设置的串口ttyS（0-64）试试。

参考：[https://blog.csdn.net/baidu_33232390/article/details/54954931](https://blog.csdn.net/baidu_33232390/article/details/54954931)

二、常见串口调试工具

1、基于QT的串口调试助手助手（源码下载）

[https://github.com/wuhui2356/SerialPort](https://github.com/wuhui2356/SerialPort)

![](https://img-blog.csdnimg.cn/20190329234710128.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9jaGVuemhlbmd5aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)

2、Ubuntu下 cutecom 图像界面串口调试工具（参考：https://blog.csdn.net/zhaoqi2617/article/details/72238546）

　　（1）终端下载命令：sudo apt-get install cutecom 

　　（2）通过终端打开软件：sudo cutecom (需要root权限，否则无法打开串口）

　　（3）在 Device 中填写上边生成的虚拟串口号  /dev/pts/24   和   /dev/pts/25,  Input  中输入文字 ，点击回车发送测试 。

![](https://images2018.cnblogs.com/blog/681250/201808/681250-20180807135632303-2122488063.png)

三、QT程序调试串口时，需要root权限，否则打开串口失败。root权限运行`qtcreator`
- 进入到QT安装目录下的`/Tools/QtCreator/bin` ,运行`sudo ./qtcreator`即可。
- 非root权限 和 root权限运行qtcreate, 文件夹颜色不一样。

