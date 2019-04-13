
# ubuntu16.04下安装kenlm - lpty的博客 - CSDN博客

2018年04月18日 21:00:24[lpty](https://me.csdn.net/sinat_33741547)阅读数：1559



## 一、背景
kenlm是一个语言模型工具，安装过程依赖较多，记录一下安装过程。
## 二、步骤
注意以下安装均在root用户下执行
### 1、boost
在boost官网下载boost：[http://www.boost.org](http://www.boost.org)，这里下载了boost 1.67
`cd boost
./bootstrap.sh
./b2 install`
### 2、xz
`wget http://tukaani.org/xz/xz-5.2.2.tar.gz
tar xzvf xz-5.2.2.tar.gz
cd xz-5.2.2
./configure
make
make install`
### 3、zlib
`wget http://zlib.net/zlib-1.2.11.tar.gz
tar xzf zlib-1.2.11.tar.gz
cd zlib-1.2.11
./configure
make
make install`
### 4、bzip
`wget https://fossies.org/linux/misc/bzip2-1.0.6.tar.gz
tar xzvf bzip2-1.0.6.tar.gz
cd bzip2-1.0.6/
make
make install`
### 5、libbz2-dev
`apt-get install libbz2-dev`
### 5、kenlm
在官网下载[http://kheafield.com/code/kenlm.tar.gz](http://kheafield.com/code/kenlm.tar.gz)，解压
`cd kenlm
mkdir build
cd build
cmake ..
make
python setup.py install`
## 三、测试
安装完成后，在python环境中执行
`import kenlm`或者
`cd /.../kenlm/python
python example.py`

