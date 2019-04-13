
# TLD（Tracking-Learning-Detection）学习与源码理解之（二） - zouxy09的专栏 - CSDN博客


2012年08月21日 20:17:04[zouxy09](https://me.csdn.net/zouxy09)阅读数：41970


**TLD****（****Tracking-Learning-Detection****）学习与源码理解之（二）**
zouxy09@qq.com
http://blog.csdn.net/zouxy09
OpenTLD下载与编译：
（1）https://github.com/arthurv/OpenTLD
下载得到：arthurv-OpenTLD-1e3cd0b.zip
或者在Linux下直接通过git工具进行克隆：
\#git clone git@github.com:alantrrs/OpenTLD.git
（2）我的编译环境是Ubuntu 12.04 + Opencv2.3
安装opencv 2.3：
\#apt-get install libcv-dev libcv2.3 libcvaux-dev libcvaux2.3 libhighgui-dev libhighgui2.3
安装cmake：
\#sudo apt-get install cmake
解压然后按照源码目录下README文件进行编译：
\#cd OpenTLD
\#mkdir build
\#cd build
\#cmake ../src/
\#make
\#cd ../bin/
（3）运行：
%To run from camera
./run_tld -p ../parameters.yml
%To run from file
./run_tld -p ../parameters.yml -s ../datasets/06_car/car.mpg
%To init bounding box from file
./run_tld -p ../parameters.yml -s ../datasets/06_car/car.mpg -b ../datasets/06_car/init.txt
%To train only in the firs frame (no tracking, no learning)
./run_tld -p ../parameters.yml -s ../datasets/06_car/car.mpg -b ../datasets/06_car/init.txt -no_tl
%To test the final detector (Repeat the video, first time learns, second time detects)
./run_tld -p ../parameters.yml -s ../datasets/06_car/car.mpg -b ../datasets/06_car/init.txt –r

