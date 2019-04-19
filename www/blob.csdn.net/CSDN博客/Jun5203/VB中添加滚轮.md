# VB中添加滚轮 - Jun5203 - CSDN博客
2018年02月24日 10:27:00[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：280
1、下载"vb6mousewheel.exe"软件包到桌面，解压并找到"VB6IDEMouseWheelAddin.dll"；
2、将该文件放到如下路径下：
(1)Windows的32位操作系统：C:\Windows\System     
(2)Windows的64位操作系统：C:\Windows\SysWOW64
3、以管理员身份运行"命令提示符"；
4、输入"regsvr32 C:\Windows\SysWOW64\VB6IDEMouseWheelAddin.dll"；
![](https://img-blog.csdn.net/20180428191256401)
5、注册成功后，新建VB工程，在“外接程序管理器”中按如图操作；
![](https://img-blog.csdn.net/20180428191304353)
6、打开代码窗口，你会发现滚轮可以上下移动，惊不惊喜？意不意外？
