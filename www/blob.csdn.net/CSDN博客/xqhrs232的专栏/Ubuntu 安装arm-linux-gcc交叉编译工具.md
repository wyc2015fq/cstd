# Ubuntu 安装arm-linux-gcc交叉编译工具 - xqhrs232的专栏 - CSDN博客
2016年03月21日 14:39:22[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：459
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址：[http://www.cnblogs.com/dartagnan/archive/2010/11/12/2003573.html](http://www.cnblogs.com/dartagnan/archive/2010/11/12/2003573.html)
相关文章
1、ubuntu10.10安装交叉编译器----[http://blog.sina.com.cn/s/blog_5f24581b01012fa9.html](http://blog.sina.com.cn/s/blog_5f24581b01012fa9.html)
2、Ubuntu下安装ARM交叉编译器----[http://wenku.baidu.com/link?url=kD2BxTFi9NhR2u0I1HoSOFrG2fRQXUBywVXPRJMUUgo8o0r7r5nINna5fIcDoAbqciGAAGYplfhIPypDRJQdPDFBtuwQFa6A1BwbNJFMGM7](http://wenku.baidu.com/link?url=kD2BxTFi9NhR2u0I1HoSOFrG2fRQXUBywVXPRJMUUgo8o0r7r5nINna5fIcDoAbqciGAAGYplfhIPypDRJQdPDFBtuwQFa6A1BwbNJFMGM7)
3、[Ubuntu下安装、配置交叉编译工具链](http://blog.csdn.net/dreambegin/article/details/6794673)----[http://blog.csdn.net/dreambegin/article/details/6794673](http://blog.csdn.net/dreambegin/article/details/6794673)
最近要开发嵌入式linux，但是手头上大多文档中介绍交叉编译环境的都是基于RedHat9的，这个版本虽说很经典，但是毕竟实在是太古老了，自己机器上本身装的是Ubuntu，因此小小的研究了一下，在ubuntu中安装交叉编译环境 arm-linux-gcc
下面所有yourname替换为你自己的位于/home目录下的用户名
首先是准本工作 ：
需要的安装包：
crosstool-0.43.tar.gz 可以在http://kegel.com/crosstool/crosstool-0.43.tar.gz获得
创建一个文件夹
mkdir /home/yourname/downloads也可以用mkdir ~/downloads
将crosstool-0.43.tar.gz复制到downloads目录下（下载完成后默认应该实在桌面上）
另外，还有一些安装包：
binutils-2.16.1.tar.bz2  
glibc-2.3.2.tar.bz2
glibc-linuxthreads-2.3.2.tar.bz2
gcc-3.3.6.tar.bz2        
linux-2.6.15.4.tar.bz2
gcc-4.1.0.tar.bz2        
linux-libc-headers-2.6.12.0.tar.bz2
gdb-6.5.tar.bz2
这些在安装过程中会自动下载，也可以自己提前下载好，放在/home/yourname/downloads下面，不过我建议提前下好，因为一会需要修改一下glibc里的一个文件。          
下面是一些安装是可能遇到的问题，提前些出来，省得大家忙活半天编译到一半发现错了还得重新编译，如果你想见识一下这些问题，可以直接跳转到下面的安装过程 。
1.安装过程中会提示缺少下面两个包
bison
flex
如果以前没有装过，下载安装一下（具体包名我忘记了，可以直接输入bison查看，如果没装过，会提示你可以下载哪些包安装，flex类似）
sudo apt-get install bison
sudo apt-get install flex
另外网上说还需要一个mm4，不过我装的时候没有提示，好象是安装flex时，解决依赖性时自动装上了。
2.还有一个莫名其妙的错误，提示说gcc版本太旧或者未安装，但是你用sudo apt-get install gcc 的时后会发现gcc已经是最新版本了，事实上这个错误是因为gcc版本太新了。解决方法如下：
sudo apt-get install gcc-3.4
cd /usr/bin
sudo rm gcc
ln -s gcc-3.4 gcc
3.安装是还出现了如下错误：
/home/xxs/crosstool-0.43/build/arm-arm9tdmi-linux-gnu/gcc-4.1.1-glibc-2.3.2/build-glibc/csu/version-info.h:1:1:  missing terminating " character
系统在编译生成version.o时候发现version-info.h文件有错误
解决方法是需要修改一个文件
解压glibc-2.3.2.tar.bz2
找到glibc-2.3.2/csu目录下的Makefile按如下修改
文件末尾有2处echo
echo
"\"Compiled on a $$os $$version system" \
                        "on
`date +%Y-%m-%d`.\\n\"" ;; \
改为：
echo
"\"Compiled on a $$os $$version system" \
                        "on
`date +%Y-%m-%d`.\\\\n\"" ;; \
echo
"\"Available extensions:\\n\"";
改为：
echo
"\"Available extensions:\\\\n\""    \
保存并重新压缩为glibc-2.3.2.tar.bz2 覆盖原来downloads目录下的glibc-2.3.2.tar.bz2
解决好上面的一些小问题就可以开始安装了
1.创建一个安装目录
sudo mkdir /opt/crosstool
这是因为安装文件默认是安装到这个目录里的
2.为你自己添加权限
sudo chown yourname /opt/crosstool
3.解压crosstools-0.43.tar.gz(此时安装包应该位于/home/yourname/downloads/目录下，不是的话自己改一下)
tar -zvxf crosstools-0.43.tar.gz
进入cross目录
cd cross-0.43
可以ls一下看看里面的文件，里面有一些脚本文件，具体功能可以到http://kegel.com/crosstool上查看FAQ
4.运行./demo-arm.sh进行安装
这步要注意，网上有人说需要修改脚本文件里的一些设置，我没改也装上了，不知道为什么。
5.剩下的就等着编译安装吧，很漫长～～
修改环境变量
安装完成后，默认的那装目录就是开始创建的那个/opt/crosstools/
1.添加环境变量
gedit ~/.bashrc
在最后添加如下
if [ -d /opt/crosstool/gcc-4.1.0-glibc-2.3.2/arm-unknow-linux-gnu ]; then
    PATH=/opt/crosstool/gcc-4.1.0-glibc-2.3.2/arm-unknow-linux-gnu/bin:$PATH
fi
就是把安装的工具bin目录添加到PATH变量中。
保存
2.source ~/.bashrc
3.重启终端。
然后可以测试一下
arm-unknow-linux-gnu-gcc
正常的话就会出现
arm-unknow-linux-gnu-gcc: no input files
说明程序能用了。
剩下的就是继续探索～
革命尚未成功，同志还需努力
