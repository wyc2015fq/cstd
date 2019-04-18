# 安装Qt及相关问题解决 - happyhorizon的算法天空 - CSDN博客
2017年09月20日 17:42:19[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：516
# 安装Qt及相关问题解决
Download Qt:
1.Qt下载
关于Qt下载，官网可以下载, 但是需要填一大堆信息, 非常麻烦. 可以打开下面的链接，里面有各版本Qt:
[http://download.qt.io/archive/qt/](http://download.qt.io/archive/qt/)
下载Qt5.5.1
2.安装
首先要cd进入软件安装包的目录，然后取得安装权限，进行图形化安装，如下：
```
$ cd /home/keith/soft
$ chmod +x qt-opensource-linux-x86-5.5.1.run
$ ./qt-opensource-linux-x86-5.5.1.run
```
3.安装过程几乎一路next，在选择时如果需要Assistant，想学习官方源码就select all就可以了。等待安装完成
4.Qt以C/C++为依托，用Python也可，先检查一下有没有gcc/g++编译器，没有的话动手安装.
运行官方示例程序如下：
![这里写图片描述](https://img-blog.csdn.net/20170920174132807?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGFwcHlob3Jpemlvbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
卸载：
1.进入Qt5.5.1的安装目录：例如我的目录：cd /home/xxx/SoftWare/Qt5.5.1；
2.运行命令：./MaintenanceTool；
3.进入图形化界面，按照操作卸载即可。
# Qt相关的编译错误及解决方案:
## 3D效果时报错
原因: 如果系统没有自带openGL库时，在做带有3D效果的界面会报错，手动安装openGL
解决方案:
sudo apt-get install mesa-common-dev
sudo apt-get install libglu1-mesa-dev -y
## 找不到-lGL
在ubuntu下使用Qt 编译时候遇上了cannot find -lGL错误
原因:缺少相应的库文件
解决方案:
使用命令
sudo apt-get install libgl1-mesa-dev或者libgl1-mesa-dev或者libglu1-mesa-dev
可以解决问题
## 找不到QT动态链接库
编译时出现类似下面的错误
```
Error:
//home/xxx/anaconda2/lib/libQt5Gui.so.5: undefined reference to `png_get_oFFs@PNG16_0'
//home/xxx/anaconda2/lib/libQt5Gui.so.5: undefined reference to `png_create_info_struct@PNG16_0'
//home/xxx/anaconda2/lib/libQt5Gui.so.5: undefined reference to `png_read_update_info@PNG16_0'
//home/xxx/anaconda2/lib/libQt5Gui.so.5: undefined reference to `png_write_image@PNG16_0'
//home/xxx/anaconda2/lib/libQt5Gui.so.5: undefined reference to `png_set_gamma@PNG16_0'
//home/xxx/anaconda2/lib/libQt5Core.so.5: undefined reference to `ucnv_getAlias_54'
//home/xxx/anaconda2/lib/libQt5Gui.so.5: undefined reference to `png_get_y_pixels_per_meter@PNG16_0'
//home/xxx/anaconda2/lib/libQt5Gui.so.5: undefined reference to `png_get_image_height@PNG16_0'
...
```
原因：QT安装在了用户目录下，当时没有配置环境变量，出现了链接错误：
解决方案：
　　利用export 命令设置环境变量：[Qt安装目录下/version number /gcc_64/lib]
　　在终端输入：export LD_LIBRARY_PATH=/home/xxx/Qt5.7.1/5.7/gcc_64/lib/
