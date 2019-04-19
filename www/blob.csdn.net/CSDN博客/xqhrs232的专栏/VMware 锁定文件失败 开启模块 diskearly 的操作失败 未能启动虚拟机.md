# VMware 锁定文件失败 开启模块 diskearly 的操作失败 未能启动虚拟机 - xqhrs232的专栏 - CSDN博客
2017年05月05日 15:05:07[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：669
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://blog.csdn.net/snowfoxmonitor/article/details/18493803](http://blog.csdn.net/snowfoxmonitor/article/details/18493803)
相关文章
1、如何解决VMware开启模块diskearly的操作失败----[http://jingyan.baidu.com/article/a501d80c0e45b8ec630f5ed6.html](http://jingyan.baidu.com/article/a501d80c0e45b8ec630f5ed6.html)
无法打开内核设备:[系统](http://www.xp510.com/xiazai/os/515/1.html)找不到指定文件。你想要在安装VMware Workstation 前重启吗？打开模块DevicePowerOn电源失败。
![](https://img-blog.csdn.net/20140119073333125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc25vd2ZveG1vbml0b3I=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
方法一:
需以下几步即可解决问题。
用记事本打开你的虚拟机所在目录下的.vmx文件
找到这一行：
vmci0.present = "TRUE"
将 TRUE 改为 FALSE
保存
再打开虚拟机就可以了
方法二:
删除虚拟机系统文件目录下的 三个 *.lck文件，启动正常
打开你存放虚拟机系统文件的文件夹，注意，是系统文件，不是虚拟机的安装目录，也就是你建立虚拟机的时候下图设置的位置：
然后以下面关键字搜索这个文件夹：*.lck,将三个文件删除.
启动就正常
