# ubuntu14.04配置caffe的python接口 - qq229873466的博客 - CSDN博客

2017年03月06日 19:01:05[qq229873466](https://me.csdn.net/qq229873466)阅读数：914



继[ubuntu14.04快速配置可用caffe环境（CPU）](http://blog.csdn.net/qq229873466/article/details/60571263) 后，继续安装python接口。

系统默认自带python2.7.6。

```cpp
cd ~/caffe-master/python

sudo apt-get install python-dev python-numpy python-scipy python-matplotlib python-sklearn python-skimage python-h5py python-protobuf python-leveldb python-networkx python-nose python-pandas python-gflags Cython ipython
sudo apt-get install protobuf-c-compiler protobuf-compiler
```

```cpp
cd ~/caffe-master
make pycaffe # 编译
```
添加~/caffe-master/python到$PYTHONPATH
```cpp
sudo gedit /etc/profile
# 添加： export PYTHONPATH=/path/to/caffe/python:$PYTHONPATH
$ source /etc/profile # 使之生效
```

参考：[Caffe学习：pycaffe接口配置](http://blog.csdn.net/u011762313/article/details/47624119)

