# CentOS安装python setuptools and pip - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月11日 21:24:41[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：366
## 安装setup-tools
wget [https://pypi.python.org/packages/2.7/s/setuptools/setuptools-0.6c11-py2.7.egg](https://pypi.python.org/packages/2.7/s/setuptools/setuptools-0.6c11-py2.7.egg)  --no-check-certificate
或
[https://yunpan.cn/cB3dDUIc6JAVC](https://yunpan.cn/cB3dDUIc6JAVC)  访问密码 9597
chmod +x setuptools-0.6c11-py2.7.egg
sh setuptools-0.6c11-py2.7.egg
## 安装pip
wget [https://pypi.python.org/packages/source/p/pip/pip-1.3.1.tar.gz](https://pypi.python.org/packages/source/p/pip/pip-1.3.1.tar.gz) --no-check-certificate
或
[https://yunpan.cn/cB3d42SfLSDib](https://yunpan.cn/cB3d42SfLSDib)  访问密码 d31c
tar zxvf pip-1.3.1.tar.gzcd pip-1.3.1python setup.py install
