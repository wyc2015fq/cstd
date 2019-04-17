# 为了编译AVX代码，升级Redhat 5.5 GCC到4.7.1 - 战斗蜗牛的专栏 - CSDN博客





2014年08月08日 00:47:04[vbskj](https://me.csdn.net/vbskj)阅读数：2820








    Redhat 的GCC编译器是4.1版本，对于SSE4，AVX，AVX2的支持不够好，官方建议4.7以上。因此开始了GCC升级之路。

    由于Redhat 5.5是在虚拟机下安装的，网上的升级够略都不能完全解决，经过若干尝试，下面过程可行。

    主要参考如下：

http://blog.sciencenet.cn/blog-714229-600604.html


http://blog.sciencenet.cn/blog-714229-600389.html


http://blog.rekfan.com/articles/13.html




    GCC下载源：http://ftp.gnu.org/gnu/gcc/

     yum（注册）下载源：http://mirror.centos.org/centos-5/5.10/os/x86_64/CentOS/




      一、安装GCC的依赖库

（1）install gmp
      下载gmp: http://gmplib.org/
       # mkdir -p /opt/gmp-4.3.2
    #tar -jxvf gmp-4.3.2.tar.bz2
    #cd gmp-4.3.2
    #./configure --prefix=/opt/gmp-4.3.2
    #make && make check && make install
(2)install mpfr
  下载mpfr http://www.mpfr.org/mpfr-current/#download
     ＃mkdir -p /opt/mpfr-2.4.2
    #tar -jxvf mpfr-2.4.2.tar.bz2
    #cd mpfr-2.4.2
    #./configure --prefix=/opt/mpfr-2.4.2 --with-gmp=/opt/gmp-4.3.2
    #make && make check && make install
（3）install mpc
     下载 http://www.multiprecision.org/index.php?prog=mpc&page=download
#mkdir -p /opt/mpc-0.8.1
    #tar -zxvf mpc-0.8.1.tar.gz
    #cd mpc-0.8.1
    #./configure --prefix=/opt/mpc-0.8.1 --with-gmp=/opt/gmp-4.3.2 --with-mpfr=/opt/mpfr-2.4.2
    #make && make check && make install
（4） build gcc
    #export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/gmp-4.3.2/lib:/opt/mpfr-2.4.2/lib:/opt/mpc-0.8.1/lib


二、使用Yum安装 guile

(1)  卸载rhel的默认安装的yum包

查看yum包

rpm -qa|grep yum

卸载之

rpm -qa|grep yum|xargs rpm -e --nodeps



(2)  下载新的yum包

64位linux系统下载安装包

wget http://mirror.centos.org/centos-5/5.10/os/x86_64/CentOS/yum-3.2.22-40.el5.centos.noarch.rpm

wget http://mirror.centos.org/centos-5/5.10/os/x86_64/CentOS/yum-fastestmirror-1.1.16-21.el5.centos.noarch.rpm

wget http://mirror.centos.org/centos-5/5.10/os/x86_64/CentOS/yum-metadata-parser-1.1.2-4.el5.x86_64.rpm




然后进行安装

rpm -ivh yum-*

注意：yum和yum-fastestmirror相互依赖，所以同时安装即可。

(3)  下载yum的配置源

wget http://docs.linuxtone.org/soft/lemp/CentOS-Base.repo 下载到 /etc/yum.repos.d/ 目录下面



(4)   运行yum makecache生成缓存





(5)  yum install guile 




三、install
 autogen

     安装过程中可能缺少libxml2-devel, guile-devel, 使用yum install xx 进行升级即可




    下载  http://rpm.pbone.net/index.php3/stat/26/dist/55/size/1314516/name/autogen-5.8.9-1.el5.kb.src.rpm
   .src.rpm 安装方法
    ＃rpm -i you-package.src.rpm 
    ＃cd /usr/src/redhat/SPECS (前两步和方法一相同) 
  ＃ rpmbuild -bb your-package.specs 一个和你的软件包同名的specs文件


  这时，在/usr/src/redhat/RPM/i386/ （根据具体包的不同，也可能是i686,noarch等等) 在这个目录下，有一个新的rpm 包，这个是编译好的二进制文件。执行：


 ＃rpm -i new-package.rpm即可安装完成。引用 http://blog.csdn.net/lphpc/article/details/565823


四、安装GCC4.7

（1）# mkdir -p /opt/gcc-4.7
         #mkdir gcc_build  （在GCC源码目录下）


  (2)  ../configure
 --prefix=/opt/gcc-4.7 -enable-threads=posix -disable-checking -disable-multilib -enable-languages=c,c++ --with-gmp=/opt/gmp-4.3.2
 --with-mpfr=/opt/mpfr-2.4.2 --with-mpc=/opt/mpc-0.8.1

 
  (3)  成功后，产生Makefile文件，然后直接make，ok-----等待-----

（4） make
 install




若想重新编译 make clean 清除上次编译遗留下的文件


uninstallation 


若旧版本是 rpm版的，查看版本号 gcc --version 然后 rpm -e 删除了


若旧版是用源码安装的，whereis gcc 或者 which gcc 来查看所在路径，删除即可


接着设置工具链:


命令文件符号连接到 /usr/bin 目录下：
- 

#  cd /usr/bin

- 

#  ln -sv /usr/local/gcc-4.7.1/bin/gcc gcc  （如果出现重复，直接删除原始gcc即可）

- 

#  ln -sv /usr/local/gcc-4.7.1/bin/g++ g++

- 





库文件设置：


将/usr/local/gcc-4.7.1/lib 路径添加到环境变量LD_LIBRARY_PATH中
- 

#vi /etc/profile



添加下列代码


setenv LD_LIBRARY_PATH  /usr/local/gcc-4.7.1/lib:$LD_LIBRARY_PATH


清理现场
- 

# rm -rf gcc-4.7.1

- 

# rm -rf gcc-b 


 到此安装完毕
可以 用 gcc -v 查看是4.7.1版本不





