# torch问题及解决方案汇总（持续更新） - xmdxcsj的专栏 - CSDN博客





2015年10月20日 21:04:50[xmucas](https://me.csdn.net/xmdxcsj)阅读数：2610








一、libjpeg.so库

问题描述：torch/install/lib/lua/5.1/libjpeg.so: undefined symbol: jpeg_mem_src

解决方案：[https://github.com/torch/image/issues/41](https://github.com/torch/image/issues/41)

wget http://www.ijg.org/files/jpegsrc.v8d.tar.gz

tar -xvf jpegsrc.v8d.tar.gz

cd jpeg-8d/

sudo ./configure

sudo make

sudo make install




git clone https://github.com/torch/distro.git ~/torch --recursive

cd ~/torch

./install.sh




二、itorch

问题描述：Unsupported ipython version. Only major versions 2.xx, 3.xx, or 4.xx (Jupyter) are supported


解决方案使用itorch模块可以画图，但是需要ipython2.2,（要求python2.7版本以上），由于系统使用的版本是python2.6，所以可以选择使用gnuplot代替

sudo yum intsall gnuplot44











