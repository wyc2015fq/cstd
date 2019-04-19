# Linux下OpenSSL的安装全过程(CentOS6.3 x86 + Openssl 1.1.0e) - 建建的博客 - CSDN博客
2018年01月26日 16:30:20[纪建](https://me.csdn.net/u013898698)阅读数：1041
写在前面：安装前先查看下老的openssl版本 （使用  openssl version -a  命令查看来版本），和服务器操作系统版本（uname   -a），升级的原因是，老版的openssl已经曝出漏洞，建议升级最新版本
1、下载地址：http://www.openssl.org/source/ 下一个新版本的OpenSSL，我下的版本是：[openssl-1.1.0e.tar.gz](https://www.openssl.org/source/openssl-1.1.0e.tar.gz)
1.1、这个地方：如果服务器可上网，也可以使用在线下载：（因为我使用的服务器，无法上网，所以直接去官网下载tar包）
wget  http://www.openssl.org/source/openssl-1.1.0e.tar.gz
2、将下载的tar包，上传到服务器根目录下，用解压命令执行：tar -zxvf   openssl-1.1.0e.tar.gz
3、进入解压的目录：#cd  /openssl-1.1.0e
4、[root@localhost openssl-1.1.0e]# 
**[plain]**[view plain](http://blog.csdn.net/lu_yonggang/article/details/62041422#)[copy](http://blog.csdn.net/lu_yonggang/article/details/62041422#)
- ./config shared zlib  --prefix=/usr/local/openssl && make && make install  
上面的命令，是安装openssl到  /usr/local/openssl 目录，安装之后，编译；这个时候执行需要有耐心，可以查看命令行一直在滚屏，等到滚屏结束，安装编译完成；
5、[root@localhost openssl-1.1.0e]# ./config -t
6、[root@localhost openssl-1.1.0e]# make depend
7、[root@localhost openssl-1.1.0e]# cd /usr/local
8、[root@localhost openssl-1.1.0e]# ln -s openssl ssl
9、在/etc/ld.so.conf文件的最后面，添加如下内容：
     /usr/local/openssl/lib
10、[root@localhost openssl-1.1.0e]# ldconfig
11、添加OPESSL的环境变量：
      在etc／的profile的最后一行，添加：
      export OPENSSL=/usr/local/openssl/bin
      export PATH=$OPENSSL:$PATH:$HOME/bin
12、退出命令界面，再从新登录。
13、移除老版本的openssl，创建新的软连接；这个地方注意路径
mv /usr/bin/openssl /usr/bin/openssl.old
mv /usr/include/openssl /usr/include/openssl.old
ln -s /usr/local/openssl/bin/openssl /usr/bin/openssl
ln -s /usr/local/openssl/include/openssl /usr/include/openssl
ln -sf /usr/local/openssl/lib/libcrypto.so.1.0.0 /lib/libcrypto.so.6
echo "/usr/local/openssl/lib" >>/etc/ld.so.conf 
ldconfig -v
14、依次如下执行：
[root@localhost openssl-1.1.0e]#  ldd /usr/local/openssl/bin/openssl
会出现类似如下信息：
        linux-gate.so.1 =>  (0x0079f000)
        libssl.so.1.1 => /usr/local/openssl/lib/libssl.so.1.1 (0x002a8000)
        libcrypto.so.1.1 => /usr/local/openssl/lib/libcrypto.so.1.1 (0x00306000)
        libz.so.1 => /lib/libz.so.1 (0x00775000)
        libdl.so.2 => /lib/libdl.so.2 (0x00725000)
        libpthread.so.0 => /lib/libpthread.so.0 (0x0072c000)
        libc.so.6 => /lib/libc.so.6 (0x00593000)
        /lib/ld-linux.so.2 (0x0056d000)
15、查看路径
...]# which openssl
/usr/local/openssl/bin/openssl
16、查看版本
...]# openssl version
OpenSSL 1.1.0e  16 Feb 2017
17、测试新版本的OpenSSL是否正常工作
[root@localhost ssl]# openssl
OpenSSL> version -a
OpenSSL 1.1.0e  16 Feb 2017
built on: reproducible build, date unspecified
platform: linux-x86
compiler: ......(这里隐藏部分信息)
OPENSSLDIR: "/usr/local/openssl/ssl"
ENGINESDIR: "/usr/local/openssl/lib/engines-1.1"
出现以上信息，说明openssl已经升级完成，可以看到版本信息，
至此openssl升级完成
方法也是参考了网上的部分办法，
            
