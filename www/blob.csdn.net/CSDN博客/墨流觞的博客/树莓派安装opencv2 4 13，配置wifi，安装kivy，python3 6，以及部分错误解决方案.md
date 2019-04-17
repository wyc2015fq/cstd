# 树莓派安装opencv2.4.13，配置wifi，安装kivy，python3.6，以及部分错误解决方案 - 墨流觞的博客 - CSDN博客





2018年09月21日 13:08:50[墨氲](https://me.csdn.net/dss_dssssd)阅读数：195








### 树莓派安装

很简单的几个步骤:
- 利用SDFormatter清空sd卡上的内容
- 利用Win32DiskImager将系统写到sd卡上，注意这个时候如果出现格式化磁盘的对话框，**一定要取消，取消，取消！！！**
- 理论上，将卡插在树莓派上，上电就安装成功了

**注意：**
- 如果你有一个显示屏，此时直接连接好显示屏，上电就可以启动linux系统了
- 如果没有，如果安装树莓派官方系统， 将卡插在树莓派上电后，会自动配置好wifi。
- 会自动连接到手机或电脑的wifi热点或宿舍内的局域网。 此时(1)可以通过手机wifi热点查到树莓派 的ip地址；(2)电脑的我没试过；(3)如果是宿舍的局域网，可以登录**无线路由器设置网址。打开192.168.1.1或者192.168.0.1，不同路由器不同，一般说明书里有说明** 在里面会有连接局域网的设备及ip地址。
- 接下来将ip地址输入到putty，vnc等就可以远程操作了。


之后就是愉快的安装opencv，TensorFlow等所需要的操作了。

**但是：**

我使用的是ubuntu-mate-16.04.2-desktop-armhf-raspberry-pi.img，所以要处理几个问题。

6. 利用Win32DiskImager将系统写到sd卡上，之后在电脑上打开sd卡，打开config.txt,将`hdmi_safe=1`前面的注释去掉，**具体看环境配置中的第5条**

7.  需要配置wifi， **具体看环境配置第1条**
### tips:
- Alt+鼠标 ： 移动屏幕

### 环境配置
- 
wifi配置
- iwconfig：查看链接wifi
- ifconfig:查看IP地址
- Wifi安装配置：[https://blog.csdn.net/ccccc_Jun/article/details/77319593](https://blog.csdn.net/ccccc_Jun/article/details/77319593)

- 
安装python3.6:
[https://gist.github.com/dschep/24aa61672a2092246eaca2824400d37f](https://gist.github.com/dschep/24aa61672a2092246eaca2824400d37f)

切换Python3版本为python3.6： [https://linux.cn/article-8668-1.html](https://linux.cn/article-8668-1.html)

需要 `whereis python3.6` 查看python3.6的位置。**切记不要修改默认python2**。- 
在linux系统上安装kivy:
[https://kivy.org/doc/stable/installation/installation-rpi.html](https://kivy.org/doc/stable/installation/installation-rpi.html)

如果是Ubuntu mate，推荐此方法：
[https://kivy.org/doc/stable/installation/installation-linux.html](https://kivy.org/doc/stable/installation/installation-linux.html)- 
安装opencv，推荐安装2.4.13版本，不推荐安装opencv3，安装Python环境为**python2**
[http://www.voidcn.com/article/p-ulryhijs-bph.html](http://www.voidcn.com/article/p-ulryhijs-bph.html)

如果是Python不需要操作步骤**相关配置**，

测试：

在命令行中输入`python`
```python
imprt cv2
cv2._version__
```

输出： ‘2.4.13’安装成功
- 
LCD显示屏安装出现no-signal,可以尝试将sd卡中，config.txt 中`hdmi_safe=1`前面的注释去掉
![在这里插入图片描述](https://img-blog.csdn.net/20180921133127321?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)- 
安装vscode:
[http://www.cnblogs.com/GeGeBoom/p/8627438.html](http://www.cnblogs.com/GeGeBoom/p/8627438.html)

- 
linux Python3 安装pygame：[https://blog.csdn.net/qq_31383385/article/details/79983019](https://blog.csdn.net/qq_31383385/article/details/79983019)

- 
安装过程中碰到的错误
- 
sudo apt-get update 得到error:  unable to lock directory   /var/lib/apt/lists/

解决方法：[https://blog.csdn.net/zyxlinux888/article/details/6358615](https://blog.csdn.net/zyxlinux888/article/details/6358615)

- 
安装库出现错误：unsupported operand type(s) for -=: ‘retry’ and ‘int’
[https://stackoverflow.com/questions/37495375/python-pip-install-throws-typeerror-unsupported-operand-types-for-retry](https://stackoverflow.com/questions/37495375/python-pip-install-throws-typeerror-unsupported-operand-types-for-retry)














