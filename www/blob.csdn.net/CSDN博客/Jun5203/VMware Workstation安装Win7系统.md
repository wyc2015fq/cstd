# VMware Workstation安装Win7系统 - Jun5203 - CSDN博客
2018年07月26日 16:38:38[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：4267
所属专栏：[Linux](https://blog.csdn.net/column/details/25476.html)
## 前言
昨天用Win10自带的虚拟机做了一次小测试，今天转战VMware Workstation，虽说有点麻烦，不过自己也庆幸麻烦了一次，因为如果没有今天的麻烦，那么我还不知道我昨天犯了多么Low的错误。废话不多说，VMware Workstation安装Win7系统过程呈上，还望笑纳~
## 过程
★ 工具 
VMware Workstation  
Windows7镜像ISO系统文件 
★ 核心步骤：安装VMware Workstation  
●步骤一：打开“VMware Workstation ”，在文件中新建一个虚拟机，或者在主页上选择新建一个虚拟机。 
![](https://img-blog.csdn.net/2018072616141730)
●步骤二：选择“典型”，点击“下一步” 
![](https://img-blog.csdn.net/20180726161435164)
●步骤三：点击“浏览”，添加“Win7镜像文件”，然后选择“稍后安装操作系统”，点击“下一步” 
![](https://img-blog.csdn.net/20180726161446184)
●步骤四：选择“Microsoft Windows”,版本选择“Win7”，点击“下一步” 
![](https://img-blog.csdn.net/201807261615071)
●步骤五：位置为默认路径即可，也可根据自己的习惯选择，点击“下一步” 
![](https://img-blog.csdn.net/20180726162020688)
●步骤六：磁盘大小可根据默认设置，也可根据个人需求，点击“下一步” 
![](https://img-blog.csdn.net/20180726162030710)
●步骤七：点击“自定义硬件”，左侧选择“新CD/DVD”，右侧点击“浏览”，添加“ISO镜像文件”，之后关闭 
![](https://img-blog.csdn.net/20180726162046820)
以上是建立虚拟机的步骤，之后就可安装系统，详情请往下看： 
♫步骤一：右键点击新创建虚拟机的名称，在电源中，选择“打开电源时进入固件” 
♫步骤二：进入BIOS设置界面——Boot 
♫步骤三：使用“+”将“CD-ROM drive”调整到第一个位置 
♫步骤四：Fn+F10保存更改，常规装系统即可
## 小结
有一点问题需要注意：在使用VMware Workstation时要将Win10自带的虚拟机关闭，很重要！很重要！很重要！ 
PS：在装完系统后我发现不能全屏，这就有点尴尬了哦哈，不过没关系，总是有解决方法的，其实很简单，只需右击桌面，点击屏幕分辨率，选择1920×1080就OK啦
