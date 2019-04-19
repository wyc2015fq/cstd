# RT73 wifi无线网卡驱动移植过程 - xqhrs232的专栏 - CSDN博客
2012年01月04日 15:56:24[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：761标签：[linux																[网络																[makefile																[编译器																[structure																[工具](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=structure&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=makefile&t=blog)](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://www.embedhq.org/html/jsbw/2010/0626/419.html](http://www.embedhq.org/html/jsbw/2010/0626/419.html)
移植环境： 主机操作系统：Ubantu 8.10 目标系统：linux-2.6.22.6 交叉编译器：arm-softfloat-linux-gnu-gcc-3.4.5 一、RT73驱动编译 1、 下载RT73驱动源码 http://www.ralinktech.com/网站上下载2009_0713_RT73_Li
**移植环境：**
主机操作系统：Ubantu 8.10
        目标系统：linux-2.6.22.6
        交叉编译器：arm-softfloat-linux-gnu-gcc-3.4.5
**一、RT73驱动编译**
**1、 下载RT73驱动源码**
http://www.ralinktech.com/网站上下载2009_0713_RT73_Linux_STA_Drv1[1].1.0.3.zip
        解压unzip 2009_0713_RT73_Linux_STA_Drv1\[1\].1.0.3.zip
        生成两个目录：
        Module（驱动）        WPA_Supplicant-0.5.8（wpa网络工具）
2、 修改Makefile
5 #PLATFORM=PC
        6 PLATFORM=CMPC
        33 #EXTRA_CFLAGS += -DDBG
        40 ifeq ($(PLATFORM),CMPC)
        41 LINUX_SRC = /source/kernel/linux-2.6.22
        42 endif
注：#EXTRA_CFLAGS += -DDBG 一定要关掉，否则终端会不停的循环打印扫描网络的信息。
**3、 配置内核支持wifi网络**
否则会出现如下错误：
        /mnt/sdc/rt73/2009_0713_RT73_Linux_STA_Drv1.1.0.3/Module/rtmp_main.c:1163:        error: structure has no member named `wireless_handlers'
![](https://blog.csdn.net/uploads/allimg/100626/112I13R8-0.jpg)
![](https://blog.csdn.net/uploads/allimg/100626/112I1Ic-1.jpg)
配置完毕后，需要编译下内核，目的是使配置在include/linux/autoconfig.h中生效。
**4、 编译源码，生成rt73.ko**
/rt73/2009_0713_RT73_Linux_STA_Drv1.1.0.3/Module# make
        拷贝驱动到目标系统
        #cp rt73.ko /source/rootfs/rt73/
**5、 拷贝固件到目标系统**
#cp rt73.bin /source/rootfs/etc/Wireless/RT73STA/
        #cp rt73.rt73.dat /source/rootfs/etc/Wireless/RT73STA/
**二、 移植wpa_supplicant**
因为现在的无线wifi网络大多是wpa加密。所以需要移植wpa_supplicant工具。
**1、 下载源码**
http://hostap.epitest.fi/wpa_supplicant/
        下载wpa_supplicant-0.7.2.tar.gz （openssl用到0.7.2提供的补丁）
        下载wpa_supplicant-0.5.8.tar.gz （最终工具版本）
                #tar xvfz wpa_supplicant-0.7.2.tar.gz
                #tar xvfz wpa_supplicant-0.5.8.tar.gz
        下载www.openssl.org/source/openssl-0.9.8e.tar.gz
                #tar zxvf openssl-0.9.8e.tar.gz
**2、编译openssl库**
将wpa_supplicant中的补丁拷贝到openssl中
        # cp wpa_supplicant-0.7.2/patches/openssl-0.9.8e-tls-extensions.patch openssl-0.9.8e/
        #cd openssl-0.9.8e
        #mkdir /usr/local/ssl
        #vim Makefile
                CC= arm-softfloat-linux-gnu-gcc
                AR= arm-softfloat-linux-gnu- r $(ARFLAGS) r
                RANLIB= arm-softfloat-linux-gnu-ranlib
                INSTALLTOP=/usr/local/ssl
                OPENSSLDIR=/usr/local/ssl
        #make
        #make install
在/usr/local/ssl目录下安装了ssl库
**3、 编译wpa_supplicant **
参照rt73驱动源码包中的：2009_0713_RT73_Linux_STA_Drv1.1.0.3/ WPA_Supplicant-0.5.8/readme文件提示的步骤完成下面的操作。
进入pa_supplicant-0.5.8.tar.gz解压生成的wpa_supplicant-0.5.8目录中：
#cp ../2009_0713_RT73_Linux_STA_Drv1.1.0.3/WPA_Supplicant-0.5.8/driver_ralink.* ./
        #cp ../2009_0713_RT73_Linux_STA_Drv1.1.0.3/WPA_Supplicant-0.5.8/driver.h ./
        #cp ../2009_0713_RT73_Linux_STA_Drv1.1.0.3/WPA_Supplicant-0.5.8/events.c ./
        #cp ../2009_0713_RT73_Linux_STA_Drv1.1.0.3/WPA_Supplicant-0.5.8/wpa_supplicant.c ./
        #cp ../2009_0713_RT73_Linux_STA_Drv1.1.0.3/WPA_Supplicant-0.5.8/wpa_supplicant_i.h ./
        #cp ../2009_0713_RT73_Linux_STA_Drv1.1.0.3/WPA_Supplicant-0.5.8/defconfig ./
        #cp ../2009_0713_RT73_Linux_STA_Drv1.1.0.3/WPA_Supplicant-0.5.8/drivers.c ./
        #cp ../2009_0713_RT73_Linux_STA_Drv1.1.0.3/WPA_Supplicant-0.5.8/Makefile ./
        #cp defconfig        .config
        #vim .config
                CC= arm-softfloat-linux-gnu-gcc -L /usr/local/ssl/lib/
                CFLAGS += -I/usr/local/ssl/include/
                LIBS += -L/usr/local/ssl/lib/
        #make
生成wpa_supplicant
#cp wpa_supplicant /source/rootfs/bin
        #cp wpa_supplicant-0.7.2/wpa_supplicant/examples/wpa-psk-tkip.conf /source/rootfs/etc/wpa_supplicant.conf
        #vi /source/rootfs/etc/wpa_supplicant.conf
修改内容如下：
 # WPA-PSK/TKIP
ctrl_interface=/var/run/wpa_supplicant
network={
                ssid="farsight"        //填写无线网络的的用户名
                key_mgmt=WPA-PSK
                proto=WPA
                pairwise=TKIP
                group=TKIP
                psk="12345678" //填写密码
        }
在开发板终端：
#mkdir –p /var/run/wpa_supplicant
**4、 wireless_tools移植**
下载wireless_tools.28.tar.bz2
#tar xvfj wireless_tools.28.tar.bz2
        #cd wireless_tools.28
        #mkdir ../wireless_tools
        #vi Makefile
                PREFIX=$(PWD)/../wireless_tools
                CC= arm-softfloat-linux-gnu-gcc
                AR= arm-softfloat-linux-gnu-ar
        #make
        #make install
        #cp ../wireless_tools/lib/* /source/rootfs/lib/
        #cp ../wireless_tools/sbin/* /source/rootfs/sbin/
**5、 设置开发板启动脚本**
insmod /rt73/rt73.ko
        ifconfig rausb0 192.168.0.12
        /bin/wpa_supplicant -B -irausb0 –c /etc/wpa_supplicant.conf –Dralink
无线网卡将被激活。
