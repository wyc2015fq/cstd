
# Linux下编译安装gcc - lpty的博客 - CSDN博客

2018年11月08日 17:19:47[lpty](https://me.csdn.net/sinat_33741547)阅读数：62标签：[gcc																](https://so.csdn.net/so/search/s.do?q=gcc&t=blog)个人分类：[杂七杂八																](https://blog.csdn.net/sinat_33741547/article/category/7513215)



# 引言
1、gcc下载地址：[ftp://ftp.mirrorservice.org/sites/sourceware.org/pub/gcc/releases](ftp://ftp.mirrorservice.org/sites/sourceware.org/pub/gcc/releases)
2、GMP、MPFR、MPC下载地址：[ftp://gcc.gnu.org/pub/gcc/infrastructure/](ftp://gcc.gnu.org/pub/gcc/infrastructure/)
# 步骤
1、安装gcc需要依赖GMP、MPFR、MPC， 而MPFR依赖GMP，而MPC依赖GMP和MPFR，所以要先安装GMP，其次MPFR，最后才是MPC。
2、下载gcc5.5.0、gmp4.3.2、mpfr2.4.2、mpc0.8.1，均放在/usr/local下
3、切换到root用户
4、安装gmp4.3.2
`# tar jxvf gmp-4.3.2.tar.bz2
# cd gmp-4.3.2
# ./configure --prefix=/usr/local/gmp-4.3.2
# make
# make install`5、安装mpfr2.4.2
`# tar jxvf mpfr-2.4.2.tar.bz2
# cd mpfr-2.4.2
# ./configure --prefix=/usr/local/mpfr-2.4.2 --with-gmp=/usr/local/gmp-4.3.2 
# make
# make install`6、安装mpc0.8.1
`# tar -zxf mpc-0.8.1.tar.gz	
# cd mpc-0.8.1
# ./configure --prefix=/usr/local/mpc-0.8.1 --with-gmp=/usr/local/gmp-4.3.2 --with-mpfr=/usr/local/mpfr-2.4.2
# make
# make install`7、添加环境变量
`export LD_LIBRARY_PATH="/usr/local/mpc-0.8.1/lib:/usr/local/gmp-4.3.2/lib:/usr/local/mpfr-2.4.2/lib:$LD_LIBRARY_PATH"`8、安装gcc
`# tar -zxf gcc-5.5.0.tar.gz
# cd gcc-5.5.0
# ./configure --prefix=/usr/local/gcc-5.5.0 -enable-threads=posix -disable-checking -disable-multilib -enable-languages=c,c++ --with-gmp=/usr/local/gmp-4.3.2 --with-mpfr=/usr/local/mpfr-2.4.2 --with-mpc=/usr/local/mpc-0.8.1
# make -j 4
# make install`9、添加gcc到环境变量
`export LD_LIBRARY_PATH="/usr/local/mpc-0.8.1/lib:/usr/local/gmp-4.3.2/lib:/usr/local/mpfr-2.4.2/lib:/usr/local/gcc-5.5.0/lib64:$LD_LIBRARY_PATH"`

