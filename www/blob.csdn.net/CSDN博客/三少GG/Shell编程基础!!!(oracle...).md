# Shell编程基础!!!(oracle...) - 三少GG - CSDN博客
2011年06月09日 16:28:00[三少GG](https://me.csdn.net/scut1135)阅读数：959标签：[shell																[编程																[脚本																[bash																[ubuntu](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)](https://so.csdn.net/so/search/s.do?q=bash&t=blog)](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)
个人分类：[Ubuntu/OS X系统](https://blog.csdn.net/scut1135/article/category/680808)
# Shell编程基础
[http://wiki.ubuntu.org.cn/Shell%E7%BC%96%E7%A8%8B%E5%9F%BA%E7%A1%80](http://wiki.ubuntu.org.cn/Shell%E7%BC%96%E7%A8%8B%E5%9F%BA%E7%A1%80)
我们可以使用任意一种文字编辑器，比如gedit、kedit、emacs、vi等来编写shell脚本，它必须以如下行开始（必须放在文件的第一行）： 
#!/bin/sh
...
注意：最好使用“!/bin/bash”而不是“!/bin/sh”，如果使用tc shell改为tcsh，其他类似。 
符号#!用来告诉系统执行该脚本的程序，本例使用/bin/sh。编辑结束并保存后，如果要执行该脚本，必须先使其可执行： 
chmod +x filename
此后在该脚本所在目录下，输入 ./filename 即可执行该脚本。 
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++UBUNTU 8.1 里如何安装SH文件首先你要打开一个终端。
然后输入sudo su
随后输入密码。这样就取得了root 用户权限。
然后找到那个文件（当然是在终端里面用命令找，你可别开一个文件管理器。）
执行./sh文件名字
就可以了。
**答案补充**
比如你有一个sh文件叫1.sh，这个文件在硬盘的/tmp目录里面，要执行的话，就要这样：
$ sudo su
pass your password:（这里输入密码的时候是看不到输入的东西的，不过确实已经输入进去了。）
#cd /tmp
#./1.sh
这样就OK了。
SH文件是Linux系统里面的类似批处理的文件，所以运行sh文件后可能会还要你输入一些信息。这个就视sh文件的内容而定了。+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# ubuntu下shell脚本运行异常：bash和dash的区别
我用[**bash**]()到语法写了一个[**shell**]()脚本（准确的说是把书上的脚本敲进电脑），在[**ubuntu**]()下，用sh [**test**]().sh来运行，但是出现了意料之外到结果，比如echo -e "/nTest/n"到执行就不同，直接用sh test.sh运行，会打印出-e（作为字符串）;而直接在终端中输入echo -e "/nTest/n"是不会输出"-e"的。
我就在想是什么原因，后来终于发现是Ubuntu下sh默认指向[**dash**]()。
echo $SHELL  -->  /bin/bash
type sh      -->  /bin/sh
然后我到/bin下看了有"sh -> dash"，原来sh是/bin/dash的链接。
后来google了一下才发现，buntu6.10已将先前默认的bashshell更换为了dash。其表现是/bin/sh链接到了/bin/dash而不是传统的/bin/bash。
bash - GNU Bourne-Again SHell
dash - Debian Almquist Shell
可以分别man bash  / man dash看一下。
那么怎么把sh改为指向bash呢？
最暴力的方法当然是直接把/bin/sh的软链接改到bash中，
如：ln -s /bin/bash /bin/sh
但是，有优雅一些的方法,
sudo dpkg-reconfigure dash
出现菜单问你是否要dash的时候，选no就可以了。
再次检查一下， ls /bin/sh -al 发现软链接指向/bin/bash就可以了。 
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
### [linux source命令](http://writeblog.csdn.net/blog/396413)
source命令用法： 
source FileName 
作用:在当前bash环境下读取并执行FileName中的命令。 
注：该命令通常用命令“.”来替代。 
如：source .bash_rc 与 . .bash_rc 是等效的。 
source命令(从 C Shell 而来)是bash shell的内置命令。点命令，就是个点符号，(从Bourne Shell而来)是source的另一名称。
同样的，当前脚本中配置的变量也将作为脚本的环境，source(或点)命令通常用于重新执行刚修改的初始化文档，如 .bash_profile 和 .profile 等等。
例如，假如在登录后对 .bash_profile 中的 EDITER 和 TERM 变量做了修改，则能够用source命令重新执行 .bash_profile 中的命令而不用注销并重新登录。 
比如您在一个脚本里export $KKK=111 ,假如您用./a.sh执行该脚本，执行完毕后，您运行 echo $KKK ,发现没有值，假如您用source来执行 ，然后再echo ,就会发现KKK=111。
**因为调用./a.sh来执行shell是在一个子shell里运行的，所以执行后，结构并没有反应到父shell里，但是source不同他就是在本shell中执行的，所以能够看到结果 **
source命令(从 C Shell 而来)是bash shell的内置命令。点命令，就是一个点符号，(从Bourne Shell而来)是source的另一名称。这两个命令都以一个脚本为参数，
该脚本将作为当前shell的环境执行，即不会启动一个新的子进程。所有在脚本中设置的变量将成为当前Shell的一部分。同样的，当前脚本中设置的变量也将作为脚本的环境，
source(或点)命令通常用于重新执行刚修改的初始化文件，如 .bash_profile 和 .profile 等等。例如，如果在登录后对 .bash_profile 中的 EDITER 和 TERM 变量做了修改，
则可以用source命令重新执行 .bash_profile 中的命令而不用注销并重新登录。象 .bash_profile 或其它类似的Shell脚本这样，文件无需可执行权限即可用source或点命令执行。 
source命令的一个妙用 
在编译核心时，常常要反复输入一长串命令，如 
make mrproper 
make menuconfig 
make dep 
make clean 
make bzImage 
....... 
这些命令既长，又繁琐。而且有时候容易输错，浪费你的时间和精力。如果把这些命令做成一个文件，让它自动按顺序执行，对于需要多次反复编译核心的用户来说，会很方便。
用source命令可以办到这一点。它的作用就是把一个文件的内容当成是shell来执行。先在/usr/src/linux- 2.4.20目录下建立一个文件，取名为make_command： 
在其中输入如下内容： 
make mrproper && 
make menuconfig && 
make dep && 
make clean && 
make bzImage && 
make modules && 
make modules_install && 
cp arch/i386/boot/bzImge /boot/vmlinuz_new && 
cp System.map /boot && 
vi /etc/lilo.conf && 
lilo -v 
文件建立好之后，以后每次编译核心，只需要在/usr/src/linux-2.4.20下输入 
source make_command 
就行了。这个文件也完全可以做成脚本，只需稍加改动即可。这里主要是让大家理解source的用法。如果你用的不是lilo来引导系统，可以把最后两句话去掉。配置你自己的引导程序来引导新内核。 
shell编程中的命令有时和C语言是一样的。&&表示与，||表示或。把两个命令用&&联接起来，如 make mrproper && make menuconfig ，表示要第一个命令执行成功才能执行第二个命令。
对执行顺序有要求的命令能保证一旦有错误发生，下面的命令不会盲目地继续执行。 
原文地址 [http://hi.baidu.com/linuxcer/blog/item/8462d1990827fc0a6e068c14.html](http://hi.baidu.com/linuxcer/blog/item/8462d1990827fc0a6e068c14.html)
----------------------------------------------- 
---------------- 我的测试 ---------------------- 
----------------------------------------------- 
1 建立test.sh 
  #!/bin/bash 
  export s=/home/jboss/ 
2 
  执行命令: source test.sh 
           echo $s 
  结果输出: /home/jboss/ 
3  新开个shell 
   执行命令:   ./test.sh 
            echo $s 
     结果:  没有输出s值 
