# USB加minicom使用串口 - Orisun - 博客园







# [USB加minicom使用串口](https://www.cnblogs.com/zhangchaoyang/articles/1926848.html)





窘况描述：

1.PC上没有串口，并且主板上也没有预留增加串口的插孔

2.虽然买了根USB转串口线，但是天嵌没有提供Windows7上的USB转串口驱动  (PS:今天才想起来，其实可以果断自己去网上下一个HL-340的驱动嘛)

解决办法，用Linux，不需要另外安装USB转串口驱动！前提是你要先安装minicom

输入命令：dmesg| grep usb，看到下图

![](https://pic002.cnblogs.com/images/2011/103496/2011011215215991.png)

注意倒数第三行写着：usb2-10: ch341-uart converter now attached to ttyUSB0。现在你可以用ttyUSB0当串口了。安装minicom后重启你才能看到这句话。

下面配置minicom.

1.启动minicom：sudominicom -s。如果不是以管理员身份配置串口，则相关配置不能保存到默认的配置文件中，本次配置只在关闭minicom之前起作用。

2.安装串口

![](https://pic002.cnblogs.com/images/2011/103496/2011010522230812.png)

3.选择a配置SerialDevice

![](https://pic002.cnblogs.com/images/2011/103496/2011010522232892.png)


注意吧HardWare Flow Control设为no，否则在minicom上无法输入字符


4.选择e，配置串口的波特率、数据位、停止位、校验等

![](https://pic002.cnblogs.com/images/2011/103496/2011010522235418.png)

5.选择e，把波特率改为115200.回车退出保存。

6.把刚才的配置信息保存到默认的配置文件中

![](https://pic002.cnblogs.com/images/2011/103496/2011010522242245.png)

7.选择Exit退出minicom配置界面，minicom会自动进度终端，终端会自动连接串口。如果串口没有连接任何设备，屏幕右下角显示Offline

8.退出minicom，使用Ctrl+A，再输入Z。

![](https://pic002.cnblogs.com/images/2011/103496/2011010522244321.png)

9.出现minicom命令菜单，这里我们选择Q退出

![](https://pic002.cnblogs.com/images/2011/103496/2011010522251335.png)

10.开发板发NandFlash启动

11.启动minicom，选择Exit，进入连接串口的终端界面，可看到下图。输入一个ls命令测试一下

![](https://pic002.cnblogs.com/images/2011/103496/2011010522253878.png)

12.从开发板向PC传输文件

[root@EmbedSky /]# sz init 

然后在你PC的path路径下多了一个文件init，path是你从PC上启动minicom时的路径

13.从PC向开发板传输文件

[root@EmbedSky /]# rz

Ctrl+A后输入Z，进入minicom 主菜单界面，选择S

upload中选择zmodem，其实我是随便选择了一种模式

[Select one or more files for upload]界面按下空格键即可选择一个要传输的文件

选择[OKay]开始传输












