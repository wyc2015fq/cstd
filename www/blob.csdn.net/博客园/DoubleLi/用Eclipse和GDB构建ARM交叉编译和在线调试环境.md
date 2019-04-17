# 用Eclipse和GDB构建ARM交叉编译和在线调试环境 - DoubleLi - 博客园







我们在 Linux 主机中搭建我们的开发环境，使用 Ubuntu 10.04 LTS 为例。


## 搭建应用开发环境



### 安装 JRE


Eclipse 依赖于Java 环境，所以必须先安装 JRE 或 JDK。


### 安装 Eclipse 和 CDT


去[Eclipse官网](http://www.eclipse.org/downloads/)下载最新的Eclipse IDE。

我们一般选择 C/C++ 版本（Eclipse IDE for C/C++ Developers），这个版本自带了CDT，不用另行安装CDT插件。下载时选择 Linux 的版本，如：eclipse-cpp-juno-linux-gtk.tar.gz

直接把下载下来的压缩包（eclipse-cpp-juno-linux-gtk.tar.gz）解压到某一目录，运行可执行文件eclipse即可。

运行 eclipse 后，可先进行Eclipse使用环境Workspace的配置。


### 创建交叉应用工程


在 Eclipse 主界面点击菜单 File > New > C/C++ Project，在弹出的“C/C++ Project”对话框中，输入项目名称。选择一个项目类型（如Executable/Empty Project），在 Toolchains 中一定要选 Cross GCC，这是CDT对交叉环境的支持，提供了额外的功能，以方便嵌入式应用程序的开发。

点击下一步，来到“Select Configurations”页面，我们采用默认，直接下一步，来到“Cross GCC Command”的设置。这里我们可以填写交叉编译工具链的交缀和路径，如：
Cross compiler prefix: arm-arago-linux-gnueabi-
Cross compiler path: /data/linux/ti-sdk5/linux-devkit/bin
上例的情况是在我们已经安装好了ARM芯片商提供的交叉编译包。一般来说，芯片商都提供了这样一个交叉编译环境，包括目标平台的编译工具链、Glibc库、二进制工具和其它常用工具。上例是TI提供的DM3730的开发包的安装路径和程序的前缀（真实程序为：arm-arago-linux-gnueabi-gcc、arm-arago-linux-gnueabi-ld等）

最后点击完成，一个交叉项目就创建好了。关于交叉编译前缀和路径的设置，在项目创建后，可以在项目的属性中改变它。

这里我们写一个简单的测试程序，如：
#include <iostream>
using namespace std;

int main(int argc, char ** argv)
{
    signal(0, 0);
    cout<<"This is a message from HelloDm3730!\n";
    return 0;
}
点击构建，对它进行编译。编译后，我们可以在左侧的“Project Explorer”中，项目目录下的Binaries中看到编译出来的程序。把它拷贝到目标ARM开发板上，运行，结果正确。


### 运行调试


程序编译成目标平台的二进制码后，怎么拷贝到目标板上运行调试是个问题。

我们可以通过TFTP把编译好的程序下载到目标板上，即在开发主机安装TFTP服务器，在目标板上使用tftp命令向主机取文件。

另一个比较方便的做法是，在开发主机上部署NFS服务，导出一个共享目录，然后目标板上使用mount命令，把开发主机上的共享目录挂载到本地。这样，开发主机和目标板使用同一个目录，就不用不断下载程序了。NFS的部署可以参照[《Linux下使用NFS共享目录》](http://www.erchashu.com/wiki/linux-nfs-share)。

终极的解决办法是搭建GUI的在线调试环境，即写完代码后，点击调试，Eclipse自动编译程序，然后把程序下载到目标板，然后运行打开GDB在线调试，这样就可以单步调试了，就像调试本地程序一样。


## 搭建在线调试环境



### gdb+gdbserver总体介绍


远程调试环境由宿主机GDB和目标机调试stub共同构成，两者通过串口或TCP连接。使用 GDB标准程串行协议协同工作，实现对目标机上的系统内核和上层应用的监控和调试功能。调试stub是嵌入式系统中的一段代码，作为宿主机GDB和目标机调试程序间的一个媒介而存在。

就目前而言，嵌入式Linux系统中，主要有三种远程调试方法，分别适用于不同场合的调试工作：
- 
普通列表项目用ROM Monitor调试目标机程序

- 
用KGDB调试系统内核

- 
用gdbserver调试用户空间程序


这三种调试方法的区别主要在于，目标机远程调试stub 的存在形式的不同，而其设计思路和实现方法则是大致相同的。

而我们最常用的是调试应用程序。就是采用gdb+gdbserver的方式进行调试。在很多情况下，用户需要对一个应用程序进行反复调试，特别是复杂的程序。采用GDB方法调试，由于嵌入式系统资源有限性，一般不能直接在目标系统上进行调试，通常采用gdb+gdbserver的方式进行调试。


### 源代码下载


嵌入式Linux的GDB调试环境由Host和Target两部分组成，Host端使用arm－linux－gdb，Target Board端使用gdbserver。这样，应用程序在嵌入式目标系统上运行，而gdb调试在Host端，所以要采用远程调试（remote）的方法。进行GDB调试，目标系统必须包括gdbserver程序（在主机上正对硬件平台编译成功后下载到目标机上），宿主机也必须安装GDB 程序。一般Linux发行版中都有一个可以运行的GDB，但开发人员不能直接使用该发行版中的GDB来做远程调试，而要获取GDB的源代码包，针对arm 平台作一个简单配置，重新编译得到相应GDB。GDB的源代码包可以从[GNU官网](http://www.gnu.org/software/gdb/download/)中下载。


### 配置编译及安装


再次强调一次，GDB远程调试套件包括Host端的gdb和Target端的gdbserver，对于gdb，宿主机上发行版本自带的PC版gdb是不能用的，它没有目标架构（ARM）相关的调试支持。所以我们应该使用gdb的源码，针对ARM平台编译一个（toolchain还是Host上的）特别的版本。当然，如果芯片商提供的交叉编译套件中已经包含了arm-linux-gdb，我们就不用重新编译gdb了，直接用它即可。不管怎么说，gdbserver还是需要用目标板的toolchain重新编译一遍，因为芯片商提供的交叉开发套件通常不包括gdbserver。

下载完后，解压：
#cd /opt
#tar xzvf /tmp/gdb-6.8.tar.gz
建立配置文件，编译：
#cd /opt
#mkdir -p arm-gdb/build
#cd arm-gdb/build
#/opt/gdb-6.8/configure --target=arm-linux --prefix=/opt/arm-gdb
#make
#make install
上面命令中，–target配置gdb的目标平台，–prefixp指定了编译结果的存放位置，也就是安装目录。编译完后可以在/opt/arm-gdb/bin目录下找到可执行的arm-linux -gdb, arm-linux -gdbtui, arm-linux-run。 拷贝arm-linux-gdb 到/usr/bin目录：
#cd /opt/arm-gdb/bin/
#cp arm-linux-gdb /usr/bin/
下面把 gdbserver 移植到ARM平台。要点是指定目标平台的交叉编译链（gcc和ar）。我们创建一个临时的编译目录，以避免弄脏原代码。
#cd /home/kim
#mkdir gdb-build
#CC="/data/Linux/ti-sdk5/linux-devkit/bin/arm-arago-linux-gnueabi-gcc" \
  AR=/data/Linux/ti-sdk5/linux-devkit/bin/arm-arago-linux-gnueabi-ar \
  /opt/gdb-6.8/gdb/gdbserver/configure  --target=arm-linux --host=arm-linux
#make
上面命令中，–target=arm-linux表示目标平台，–host表示主机端运行的是arm-linux-gdb，不需要配置—prefix，因为gdbserver不在主机端安装运行。临时环境变量CC和AR用于指定交叉编译和汇编选项，在同一行命令的configure执行时将应用这些选项。

没有错误的话就在/home/kim/gdb-build目录下生成gdbserver可执行文件，注意此时要更改其属性，否则可能会出现无法访问的情况，chmod 777 gdbserver将其更改为任何人都可以读写执行；使用arm-linux-strip命令处理一下gdbserver，将多余的符号信息删除，可让elf文件更精简，通常在应用程序的最后发布时使用；然后把它烧写到flash的根文件系统分区的/usr/bin（在此目录下，系统可以自动找到应用程序，否则必须到gdbserver所在目录下运行之），或通过nfs mount的方式都可以。只要保证gdbserver能在开发板上运行就行。

编译好gdbserver后，把它拷贝到目标板上的 /usr/bin 目录下，运行，如果能显示帮助信息，则交叉编译成功，如：
#gdbserver
Usage:  gdbserver [OPTIONS] COMM PROG [ARGS ...]
        gdbserver [OPTIONS] --attach COMM PID
        gdbserver [OPTIONS] --multi COMM

COMM may either be a tty device (for serial debugging), or 
HOST:PORT to listen for a TCP connection.

Options:
  --debug               Enable general debugging output.
  --remote-debug        Enable remote protocol debugging output.
  --version             Display version information and exit.
  --wrapper WRAPPER --  Run WRAPPER to start new programs.
  --once                Exit after the first connection has closed.
如果提示其它错误信息，如二进制文件无法执行，则表示编译不成功。注意我们交叉编出来的gdbserver是无法在开发主机上运行的。


### 命令行远程调试


在目标板上，运行 gdbserver 命令启动测试程序的调试，并指定目标板的IP和监听端口号，如：
#gdbserver 192.168.188.120:12345 HelloDm3730 （注，HelloDm3730 为要调试的程序）
Process HelloDm3730 created; pid = 625
Listening on port 12345
我们看到gdbserver已经正常启动了，正在等待客户端程序gdb的连接。

此时我们在开发主机上运行 arm-linux-gdb，指定gdbserver的IP和端口，连接上去，如：
# arm-arago-linux-gnueabi-gdb 
(gdb) target remote 192.168.188.120:12345
Remote debugging using 192.168.188.120:12345
0x400b57f0 in ?? ()
显示“0x400b57f0 in ?? ()”表示已经连接到远端的gdbserver并且开始调试了，此时目标板终端会显示“Remote debugging from host 192.168.188.201”，再次确认远程调试连接成功。接下来的调试方法与普通本机的gdb使用相同。


## CDT + GDB 图形化远程调试


我们可以直接使用 arm-linux-gdb 对应用程序进行远程调试，但命令行界面实在是不够友好。这里我们想办法在Eclipse上，通过CDT使用GDB进行远程在线调试。

在Eclipse中选择一个项目，点击菜单 Run > Debug Configurations，在左侧的Dubug类型中找到“C/C++ Remote Application”，右击点“New”，创建一个远程调试配置，如下图：

![](http://www.erchashu.com/wiki/lib/exe/fetch.php?hash=848ec3&w=688&h=484&media=http%3A%2F%2Fwiki.eclipse.org%2Fimages%2Fe%2Fe3%2FAutoRemote.png)

如果安装的CDT插件不全，可能没有“C/C++ Remote Application”这个类型，意味着CDT没有远程调试功能。这时需要安装一个叫“Remote System Explorer End-User Runtime ”的插件，如何安装请参考：[How do I debug a remote application?](http://wiki.eclipse.org/CDT/User/FAQ#How_do_I_debug_a_remote_application.3F)。

Eclipse的C/C++插件CDT已经很好的支持gdb在远程调试了。调试一个应用程序时，CDT有三种运行方式：
- 
Automatic Remote Launcher ：远程自动运行，这是最方便好用的一种方式

- 
Manual Remote Launcher : 远程手动运行。用户自己在目标板上运行gdbserver，然后在开发主机上指定远程连接的方式（如IP地址和端口），连接到gdbserver

- 
Remote Attach Launcher ：远程依附运行。类似于上一种，但它不是重新运行程序开启一个debug会话，而是直接Attach到一个已经运行的程序，然后调试


在Debug Configurations 对话框中，创建一个远程调试配置，这个配置在创建时会根据项目情况提供一个默认的配置，默认将使用第一种Automatic Remote Launcher方式，这在Main标签中下方“GDB (DSF) Automatic Remote Debugging Launcher”可以看出，点击右边的“Select other…”可以切换其它方式。


### 远程手动运行方式


我们希望Eclipse每次生成一个项目之后，自动把生成出来的二进制程序拷贝到目标板上，这可以通过NFS挂载共享目录来实现，我们只需要配置项目属性（依次展开：C/C++Build > Settings > Build Steps > Post-build steps，在Command中输入“cp ProgramBin /mnt/share”）即可。

接下来配置CDT的Debug选项，步骤如下：
- 
选中项目→菜单栏 ”Run“→Debug Configurations…

- 
双击 C/C++ Remote Application 新建一个配置，Eclipse会根据当前选择的项目初始化大部分配置，这里只需修改Debugger配置页

- 
在右下方点击“Select other”，选择“GDB(DSF) Manual Remote Debugging Launcher”，确认

- 
选择进入Debugger配置页，在Main标签中，GDB debugger 填写 arm-linux-gdb，如果未加入PATH环境变量则应该填入绝对路径

- 
在Debugger配置页的Shared Libraries标签中，可以添加库路径，比如调试过程中要步入外部函数，就必须在这里给出带调试信息的库文件路径，否则会找不到该函数的定义

- 
在Debugger配置页的Connection标签中，Type选“TCP”，并填写目标板上gdbserver监听的IP和端口号（这个在下面文档会提及）

- 
所有配置完成后，点“Apply”保存配置，并关掉配置窗口


接下来在目标板上运行 gdbserver，假如测试程序HelloDm3730已经下载到当前目录，如：
#gdbserver 192.168.188.120:12345 HelloDm3730
Process HelloDm3730 created; pid = 625
Listening on port 12345
目标板的 GDB 服务开启后，我们就可以在开发主机中，点击Eclipse的调试按钮（指定调试配置为刚才配置好的），开始应用程序的远程调试。我们在连接目标板的终端中，可以看到程序的标准输出；也可以在这里标准输入。

当我们结束调试后，目标板上的gdbserver进程会自动退出。如果我们在修改代码后想再次调试，仍然需要在目标板上运行gdbserver，这样会很麻烦，但毕竟可以进行远程的图形化调试了。


### 远程自动运行调试


我们希望在开发主机上查看目标板（远端主机）的系统状况，如查看远程的文件，查看进程列表等，甚至打开终端运行远程机器上的程序。我们还希望这样一个工具软件能集成在Eclipse上，并且提供了API接口让Eclipse的插件能调用远程机器上的程序。

TM/RSE 就是这样一个工具，全称是 Target Management / Remote Systems Explorer，是Eclipse上的一个插件。Eclipse的远程管理插件有很多，但这个最常用，所以我们安装了 Eclipse 的C++版，就自带了这个插件。ARM的软件开发环境DS-5 也使用了这个插件，其实整个DS-5的基础都是Eclipse社区的开源软件搭建起来的。关于TM/RSE的更多介绍，可参考其官方资料：[TM and RSE FAQ](http://wiki.eclipse.org/TM_and_RSE_FAQ)

我们可以在安装了TM/RSE的Eclipse上，点击右上角的“Open Perspective”按钮，打开“Remote System Explorer”，在 Remote System Explorer 上，你可以很方便的管理多个远程连接，查看远程文件和进程，运行远程程序等。

RSE 支持很多网络连接类型，常用的有：
- 
FTP/sFTP

- 
SSH

- 
Telnet


支持连接到Unix/Linux/Windows主机和Local（本地）。要在开发主机上连接远程机器，必须先在远程机器上安装并启动某一远程控制服务，如 Telnet 服务、FTP服务、SSH服务。如果远程主机安装了文件服务（如FTP、SFTP）和Shell服务（如SSH、Telnet），那么可以在本地开发主机上，CDT可以调用RSE的API接口拷贝生成的目标程序到远端，并在远端目标板上运行gdbserver，从而做到自动下载和调试程序的功能。

CDT调用RSE的API接口实现远程调试的过程可分解为：
- 
通过文件服务接口下载编译的程序到目标板（可用的文件服务有：FTP、SFTP）

- 
通过Shell运行目标板上的gdbserver加载下载的程序（可用的Shell服务有：SSH、Telnet）


上面的第一步不是必需的，我们可以通过其它手段来实现，如在目标板上mount一个NFS文件系统，让开发主机（也挂载了同一个NFT文件系统）在编译后执行一条拷贝命令即可，这可以在项目的编译选项中配置。需要注意的是，如果想忽略这一步骤，还要在项目的Debug Configuration 中，勾选“Skip download to target path”，这样CDT才不会每次启动调试时都尝试打开文件服务接口。

为了方便，我们还是希望让目标板同时提供文件服务和Shell服务。我们有两种选择：
- 
在目标板上部署 OpenSSH。OpenSSH 本身带了SSH服务和SFTP服务，所以一个软件包可以搞定。缺点是编译出来的文件很大

- 
在目标板上部署 Dropbear 和 vsFTP（或者其它FTP服务器）


下面将分别介绍这两种方案。


#### 方案一：编译和部署OpenSSH


请参考：[《Openssh服务器移植到2440处理器上》](http://wenku.baidu.com/view/9f05216eaf1ffc4ffe47ac47.html)


#### 方案二：编译和部署Dropbear 和 vsFTP


因为目标板的资源有限，在目标板上部署Telnet/FTP/SSH服务有点麻烦。好在Linux社区永远是开放的，我们有太多选择，如SSH服务可使用DropBear或OpenSSH，FTP可使用vsftpd等。如果我们的目标板已经在这些当中的某一服务了，那好办，直接连接即可；如果目标板上没有这些服务，我们还得自己编译和部署——用交叉编译器编译出目标平台的程序，然后部署和配置它。这里我们使用Dropbear来部署SSH服务。Dropbear 依赖 zlib 库，所以如果目标板上没有 libz.so，还得自己编译。

先在网上下载最新的 zlib 和 dropbear。我下载的版本是：dropbear-2012.55.tar.gz 和 zlib-1.2.7.tar.gz 。

编译的基本流程是：解压，配置，编译，安装。我们可以在./configure时就指定交叉编译工具。

对于 zlib，我们指定安装路径为~/soft/install，这样make install后，make就会把编译后的程序拷贝安装到这个目录，方便我们取程序。如：
#tar -xzf zlib-1.2.7.tar.gz
#cd zlib-1.2.7
#CC="/data/Linux/ti-sdk5/linux-devkit/bin/arm-arago-linux-gnueabi-gcc"   \
 AR=/data/Linux/ti-sdk5/linux-devkit/bin/arm-arago-linux-gnueabi-ar  \
  ./configure --shared --prefix=~/soft/install/
#make
#make install
“安装”后，可在我们指定的目录~/soft/install/的lib下找到共享库文件（so），我们要用到的是：libz.so libz.so.1 libz.so.1.2.7

对于 Dropbear ，我们需要多指定ranlib和strip，并且要指定目标平台为arm-linux。安装路径要使用绝对路径，虽然我们不安装。如：
#tar -xzf dropbear-2012.55.tar.gz
#cd dropbear-2012.55
#CC="/data/Linux/ti-sdk5/linux-devkit/bin/arm-arago-linux-gnueabi-gcc"   \
 AR=/data/Linux/ti-sdk5/linux-devkit/bin/arm-arago-linux-gnueabi-ar  \
 RANLIB="/data/Linux/ti-sdk5/linux-devkit/bin/arm-arago-linux-gnueabi-ranlib" \
 STRIP="/data/Linux/ti-sdk5/linux-devkit/bin/arm-arago-linux-gnueabi-strip" \
 ./configure --prefix=/home/kim/soft/dropbear-bin/ --host arm-linux
#make
编译后，在当前目录可以看到我们需要的程序：
dropbear： ssh2 server 
dropbearkey: 密钥生成器 
dropbearconvert: 可以转换openssh的密钥
dbclient: ssh2 client 
把zlib的三个so（libz.so libz.so.1 libz.so.1.2.7，前两个是连接文件）拷贝到目标板上的 /usr/lib；把dropbear生成的四个程序拷贝到目标板的 /usr/sbin 目录下。

然后（在目标板上）建立配置文件，生成SSH需要的密钥，如：
#mkdir /etc/dropbear 
#cd dropbear 
#dropbearkey -t rsa -f dropbear_rsa_host_key
#dropbearkey -t dss -f dropbear_dss_host_key
最后启动服务：
#dropbear -E
[746] Jan 01 03:30:41 Running in background （提示在后台运行了）
最好为目标板的用户建立密码。然后在开发机上，先测试一下我们的SSH服务：
#ssh root@192.168.188.55
root@192.168.188.55's password: （这里输入密码）
如果输入正确的密码，但总是登录不成功（在目标板上的终端提示：user 'root' has invalid shell, rejected），可能是由于 root 帐号没有设置正确的登录shell。各大多数程序一样，Dropbear 只允许 /etc/passwd 中列举的用户登录，并且在/etc/passwd中该用户需要设置正确的shell（/bin/sh 或 /bin/csh）。有些busybox用的是/bin/bash，所以被Dropbear拒绝了。这里我们使用 /bin/sh 以后，就可以正常登录了。

vsFTP 暂时没有编译成功，它没有提供 configure 工具。


#### 使用CDT远程调试


现在回过头来看CDT的远程调试。假设我们已经完成了以下工作：
- 
在远端部署了SSH或Tenlet服务（Telnet 在当前版本的RSE属于试验性质，远程调试没调通，不知是什么原因）

- 
在远端部署了某一个文件服务器（如SFTP或vsFTP）（这项没有也行，但调试会稍微麻烦点）

- 
本地RSE能访问远端功能（如Shell）

- 
在远端部署了 gdbserver，并存放在/usr/bin 中

- 
远端运行 gdbserver 时，开发主机能用 gdb (命令行)连接过去


我们应该先在Remote System Explorer上创建连接，让RSE能连接到目标机器和管理目标机器。然后在项目的Debug Configurations中，创建“C/C++ Remote Application”的一个配置，在Main标签中，Connection选择RSE上建立的连接，然后输入远端的程序存放的路径，这个路径是包含最终的程序文件名的，不是目录。注意：如果在远端没有部署文件服务（SFTP或FTP），那么还需要勾选“Skip download to target path”，这样CDT才不会每次启动调试时都尝试打开文件服务接口，造成调试进行不下去。但勾选“Skip download to target path”了后，必须有途径把最新的程序拷贝到刚才指定的远端程序路径，如每次手动拷贝，或者在项目配置中，Post-build steps 中输入自动拷贝命令，让项目每次编译后自动拷贝到目标机器。


## 参考

- 
[Eclipse-cdt 配合 gdbserver 进行 arm 程序远程调试 上](http://bashell.sinaapp.com/archives/eclipse-cdt-gdbserver-arm-remote-debugging-1.html)

- 
[Eclipse-cdt 配合 gdbserver 进行 arm 程序远程调试 下](http://bashell.sinaapp.com/archives/eclipse-cdt-gdbserver-arm-remote-debugging-2.html)

- 
[How do I debug a remote application?](http://wiki.eclipse.org/CDT/User/FAQ#How_do_I_debug_a_remote_application.3F)

- 
[gdb+gdbserver方式进行ARM程序调试](http://blog.sina.com.cn/s/blog_45ef163d0100dx83.html)

- 
[Remote System Explorer之DropBear移植](http://blog.csdn.net/ce123/article/details/6613850)

- 
[Remote System Explorer之FTP移植(vsftpd)](http://blog.csdn.net/ce123/article/details/6613976)










   最精在[研究](http://www.haogongju.net/tag/%E7%A0%94%E7%A9%B6)u-boot，之前用的编译[方法](http://www.haogongju.net/tag/%E6%96%B9%E6%B3%95)是在[window](http://www.haogongju.net/tag/window)s下用vs进行就该，然后用[虚拟机](http://www.haogongju.net/tag/%E8%99%9A%E6%8B%9F%E6%9C%BA)里的ubuntu进行编译，中间使用[共享](http://www.haogongju.net/tag/%E5%85%B1%E4%BA%AB)[文件](http://www.haogongju.net/tag/%E6%96%87%E4%BB%B6)夹。后来觉得太麻烦了，干脆直接在[linux](http://www.haogongju.net/tag/linux)下进行[修改](http://www.haogongju.net/tag/%E4%BF%AE%E6%94%B9)和编译，因此安装了[Eclipse](http://www.haogongju.net/tag/Eclipse)[for](http://www.haogongju.net/tag/for) c/c++ developer。下面介绍两种交叉编译环境搭建的方法：



一.使用工程中自带的Makefile。

　　1.[选择](http://www.haogongju.net/tag/%E9%80%89%E6%8B%A9)New->Project，而不是Import

![](https://pic002.cnblogs.com/images/2011/311944/2011113022125220.png)

　　2.再选择Makefile Project with Exist[in](http://www.haogongju.net/tag/in)g Code

![](https://pic002.cnblogs.com/images/2011/311944/2011113022134288.png)

　　3.之后再在Project Name中填入工程名称

![](https://pic002.cnblogs.com/images/2011/311944/2011113022265447.png)

　　4.最后直接Build All或者Build Project即可

![](https://pic002.cnblogs.com/images/2011/311944/2011113022141965.png)

二.采用[IDE](http://www.haogongju.net/tag/IDE)自动生成Makefile

　　1.同样选择New Project，[不过](http://www.haogongju.net/tag/%E4%B8%8D%E8%BF%87)之后选择C Project而不是Makefile Project with Existing Code

![](https://pic002.cnblogs.com/images/2011/311944/2011113022195770.png)

　　2.这里[输入](http://www.haogongju.net/tag/%E8%BE%93%E5%85%A5)工程名称，[保证](http://www.haogongju.net/tag/%E4%BF%9D%E8%AF%81)跟workspace里的工程文件夹名称相同。在下面Project type里面选择Cross-Compile Project

![](https://pic002.cnblogs.com/images/2011/311944/2011113022210673.png)

　　3.之后右键工程选择Properties

![](https://pic002.cnblogs.com/images/2011/311944/2011113022230127.png)

　　4.选择C/C++ Build->Settings。在[Too](http://www.haogongju.net/tag/Too)l Settings[标签](http://www.haogongju.net/tag/%E6%A0%87%E7%AD%BE)栏里面选择Cross Settings。之后在Prefix里面填入形如powerpc-linux-gnu-的前缀，再在Path里面填上Tool Chain的[路径](http://www.haogongju.net/tag/%E8%B7%AF%E5%BE%84)，就OK了！

![](https://pic002.cnblogs.com/images/2011/311944/2011113022234928.png)

OK这样就[可以](http://www.haogongju.net/tag/%E5%8F%AF%E4%BB%A5)在Eclipse这个强大的IDE下进行交叉编译了。





经过一段时间的摸索，终于把linux下搭建基于Eclipse的arm的开发环境给搭建起来了，做下笔记，以便日后查看。

0. 用的linux版本是Ubuntu 9.04

    用的板子是FriendlyARM s3c2440

1. 准备好相应的软件包：

    arm-linux-gcc-3.4.1.tar.bz2（网上现成的交叉编译工具，有兴趣的话也可以自己重头编译）

                                 地址：[ftp://ftp.handhelds.org/projects/toolchain/arm-linux-gcc-3.4.1.tar.bz2](ftp://ftp.handhelds.org/projects/toolchain/arm-linux-gcc-3.4.1.tar.bz2)

    jdk-6u21-linux-i586.bin（其实这里jre应该也够了，装个jdk以备以后开发java程序需要）

                                 地址：[http://www.oracle.com/us/sun/index.html](http://www.oracle.com/us/sun/index.html)

    eclipse-cpp-helios-linux-gtk.tar.gz

                                 地址：[http://www.eclipse.org/downloads/](http://www.eclipse.org/downloads/)



2. 首先建立交叉编译环境

    (1) 将arm-linux-gcc的压缩包arm-linux-gcc-2.95.3.tar.gz解压缩；eclipse-cpp-helios-linux-gtk.tar.gzeclipse-cpp-helios-linux-gtk.tar.gz

         $ sudo tar vxjf arm-linux-gcc-3.4.1.tar.bz2

    (2) 将路径usr/local下的arm目录拷贝到你自己系统的/usr/local下；

         cd ./usr/local
         $ sudo mv arm /usr/local

    (3) 修改环境变量，把交叉编译器的路径加入到PATH。（有三种方法，强烈推荐使用方法一，我也不知道为啥，网上说的）

         方法一：修改/etc/bash.bashrc文件
               $ sudo vi /etc/bash.bashrc    在最后加上：
                export PATH＝$PATH:/usr/local/arm/3.4.1/bin（注意，这里的等号前后不能有空格，下同）

                export PATH
　 　 方法二：修改/etc/profile文件：（经测试发现，方法二必须重新启动，使用source的话换个终端就无效了）
               $ sudo  vi /etc/profile        增加路径设置，在末尾添加如下：
                export PATH＝$PATH:/usr/local/arm/3.4.1/bin
　　   方法三：#export PATH＝$PATH:/usr/local/arm/3.4.1/bin   注：(这只能在当前的终端下才是有效的！)

         总结：在这里我们还是用第二种方式吧，第一种方式Eclipse识别不出来的  -- 2010-10-10 9:47

    (4) 保存对profile的修改后，执行source /etc/bash.bashrc 就OK了，该命令能立即使新的环境变量生效，不用重启电脑； 

          用$ echo $PATH 检查是否将路径加入到PATH

    (5) 测试是否安装成功：$ arm-linux-gcc -v



3. 安装Eclipse运行所需的jre，这里以jdk替代

    (1) 安装jdk-6u21-linux-i586.bin





**[c-sharp]**[view plain](http://blog.csdn.net/signsmile/article/details/5930846)[copy](http://blog.csdn.net/signsmile/article/details/5930846)





- $ cd /usr/local/lib   
- $ sudo cp /home/jdk-6u4-linux-i586.bin ./   
- $ sudo chmod +x jdk-6u4-linux-i586.bin   
- $ sudo ./jdk-6u4-linux-i586.bin   




    (2) 设置环境变量

         在/etc/profile中加入如下的内容：

         (由上面可知这里用的是第一种方法，其实我暂时也没搞明白具体区别在哪里，姑且这样这着吧)




**[c-sharp]**[view plain](http://blog.csdn.net/signsmile/article/details/5930846)[copy](http://blog.csdn.net/signsmile/article/details/5930846)





- JAVA_HOME=/usr/local/lib/jdk1.6.0_21   
- JRE_HOME=/usr/local/lib/jdk1.6.0_21/jre   
- CLASSPATH=.:$JAVA_HOME/lib:$JRE_HOME/lib   
- PATH=$PATH:$JAVA_HOME/bin  
- export JAVA_HOME JRE_HOME CLASSPATH   




    (3) source /etc/profile

    (4) java -version



4. 安装Eclipse

    (1) 解压Eclipse





**[c-sharp]**[view plain](http://blog.csdn.net/signsmile/article/details/5930846)[copy](http://blog.csdn.net/signsmile/article/details/5930846)





- sudo tar zxvf eclipse-cpp-helios-linux-gtk.tar.gz -C /usr/local/dev   




    (2) 把Eclipse的编译器配置成arm-linux-gcc

          解压完以后我们就能在相应的目录找到Eclipse文件了，双击打开运行。

          新建一个工程以后选择工程的Properties，在弹出的页面上进行如下图设置：

![](http://hi.csdn.net/attachment/201010/9/0_1286646672E8FL.gif)

          加了一个名为arm的编译方式以后，将它下面GCC C Compiler和GCC C Linker的Command改成arm-linux-gcc，然后就万事OK了，你现在就可以通过Eclipse利用arm-linux-gcc来进行交叉编译了。

![](http://hi.csdn.net/attachment/201010/9/0_12866468862GSY.gif)



5.PC开启NFS与开发板链接

      PC上编译好的东西必须拿到板子上才能运行，拿U盘拷来拷去好像有点不太现实，我们利用NFS挂载就可以轻松解决这个问题。

   (1) 安装

      在Ubuntu系统下，使用NFS需要首先安装以下NFS的软件包

      服务器端 ： nfs-common、nfs-kernel-server、portmap；

      客户端：nfs-common、portmap；

      在服务器端安装软件包的命令式：$sudo apt-get install nfs-kernel-server

       （安装nfs-kernel-server时，apt会自动安装nfs-common和portmap）

      目标板上的Linux系统由Linux内核和busybox共同提供对NFS的支持，不必安装其他的NFS客户端。

   (2) 配置

      NFS的配置文件是/etc/exports。

      在该文件后面加一行：/home/xxx/workspace *(rw,no_root_squash)  使目录/home/xxx/workspace允许所有的IP以读写的权限来访问。

   (3) 使用

      使用之前必需开启NFS服务：$sudo /etc/init.d/nfs-kernel-server start

      当然，使用之后可以关闭NFS服务：$sudo /etc/init.d/nfs-kernel-server stop

      首先在Linux服务器上进行NFS服务器的回环测试，验证共享目录能否被访问。

       $ mount -t nfs 192.168.1.1:/home/xxx/workspace /mnt

       $ ls /mnt

      启动开发板后在目标系统的Linux Shell下

       $ mount -t nfs 192.168.1.1:/home/xxx/workspace /mnt/nfs -o nolock

       $ ls /mnt/nfs

      如果成功开发板就可以直接运行主机上的程序了。












