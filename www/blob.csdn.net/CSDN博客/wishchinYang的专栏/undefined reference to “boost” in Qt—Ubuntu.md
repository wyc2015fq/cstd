# undefined reference to “boost” in Qt—Ubuntu - wishchinYang的专栏 - CSDN博客
2014年10月06日 12:42:22[wishchin](https://me.csdn.net/wishchin)阅读数：880
一：使用PCL时遇到的问题
       原因：缺少boost的lib包含；
在Pro文件里面添加：
        LIBS += -lboost_system
        .....................等库文件包含
二：编译Boost库
       对于不同的编译器使用不同库，必须通过源码编译：参考链接：[http://www.linuxidc.com/Linux/2013-07/87573.htm](http://www.linuxidc.com/Linux/2013-07/87573.htm)
在boost官方网站下载boost版本，我下载的是boost1.50，可以与PCL1.7.1配合
命令历史：     
```python
apt-get install mpi-default-dev　　#安装mpi库
apt-get install libicu-dev　　　　　#支持正则表达式的UNICODE字符集　
apt-get install python-dev　　　　　#需要python的话
apt-get install libbz2-dev　　　　　#如果编译出现错误：bzlib.h: No such file or directory
```
上述函数库装好之后，就可以编译boost库了。解压boost_1_49_0.tar.bz2，得到/boost_1_49_0，将当前工作目录切换到此文件夹下。
```python
./bootstrap.sh
using mpi ;　　#如果需要MPI功能，需要在 /tools/build/v2/user-config.jam 文件的末尾添加
```
利用生成的bjam脚本编译源代码了
```python
./b2 -a -sHAVE_ICU=1　　#-a参数，代表重新编译，-sHAVE_ICU=1代表支持Unicode/ICU
./bjam
./b2 install
```
结果：实验成功
