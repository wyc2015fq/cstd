# 【Python】python如何将自己写的代码打包供他人使用 - zkq_1986的博客 - CSDN博客





2018年08月31日 15:46:17[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：807








# [python如何将自己写的代码打包供他人使用](https://www.cnblogs.com/smileyes/p/7657591.html)

背景： 利用setuptools来实现，所以先安装setuptools，其实Python2.7.9及之后的版本都自带安装setuptools了，无需在另外安装

如果没有安装setuptools的直接下载源码包解压安装就行了

示例：以linux环境为例

1.创建文件

创建文件结构如下：

setuptools_demo/
├── pkDemo
│   └── __init__.py
└── setup.py

setup.py内容如下：

from setuptools import find_packages,setup
setup(
    name = 'pkDemo',
    version = '0.1',
    packages = find_packages(),
)
__init__.py的内容如下：

![复制代码](https://common.cnblogs.com/images/copycode.gif)

#!/usr/bin/env python
#-*- coding:utf-8 -*-

def demo():
    print "This is a demo package!"  

if __name__ == '__main__':
    demo()
![复制代码](https://common.cnblogs.com/images/copycode.gif)

2.执行打包命令

cd setuptools_demo
python setup.py bdist_egg
出现如下界面说明打包成功了

![](https://images2017.cnblogs.com/blog/752966/201710/752966-20171012184055371-1712352938.png)

再次查看文件结构如下：

![](https://images2017.cnblogs.com/blog/752966/201710/752966-20171012184213652-269856579.png)

3.执行命令安装包

setuptools_demo]#python setup.py install
出现如下界面说明安装成功了

![](https://images2017.cnblogs.com/blog/752966/201710/752966-20171012184414121-1864701750.png)

4.导入包并使用

![复制代码](https://common.cnblogs.com/images/copycode.gif)

python
----------------------------------------------------
Python 2.7.12 (default, Sep  1 2016, 22:14:00) 
[GCC 4.8.3 20140911 (Red Hat 4.8.3-9)] on linux2
Type "help", "copyright", "credits" or "license" for more information.
>>> import pkDemo
>>> pkDemo.demo()
This is a demo package!
![复制代码](https://common.cnblogs.com/images/copycode.gif)

完美~~

PS：这只是一个很简单的例子，当然你将你做的代码打包上传到某个网盘，供其他人下载安装使用



转载自：[https://www.cnblogs.com/smileyes/p/7657591.html](https://www.cnblogs.com/smileyes/p/7657591.html)



