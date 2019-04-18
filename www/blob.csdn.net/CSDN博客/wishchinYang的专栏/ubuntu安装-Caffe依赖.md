# ubuntu安装-Caffe依赖 - wishchinYang的专栏 - CSDN博客
2015年03月30日 19:54:29[wishchin](https://me.csdn.net/wishchin)阅读数：990
参考链接：[http://my.oschina.net/u/939893/blog/163921](http://my.oschina.net/u/939893/blog/163921)
1. 安装numpy相对简单，以下命令可以完成 
apt-get install python-numpy
apt-get install python-scipy
2. 安装matplotlib相对复杂一些
需要先安装其依赖的包libpng和freetype
安装libpng: 
sudo apt-get install libpng-dev
安装freetype: 
```python
<pre name="code" class="python">    mkdir ~/Downloads
```
 cd ~/Downloads wget http://download.savannah.gnu.org/releases/freetype/freetype-2.4.10.tar.gz tar zxvf freetype-2.4.10.tar.gz cd freetype-2.4.10/ ###############./congfigure make sudo make install
3. 安装pip，然后通过pip来安装matplotlib: sudo apt-get install python-pip安装好pip后就可以用下面的命令来查找matplotlib和查看其安装状态sudo pip search matplotlib安装matplotlibsudo pip install matplotlib
4. 安装Cython 等
```python
sudo pip install Cython
sudo pip install Cython
sudo pip install Cython
sudo pip install Cython
sudo pip install Cython
sudo pip install Cython
sudo pip install Cython
```

