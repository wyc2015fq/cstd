# linux 编译安装TRMPdump(libRTMP) - DoubleLi - 博客园






需要编译libRTMP，首先需要安装配置编译环境。网上能够找到的资料多是在Windows环境编译。这里介绍一下在[Linux](http://lib.csdn.net/base/linux)系统中编译安装libRTMP，一来给后来者一个参考，二来也给自己做一个备忘录。

    编译libRTMP之前，需要安装下面三个软件：openssl , zlib, libssl-dev



**（一）openssl安装**

    这里需要特别的注意：openssl 版本不能太高，太高有些接口与libRTMP 的接口不一样，会导致libRTMP编译不能通过。我这里安装的是openssl-1.0.1f。



1、下载地址：[http://www.openssl.org/source/](http://www.openssl.org/source/) 下一个新版本的OpenSSL，我下的版本是：openssl-1.0.1f

2、在下载的GZ目录中，用命令执行：tar -xvf openssl-1.0.1f.tar.gz

3、进入解压的目录：openssl-1.0.1f  [.......]#cd openssl-1.0.1f

4、[.....openssl-1.0.1f]# ./config --prefix=/usr/local/openssl

5[...../openssl-1.0.1f]# ./config -t

6[...../openssl-1.0.1f]# make depend

7[...../openssl-1.0.1f]# cd /usr/local

8/usr/local]# ln -s openssl ssl

9 [...../openssl-1.0.1f]#sudo make install

10在/etc/ld.so.conf文件的最后面，添加如下内容：

/usr/local/openssl/lib

11...]# ldconfig

12添加OPESSL的环境变量：

在etc／的profile的最后一行，添加：

export OPENSSL=/usr/local/openssl/bin

export PATH=$OPENSSL:$PATH:$HOME/bin

13退出命令界面，再从新登录。

14、以上OPENSSL就安装完毕，下面进行一些检查。

15依次如下执行：

[root@localhost /]# cd /usr/local

[root@localhost local]# ldd /usr/local/openssl/bin/openssl

会出现类似如下信息：

[linux](http://lib.csdn.net/base/linux)-vdso.so.1 =>  (0x00007fff2116a000)
        libdl.so.2 => /lib/x86_64-linux-gnu/libdl.so.2 (0x00007f378e239000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f378de7c000)
        /lib64/ld-linux-x86-64.so.2 (0x00007f378e44f000)
16查看路径

...]# which openssl

/usr/local/openssl/bin/openssl

17查看版本

...]# openssl version

OpenSSL 1.0.1f 6 Jan 2014



**（二）zlib安装**

    安装的版本为zlib-1.2.11.tar.gz ,可直接安装

1 lcb@ubuntu:~/test/RTMP$ tar xvf zlib-1.2.11.tar.gz 

2 lcb@ubuntu:~/test/RTMP$ cd zlib-1.2.11/

3 lcb@ubuntu:~/test/RTMP/zlib-1.2.11$ sudo ./configure 

4 lcb@ubuntu:~/test/RTMP/zlib-1.2.11$ make

5 lcb@ubuntu:~/test/RTMP/zlib-1.2.11$ sudo make install



**（三）libssl-dev 安装**

    libssl-dev 的安装，可直接使用命令安装：

    sudo apt-get install libssl-dev



**（四）编译libRTMP**

1 添加openssl的头文件查找路径：在/etc/profile文件最后面添加上

C_INCLUDE_PATH=/usr/local/openssl/include/
export C_INCLUDE_PATH 
CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:/usr/local/openssl/include/
export CPLUS_INCLUDE_PATH

2 下载rtmpdump-2.3 可直接编译安装, rtmpdump主页： [http://rtmpdump.mplayerhq.hu/](http://rtmpdump.mplayerhq.hu/)

tar -xvf  rtmpdump-2.3.tgz

cd rtmpdump-2.3

make

make install

**完成**



**安装包:**

[openssl-1.0.1f.tar.gz](http://download.csdn.net/detail/li_wen01/9831772)

    zlib-1.2.11.tar.gz

    rtmpdump-2.3.tgz



注：上面安装方法在ubuntu-12.04 中安装成功，其他linux系统亦可参照









