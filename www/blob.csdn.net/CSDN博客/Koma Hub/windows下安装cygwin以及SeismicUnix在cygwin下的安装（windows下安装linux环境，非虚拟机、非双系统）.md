# windows下安装cygwin以及SeismicUnix在cygwin下的安装（windows下安装linux环境，非虚拟机、非双系统） - Koma Hub - CSDN博客
2018年01月17日 22:23:55[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：529
一直想在我的笔记本上装个linux，之前装过win10和Ubuntu双系统，后来觉得卡，就重装了一个单Ubuntu系统，在重复的装软件，各种软件（办公、学习、编程），就连微软的office都已经让我安装在wine上了，网易云音乐......后来就在一天早上，开机，停留在开机输入密码的登录界面，键盘无效有木有，度娘了半天无果，去修了，果断修不好啊，这可是我多月来的心血，无奈，再重装。
这次我选择了CentOS7，上来就来个高级版本，可是，用ultraiso装好了一个CentOS-7-x86_64-Everything-1708.iso安装盘，结果，安装完重启就关机，一开始觉得是开机启动项的问题，有可能数值设置成了关机，结果再用ubuntu的启动盘改已经安装好的CentOS7的文件，结果开启动项就是多用户，这下没办法了，又装回了win10，结果吧win10上的所有必要软件安装了一番，不死心，继续下载了一个CentOS6.5，安装了一番，开机正常了，然后又进行学习软件的安装（SU，madagascar，mpich，openmp，java，python），一顿乱装之后，发现很多东西都干不了（办公），每次都要借别人电脑整理个东西什么的，无奈，有装回了win10，准便说一下，我的电脑联想Y470，老电脑了。
然后我又在win10下安装了各种linux的开发环境：
**1.虚拟机**
在vmware workstation上安装linux虚拟机就不用说了，可能唯一一点需要注意的就是共享文件夹的安装配置了，度娘上很多这样的文章。
**2.Ubuntu on Windows10**
这是一个新玩意，下面是几篇技术文章：
[Windows 10 loves Ubuntu](https://insights.ubuntu.com/2017/07/11/windows-10-loves-ubuntu-loveislove/)
[Win10一周年纪念版，安装体验 Ubuntu on Windows](http://www.linuxidc.com/Linux/2016-12/138269.htm)
[Win10安装Ubuntu子系统教程（附安装图形化界面）](https://www.windows10.pro/bash-on-ubuntu-on-windows/)
**3.尝试在cmd中用linux指令**
这个在GitHub上有开源项目，搜一搜就找到了。
**4.cygwin（最后的尝试）**
这是今天主要介绍的，
（1）下载该软件：[链接1](https://sourceforge.net/projects/cygwinportable/files/)，[链接2](https://www.cygwin.com/)，[链接3](https://www.cygwin.com/install.html)，网上很多链接，随便下载一个就行了。
                 软件如下：![](https://img-blog.csdn.net/20180117213001768?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
（2）双击安装：
![](https://img-blog.csdn.net/20180117213425617?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
点击下一步，注意期间需要两个文件路径，一个是cygwin的安装路径（这个目录对应你的linux的根目录“/”），另一个是软件安装包的下载路径（就是存放例如gcc-core***.tar.gz的路径）。
（3）然后来到一个选择你需要的安装包的界面，点击default，将其变成install（如果是添加package过程，会显示你已经安装的package是keep），选择你需要的就行了（比如SeismicUnix需要：gcc-core，gcc-fortran，gdb，make(devel中的程序)，gfortran，libx11-dev，libxt-dev，libglut-dev，libxmu-dev，libxi-dev，X11中所有的程序），切勿选择所有package，内容太多，很多用（选择必须的system等就好了，有必要把vim也选上）：
![](https://img-blog.csdn.net/20180117220139772?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后点击下一步进入如下界面：
![](https://img-blog.csdn.net/20180117214237801?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后等待安装结束就行了，过程时间较长。
（4）安装结束后，把D:\cygwin\bin加入win10的环境变量（填你的cygwin安装路径下的bin），这样你就可以在cmd下使用linux指令，也可以双击安装后声称的快捷方式![](https://img-blog.csdn.net/20180117215607337?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)使用cygwin终端。
下面简单介绍我安装后的目录：
![](https://img-blog.csdn.net/20180117215644363?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
进入根目录：cd /， 内容如上图，cygdrive文件夹下是你的windows盘的文件夹（如上图所示，我的电脑分区为:c,d,e,f，可以进入这些文件夹访问你的电脑盘）；
这里还需要说一下，你之前安装的软件也可以用，比如我之前安装了JDK（Java  SE），下面是我在cygwin终端下显示的JAVA_HOME：
![](https://img-blog.csdn.net/20180117220440866?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后我使用环境变量进入JDK的目录结果是这样：
![](https://img-blog.csdn.net/20180117220638316?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
是不是很神奇！到此，cygwin的安装就结束了，如果你需要添加package的话，请重新双击setup_x86.exe下一步到select package那一步搜索并勾选你需要的package进行安装就好了。
下面介绍在cygwin下安装SeismicUnix的过程：
（1）下载就不说了，百度“CWP”下载最新版本的CWP****.tar.gz就行了
（2）安装依赖的包：再次装机setup_x86.exe到选择package那一步，搜索并将skip变为install以下包：
gcc-core，gcc-fortran，gdb，make(devel中的程序)，gfortran，libx11-dev，libxt-dev，libglut-dev，libxmu-dev，libxi-dev，X11中所有的程序
然后下一步安装，直至结束；
（3）解压CWP：tar -zxvf cwp***.tar.gz
（4）将生成的目录移动至你想安装的目录下（我的安装目录为/home/SU）；
（5）修改环境变量：vim /etc/profile
        按“i”编辑，添加如下内容：export CWPROOT=/home/SU            
                                  export PATH=$PATH:/home/SU/bin
         按Esc，输入“：wq”保存退出；
（6）将\home\SU\src\configs文件夹中的Makefile.config_Cygwin_32文件替换上一级目录的Makefile.config；
（7）开始安装：
基础安装：
make install   (to install the basic set of codes)
make xtinstall (to install the X-toolkit applications)
非基础安装：
make xminstall (to install the Motif application)(nonessential)
make finstall (to install the Fortran codes)
make mglinstall (to install the Mesa/ Open GL items)
make utils     (to install libcwputils) (nonessential)
（8）cygwin terminal不支持图形显示，需要在命令行中执行startx或startxwin语句进入图形界面。
（9）测试：输入ximage
![](https://img-blog.csdn.net/20180117222244651?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
cygwin terminal不支持图形显示，需要在命令行中执行startx或startxwin语句进入图形界面：详述
首先在非startxwin下结果如下图所示：
![](https://img-blog.csdn.net/20180120155652037?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
无法显示，进入startxwin：
![](https://img-blog.csdn.net/20180120155717081?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
任务栏中出现如图：
![](https://img-blog.csdn.net/20180120155736476?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
右击上图右侧图标，出现如下图：
![](https://img-blog.csdn.net/20180120155804531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
就出现一般linux系统下的工具栏（个人感觉很像CentOS），然后打开终端（如下图）：
![](https://img-blog.csdn.net/20180120155902643?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
再测试安装好的SeismicUnix，如下图：
![](https://img-blog.csdn.net/20180120155931337?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**完美！！！！！！！！！！！！！！！！**
