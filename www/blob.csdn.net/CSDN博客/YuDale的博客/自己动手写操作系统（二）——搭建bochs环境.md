# 自己动手写操作系统（二）——搭建bochs环境 - YuDale的博客 - CSDN博客
2017年03月30日 21:02:48[YuDale](https://me.csdn.net/YuDale)阅读数：3842
# 搭建第三章pmtest1.com的bochs运行环境
## 一、前言
本来想跳过这本书的第二章直接进入第三章的，但是我还是too young too simply。第三章一开始讲的是实模式到保护模式的转换，所以什么是实模式，什么是保护模式，这些东西有啥子用？带着一大堆问题，翻来覆去的看了十几遍书，在网上看了许多的博客终于有些模糊的认识了，结果代码一敲，然后...这代码要怎么运行，我要怎样才能看到这个代码的效果呢？！
这里bochs的重要性就突显出来了。首先看到书上面的一段话（这段话在2.5小节里面）：我们要写的是基于x86平台的32位操作系统，这需要我们队保护模式有非常好的了解。而学习保护模式时编写的代码需要依赖运行于实模式的DOS，而不是V86模式下的DOS。这段话告诉我们一个信息，第三章那该死的代码是要在DOS上面运行的！但是ubuntu是没有DOS的！所以我搜了几天的“在ubuntu/linux下运行pmtest1”根本就毫无意义。那么为了第三章能顺利的学习下去，但是又不想再安装一个VirtualPC，我决定在ubuntu内再装一个bochs。
首先，对bochs的理解，它是一款x86的模拟器。不过bochs它很有意思，它的界面是这样的，作为一款开源软件，感觉就是和商业软件那种妖艳的贱货不一样。而且在书上和众多博客上面都强烈的称赞了它的调试功能（虽然我现在并不会用，但是以后应该会慢慢接触到的）。
![](https://img-blog.csdn.net/20170330195748540?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvWXVEYWxl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
如此清新脱俗的模拟器，是不是很让人心动呢？现在我们开始将它收入囊中。（以下都是在权限模式下的命令）
## 二、bochs的安装
1.去http://nchc.dl.sourceforge.net/project/bochs/bochs/2.4.5/bochs-2.4.5.tar.gz 下载【bochs-2.4.5.tar.gz】
2.解压下载的源代码：
tar zxvf bochs-2.4.5.tar.gz
3.进入bochs-2.4.5目录，运行configure脚本：
./configure –enable-debugger –enable-disasm
（–enable-debugger和 –enable-disasm 表示开启调试和反汇编功能）
4.第3步正确运行后，会产生一个Makefile文件，于是可以编译：
make
5.安装：
make install
好了，我们已经把bochs安装完了，但是说着容易做着难，在安装过程中你会遇到许许多多，形形色色，奇奇怪怪的问题。bochs是基于C++的，所以在安装之前我们需要安装相关的依赖环境，也就是说在我们安装之前还要装一些东西。
1.apt-get install build-essential
2.apt-get install xorg-dev
3.apt-get install libgtk2.0-dev
4.apt-get install bison
到此为止，我们的安装还是不会很顺利。好了现在我们把一些出现的问题大致梳理一下（每次执行命令时候都会有一大串提示，只要看后面有没有报错就行）：
1.error: C++ preprocessor “/lib/cpp” fails sanity check
因为Bochs是用C++写的，所以这里要安装GNU g++编译器。
解决方法：
apt-get install build-essential
apt-get install g++
2.checking for default gui on this platform… x11
ERROR: X windows gui was selected, but X windows libraries were not found.
解决方法：
apt-get install xorg-dev
3.ERROR: pkg-config was not found, or unable to access the gtk+-2.0 package.
解决方法：
apt-get install libgtk2.0-dev
4.make的时候提示 
/usr/bin/ld: gui/libgui.a(gtk_enh_dbg_osdep.o): undefined
reference to symbol pthread_create@@GLIBC_2.1 //
lib/i386-linux-gnu/libpthread.so.0: error adding symbols: DSO missing
from command line collect2: error: ld r
解决方法1：修改"Makefile.in" 92 和 93行 为 "CC = @CC@ -pthread" and "CXX = @CXX@ -pthread"
解决方法2：在Makefile.in中183行
bochs@EXE@: @IODEV_LIB_VAR@ @DEBUGGER_VAR@ \
cpu/libcpu.a memory/libmemory.a gui/libgui.a \
@DISASM_VAR@ @INSTRUMENT_VAR@ $(BX_OBJS) \
$(SIMX86_OBJS) @FPU_VAR@ @GDBSTUB_VAR@ @PLUGIN_VAR@
@LINK@ @EXPORT_DYNAMIC@ $(BX_OBJS) $(SIMX86_OBJS) \
@IODEV_LIB_VAR@ @DEBUGGER_VAR@ cpu/libcpu.a memory/libmemory.a gui/libgui.a \
@DISASM_VAR@ @INSTRUMENT_VAR@ @PLUGIN_VAR@ \
@GDBSTUB_VAR@ @FPU_VAR@ \
@NONPLUGIN_GUI_LINK_OPTS@ \
$(MCH_LINK_FLAGS) \
$(SIMX86_LINK_FLAGS) \
$(READLINE_LIB) \
$(EXTRA_LINK_OPTS) \
$(LIBS)
在最后$(LIBS)后面加上 \ -lpthread如下：
bochs@EXE@: @IODEV_LIB_VAR@ @DEBUGGER_VAR@ \
cpu/libcpu.a memory/libmemory.a gui/libgui.a \
@DISASM_VAR@ @INSTRUMENT_VAR@ $(BX_OBJS) \
.
.
.
$(LIBS)\
-lpthread
5.make install：
无法获取“./bochsdbg”的文件状态（stat）：没有那个文件或目录
make;[install_bin] 错误1(忽略)
解决方法：
在make之后将bochs拷贝一份，命名为bochsdbg
到此，bochs安装过程中的大部分问题都得到了解决，现在我们可以开心的用bochs了: )
这些博客对我起到了莫大的帮助，仅此对博主表示感谢：
[http://blog.chinaunix.net/uid-24807808-id-3070251.html](http://blog.chinaunix.net/uid-24807808-id-3070251.html)
[http://blog.csdn.net/xiaoheiyohehe/article/details/19034541](http://blog.csdn.net/xiaoheiyohehe/article/details/19034541)
[http://blog.csdn.net/geeker_12/article/details/11409009](http://blog.csdn.net/geeker_12/article/details/11409009)
[http://www.linuxdiyf.com/linux/15074.html](http://www.linuxdiyf.com/linux/15074.html)
虽然我们已经安装好了bochs，但是我们现在还是不能使用，因为我们还需要写配置文件，做虚拟软盘，加载dos系统，这些我将会继续整理总结。
## 二、bochs的配置文件与镜像的生成
现在我将对bochs的使用进行总结，让pmtest1.com能够运行起来。首先我们得写配置文件，配置文件写在有镜像文件的目录里面，也就是和镜像文件同目录。
创建配置文件
vim bochsrc
配置文件的内容如下：
![](https://img-blog.csdn.net/20170331203534233?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvWXVEYWxl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
下面是几个值得注意的地方
1.#filename of ROM images下面的两行。
这两行是写的两个文件的路径，可以在cd /usr/local/share/doc/bochs里面的bochsrc-sample.txt文件中找到。
2.#what disk images will be used下面的这行。
这行是用来告诉bochs运行哪个镜像的。
接下来再来说一说如何创建镜像，bochs创建镜像是特别简单的，只需要敲下面命令：
bxiamge
![](https://img-blog.csdn.net/20170331203136277?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvWXVEYWxl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后会有几个选项（红框框里面的），第一个是选择生成硬盘镜像还是软盘，我们选择fd；第二个是镜像大小选择，1.44就可以了；第三个是命名，这里随便自己了。
接下来把第一章的boot.bin冲洗到这个镜像里面
dd if=boot.bin of=a.img bs=512 count=1 conv=notrunc
下面我们便来运行一下看一下效果
bochs -f bochsrc
![](https://img-blog.csdn.net/20170331204730534?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvWXVEYWxl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
出来了这个界面，我们选择6。然后会出现一个黑框，这时我们在红圈圈出的这里输入c。
![](https://img-blog.csdn.net/20170331205125852?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvWXVEYWxl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
再切换到bochs里面，点击左边的那个纸盒子就可以了，我们便会看到亲切的Hello，myOS！了。
![](https://img-blog.csdn.net/20170331205255678?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvWXVEYWxl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然而，我们到这里就结束了吗？并没用！我们的目的远远不止于这里，我们是要能把第三章的pmtest1.com运行起来。前面说过，pmtest1.com这个程序的运行条件是在dos系统里面，所以我们需要在bochs里面安装dos。
## 三、bochs中的dos安装和pmtest1.com的运行
如何在bochs里面安装dos呢，下面我们就慢慢来学习。
1.在[http://bochs.sourceforge.net/guestos/freedos-img.tar.gz](http://bochs.sourceforge.net/guestos/freedos-img.tar.gz)上面下载FreeDos（需要翻墙）
2.解压
tar vxzf freedos-img.tar.gz
3.采用上节的方法bximage生成镜像pm.img，然后将freedos-img中的a.img改名为freedos.img并复制到工作目录下，也就是和pm.img同一目录下。
4.修改bochsrc配置文件，修改如图：
![](https://img-blog.csdn.net/20170331210527340?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvWXVEYWxl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
5.启动bochs
6.格式化B盘，退出bochs，过程见图。
![](https://img-blog.csdn.net/20170331211048577?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvWXVEYWxl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这时可能会出现问题。
![](https://img-blog.csdn.net/20170331211228374?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvWXVEYWxl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
没关系，我们先继续下面的步骤
6.将pmtest1.com复制到虚拟软驱pm.img中
mkdir /mnt/floppy
mount -o loop pm.img /mnt/floppy
cp pm1.bin /mnt/floppy
umount /mnt/floppy
如果是出现步骤5的那个问题的，将前面的步骤再做一遍，重新格式化一遍B盘。
7.启动bochs
![](https://img-blog.csdn.net/20170331212342380?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvWXVEYWxl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
出现红色的P，这就表示我们成功了。（因为我将pmtest1.com命名为pm1.bin，所以我的B盘里面输入的是pm1.bin）。
再次特别感谢[faileast_weiwei](http://blog.csdn.net/faileast_weiwei/article/details/42587923)和[河西无名氏](http://blog.csdn.net/trochiluses/article/details/8970292)的博客（点击即可进入他们的博客）
