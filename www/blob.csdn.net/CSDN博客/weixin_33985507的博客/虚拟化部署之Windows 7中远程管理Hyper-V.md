# 虚拟化部署之Windows 7中远程管理Hyper-V - weixin_33985507的博客 - CSDN博客
2010年07月21日 22:53:02[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
远程桌面连接工具相信IT管理员们都不会寞生，轻轻地点击一个链接就可以连接到我们想要控制的服务器上，这样一来便可以不用为了添加一个USER而老远跑到机房操作了，有了它我们便不用在没有出现服务器硬件故障的情况下整天呆在机房了。那么管理Hyper-V是不是也可以同样使用这种方法呢？当然可以，只不过你会觉得在管理上没有在服务器上管理那直观，界面本身就受到了远程桌面连接工具的设置影响。那么有没有更好的办法来管理Hyper-V呢？有，今天我要给大家介绍的内容就是一种非常不错的方法，使用“Windows 7 远程服务器管理工具”中的Hyper-V工具便可以像在服务器端一样对Hyper-V管理器进行准确无误的操作。心动了吧，那就跟随我一起去了解它吧。
1.如何下载“Windows 7 远程服务器管理工具”。
点击下面的链接进行下载，注意选择下载需要根据你的CUP类型决定。
[http://www.microsoft.com/downloads/details.aspx?displaylang=zh-cn&FamilyID=7d2f6ad7-656b-4313-a005-4e344e43997d](http://www.microsoft.com/downloads/details.aspx?displaylang=zh-cn&FamilyID=7d2f6ad7-656b-4313-a005-4e344e43997d)
![image](http://lqlaps169.blog.51cto.com/attachment/201007/21/855319_1279723964Qeut.png)
2.下载完成后，双击此下载包进行安装，记注我们的安装一定是在Windows7系统上。
![image](http://lqlaps169.blog.51cto.com/attachment/201007/21/855319_1279723965ClHW.png)
3.在安装更新检测后将弹出“Windows Update 独立安装程序”对话框，直接点击“是”进行安装。
![image](http://lqlaps169.blog.51cto.com/attachment/201007/21/855319_1279723966Ytpl.png)
4.点击“我接受”。
![image](http://lqlaps169.blog.51cto.com/attachment/201007/21/855319_1279723968YVnD.png)
5.正在初始化安装，请耐心等待。
![image](http://lqlaps169.blog.51cto.com/attachment/201007/21/855319_1279723970GZso.png)
 6.正在安装工具包。
![image](http://lqlaps169.blog.51cto.com/attachment/201007/21/855319_1279723971xqyc.png)
7.安装成功后点击“关闭”。
![image](http://lqlaps169.blog.51cto.com/attachment/201007/21/855319_1279723972USfE.png)
8.打开“控制面板”－－“程序和功能”－－“打开和关闭Windows功能”，并依次展开“远程服务器管理工具”－－“角色管理工具”，选择“Hyper-V工具”并点击“确定”。
![image](http://lqlaps169.blog.51cto.com/attachment/201007/21/855319_12797239745OKX.png)
9.正在安装“Hyper-V工具”，更改完成后将自动关闭掉“打开和关闭Windows功能”窗口。
![image](http://lqlaps169.blog.51cto.com/attachment/201007/21/855319_1279723975FGGx.png)
10.现在我们可以点击“开始”－－“所有程序”－－“管理工具”－－“Hyper-V管理器”来打开它了。
![image](http://lqlaps169.blog.51cto.com/attachment/201007/21/855319_1279723976HnBt.png)
11.在打开的“Hyper-V管理器”中最初是没有任何服务器和虚拟机的，我们可以在“操作”窗口直接点击“连接到服务器”。
![image](http://lqlaps169.blog.51cto.com/attachment/201007/21/855319_1279723978NrmS.png)
12.在弹出的“选择计算机”对话框中选择“另一台计算机”，通过浏览我们可以选择所需控制的Hyper-V服务器。
![image](http://lqlaps169.blog.51cto.com/attachment/201007/21/855319_1279723979gzjX.png)
13.成功连接到所需管控的Hyper-V服务器后，我们可以看到当前“Hyper-V管理器”中出现在服务器端“Hyper-V管理器”中一样的内容。
![image](http://lqlaps169.blog.51cto.com/attachment/201007/21/855319_1279723980N4WC.png)
14.现在我们就可以坐在自己的电脑旁，远程指挥着Hyper-V工具了，这样一来就像在服务器端一样可以进行开机、关机、创建虚拟机和创建快照等等操作了。
![image](http://lqlaps169.blog.51cto.com/attachment/201007/21/855319_1279723981B9oG.png)
**注意事项：**
1.运用“Windows 7 远程服务器管理工具”必须使Hyper-V服务器和当前Windows7在同一个域中，当然Hyper-V服务器也可以是一个域控。
2.服务器和客户端防火墙都必须允许Hyper-V工具出入站。
3.客户端当前用户必须是一个有管理Hyper-V服务器权限的域用户。
请关注我后续发布的Hyper-V系列文章。
[](http://www.example.com/%E8%BF%9C%E7%A8%8B%E7%AE%A1%E7%90%86)
