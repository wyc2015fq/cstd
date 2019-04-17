# MinGW下载并配置gcc/g++编译环境 - DoubleLi - 博客园






本文将讲解如何下载MinGW并配置gcc\g++编译环境

一、下载MinGW 
在MinGW官网中下载“mingw-get-setup.exe” 
官网传送门：[http://www.mingw.org/](http://www.mingw.org/)
![MinGW官网](https://img-blog.csdn.net/20170119011128615?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzY3MzE2Nzc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
据说有离线压缩包可以下载，但是我没有找到——慌。

二、安装MinGW 
打开下载好的软件 
![软件](https://img-blog.csdn.net/20170119011627980?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzY3MzE2Nzc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
选择安装并等待（此时电脑需要保持联网） 
![Install](https://img-blog.csdn.net/20170119011746090?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzY3MzE2Nzc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![Continue](https://img-blog.csdn.net/20170119011823640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzY3MzE2Nzc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![Wating](https://img-blog.csdn.net/20170119011844950?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzY3MzE2Nzc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

三、下载gcc 
安装完成后点击“continue”，打开MinGW Installation Manager。 
如图，在含有gcc字样的那行右键，选择”Mark for Installtion” 
并且，选择“Installtion”-> “Apply Changes” 
然后，点击“Apply”确认操作 
最后，在确定安装完成后点击“Close”关闭对话框 
![gcc](https://img-blog.csdn.net/20170119012708294?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzY3MzE2Nzc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![apply changes](https://img-blog.csdn.net/20170119013037814?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzY3MzE2Nzc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![apply](https://img-blog.csdn.net/20170119013113988?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzY3MzE2Nzc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![Close](https://img-blog.csdn.net/20170119013358738?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzY3MzE2Nzc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
//如果网络质量不好可能会弹出警告，手动点击确认应该就可以继续了。

四、配置环境变量 
打开“系统属性”->“环境变量” 
在“系统变量”中选择“Path”并点击“编辑” 
点击“新建”，并输入“C:\MinGW\bin”（如果自行更改了MinGW的安装路径请自行对照修改此处的路径） 
然后，全部点“确定”关闭所有对话框。 
重启电脑！！！！

五、检验gcc是否安装成功 
win+R （打开运行） 
输入“cmd” 
键入“gcc -v” 
如果出现如图所示的情况，说明安装成功。 
![cmd](https://img-blog.csdn.net/20170119014253742?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzY3MzE2Nzc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

完~~~









