# VS2017创建Linux项目实现远程GDB调试 - fanyun的博客 - CSDN博客
2017年03月31日 12:36:47[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：16309
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
第一步：正版VS2017exe安装器下载。
下载链接：http://download.csdn.net/detail/fanyun_01/9787333
第二步：VS2017安装，下载好安装器，点击运行，选择自己所需要的安装配置，在这我们选择，点击安装，差不多十几分钟可以安装完。
第三步：安装完，启动VS2017，新建一个linux工程，如下图所示：
![](https://img-blog.csdn.net/20170320162832730?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmFueXVuXzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
第四步，会弹出一个对话框，点击确认，设置远程服务器配置，如下所示：
![](https://img-blog.csdn.net/20170320163132957?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmFueXVuXzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170320163152051?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmFueXVuXzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
如果安装了杀毒软件和防火墙，会弹出一个对话框，点击允许就可以了。
第五步：编译，如果服务器都配好了，点击编译就可以进入GDB调试断点，如果服务器环境没有配置好，则需要配置linux服务器环境。
一般报错有以下几种：
（1）. unrecognized command line option"-std=c++11"
一种原因是工程配置不对，如下图配置工程即可，另外一种原因就是，你的linux服务器不支持c++11，这时，需要，升级你的linux服务器编译器，一般需要gcc-g++4.8以上版本，具体升级过程如下：
1.如有安装过先卸载gdb旧版本
rpm -qa | grep gdb
rpm -e --nodeps gdb-7.2-60.el6_4.1.i686
2. 安装Texinfo文档系统
yum install ncurses-devel
3. 下载gdb7.6.1[源码](http://dev.ailab.cn/)包
http://ftp.gnu.org/gnu/gdb/gdb-7.6.1.tar.gz
将[源码](http://dev.ailab.cn/)包放在home目录的Download目录中
4. 解压缩gdb7.6.1[源码](http://dev.ailab.cn/)包
cd /root/Downloads/
tar -zxvf gdb-7.6.1.tar.gz
5. 生成makefile
cd /root/Downloads/gdb-7.6.1
./configure
make
sudo make install
6. 将编译好的gdb拷贝到/usr/bin目录中
sudo cp /root/Downloads/gdb-7.6.1/gdb/gdb/usr/bin/gdb
7. 查看安装是否成功。
gdb –v
![](https://img-blog.csdn.net/20170320163333817?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmFueXVuXzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
（2）.无法启动gdb-server,如下图所示：
![](https://img-blog.csdn.net/20170320164053935?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmFueXVuXzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
下载gdb-7.6.1 tar.gz([http://www.gnu.org/software/gdb/download/](http://www.gnu.org/software/gdb/download/))
解压gdb-7.6.1.tar.gz
tar -zxvf gdb-7.6.1.tar.gz
这时，则说明linux服务器没有安装gdb-server,安装过程如下：
下载gdb：如果放到了/home/cby目录：
#cd /home/cby
#tar zxvf gdb-7.6.1.tar.gz
#cd gdb-7.6.1
#./configure --target=arm-linux --prefix=/home/cby/arm-gdb
#make
#make install
然后建立gdbserver:
#mkdir /home/cby/gdbserver
#cd ../gdbserver
#chmod +x /home/cby/gdb-7.6.1 /gdb/gdbserver/configure
#CC=arm-linux-gcc ../gdb-7.6.1 /gdb/gdbserver/configure /
--host=arm-linux  --prefix=/home/cby/gdbserver
#make
#make install
#arm-linux-strip gdbserver
#cp gdbserver /tftpboot
调试步骤:
1、交叉编译，带参数－gstabs 或 -g 加入调试信息。
假设要调试的程序为hello.c。
#arm-linux-gcc -g hello.c -o hello
2、在Target Board开启gdbserver
#gdbserver  <host-ip>:22 hello   (我的host-ip是118.190.81.4)
gdbserver开始监听22端口（你也可以设其他的值），然后启动hello，你会看到“Process test created：pid＝88”
3、回到Host端
#export PATH=$PATH:/home/cby/arm-gdb/bin（arm-linux-gdb的路径）
#arm-linux-gdb hello
最后一行显示：This GDB was configured as “－－host＝i686－pc－linux－gnu，－－target＝arm－linux”...
说明此gdb在X86的Host上运行，但是调试目标是ARM代码。
(gdb)target remote <target-board-ip>:22   （我的target-board-ip is 118.190.81.4）
注意：你的端口号必须与gdbserver开启的端口号一致，这样才能进行通信。
建立链接后，就可以进行调试了。调试在Host端，跟gdb调试方法相同。注意的是要用“c”来执行命令，不能用“r”。因为程序已经在Target Board上面由gdbserver启动了。结果输出是在Target Board端，用超级终端查看。
安装完后，检查gdb-server的版本：
#：rpm -qa | grep gdb
![](https://img-blog.csdn.net/20170320163317487?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmFueXVuXzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
IT技术交流群：62775887
