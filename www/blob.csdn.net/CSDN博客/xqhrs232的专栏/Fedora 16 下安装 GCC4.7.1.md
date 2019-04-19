# Fedora 16 下安装 GCC4.7.1 - xqhrs232的专栏 - CSDN博客
2016年04月15日 11:34:03[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：193
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://www.linuxidc.com/Linux/2012-07/66808.htm](http://www.linuxidc.com/Linux/2012-07/66808.htm)
去[gcc官方网站](http://gcc.gnu.org/) 下载最新版本的gcc-4.7.0.tar.bz2，同时在infrastructure目录下寻找下载【必须】的mpc-0.8.1.tar.gz、mpfr-2.4.2.tar.bz2，去[http://gmplib.org/](http://gmplib.org/) 下载最新的gmp-5.0.4.tar.bz2
或者到这里下载
开始安装gcc 4.7.1 依赖包，以下包的安装步骤不能错乱
- tar jxvf gmp-5.0.4.tar.bz2  
- cd gmp-5.0.4  
- ./configure --prefix=/usr/local/gmp-5.0.4  
- make && make install  
- cd ../  
- 
- tar jxvf mpfr-2.4.2.tar.bz2  
- cd mpfr-2.4.2  
- ./configure --prefix=/usr/local/mpfr-2.4.2 --with-gmp=/usr/local/gmp-5.0.4  
- make && make install  
- cd ../  
- 
- tar zxvf mpc-0.8.1.tar.gz  
- cd mpc-0.8.1  
- ./configure --prefix=/usr/local/mpc-0.8.1  --with-gmp=/usr/local/gmp-5.0.4 --with-mpfr=/usr/local/mpfr-2.4.2  
- make && make install  
- cd ../  
库加入系统库路径，使用在/etc/ld.so.conf中添加或LD_LIBRARY_PATH环境变量中添加的方式
如在~/.bash_profile 中添加上下面语句
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/gmp-5.0.4/lib:/usr/local/mpfr-2.4.2/lib:/usr/local/mpc-0.8.1/lib
然后  source  ~/.bash_profile
安装gcc 4.7.1，此处的参数，只针对C、C++,fortran语言，如果需要其能编译出其他工具链(如all, ada, fortran, go, java, objc, obj-c++之类的)，在enable-language后面添加上，然后添加上相应的参数就是
mkdir gcc-4.7.1-built
cd gcc-4.7.1-built
../gcc-4.7.1/configure --with-gmp=/usr/local/gmp-5.0.4 --with-mpfr=/usr/local/mpfr-2.4.2 --with-mpc=/usr/local/mpc-0.8.1 --enable-languages=c,c++,fortran --enable-threads=posix --enable-__cxa_atexit --with-cpu=generic --disable-multilib   
然后 就是 make   make install 了
最后可以将 gcc 中bin  lib等目录加到相应等环境变量中。
