# 如何从源码安装最新版本git，简明教程 - alw2009的博客 - CSDN博客
2019年01月01日 12:26:28[traveler_zero](https://me.csdn.net/alw2009)阅读数：70
0、准备
`sudo yum install -y gcc gcc-c++ zlib zlib-devel perl-ExtUtils-MakeMaker curl curl-devel`
1、下载git
下载地址：[https://mirrors.edge.kernel.org/pub/software/scm/git/](https://mirrors.edge.kernel.org/pub/software/scm/git/)
```
wget https://mirrors.edge.kernel.org/pub/software/scm/git/git-2.9.5.tar.gz
tar xf git-2.9.5.tar.gz
mkdir -p ~/local/git
./configure --prefix=$HOME/local/git
make && make install
```
2、设置环境变量
```
export PATH=$HOME/local/git/bin:$PATH
git --version
```
