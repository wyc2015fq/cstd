# arm仿真器Skyeye的安装及使用 - xiahouzuoxin - CSDN博客





2012年10月02日 17:08:21[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：2969








    SkyEye是一个开源软件（OpenSource Software）项目，中文名字是"天目"。SkyEye的目标是在通用的Linux和Windows平台上实现一个纯软件集成开发环境，模拟常见的嵌入式系统，可在SkyEye上运行μCLinux以及μC/OS-II等，以及各种系统软件（如TCP/IP，图形子系统，文件子系统等），并可对它们进行源码级的分析和测试。


目前SkyEye可以模拟的CPU主要是基于ARM内核的CPU，包括ARM7TDMI，ARM720T，ARM9TDMI，ARM9xx，ARM10xx，StrongARM，XScale等，还可以仿真各种开发板。




    注：可以到[http://sourceforge.net/projects/skyeye/files/](http://sourceforge.net/projects/skyeye/files/)下载sky最新版本。



一、操作环境

1、操作系统：RHEL5.1

2、arm交叉编译环境，没有安装的的参考CSDN客[http://blog.csdn.net/xiahouzuoxin/article/details/8034502](http://blog.csdn.net/xiahouzuoxin/article/details/8034502)

3、skyeye版本：v1.3.1（解压后文件名夹为skyeye-1-3-1_rc1）




二、Sky的安装

1、进入解压后skyeye的根目录

cd /mnt/hgfs/Linux/skyeye-1-3-1_rc1




    若当前非root用户，则切换到root用户，

    su root




    关闭SELinux，使程序安装过程具有permissive

     vi /etc/selinux/config，修改为SELINUX=disable，重启Linux,

     若不想重启，则执行setenforce 0




 2、依次执行一下命令安装

       ./configure

       make lib

       make

       make lib_install

       make install




       注：默认情况下，skyeye安装到/opt/skyeye目录下，若要修改目录，则./cofigure中添加--prefix [dir] 选项




3、添加环境变量

      为使用方便，因此为skyeye命令（/opt/skyeye/bin目录下）添加环境变量。

  vi /etc/profile

      找到相应位置(关键字pathmunge)处添加

pathmunge /opt/skyeye/bin

返回SHELL，执行 source /etc/profile 使修改有效




三、Skyeye使用测试




      Skyeye安装后提供的例程在/opt/skyeye/testsuite

      Sky的运行必须到skyeye的配置文件（skyeye.conf）所在目录下，

cd /opt/skyeye/testsuite/arm_hello




打开skyeye，-e参数不能少，否则运行时可能出现bus error

      skyeye -e arm_hello  




启动skyeye，此时弹出串口框

      start                          



      运行，串口框不停输出输出Hello Wrold!

run




      停止运行，及退出

stop                           //停止运行

quit                            //退出skyeye




      注：更多命令在进入skyeye终端输入help。




 四、更进一步使用方法参考

**[htttw](http://my.csdn.net/htttw)的CSDN博文**[http://blog.csdn.net/htttw/article/details/7231851](http://blog.csdn.net/htttw/article/details/7231851)，作者**[htttw](http://my.csdn.net/htttw)的博文对我帮助很大，甚是感谢！**







