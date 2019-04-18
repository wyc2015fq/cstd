# 问题解决：matplotlib动画保存为mp4报错KeyError: 'ffmpeg' - happyhorizon的算法天空 - CSDN博客
2017年09月08日 14:09:39[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：791

**官网例子：basic_example_writer.py**
报错：
Writer = animation.writers['ffmpeg']
KeyError: 'ffmpeg'
**问题解决方案：安装ffmpeg**
ffmpeg是一套可以用来记录、转换数字音频、视频，并能将其转化为流的开源计算机程序。
** conda安装 **
如果是安装了anaconda，推荐采用标准安装方法：
conda install -c [https://conda.anaconda.org/menpo](https://conda.anaconda.org/menpo) ffmpeg
**从ffmpeg官网下载安装**
首先安装可能依赖的库：
先安装yasm
sudo apt-get install yasm  
- 
然后安装libx264
sudo apt-get install libx264-dev  
- 
然后安装ffmpeg,ffmpeg有许多依赖包,需要一个一个安装
- 
libfaac
sudo apt-get install libfaac-dev  
- 
libmp3lame
sudo apt-get  install libmp3lame-dev  
- 
libtheora
aptitude install libtheora-dev  
- 
libvorbis
sudo apt-get  install libvorbis-dev  
- 
libxvid
sudo apt-get  install libxvidcore-dev  
- 
libxext
sudo apt-get  install libxext-dev  
- 
libxfixes
sudo apt-get  install libxfixes-dev  
从ffmpeg官网上下载，解压后用终端进入文件夹：
./configure
make
make install
卸载还是进入该文件夹：
make uninstall 即可
