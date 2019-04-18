# linux系统centOS6.5使用goaccess工具分析nginx网站日志 - weixin_33985507的博客 - CSDN博客
2017年07月17日 11:59:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：10
网站的log日志分析是每个站长经常做的必备工作，通过网站日志文件我们可以分析各大搜索引擎对网站的爬取情况。最近我的网站做了一些调整，所以想看下日志文件，但因为网站服务器环境是LNMP，所以我就找了一款nginx日志文件分析工具——goaccess。本文我们将一起分享如何在linux（centos）中安装goaccess来分析网站日志。
![5996435-0ae1390237bf1d6a.png](https://upload-images.jianshu.io/upload_images/5996435-0ae1390237bf1d6a.png)
### 准备工作：
系统：CentOS6.5(我在本地搭建的虚拟机)
web服务：nginx
日志文件：access.log文件(从自己的环境中拷贝具体日志文件)
工具：goaccess(官网：https://goaccess.io/)
### 安装流程：
安装goacess的一些支持库，如果后面操作出现问题，很有可能是因为某些库位安装，主要包括：glib2,glib2-devel,geoip,geoip-devel,ncurses-devel,zlib,zlib-devel,gcc。
yum install glib2 glib2-devel GeoIP-devel  ncurses-devel zlib zlib-develyum install gcc -y
如果是一键安装，注意安装过程中的提示，哪些未安装成功，需要单独安装，比如我在本地操作时，ncurses-devle、zlib、zlib-devel默认已安装，geo-ip就未安装成功，需要单独安装。
![5996435-945d49e375613699.png](https://upload-images.jianshu.io/upload_images/5996435-945d49e375613699.png)
cd /usr/local/src
wget http://geolite.maxmind.com/download/geoip/api/c/GeoIP-1.4.6.tar.gz
wget http://geolite.maxmind.com/download/geoip/database/GeoLiteCountry/GeoIP.dat.gz
tar xzvf GeoIP-1.4.6.tar.gz
cd GeoIP-1.4.6
./configure && make && make install
cd ..
mv GeoIP.dat.gz /usr/local/share/GeoIP/
接下来我们就可以安装goaccess了，这里我是用的编译安装：
wget http://tar.goaccess.io/goaccess-0.8.1.tar.gz
tar xzvf goaccess-0.8.1.tar.gz
cd goaccess-0.8.1/
./configure --enable-geoip --enable-utf8
make && make install
运行goaccess命令查看是否安装成功，结果我在本地安装时提示下面错误：
![5996435-b0f3d42d849d373e.png](https://upload-images.jianshu.io/upload_images/5996435-b0f3d42d849d373e.png)
32位系统使用下面命令解决：
ln -s /usr/local/lib/libGeoIP.so* /lib/
64位系统尝试使用下面命令解决：
ln -s /usr/local/lib/libGeoIP.so* /lib64/
再运行goaccess命令正常：
![5996435-5f4684d6e4975080.png](https://upload-images.jianshu.io/upload_images/5996435-5f4684d6e4975080.png)
### goaccess使用：
找到日志文件access.log所在目录，最简单直接的使用方法：
goaccess -f access.log -c -a
接着会提示如下界面，使用上下方向键选择第三个，用空格键确认选择，然后回车确定即可。
![5996435-5102bdf0237627df.png](https://upload-images.jianshu.io/upload_images/5996435-5102bdf0237627df.png)
当然也可以生成HTML报告，更为直观的查看数据。
goaccess -f access.log -a > report.html
本文我们分享了如何在linux 系统中安装goaccess来分析网站日志，如果大家在实际操作过程中遇到什么问题，欢迎一起交流讨论，我们共同学习，共同进步。
