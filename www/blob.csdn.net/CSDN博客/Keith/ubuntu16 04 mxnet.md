# ubuntu16.04  mxnet - Keith - CSDN博客





2016年09月15日 00:49:59[ke1th](https://me.csdn.net/u012436149)阅读数：1906










```php

```
最近一直在配置深度学习的几个框架，坑无限多，在这里记录一下配置mxnet的过程。

首先：安装依赖包



```php
sudo apt-get update
sudo apt-get install -y build-essential git libatlas-base-dev libopencv-dev
```


第二步：下载+编译





```php
git clone --recursive https://github.com/dmlc/mxnet
cd mxnet; make -j$(nproc)
```


这里有一些问题， 我第一次使用的是gcc-4.9，在编译的时候一直报错，推荐使用gcc-5.4（我就是这么成功的）。如果你想使用GPU的话，需要将 make/configure 文件copy到mxnet主目录进行修改 `USE_CUDA=1`    并将 CUDA_PATH = /usr/local/cuda （我没有使用GPU，因为前几次用gcc-4.9编译的时候伤心了，你们大可试一下）

第三步：让你的python找到mxnet



```php
sudo apt-get install python-setuptools
sudo apt-get install python-numpy
cd python   //   mxnet/python
sudo python setup.py install

最后：
vim $HOME/.bashrc 
在此文件的最后添加
export PYTHONPATH=~/mxnet/python
保存，退出
然后 source $HOME/.bashrc
```


OK了









