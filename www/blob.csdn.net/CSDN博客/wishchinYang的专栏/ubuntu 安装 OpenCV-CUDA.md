# ubuntu 安装 OpenCV-CUDA - wishchinYang的专栏 - CSDN博客
2015年03月30日 17:22:22[wishchin](https://me.csdn.net/wishchin)阅读数：1802
                
参考链接：[http://www.cnblogs.com/platero/p/3993877.html](http://www.cnblogs.com/platero/p/3993877.html)
官方指导：[https://help.ubuntu.com/community/OpenCV](https://help.ubuntu.com/community/OpenCV)
0.这个尽量不要手动安装， Github上有人已经写好了完整的安装脚本：
https://github.com/jayrambhia/Install-OpenCV
下载该脚本，进入Ubuntu/2.4 目录, 给所有shell脚本加上可执行权限
```
chmod +x *.sh
```
然后安装最新版本 （当前为2.4.9）
```
sudo ./opencv2_4_9.sh
```
脚本会自动安装依赖项，下载安装包，编译并安装OpenCV。
整个过程大概半小时左右。 
注意，中途可能会报错
```
opencv-2.4.9/modules/gpu/src/nvidia/core/NCVPixelOperations.hpp(51): 
error: a storage class is not allowed in an explicit specialization
```
解决方法在此：[http://code.opencv.org/issues/3814](http://code.opencv.org/issues/3814)  下载 [NCVPixelOperations.hpp](http://code.opencv.org/projects/opencv/repository/revisions/feb74b125d7923c0bc11054b66863e1e9f753141/raw/modules/gpu/src/nvidia/core/NCVPixelOperations.hpp) 替换掉opencv2.4.9内的文件，
 重新build。
1.使用Apt-get install    //安装的不是特别完备！
Apt-get install python-opencv
apt-get install harpia  libcv2.4.8 libcvaux2.4.8  libhighgui2.4.8  libcv-dev libcvaux-dev libhighgui-dev opencv-doc python-opencv 
