# 【Python学习系列七】Windows下部署Python推荐系统recsys - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月08日 12:04:53[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2947
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)










python-recsys是一个用来实现推荐系统的python库。




安装参考：http://ocelma.net/software/python-recsys/build/html/installation.html



1、安装Microsoft Visual C++ Compiler for Python 2.7

   下载地址：https://www.microsoft.com/en-us/download/details.aspx?id=44266

   下载VCForPython27.msi并安装

   否则直接安装recsys会提示：

![](https://img-blog.csdn.net/20170608120423942?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


2、安装依赖库：

   pip install csc-pysparse networkx divisi2


   这一步应该可以忽略，应该pip安装recsys时会安装相关依赖库。



3、安装recsys

   pip install recsys

   安装成功如下：

![](https://img-blog.csdn.net/20170608120432717?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



4、recsys使用

   参考：http://ocelma.net/software/python-recsys/build/html/index.html

   Github地址：https://github.com/ocelma/python-recsys

   可上去研究源码并应用推荐算法。




