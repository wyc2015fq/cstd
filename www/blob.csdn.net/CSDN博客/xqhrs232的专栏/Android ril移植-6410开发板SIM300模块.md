# Android ril移植-6410开发板SIM300模块 - xqhrs232的专栏 - CSDN博客
2013年01月24日 21:08:23[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：698
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/mr_raptor/article/details/6598891](http://blog.csdn.net/mr_raptor/article/details/6598891)
++++++++++++++++++++++++++++++++++++++++++
本文系本站原创,欢迎转载! 转载请注明出处:
[http://blog.csdn.net/mr_raptor/article/details/6598891](http://blog.csdn.net/mr_raptor/article/details/6598891)
++++++++++++++++++++++++++++++++++++++++++
环境介绍：
    1. ubuntu 10.10，作为Android编译环境同时作为NFS让6410开发板挂载作为根文件系统。
    2. Android使用的是友善给的源码，使用./build-android命令编译出来，使用./genrootfs.sh命令在主源码主目录下生成rootfs，删除掉data/app下的安装包（不删除也可以）
   3. 将rootfs里的文件系统拷贝到NFS挂载目录下，我的NFS目录是/work/nfs_root，安装并开启NFS，通过exportfs命令测试是否将NFS共享。
   4. 修改uboot中的kernel启动参数为：
setenv bootargs noinitrd  init=/linuxrc root=/dev/nfs nfsroot=192.168.11.11:/work/nfs_root ip=192.168.11.13:192.168.11.11:192.168.11.1:255.255.255.0:www.android.com:eth0:off console=ttySAC0,115200
   开发板使用192.168.11.13地址，NFS主机使用192.168.11.11地址。注意：每次开发板重启都要重新设置下IP，因为ubuntu会自动更新IP地址。
 1. 在进行ril移植之前建议先了解下RIL的框架，推荐下面的文章：[http://blog.sina.com.cn/s/blog_682793a50100jeo5.html](http://blog.sina.com.cn/s/blog_682793a50100jeo5.html)
2. 主要修改的是hardware/ril/reference-ril/reference-ril.c文件其它文件基本上不用修改，因为gooooogle已经将框架封装好了。
   将mainLoop函数中的2043行进行修改：
> 
if ( fd >= 0 && !memcmp( s_device_path, "/dev/ttyS", 9 ) ) {
                    /* disable echo on serial ports */
                    struct termios  ios;
                    tcgetattr( fd, &ios );
                    ios.c_lflag = 0;  /* disable ECHO, ICANON, etc... */
                    tcsetattr( fd, TCSANOW, &ios );
 }
修改之后：
if ( fd >= 0 && !memcmp( s_device_path, "/dev/s3c2410_serial1", 20 ) ) {
                    /* disable echo on serial ports */
                    struct termios  ios;
                    bzero(&ios, sizeof(ios));
                    tcgetattr( fd, &ios );
                    ios.c_iflag |=
                    ios.c_cflag |= CLOCAL | CREAD | CS8;   // 用于本地连接和接收使能数据位长度
                    ios.c_iflag |= IGNPAR;
                    cfsetispeed(&ios, B115200);           //设置波特率
                    cfsetospeed(&ios, B115200);
                    ios.c_lflag = 0;  /* disable ECHO, ICANON, etc... */
                    tcflush(fd, TCIFLUSH);
                    tcsetattr( fd, TCSANOW, &ios );
 }
3.  修改initializeCallback函数中AT命令（建议参照google提供的命令和SIM300模块给的AT命令参考文档）
修改完之后，重新编译Android源码，将新生成的libreference-ril.so拷贝到NFS根目录下，重启开发板即可打电话，接电话。
扩展：如果使用的FriendlyArm提供的源码，那你看不到状态栏的电话信号图标和电池图标，这是因为FriendlyArm已经将其注释掉了，请将frameworks/base/packages/systemui/src/com/android/systemui/statusbar/StatusBarPolicy.java文件里的注释支持即可。
++++++++++++++++++++++++++++++++++++++++++
本文系本站原创,欢迎转载! 转载请注明出处:
[http://blog.csdn.net/mr_raptor/article/details/6598891](http://blog.csdn.net/mr_raptor/article/details/6598891)
++++++++++++++++++++++++++++++++++++++++++
