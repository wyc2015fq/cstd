# 在Ubuntu下编译wpa_supplicant - DoubleLi - 博客园






最近在研究WiFi联盟的多屏互动协议Miracast，其中需要用到wpa_supplicant，编译中遇到了一些问题，经过一下午的折腾也都解决了，下面分享给各位。

一、编译需要用到三个库源码包，分别是wpa_supplicant, openssl, libnl

> 
1. wpa_supplicant是我们需要编译的源文件包，目前最新版本是1.1。在终端命令行下输入： wget http://hostap.epitest.fi/releases/wpa_supplicant-1.1.tar.gz 进行下载。

2. openssl是ssl协议的开源库，wpa_supplicant文档中声明支持的是0.9.8d版本，在实践中最新的1.0.1c版本也能通过编译。在终端命令行下输入： http://openssl.org/source/openssl-1.0.1c.tar.gz 进行下载。

3. libnl也是需要的，在编译中我首先尝试的是libnl3，在编译过程中发现它的接口和wpa_supplicant调用的已经有了变化，所以改成较早的libnl1.1。同样，在终端命令行下输入：wget http://www.infradead.org/~tgr/libnl/files/libnl-1.1.tar.gz 下载它。




二、下载完成并解压三个库的源代码，下面可以开始编译：

> 
1. 编译libnl. 

> 
1.1 tar zxvf libnl-1.1.tar.gz 解压源代码包。进入libnl1.1目录，执行 ./configure 配置编译环境；




> 
1.2 执行make进行编译，这时会报出编译错误：“../include/netlink_local.h:218:error 'ULONG_MAX' undeclared"，我们在对应文件添加一个头文件 #include<limits.h>即可解决问题；




> 
1.3 再次编译通过，执行sudo make install，libnl.so会被安装至/usr/local/lib/目录下，相应的头文件也会被copy到/usr/local/include/netlink下。




2. 编译openssl

> 
2.1进入openssl目录，执行make进行编译，完成后执行make install，编译好的openssl库和头文件等被安装在目录/usr/local/ssl下




3. 编译wpa_supplicant

> 
3.1 进入hostapd/wpa_supplicant目录，首先执行cp defconfig .config 拷贝生成编译配置




> 
3.2 修改配置文件 .config，在"#Uncomment following two linee and fix..."下增加如下语句，用来添加openssl和libnl的头文件和库文件目录，更新编译链接环境变量


> 
> 
CFLAGS += -I/usr/local/ssl/include
CFLAGS += -I/usr/local/include/netlink
LIBS += -L/usr/local/ssl/lib
LIBS += -L/usr/local/lib
LIBS_p += -L/usr/local/ssl/lib



> 
3.3 执行make进行编译，成功后生成三个目标文件wpa_supplicant, wpa_cli, wpa_passphrase，至此编译完成。



































参考文章：

编译libnl时候的问题 [http://www.docin.com/p-352420829.html](http://www.docin.com/p-352420829.html)

Netlink Library - libnl-1.1 移植到ARM Linux [http://blog.csdn.net/hinyunsin/article/details/6029403](http://blog.csdn.net/hinyunsin/article/details/6029403)

ubuntu下编译wpa_supplicant [http://hi.baidu.com/yyangjjun/item/930f46ab9d21431fa8cfb7ba](http://hi.baidu.com/yyangjjun/item/930f46ab9d21431fa8cfb7ba)

wpa_supplicant的移植 [http://blog.csdn.net/heshanxingzhe/article/details/6730397](http://blog.csdn.net/heshanxingzhe/article/details/6730397)









