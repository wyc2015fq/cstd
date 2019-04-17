# Redhat下 Boost库1.54编译安装 - 战斗蜗牛的专栏 - CSDN博客





2013年12月14日 11:17:18[vbskj](https://me.csdn.net/vbskj)阅读数：4207
个人分类：[Linux开发](https://blog.csdn.net/vbskj/article/category/643637)








a. 在 www.boost.org 下载 boost1.54 源码包，解压缩。

b. 进入目录后，运行 ./bootstrap.sh ,会生成一个 bjam 的可执行程序。


c. 运行 ./bjam 进行编译。等待...


d. 编译成功后，在 bjam 同级文件夹下，会有 stage、bin.v2 两个目录。bin.v2 中存的是编译时生成的目标文件，stage 中放的是 boost 的库文件。


e. 新建目录  /usr/share/boost_1_54/ 。把 stage、boost 两个目录拷贝至  /usr/share/boost_1_54/ 下。stage中为
 boost 库文件，boost中为 boost 头文件。


f. 修改 /etc/profile 文件，在后面添加三个环境变量：

- 
export BOOST_ROOT=/usr/share/boost_1_54
- 
export BOOST_INCLUDE=/usr/share/boost_1_54
- 
export BOOST_LIB=/usr/share/boost_1_54/stage/lib



   运行 source /etc/profile 使之生效。


g. 新建一个到头文件目录的链接：

   ln -s /usr/share/boost_1_43/boost/ /usr/include/boost 

h. 至此，boost 可用。




===============或者=============



linux平台上编译安装boost库

1.首先解压缩

      tar -zxvf boost.1.49.0.tar.gz 

2.然后进入解压缩后的文件夹编译boost的编译器jam

3.执行编译出bjam

     ./bootstrap.sh

4.编译完后在这个目录下有编译出的bjam文件

     bjam

5.进行编译

      ./bjam release install

      系统默认会将include拷贝到/usr/local/include/boost/中

                将lib拷贝到/usr/local/lib下

===============配置环境变量=============

1）~/.bash_profile中增加



export BOOST_ROOT=/usr/share/boost_1_54

export BOOST_INCLUDE=/usr/local/include

export BOOST_LIB=/usr/local/lib



2）/etc/ld.so.conf.d

中新建 ld.custom.so.conf



填入：/usr/lib64

/usr/local/lib



（注：第一个为其他动态库位置；第二个为boost动态库位置）



然后输入：ldconfig即可












