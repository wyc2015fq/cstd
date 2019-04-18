# 如何在Linux上安装最新版本的gcc - alw2009的博客 - CSDN博客





2018年12月30日 16:48:16[traveler_zero](https://me.csdn.net/alw2009)阅读数：20标签：[gcc																[c++11/14/17](https://so.csdn.net/so/search/s.do?q=c++11/14/17&t=blog)](https://so.csdn.net/so/search/s.do?q=gcc&t=blog)
个人分类：[工具](https://blog.csdn.net/alw2009/article/category/8564835)








为了在Linux上使用c++17, 我安装了gcc 8.2, 以下是安装步骤：

**一、下载安装包**

```
wget https://ftp.gnu.org/gnu/gcc/gcc-8.2.0/gcc-8.2.0.tar.gz
wget https://ftp.gnu.org/gnu/gmp/gmp-4.3.2.tar.gz
wget https://ftp.gnu.org/gnu/mpfr/mpfr-2.4.2.tar.gz
wget https://ftp.gnu.org/gnu/mpc/mpc-1.0.1.tar.gz
```

**二、编译安装**

```
mkdir ~/local/gcc

tar xf gmp-4.3.2.tar.gz
cd gmp-4.3.2
sudo yum install -y m4
./configure --prefix=$HOME/local/gcc
make && make install
ls ~/local/gcc/lib/

cd ..
tar xf mpfr-2.4.2.tar.gz
cd mpfr-2.4.2
./configure --prefix=$HOME/local/gcc --with-gmp=$HOME/local/gcc
make && make install
ls ~/local/gcc/lib/

cd ..
tar xf mpc-1.0.1.tar.gz
cd mpc-1.0.1
./configure --prefix=$HOME/local/gcc --with-gmp=$HOME/local/gcc --with-mpfr=$HOME/local/gcc
make && make install
ls ~/local/gcc/lib/

cd ..
tar xf gcc-8.2.0.tar.gz
cd gcc-8.2.0

./configure --prefix=$HOME/local/gcc --with-gmp=$HOME/local/gcc --with-mpfr=$HOME/local/gcc --with-mpc=$HOME/local/gcc --disable-multilib
export LD_LIBRARY_PATH=$HOME/local/gcc/lib:$LD_LIBRARY_PATH
make && make install
```



**三、测试**

```
export PATH=$HOME/local/gcc/bin:$PATH
gcc --version
```






