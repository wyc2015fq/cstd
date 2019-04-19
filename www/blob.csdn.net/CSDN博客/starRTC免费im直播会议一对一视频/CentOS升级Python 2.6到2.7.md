# CentOS升级Python 2.6到2.7 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年10月11日 18:01:12[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：240
查看python的版本
python  -VPython 2.6.6 
下载Python   Python-2.7.8.tar.xz      链接：[http://pan.baidu.com/s/1i4cCIR7](http://pan.baidu.com/s/1i4cCIR7) 密码：j570
[https://yunpan.cn/cB3ddbtUE8yeu](https://yunpan.cn/cB3ddbtUE8yeu)  访问密码 0e70
[https://www.python.org/ftp/python/2.7.8/Python-2.7.8.tgz](https://www.python.org/ftp/python/2.7.8/Python-2.7.8.tgz)
yum -y install gcc automake autoconf libtool makeyum install zlib-develyum install -y openssl*yum install -y bzip2*
安装
tar -xvf Python-2.7.8.tar.xz./configure  make all             
make install  make clean  
make distclean 
查看版本信息
/usr/local/bin/python2.7 -V  
Python 2.7.8
建立软连接，使系统默认的 python指向 python2.7
mv /usr/bin/python /usr/bin/python2.6.6  ln -s /usr/local/bin/python2.7 /usr/bin/python
重新检验Python 版本
python  -VPython 2.7.8
解决Python软链接指向 Python2.7 版本后，yum不能正常工作的问题。因为yum是不兼容 Python 2.7的，所以需要指定 yum 的Python版本
vim /usr/bin/yum  
将文件头部的
#!/usr/bin/python
改成
#!/usr/bin/python2.6.6
## 艺搜参考
[https://www.python.org/ftp/python/2.7.8/](https://www.python.org/ftp/python/2.7.8/)
[https://www.python.org/download/releases/2.7.8/](https://www.python.org/download/releases/2.7.8/)
