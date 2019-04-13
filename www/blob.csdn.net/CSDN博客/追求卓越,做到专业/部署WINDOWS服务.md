
# 部署WINDOWS服务 - 追求卓越,做到专业 - CSDN博客


2016年05月12日 11:03:06[Waldenz](https://me.csdn.net/enter89)阅读数：280


# [部署WINDOWS服务](http://www.cnblogs.com/zwleisa/archive/2006/02/20/334044.html)
https://jingyan.baidu.com/article/fa4125acb71a8628ac709226.html
部署windows服务--制作安装包
(1)、建立一个新的windows服务项目Server1
(2)、打开Service1代码视图，找到OnStart部分，加入代码
(3)、切换到设计视图，右键－添加安装程序
(4)、切换到新生成的ProjectInstaller.cs设计视图，找到serviceProcessInstaller1对Account属性设置为LocalSystem，对serviceInstaller1的ServiceName属性设置为Server1（服务的名字），StartType属性设置为Automatic（系统启动的时候自动启动服务）
(5)、建立一个新的安装项目ServerSetup（我们为刚才那个服务建立一个安装项目）
(6)、右键－添加－项目输出－主输出－选择Service1－确定
(7)、右键－视图－自定义操作－自定义操作上右键－添加自定义操作－打开应用程序文件夹－选择刚才那个主输出－确定
(8)、重新生成这个安装项目－右键－安装
(9)、在服务管理器中（我的电脑－右键－管理－服务和应用程序－服务）找到Server1服务，启动服务
部署windows服务
1. cmd启动命令行窗口；
2. 输入 cd C:\Windows\Microsoft.NET\Framework\v4.0.30319 回车
64位的应用程序目录为C:\Windows\Microsoft.NET\Framework64\v4.0.30319
切换当前目录，此处需要注意的是，在C:\Windows\Microsoft.NET\Framework目录下有很多类似版本，具体去哪个目录要看项目的运行环境，例 如果是.net framework2.0则需要输入 cd C:\Windows\Microsoft.NET\Framework\v2.0.50727
3. 输入 InstallUtil.exe E:\TestApp\AAA.exe 回车
说明：E:\TestApp\AAA.exe表示项目生成的exe文件位置
4. 打开服务，就可以看到已经安装的服务了
卸载服务
1. cmd启动命令行窗口；
2. 输入 sc delete 服务名称





