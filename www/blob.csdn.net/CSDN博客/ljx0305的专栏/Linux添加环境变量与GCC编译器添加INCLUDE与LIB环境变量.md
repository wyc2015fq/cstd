# Linux添加环境变量与GCC编译器添加INCLUDE与LIB环境变量 - ljx0305的专栏 - CSDN博客
2012年11月07日 16:50:24[ljx0305](https://me.csdn.net/ljx0305)阅读数：609
个人分类：[Linux程序及使用																[Linux																[C](https://blog.csdn.net/ljx0305/article/category/394717)](https://blog.csdn.net/ljx0305/article/category/364133)](https://blog.csdn.net/ljx0305/article/category/394700)
# [Linux添加环境变量与GCC编译器添加INCLUDE与LIB环境变量](http://www.cnblogs.com/image-eye/archive/2011/08/20/2147133.html)
对所有用户有效在/etc/profile增加以下内容。只对当前用户有效在Home目录下的
.bashrc或.bash_profile里增加下面的内容：
(注意：等号前面不要加空格,否则可能出现 command not found)
#在PATH中找到可执行文件程序的路径。
export PATH =$PATH:$HOME/bin
#gcc找到头文件的路径
C_INCLUDE_PATH=/usr/include/libxml2:/MyLib
export C_INCLUDE_PATH
#g++找到头文件的路径
CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:/usr/include/libxml2:/MyLib
export CPLUS_INCLUDE_PATH
#找到动态链接库的路径
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/MyLib
export LD_LIBRARY_PATH
#找到静态库的路径
LIBRARY_PATH=$LIBRARY_PATH:/MyLib
export LIBRARY_PATH
库 文件在连接（静态库和共享 库）和运行（仅限于使用共享库的程序）时被使用，其搜索路径是在系统中进行设置的。一般 Linux 系统把 /lib 和 /usr/lib 两个目录作为默认的库搜索路径，所以使用这两个目录中的库时不需要进行设置搜索路径即可直接使用。对于处于默认库搜索路径之外的库，需要将库的位置添加到 库的搜索路径之中。设置库文件的搜索路径有下列两种方式，可任选其一使用：
在环境变量 LD_LIBRARY_PATH 中指明库的搜索路径。
在 /etc/ld.so.conf 文件中添加库的搜索路径。
将自己可能存放库文件的路径都加入到/etc/ld.so.conf中是明智的选择
添加方法也极其简单，将库文件的绝对路径直接写进去就OK了，一行一个。例如：
/usr/X11R6/lib
/usr/local/lib
/opt/lib
需 要注意的是：第二种搜索路径的设置方式对于程序连接时的库（包括共享库和静态库） 的定位已经足够了，但是对于使用了共享库的程序的执行还是不够的。这是因为为了加快程序执行时对共享库的定位速度，避免使用搜索路径查找共享库的低效率， 所以是直接读取库列表文件 /etc/ld.so.cache 从中进行搜索的。/etc/ld.so.cache 是一个非文本的数据文件，不能直接编辑，它是根据 /etc/ld.so.conf 中设置的搜索路径由 /sbin/ldconfig
 命令将这些搜索路径下的共享库文件集中在一起而生成的（ldconfig 命令要以 root 权限执行）。因此，为了保证程序执行时对库的定位，在 /etc/ld.so.conf 中进行了库搜索路径的设置之后，还必须要运行 /sbin/ldconfig 命令更新 /etc/ld.so.cache 文件之后才可以。ldconfig ,简单的说，它的作用就是将/etc/ld.so.conf列出的路径下的库文件缓存到/etc/ld.so.cache 以供使用。因此当安装完一些库文件，(例如刚安装好glib)，或者修改ld.so.conf增加新的库路径后，需要运行一下
 /sbin/ldconfig使所有的库文件都被缓存到ld.so.cache中，如果没做，即使库文件明明就在/usr/lib下的，也是不会被使用 的，结果编译过程中抱错，缺少xxx库，去查看发现明明就在那放着，搞的想大骂computer蠢猪一个。
在程序连接时，对于库文件（静态库和共享库）的搜索路径，除了上面的设置方式之外，还可以通过 -L 参数显式指定。因为用 -L 设置的路径将被优先搜索，所以在连接的时候通常都会以这种方式直接指定要连接的库的路径。
前 面已经说明过了，库搜索路径的设置有两种方式：在环境变量 LD_LIBRARY_PATH 中设置以及在 /etc/ld.so.conf 文件中设置。其中，第二种设置方式需要 root 权限，以改变 /etc/ld.so.conf 文件并执行 /sbin/ldconfig 命令。而且，当系统重新启动后，所有的基于 GTK2 的程序在运行时都将使用新安装的 GTK+ 库。不幸的是，由于 GTK+ 版本的改变，这有时会给应用程序带来兼容性的问题，造成某些程序运行不正常。为了避免出现上面的这些情况，在
 GTK+ 及其依赖库的安装过程中对于库的搜索路径的设置将采用第一种方式进行。这种设置方式不需要 root 权限，设置也简单：
$ export LD_LIBRARY_PATH=/opt/gtk/lib:$LD_LIBRARY_PATH
可以用下面的命令查看 LD_LIBRAY_PATH 的设置内容：
$ echo $LD_LIBRARY_PATH
至此，库的两种设置就完成了。
引用:[http://www.cnblogs.com/image-eye/archive/2011/08/20/2147133.html](http://www.cnblogs.com/image-eye/archive/2011/08/20/2147133.html)
