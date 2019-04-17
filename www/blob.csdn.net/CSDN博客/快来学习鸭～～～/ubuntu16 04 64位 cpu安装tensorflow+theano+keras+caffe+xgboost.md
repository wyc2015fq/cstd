# ubuntu16.04 64位 cpu安装tensorflow+theano+keras+caffe+xgboost - 快来学习鸭～～～ - CSDN博客





2017年02月08日 14:46:25[DivinerShi](https://me.csdn.net/sxf1061926959)阅读数：3153








配置：Ubuntu16.04 64位 cpu版 

送上两个中文文档 

keras中文文档 
[http://keras-cn.readthedocs.io/en/latest/](http://keras-cn.readthedocs.io/en/latest/)

tensorflow中文文档 
[http://wiki.jikexueyuan.com/project/tensorflow-zh/how_tos/reading_data.html](http://wiki.jikexueyuan.com/project/tensorflow-zh/how_tos/reading_data.html)
**

## **以下各个框架可分开单独安装**

**

## 安装tensorflow

(1) 先安装pip
`1.sudo apt-get install python-pip python-dev`
（2）用pip安装tensorflow
`2.sudo pip install --upgrade https://storage.googleapis.com/tensorflow/linux/cpu/tensorflow-0.8.0-cp27-none-linux_x86_64.whl`
等待一段时间，到提示安装成功 

（3）检查tensorflow是否安装成功

```python
$ python
...
>>> import tensorflow as tf
>>> hello = tf.constant('Hello, TensorFlow!')
>>> sess = tf.Session()
>>> print(sess.run(hello))
Hello, TensorFlow!
>>> a = tf.constant(10)
>>> b = tf.constant(32)
>>> print(sess.run(a + b))
42
```

## 安装keras

安装开发包，打开终端输入：

```bash
#系统升级
>>> sudo apt update
>>> sudo apt upgrade
# 安装python基础开发包
>>> sudo apt install -y python-dev python-pip python-nose gcc g++ git gfortran vim
```

安装运算加速库，打开终端输入：
`>>> sudo apt install -y libopenblas-dev liblapack-dev libatlas-base-dev`
## keras框架搭建

终端输入：

```
>>> sudo pip install -U --pre pip setuptools wheel
>>> sudo pip install -U --pre numpy scipy matplotlib scikit-learn scikit-image
>>> sudo pip install -U --pre theano
>>> sudo pip install -U --pre keras
```

安装完毕后，输入python，然后输入：

```python
>>> import theano
>>> import keras
```

如果没有任何提示，则表明安装成功，这里一起安装了**theano**

## keras环境设置

修改默认keras后端（backend），在终端输入：
`>>> gedit ~/.keras/keras.json`
配置theano文件 在终端中输入:

```python
>>> gedit ~/.theanorc
```

并写入以下：

```
[global]
openmp=True 
device = cpu 
floatX = float32  
allow_input_downcast=True  
[blas]
ldflags= -lopenblas
```

之后可以验证keras是否安装成功,在命令行中输入Python命令进入Python变成命令行环境：

```java
>>>import keras
```

没有报错，并且会打印出关于显卡信息以及cnmem等信息（CPU版本没有）那么Keras就已经成功安装了。

## 安装caffe

第一部分: 安装dependencies

```
sudo apt-get install libprotobuf-dev libleveldb-dev libsnappy-dev libopencv-dev libhdf5-serial-dev protobuf-compiler
sudo apt-get install --no-install-recommends libboost-all-dev
sudo apt-get install libatlas-base-dev
sudo apt-get install libhdf5-serial-dev
```

PYTHON需要2.7版本,这是操作系统本身已经安装好的. 输入python2.7 –version 会显示具体的版本号说明安装了. 

但是还需要

```
sudo apt-get install python-dev
sudo apt-get install libgflags-dev libgoogle-glog-dev liblmdb-dev
```

然后把CAFFE的源代码下载下来: 

```php
git clone https://github.com/BVLC/caffe.git
```

(**当然没有安装GIT的得先安装一下**) 

下载完成之后,进入CAFFE文件夹, 进入里面的PYTHON文件夹

```bash
>>>cd caffe
>>>cd python
```

然后输入

```bash
for req in $(cat requirements.txt); do pip install $req; done
```

(PIP如果没有安装得先安装一下:**sudo apt install python-pip**) 

注意：如果网速较慢的，可以修改pip的源，参考如下： 
[http://blog.csdn.net/sxf1061926959/article/details/54091748](http://blog.csdn.net/sxf1061926959/article/details/54091748)
## 第二部分: 安装CAFFE

到CAFFE文件夹下：
`cp Makefile.config.example Makefile.config`
打开Makefile.config

```
vi Makefile.config
```

**-因为CPU MODE, 所以在CPU_ONLY := 1前面的#要去掉.**

-两个路径要改成这样:(添加后面的两个hdf5的路径, 否则编译时报hdf5错误)

```
# Whatever else you find you need goes here.
INCLUDE_DIRS := $(PYTHON_INCLUDE) /usr/local/include /usr/include/hdf5/serial
LIBRARY_DIRS := $(PYTHON_LIB) /usr/local/lib /usr/lib /usr/lib/x86_64-linux-gnu/hdf5/serial
```

最后:

```
make pycaffe
make all
make test
make runtest
```

结果显示ALL TESTS PASSED就安装好了, 只需要再加上一个PYTHONPATH; 

## 第三部分: 设置 Python Caffe

去到CAFFE文件夹里面的python文件夹, 把当前路径记录下来(pwd). 然后输入以下命令(把记下的路径放在相应地方)
`export PYTHONPATH=/path/to/caffe/python:$PYTHONPATH`
这时候应该可以了,试验一下:

```
$ python2.7
Python 2.7.12 (default, Jul  1 2016, 15:12:24) 
[GCC 5.4.0 20160609] on linux2
Type "help", "copyright", "credits" or "license" for more information.
>>> import caffe
>>>>
```

说明安装全部完成!同样方法可以配置matlab接口

## xgboost安装
`1.sudo -H pip install --pre xgboost`
会安装一些需要的模块，等待一段时间，显示如下消息表示安装成功 

Successfully built xgboost 

Installing collected packages: xgboost 

Successfully installed xgboost-0.6a2
在终端输入如下，不报错则安装成功

```
shi@ThinkStation-P300:~/桌面$ python
Python 2.7.12 (default, Nov 19 2016, 06:48:10) 
[GCC 5.4.0 20160609] on linux2
Type "help", "copyright", "credits" or "license" for more information.
>>> import xgboost
/usr/local/lib/python2.7/dist-packages/sklearn/cross_validation.py:44: DeprecationWarning: This module was deprecated in version 0.18 in favor of the model_selection module into which all the refactored classes and functions are moved. Also note that the interface of the new CV iterators are different from that of this module. This module will be removed in 0.20.
  "This module will be removed in 0.20.", DeprecationWarning)
>>>
```

我这里还报了个警告，正常情况不显示东西：

```
shi@ThinkStation-P300:~/桌面$ python
Python 2.7.12 (default, Nov 19 2016, 06:48:10) 
[GCC 5.4.0 20160609] on linux2
Type "help", "copyright", "credits" or "license" for more information.
>>> import xgboost
>>>
```

完






