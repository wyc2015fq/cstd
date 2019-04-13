
# 利用eclipse + qemu| Vmware + gdb 调试linux内核 - 3-Number - CSDN博客


置顶2017年09月08日 09:40:31[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：787标签：[qemu																](https://so.csdn.net/so/search/s.do?q=qemu&t=blog)[eclipse																](https://so.csdn.net/so/search/s.do?q=eclipse&t=blog)[vmware																](https://so.csdn.net/so/search/s.do?q=vmware&t=blog)[调式linux内核																](https://so.csdn.net/so/search/s.do?q=调式linux内核&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=vmware&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=eclipse&t=blog)个人分类：[Linux 网络协议栈																](https://blog.csdn.net/pangyemeng/article/category/7130138)[linux																](https://blog.csdn.net/pangyemeng/article/category/6411338)[
							](https://blog.csdn.net/pangyemeng/article/category/7130138)
[
				](https://so.csdn.net/so/search/s.do?q=eclipse&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=eclipse&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=qemu&t=blog)

# 0x01 缘由
最近想调试linux内核协议栈，想单步调试，所以折腾了几天，利用qemu + eclpise去调试相关。
# 0x02 内核初始化调试环境
centos 6.5 内核版本 2.6.32-431.el6.i686
qemu-0.11.0
eclipse cdt
1.首先我们要从www.kernel.org下载内核源码，在这里我选择的是 linux-2.6.32.tar.bz2。我将其下载到我的主目录下，然后在terminal下输入以下命令。
$ cd (回到主目录)
$ tar xf linux-2.6.32.tar.bz2 (解压源码)
$ mkdir linux-2.6.32-obj （创建一个编译内核的目标文件输出目录）
$ cd linux-2.6.32 (进入内核源码树根目录)
$ make O=~/linux-2.6.32-obj menuconfig (这里我们要配置内核，并在~/linux-2.6.32-obj目录下生成内核配置文件.config)
$ make mrproper

2. 接下来我们打开elicpse，第一次打开时有一个欢迎画面，
我们单击右边的workbench图片关掉欢迎画面。由于eclipse cdt是一个非常强大的c/c++ ide，它默认会自动的解析工程中源程序并编译工程和产生智能提示信息。但由于我们调试内核过程中暂不会用到这些功能，所以要关闭他们。
首先我们到Window->Preferences->General->Workspace 中将Build Automatically选项去掉。
然后到Window->Preferences->C/C++ -> Indexer中，将默认的Fast c/c++ indexer改为No indexer。
然后我们开始创建一个新的工程。
从菜单中选择File -> New -> Project... -> C/C++ -> C Project 然后单击Next按钮。
这时出现一个对话框，在Project Name 一栏填入一个工程名，比如 linux-kernel. 然后将 Use default location 选项给勾掉。在Location一栏填入刚才解压的内核源码的路径。Project type选择Makefile project -> Empty project。Toolchains
 选择 Linux GCC.最终结果如下图所示：
![](https://img-blog.csdn.net/20170908093352540?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

然后单击Next按钮，出现下一个对话框。这时候因为工程的默认配置还不完全符合我们的要求。所以我们单击 Advanced Settings 按钮会打开一个工程配置对话框。我们选择左侧的 C/C++ Build 选项，在相应的右侧页面里我们将Builder Settings 选项页下的
 Use default build command 勾选掉，然后指定一个新的Build command 为 make O=/home/xxx/linux-2.6.32-obj.然后单击Build location中的File system...按钮更改obj文件的输出目录为/home/xxx/linux-2.6.32-obj. 最终结果如下图所示：
![](https://img-blog.csdn.net/20170908093410655?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后单击Behaviour选项页，将build(Incremental Build)中的all更改为一个空格，将下面的clean更改为mrproper（这是默认清除内核obj文件的命令目标）。弄好后如下图所示：
![](https://img-blog.csdn.net/20170908093427305?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

这个时候你就可以编译内核了。Ctrl ＋ B 开始编译内核。
编译完内核。在/home/xxx/linux-2.6.32-obj/arch/x86/boot/下将生成一个bzImage的大内核镜像。
此时我们在xterm下运行qemu虚拟机，命令行如下：
$ qemu -s -S -kernel /home/xxx/linux-2.6.32-obj/arch/x86/boot/bzImage -append root=/dev/hda
我解释一下这个命令行上的参数。
-s 选项表示我们运行虚拟机时将1234端口开启成调试端口，供eclipse网络调试时使用
-S 选项表示我们启动虚拟机时要“冻住”虚拟机，等待调试器发出继续运行的命令。
-kernel /home/xxx/linux-2.6.32-obj/arch/x86/boot/bzImage 表示我们要调试的内核镜像
-append root=/dev/hda 表示我们要传递给内核的参数（这里你可能感觉有点奇怪为啥是/dev/hda而不是/dev/hda1或是/dev/hda2呢？因为这个镜像仅仅是一个分区的二进制镜像，不是整个硬盘的镜像）
启动后，你会看到屏幕黑黑的啥也没有。因为我们指定了-S参数把虚拟机给“冻”住了。
继续回到eclipse中，这时候我们在菜单中选择Run -> Debug Configurations ，这时候打开一个配置页面。然后我们在C/C++ Application上双击鼠标，这时在右侧的页面弹出了debug配置选项，然后我们在project一栏选择 linux-kernel ,在下面的C/C++
 Application中填入/home/xxx/linux-2.6.32-obj/vmlinux,如下图所示：
![](https://img-blog.csdn.net/20170908093446470?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后我们单击Debugger选项页，将Debugger改为 GdbServer Debugger . 将Stop on startup at 改为start_kernel,将Connection子选项页下的Type改为TCP将Port number改为1234，最终如下图所示：
![](https://img-blog.csdn.net/20170908093502488?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后单击Debug按钮开始调试！这时候Eclipse会提示我们切换 Perspective ，选择是，不再提示。
这时候就开始调试内核，而且是源码级的调试！如下图：
![](https://img-blog.csdn.net/20170908093519285?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

设置相关断点，可以浏览相关内核初始化过程，下面截取相关图：
![](https://img-blog.csdn.net/20170908093534631?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170908093633075?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

# 0x03 通过qemu调试tcp/ip协议栈
# (此方案失败了)
本来是想复习下linux内核tcp/ip相关内容，但是以上方法对与调试来说无法单步进行，所以再到网上收集了相关资料。
实验环境：
Linux dev 2.6.32-431.el6.x86_64 \#1 SMP Fri Nov 22 03:15:09 UTC 2013 x86_64 x86_64 x86_64 GNU/Linu
默认安装qemu-0.11.0.tar.gz:
tar xzvf
yum install zlib-devel -y
yum install SDL* -y
./configure && make && make install
编译linux内核：
tar xvf linux-2.6.32.tar.bz2
yum install ncurses-devel -y
类似上面的编译方式：
编译busybox-1.21.1.tar.bz2：
tar xvf busybox-1.21.1.tar.bz2
make menuconfig
![](https://img-blog.csdn.net/20170908093708666?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
make & make install

制作rootfs:
\#!/bin/sh
mkdir -p /home/KERNEL/BUSYBOX
mkdir -p /home/KERNEL/LINUX

cp -r /home/busybox-1.21.1/_install /home/KERNEL/BUSYBOX

BUSYBOX=/home/KERNEL/BUSYBOX
KERNEL==/home/KERNEL

cd $BUSYBOX/_install
mkdir -pv proc sys dev etc etc/init.d
cat << EOF > etc/init.d/rcS
\#!/bin/sh
mount -t proc none /proc
mount -t sysfs none /sys
/sbin/mdev -s
EOF

chmod 777 ./etc/init.d/rcS
cd -

\#create cpio img
cd $BUSYBOX/_install
find . | cpio -o --format=newc > $KERNEL/rootfs.img
cd -

\#create zip img
cd $KERNEL
gzip -c rootfs.img > rootfs.img.g

运行测试：
qemu-system-x86_64 -kernel LINUX/bzImage -initrd rootfs.img.gz -append "root=/dev/ram rdinit=sbin/init noapic console=tty0
" -m 256 -k en-us
运行结果：
![](https://img-blog.csdn.net/20170908093744997?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGFuZ3llbWVuZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
有初始化过程，而仅仅是可以实现了简单命令交互和文件系统的无内核裸机但是无网络设备，咋调linux tcp/ip协议栈！！
继续捣鼓：

# 0x04相关问题
编译linux内核报错：
报错：
rivers/net/igbvf/igbvf.h:129:15: error: duplicate member ‘page’
make[3]: *** [drivers/net/igbvf/ethtool.o] Error 1
make[2]: *** [drivers/net/igbvf] Error 2
make[1]: *** [drivers/net] Error 2
make: *** [drivers] Error
解决办法：
1、方法一
根据linux社区的建议，此错误是由于gcc版本与内核版本的冲突导致的。他们的建议是更换新版本的内核，但是某些特殊条件下，我们不能更换内核版本，于是我们修改内核代码适应当前的编译器（即接下来的方法二）。
2、方法二
a、按照错误的指示，错误的代码是在drivers/net/igbvf/igbvf.h文件的第129行。
b、打开文件，看129行，代码为：struct page *page;再往上看，第124行，也有struct page *page;这行代码，这个结构定义在内部的一个结构体中。就是他的名字与129行的重复了，而4.6.3的编译器对不支持这种方式的定义，我们修改129行的代码为struct page *pagep;保存退出。
接下来重新编译，编译通过。
报错：
gcc: error: elf_i386: No such file or directory
make[1]: *** [arch/x86/vdso/vdso32-int80.so.dbg] Error 1
make: *** [arch/x86/vdso] Error 2
解决办法：
1.在内核目录arch/x86/vdso/Makefile中，大约在28,29行 找到 VDSO_LDFLAGS_vdso.lds = -m elf_x86_64 -Wl,-soname=linux-vdso.so.1 \   -Wl,-z,max-page-size=4096 -Wl,-z,common-page-size=4096 把"-m elf_x86_64" 替换为 "-m64"
2然后再继续找，大约在72行左右，找到VDSO_LDFLAGS_vdso32.lds = -m elf_i386 -Wl,-soname=linux-gate.so.1中的 "-m elf_i386" 替换为 "-m32"
然后继续编
报错：
Unable to find the ncurses libraries
解决办法：
CentOS：
yum install -y ncurses-devel
Ubuntu：
sudo apt-get insatll ncurses-dev
报错：
/usr/bin/ld: cannot find -lcrypt
collect2: ld returned 1 exit status

解决办法：
yum install glibc-static -y

报错：
qemu界面无法出来
解决：
yum install DSL* -y

报错：
键盘和普通键盘映射不对
解决办法：
添加 -k en-us -sdl
# 0x05 采用VMware + gdb调式Linux内核（一条路走不通，找其他路子。。。）
步骤如下：
1、安装host虚拟机：vmware10.0.7和Centos 6.5 x86_64 的安装；
2、安装guest虚拟机：vmware 12.5.0和 Centos 6.2 x86；
3、在guest虚拟机编译linux内核源码linux 2.6.32并安装到guest主机，替换原来的;
4、将编译的文件拷贝到host虚拟机的相关目录；
5、设置虚拟机配置：/root/vmware/CentOS/CentOS.vm 添加debugStub.listen.guest32 = "1"；**此步骤一定要将guest虚拟机关闭，否则设置不成功；**
6、vmware和gdb主机的配置
详细步骤：
参照：[http://blog.csdn.net/qiangchen99/article/details/43169615](http://blog.csdn.net/qiangchen99/article/details/43169615)
# 0x06 总结
后续可以根据方法二，浏览和单独linux内核的源码。此次环境搭建耗时较多，一定要记住，详细的逻辑推理才能提高效率。

