
# boost库linux编译安装 - 高科的专栏 - CSDN博客

2013年08月14日 10:41:23[高科](https://me.csdn.net/pbymw8iwm)阅读数：6942


0.下载
1.解压boost_1_49_0.tar.g然后放到/opt/
2. 进入解压后的文件夹
cd /opt/boost_1_49_0
3.将boost安装配置在/boost/prefix目录下
不过之前先
mkdir -p/boost/prefix
mkdir
 -p/boost/include
mkdir
 -p/boost/lib
./bootstrap.sh --prefix=/boost/prefix  --includedir=/boost/include   --libdir=/boost/lib
/boost/prefix,/boost/include,/boost/lib为我自己建的目录，boost装在自己建的目录下便于管理。其中--prefix表示程序安装在哪个目录，--includedir表示头文件存放的目录，--libdir表示库文件的目录。
4.安装boost
./bjam install
5.由于库文件没有安装在默认目录下，所以必须显示的表明。
我们可以在/etc/ld.so.conf中修改。
vi /etc/ld.so.conf
vi /etc/ld.so.conf 增加/usr/local/boost/boost_1_53_0/stage/lib
然后执行ldconfig更新ld.so.cache文件。


