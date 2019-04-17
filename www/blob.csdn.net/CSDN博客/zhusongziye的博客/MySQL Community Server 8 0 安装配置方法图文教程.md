# MySQL Community Server 8.0 安装配置方法图文教程 - zhusongziye的博客 - CSDN博客





2018年06月30日 18:56:29[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：1231











**一、安装 **

首先，从mysql的中文社区下载，我尝试过其他下载方式，发现还是中文社区下载的安装包最好， 下载完成后，运行安装文件，运行后，出现如下界面：

![](https://img-blog.csdn.net/20180630184626159?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![](https://files.jb51.net/file_images/article/201805/2018527143840197.jpg?2018427143851)

等待几十秒左右，就会出现安装界面啦。

![](https://img-blog.csdn.net/2018063018465658?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


和安装其他软件一样需要勾选“I accept the license terms”,然后点击”next”：

![](https://img-blog.csdn.net/20180630184735218?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


这里选择”custom”，也可以根据自己需求选择要安装的方式：

![](https://img-blog.csdn.net/20180630184754640?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


我需要安装mysql server，所以在Available Products里展开第一项选择“MySql server8.0.11-X64”：

![](https://img-blog.csdn.net/20180630185028729?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


然后点击绿色箭头将当前选择的相移动到安装列表

![](https://img-blog.csdn.net/20180630185048948?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


接着”next”,可以根据需要选择是否安装VC++2015,不需要点击“next”,需要的话点击”Execut”,这里我选择了”Execut”：

![](https://img-blog.csdn.net/2018063018510147?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


VC++2015安装过之后，接着”next”：

![](https://img-blog.csdn.net/20180630185113349?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


点击“Execute”开始安装，安装过程中会显示安装的进度，等待安装完成后Status会显示Complete，mysql图标前会出现一个绿色的勾，然后点击“Next”按钮进入产品配置界面

![](https://img-blog.csdn.net/20180630185128345?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


接着”next”,选择默认选项，进入下个页面：

![](https://img-blog.csdn.net/20180630185155699?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


选择“Development Machine”，选择此项将使用较小的内容来运行我们的mysql server，对应小型软件、学习是完全够用的。之后“next”

![](https://img-blog.csdn.net/20180630185209652?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


按默认选择，然后“next”

![](https://img-blog.csdn.net/20180630185227252?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


在Root Account Password设置数据库root账号的密码，我填的是123456所以程序提醒我密码强度为弱，我们需要牢记这个密码，然后点击“next”

![](https://img-blog.csdn.net/20180630185238925?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


这里可以设置mysql server的名称和是否开机启动，我没有改名字，只是取消了开机启动，然后”next”

![](https://img-blog.csdn.net/2018063018525469?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


还是”next”

![](https://img-blog.csdn.net/20180630185307176?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


此界面将之前设置的配置内容应用到我们的mysql server，点击“Execute”，等待完成就可以了 


![](https://img-blog.csdn.net/20180630185321290?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


点击“Finish”完成配置

![](https://img-blog.csdn.net/20180630185334936?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


回到安装程序，点击“next”继续

![](https://img-blog.csdn.net/20180630185346101?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


提示安装已完成，点击“Finish”

![](https://img-blog.csdn.net/20180630185357170?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


**二、接下来需要配着mysql的环境变量，步骤是：**

1. 和其实环境变量的配置方法一样，我们打开环境变量配置窗口（组合键win+Pause ->高级系统设置 ->高级 -> 环境变量） 
2. 选中系统变量中的“path”，在path值开头处输入mysql安装目录下的bin文件夹所在路径：C:\Program Files\MySQL\MySQL Server 8.0\bin，保存退出 
注意：mysql server安装的默认路径为：C:\Program Files\MySQL\MySQL Server 8.0 
3. 测试是否配置成功：打开cmd，输入“mysql -u root -p”回车，然后输入mysql安装时设置的root账号的密码（123456），若提示“Welcome to the MySQL monitor.”说明配置成功了。

![](https://img-blog.csdn.net/20180630185414473?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


**三、启动mysql**

以管理员的身份运行cmd，输入“net start mysql80”（cmd里不区分大小写）

![](https://img-blog.csdn.net/20180630185429151?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




