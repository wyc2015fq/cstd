# Windows7+Python3.6.1+Anaconda 4.4.10成功安装opencv - qq_37385726的博客 - CSDN博客





2018年08月22日 14:45:07[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：87
个人分类：[七里八里](https://blog.csdn.net/qq_37385726/article/category/6706622)









# **目录**



[安装在anaconda下：](#%E5%AE%89%E8%A3%85%E5%9C%A8anaconda%E4%B8%8B%EF%BC%9A)

[单纯的安装opencv：](#%E5%8D%95%E7%BA%AF%E7%9A%84%E5%AE%89%E8%A3%85opencv%EF%BC%9A)

# 安装在anaconda下：

下面是三个方法，如果第一个不行就试第二个，不行就第三个
- conda install opencv
- conda install -c [https://conda.binstar.org/menpo](https://conda.binstar.org/menpo) opencv 找到如下文件夹Anaconda\pkgs\opencv-2.4.9.1-np19py27_0\Lib\site-packages 
- 把所有的文件复制到： Anaconda\Lib\site-packages 在Anaconda的网站上（[https://conda.anaconda.org](https://conda.anaconda.org/)）下载安装包。 使用本地安装命令： conda install –use-local opencv-2.4.11-py27_1.tar.bz2



# 单纯的安装opencv：

直接使用pip install opencv-python命令



