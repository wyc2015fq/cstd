# Auto install opencv after installing Ubuntu - 数据之美的博客 - CSDN博客
2017年03月18日 16:59:30[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：209
个人分类：[opencv																[linux(ubuntu)](https://blog.csdn.net/oppo62258801/article/category/6738406)](https://blog.csdn.net/oppo62258801/article/category/6796594)
## Here is the shell code
```bash
#!/bin/bash
sudo apt-get install -y build-essential
sudo apt-get install -y cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
sudo apt-get install -y python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev
cd opencv
mkdir release
cd release
cmake -D WITH_IPP=OFF -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D OPENCV_EXTRA_MODULES_PATH=/home/duino/src/opencv/opencv_contrib/modules ..
make -j5
sudo make install
cd ~/
git clone https://github.com/duinodu/testopencvinstall.git
cd testopencvinstall
sudo chmod +x auto.sh
./auto.sh
cd build
./DisplayImage ../test.jpg
echo ""
echo "opencv install successfully"
echo ""
cd ~
rm -rf testopencvinstall
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
## How to do
- step1: copy this into a file named installOpencv.sh
- step2:
```bash
sudo chmod +x installOpencv.sh
```
- 1
- 1
- step3:
```php
git clone https://github.com/Itseez/opencv.git
git clone https://github.com/Itseez/opencv_contrib.git
```
- 1
- 2
- 1
- 2
- 
step4: check your file directory, it should be like this: 
—–installOpencv.sh 
—–[OpenCV](http://lib.csdn.net/base/opencv)
—–opencv_contrib
- 
step5: edit installOpencv.sh, change OPENCV_EXTRA_MODULES_PATH to your own.
- 
step6: ./installOpencv.sh
You may need to input password when sudo make install, because compiling opencv costs much time.
You may notice that there is a test demo for opencv. It is simple test of using opencv to show an image. After installation, this test demo will be deleted.
## Version 2
```bash
#!/bin/bash
sudo apt-get install -y build-essential
sudo apt-get install -y cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
sudo apt-get install -y python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev
git clone https://github.com/Itseez/opencv.git
git clone https://github.com/Itseez/opencv_contrib.git
cd opencv
mkdir release
cd release
cmake -D WITH_IPP=OFF -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D OPENCV_EXTRA_MODULES_PATH=/home/duino/src/opencv/opencv_contrib/modules ..
make -j5
sudo make install
cd ~/
git clone https://github.com/duinodu/testopencvinstall.git
cd testopencvinstall
sudo chmod +x auto.sh
./auto.sh
cd build
./DisplayImage ../test.jpg
echo ""
echo "opencv install successfully"
echo ""
cd ~
rm -rf testopencvinstall
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
In this shell, you still need to change OPENCV_EXTRA_MODULES_PATH to your own. It will [Git](http://lib.csdn.net/base/git) clone
 opencv src automatically.
## Problem
- “Unable to fetch http://….” You need to change your apt source in /etc/apt/sources.list. Different version of ubuntu uses different nickname, and the others are the same. For 16.04, it is xenial. Without essential packages
 correctly installed, it will get error when compiling opencv. It is important.
