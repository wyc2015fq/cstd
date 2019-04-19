# linux c++ 头文件及库文件的搜索路径问题 - KinboSong的博客 - CSDN博客
2017年07月31日 23:07:40[songjinbo3](https://me.csdn.net/KinboSong)阅读数：1100
个人分类：[C++开发](https://blog.csdn.net/KinboSong/article/category/6854166)
**头文件：**
1.#include
 “headfile.h”
搜索顺序为：
①先搜索当前目录
②然后搜索-I指定的目录
③再搜索gcc的环境变量CPLUS_INCLUDE_PATH（C程序使用的是C_INCLUDE_PATH）
④最后搜索gcc的内定目录
/usr/include
/usr/local/include
/usr/lib/gcc/x86_64-redhat-[Linux](http://lib.csdn.net/base/linux)/4.1.1/include
各目录存在相同文件时，先找到哪个使用哪个。
2.#include
 <headfile.h>
①先搜索-I指定的目录
②然后搜索gcc的环境变量CPLUS_INCLUDE_PATH
③最后搜索gcc的内定目录
/usr/include
/usr/local/include
/usr/lib/gcc/x86_64-redhat-[linux](http://lib.csdn.net/base/linux)/4.1.1/include
与上面的相同，各目录存在相同文件时，先找到哪个使用哪个。这里要注意，#include<>方式不会搜索当前目录！
这里要说下include的内定目录，它不是由$PATH环境变量指定的，而是由g++的配置prefix指定的(知道它在安装g++时可以指定，不知安装后如何修改的，可能是修改配置文件，需要时再研究下)：
-bash-3.2$ g++ -v
Using built-in specs.
Target: x86_64-redhat-linux
Configured with: ../configure **–prefix=/usr** –mandir=/usr/share/man –infodir=/usr/share/info –enable-shared
 –enable-threads=posix –enable-checking=release –with-system-zlib –enable-__cxa_atexit –disable-libunwind-exceptions –enable-libgcj-multifile –enable-languages=c,c++,objc,obj-c++,java,fortran,ada –enable-java-awt=gtk –disable-dssi –enable-plugin –with-java-home=/usr/lib/jvm/java-1.4.2-gcj-1.4.2.0/jre
 –with-cpu=generic –host=x86_64-redhat-linux
Thread model: posix
gcc version 4.1.2 20080704 (Red Hat 4.1.2-46)
在安装g++时，指定了prefix，那么内定搜索目录就是：
Prefix/include
Prefix/local/include
Prefix/lib/gcc/–host/–version/include
编译时可以通过-nostdinc++选项屏蔽对内定目录搜索头文件。
**库文件：**
编译的时候：
①gcc会去找-L
②再找gcc的环境变量LIBRARY_PATH
③再找内定目录/lib /usr/lib /usr/local/lib这是当初compile
 gcc时写在程序内的（不可配置的？）
**运行时动态库的搜索路径：**
动态库的搜索路径搜索的先后顺序是：
①编译目标代码时指定的动态库搜索路径（这是通过gcc的参数“-Wl,-rpath,”指定。当指定多个动态库搜索路径时，路径之间用冒号”：”分隔）
②环境变量LD_LIBRARY_PATH指定的动态库搜索路径（当通过该环境变量指定多个动态库搜索路径时，路径之间用冒号”：”分隔）
③配置文件/etc/ld.so.conf中指定的动态库搜索路径；
④默认的动态库搜索路径/lib；
⑤默认的动态库搜索路径/usr/lib。
（应注意动态库搜寻路径并不包括当前文件夹，所以当即使可执行文件和其所需的so文件在同一文件夹，也会出现找不到so的问题，类同#include <header_file>不搜索当前目录）
C/C++程序在linux下被编译和连接时，GCC/G++会查找系统默认的include和link的路径，以及自己在编译命令中指定的路径。自己指定的路径就不说了，这里说明一下系统自动搜索的路径。
【1】include头文件路径
除了默认的/usr/include, /usr/local/include等include路径外，还可以通过设置环境变量来添加系统include的路径：
# C
export C_INCLUDE_PATH=XXXX:$C_INCLUDE_PATH
# CPP
export CPLUS_INCLUDE_PATH=XXX:$CPLUS_INCLUDE_PATH
以上修改可以直接命令行输入（一次性），可以在/etc/profile中完成（对所有用户生效），也可以在用户home目录下的.bashrc或.bash_profile中添加（针对某个用户生效），修改完后重新登录即生效。
【2】link链接库文件路径
链接库文件在连接（静态库和共享库）和运行（仅限于使用共享库的程序）时被使用，其搜索路径是在系统中进行设置的（也可以在编译命令中通过 -l -L 来指定，这里讲的是使用系统默认搜索路径）。
一般 Linux 系统把 /lib /usr/lib /usr/local/lib 作为默认的库搜索路径，所以使用这几个目录中的链接库文件可直接被搜索到（不需要专门指定链接库路径）。对于默认搜索路径之外的库，则需要将其所在路径添加到gcc/g++的搜索路径之中。
链接库文件的搜索路径指定有两种方式：1）修改/etc/so.ld.conf 2）修改环境变量，在其中添加自己的路径
1）在环境变量中添加
动态链接库搜索路径：
export LD_LIBRARY_PATH=XXX:$LD_LIBRARY_PATH
静态链接库搜索路径：
export LIBRARY_PATH=XXX:$LIBRARY_PATH
以上修改可以直接命令行输入（一次性），可以在/etc/profile中完成（对所有用户生效），也可以在用户home目录下的.bashrc或.bash_profile中添加（针对某个用户生效）,修改完后重新登录即生效。
2）在/etc/ld.so.conf 中添加指定的链接库搜索路径（需要root权限），然后运行 /sbin/ldconfig，以达到刷新 /etc/ld.so.cache的效果。
以上两种方式均可以达到指定链接库搜索路径的效果。
