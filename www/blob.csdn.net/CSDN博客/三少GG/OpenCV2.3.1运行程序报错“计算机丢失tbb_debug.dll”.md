# OpenCV2.3.1运行程序报错“计算机丢失tbb_debug.dll” - 三少GG - CSDN博客
2012年03月07日 16:21:38[三少GG](https://me.csdn.net/scut1135)阅读数：13000
[http://www.opencv.org.cn/forum/viewtopic.php?p=52223](http://www.opencv.org.cn/forum/viewtopic.php?p=52223)
是一个非常简单的小程序，就是“学习opencv”那本书上的一个小例子，我把程序抄下来运行看看的，出现了问题，程序如下。
编译无错，运行的时候跳出弹框“无法启动此程序，因为计算机中丢失tbb_debug.dll。尝试重新安装此程序以解决此问题”。
不知道如何解决？
系统环境以及程序环境我都安装搜集到的资料进行了设置。
关于这个dll文件，我查了相关资料，说是很重要的系统文件，假如出问题，那么系统应该是会有不少运行错误的，现在我只是运行程序出错，应该不会是我系统真的丢失这个dll文件吧？
方法1：(已试)
1.我也遇到过。我**将\opencv\build\common\tbb\ia32目录下的tbb.dll复制改名为tbb_debug.dll 搞定！(亲测OK)**哈哈
楼上好方法，反正又不调试tbb
 ~ 这招好用！！赞啊~还是LS的方法好使啊。。NB
方法2.
你用的opencv2.3的动态库with tbb，但是你的系统没有安装tbb，所以回报错。
建议下载INTELTBB3.0
[http://www.threadingbuildingblocks.org/
 ... ss_win.zip](http://www.threadingbuildingblocks.org/uploads/77/151/3.0/tbb30_20100406oss_win.zip)
检查..\\Intel TBB 3.0\bin\ia32\vc10是否已经被加入到环境变量(我的电脑-属性-环境变量)PATH，如果没有，请加入。加入后需要注销当前Windows用户（或重启）后重新登陆才生效。(可以在任务管理器里重启explorer.exe)
这个问题我在用PACT算法做特征提取时，也遇到了这个问题，也是在win7，vs2010,opencv2.3的配置下，出现“计算机丢失tbb_debug.dll,请重新安装”的错误，我解决这个问题的步骤是：1，下载上面楼主http://www.threadingbuildingblocks.org/
 ... ss_win.zip的文件并解压。
2，在解压的文件中进入...\bin\ia32\vc10文件夹找到tbb_debug.dll文件。
3，将上面找到的tbb_debud.dll文件复制到你opencv安装的...\opencv\build\common\tbb\ia32\vc10文件夹中，然后重新编译即可。
其实，这是我们安装的opencv2.3动态链接库中由于某种原因丢失了tbb_debug.dll文件，我们只需要想办法弄到此文件，然后把它加到指定的文件夹中即可，这个特定的文件夹一般是放有大量同类文件的文件夹，不知道是哪个文件夹时可以在网上搜搜。
