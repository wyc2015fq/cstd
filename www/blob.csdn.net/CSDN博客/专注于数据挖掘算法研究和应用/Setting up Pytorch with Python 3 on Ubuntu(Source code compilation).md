# Setting up Pytorch with Python 3 on Ubuntu(Source code compilation) - 专注于数据挖掘算法研究和应用 - CSDN博客
2018年12月18日 11:47:48[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：95
1、安装yaml依赖：sudo apt-get install python-yaml python3-yaml
2、git clone pytorch 源码：
git clone https://github.com/pytorch/pytorch.git
git clone https://github.com/pytorch/vision.git
3、pytorch安装：进入pytorch目录
git submodule update --init --recursive
sudo python3 setup.py install
4、 pytorchvision安装：进入vision目录
sudo apt-get install cmake
sudo python3 setup.py install
5、python3进入验证：
import torch
import torchvision
