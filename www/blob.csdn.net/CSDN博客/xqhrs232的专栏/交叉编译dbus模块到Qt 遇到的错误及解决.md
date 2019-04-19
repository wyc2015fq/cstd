# 交叉编译dbus模块到Qt 遇到的错误及解决 - xqhrs232的专栏 - CSDN博客
2016年06月12日 19:37:14[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：325
原文地址::[http://blog.csdn.net/yanzi1225627/article/details/7855124](http://blog.csdn.net/yanzi1225627/article/details/7855124)
由于用qt来检测U盘插拔，要用dbus。pc调试通过后，发现到板子里没有dbus。原来默认情况下，编译arm版本qt时是不带dbus的，因此要重新交叉编译qt。
我的配置：
./configure -prefix /usr/local/arm/arm2-qt-4.8.1 -opensource -embedded arm -xplatform qws/linux-arm-g++  -no-webkit -qt-libtiff -qt-libmng -qt-mouse-tslib -qt-mouse-pc -no-mouse-linuxtp -no-neon -nomake examples -nomake docs -nomake
 demos -nomake tools -no-qt3support -dbus
版本为qt4.8.1，注意最后的“-dbus”就是让qt带dbus模块。
         结果报错：The QtDBus module cannot be enabled because libdbus-1 version 0.93 was not found. 说需要一个libdbus-1，yum install了一下，发现这个东西已经安装了在pc上，否则我程序在pc下也不会跑通。只是没有嵌入式版本的，需要自己用源码交叉编译。
     我下载的dbus源码的版本是1.0.2，看到网上移植这个的多。首先新建一个文件夹，mkdir /usr/local/arm/dbus-1.0.2,然后解压源码，进到dbus-1.0.2目录里，输入：
./configure --host=arm-linux --prefix=/usr/local/arm/dbus-1.0.2 CC=arm-linux-gcc --cache-file=arm-linux.cache --without-x 
**报错**：checking abstract socket namespace... configure: error: cannot run test program while cross
 compiling
**解决**：1，首先用locate pkgconfig查看下pkgconfig安装了没有，一般都安装了。找到路径配置到环境变量里：export PKG_CONFIG_PATH=/usr/lib/pkgconfig
2，光设这个环境变量还是不行的，在终端里输入：
**#echo ac_cv_have_abstract_sockets=yes>arm-linux.cache**
之后，./configure通过！
然后make，
报错如下：
make[3]: *** [dbus-sysdeps-unix.lo] 错误 1
make[3]: 离开目录“/home/yan/download/dbus-1.0.2/dbus”
make[2]: *** [all] 错误 2
make[2]: 离开目录“/home/yan/download/dbus-1.0.2/dbus”
make[1]: *** [all-recursive] 错误 1
make[1]: 离开目录“/home/yan/download/dbus-1.0.2”
make: *** [all] 错误 2
**解决：**进到dbus-1.0.2目录下的dbus目录，gedit dbus-sysdeps-unix.c，在文件的开头处添加：
struct ucred
{
pid_t pid; /* PID of sending process. */
uid_t uid; /* UID of sending process. */
gid_t gid; /* GID of sending process. */
};
注意上面最后的“；”要带的。
之后make ，make install一路顺利！
进入到dbus的-prefix的目录查看下是否安装完了，截图如下：
![](https://img-my.csdn.net/uploads/201208/11/1344681073_7917.jpg)
     接下来就是要切换到qt的解压缩目录，重新./configure,在-dbus后面加上路径。也就是两个 -I 和一个 -L 。坑爹的是网上没人说清这一点，有人自己搞出来了说的模棱两可。我本来是按照安装目录里dbus-1.0.2找到include和lib，分别把里面的最深目录配置进去，发现总是错误。原来根本不是这么回事！找到-prefix目录下的lib文件下的pkgconfig文件夹，打开dbus-1.pc，里面内容如下：
**[html]**[view
 plain](http://blog.csdn.net/yanzi1225627/article/details/7855124#)[copy](http://blog.csdn.net/yanzi1225627/article/details/7855124#)
[print](http://blog.csdn.net/yanzi1225627/article/details/7855124#)[?](http://blog.csdn.net/yanzi1225627/article/details/7855124#)
- <spanstyle="font-size:18px;">prefix=/usr/local/arm/dbus-1.0.2  
- exec_prefix=${prefix}  
- libdir=${exec_prefix}/lib  
- includedir=${prefix}/include  
- system_bus_default_address=unix:path=/usr/local/arm/dbus-1.0.2/var/run/dbus/system_bus_socket  
- sysconfdir=/usr/local/arm/dbus-1.0.2/etc  
- session_bus_services_dir=/usr/local/arm/dbus-1.0.2/share/dbus-1/services  
- 
- Name: dbus  
- Description: Free desktop message bus  
- Version: 1.0.2  
- <u><spanstyle="color:#ff0000;">Libs: -L${libdir} -ldbus-1    
- Cflags: -I${includedir}/dbus-1.0 -I${libdir}/dbus-1.0/include</span></u></span>
    重点看最后两句，这里-L和-I并没有配置到最深目录。（坑爹的是网上有人说要配置到最深目录，还有人说要改这个dbus-1.pc文件，NND！![发火](http://static.blog.csdn.net/xheditor/xheditor_emot/default/angry.gif)）仿照上面最后两句，**正确的-dbus后的参数应该是：**
**-dbus -dbus-linked -I/usr/local/arm/dbus-1.0.2/include/dbus-1.0 -I/usr/local/arm/dbus-1.0.2/lib/dbus-1.0/include -L/usr/local/arm/dbus-1.0.2/lib -ldbus-1 -lpthread -lrt**
保险起见，export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/arm/dbus-1.0.2/lib/pkgconfig 把dbus的pkgconfig这个文件夹路径配置进去，这一步不弄应该也可以！
**最终的qt的./configure内容是：**
./configure -prefix /usr/local/arm/arm2-qt-4.8.1 -opensource -embedded arm -xplatform qws/linux-arm-g++  -no-webkit -qt-libtiff -qt-libmng -qt-mouse-tslib
 -qt-mouse-pc -no-mouse-linuxtp -no-neon -nomake examples -nomake docs -nomake demos -nomake tools -no-qt3support -dbus -dbus-linked -I/usr/local/arm/dbus-1.0.2/include/dbus-1.0 -I/usr/local/arm/dbus-1.0.2/lib/dbus-1.0/include -L/usr/local/arm/dbus-1.0.2/lib
 -ldbus-1 -lpthread -lrt
    然后就是make、make install，将生成的lib文件夹，打包压缩，拷贝到板子里再解压放到相应位置就可以了！如有疑问，参考我前几篇博客。
