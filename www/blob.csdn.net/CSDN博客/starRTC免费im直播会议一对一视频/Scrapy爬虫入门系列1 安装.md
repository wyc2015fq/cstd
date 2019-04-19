# Scrapy爬虫入门系列1  安装 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年04月27日 14:19:57[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：429
## 安装python2.7
参见[CentOS升级python 2.6到2.7](http://192.168.1.100/elesos_com/index.php?title=CentOS%E5%8D%87%E7%BA%A7python_2.6%E5%88%B02.7)
## 安装pip
参见[CentOS安装python setuptools and pip‎](http://192.168.1.100/elesos_com/index.php?title=CentOS%E5%AE%89%E8%A3%85python_setuptools_and_pip)
依赖
[https://docs.scrapy.org/en/latest/intro/install.html](https://docs.scrapy.org/en/latest/intro/install.html)
- [lxml](http://lxml.de/), an efficient XML and HTML parser
- [parsel](https://pypi.python.org/pypi/parsel), an HTML/XML data extraction library written on top of lxml,
- [w3lib](https://pypi.python.org/pypi/w3lib), a multi-purpose helper for dealing with URLs and web page encodings
- [twisted](https://twistedmatrix.com/), an asynchronous networking framework
- [cryptography](https://cryptography.io/) and [pyOpenSSL](https://pypi.python.org/pypi/pyOpenSSL),
 to deal with various network-level security needs

## 安装lxml
pip install lxml
如果报错：
AttributeError: 'module' object has no attribute 'HTTPSConnection'
需要在安装python之前先安装openssl,此外还要安装libxml2 and libxslt，以及libffi
yum install -y libxml2 libxml2-devel  libxslt libxslt-devel  libffi-devel  python-devel openssl-devel
pip install cryptography
pip install pyopenssl
pip install parsel
pip install twisted
安装Scrapy
pip install Scrapy
输入scrapy命令验证一下
报错：
Traceback (most recent call last):
  File "/usr/local/bin/scrapy", line 5, in <module>
    from pkg_resources import load_entry_point
  File "/usr/local/lib/python2.7/site-packages/setuptools-0.6c11-py2.7.egg/pkg_resources.py", line 2607, in <module>
  File "/usr/local/lib/python2.7/site-packages/setuptools-0.6c11-py2.7.egg/pkg_resources.py", line 565, in resolve
pkg_resources.DistributionNotFound: setuptools>=1.0
解决
pip install --upgrade scrapy
## 艺搜参考
[http://doc.scrapy.org/en/latest/intro/install.html#intro-install](http://doc.scrapy.org/en/latest/intro/install.html#intro-install)
