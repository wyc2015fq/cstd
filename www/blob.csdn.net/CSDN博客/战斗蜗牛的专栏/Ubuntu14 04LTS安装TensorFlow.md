# Ubuntu14.04LTS安装TensorFlow - 战斗蜗牛的专栏 - CSDN博客





2016年08月03日 21:23:43[vbskj](https://me.csdn.net/vbskj)阅读数：12307








1. Pip


如果已经安装过pip，可以跳过这里的安装步骤。


```
$
 sudo apt-get install python-pip python-dev python-setuptools build-essential

$ sudo pip install --upgrade pip

$ sudo pip install --upgrade virtualenv
```



为了检测是否安装好，可以查看pip的版本：


```
$
 pip --version
```


pip 8.1.2 from /usr/local/lib/python2.7/dist-packages (python 2.7)

2. Tensorflow


接下来，就可以按照Tensorflow Download and Setup中的Pip Installation开始安装，命令如下：


# Ubuntu/Linux 64-bit, CPU only:

```
$
 sudo pip install --upgrade https://storage.googleapis.com/tensorflow/linux/cpu/tensorflow-0.8.0-cp27-none-linux_x86_64.whl
```




# Ubuntu/Linux 64-bit, GPU enabled. Requires CUDA toolkit 7.5 and CuDNN v4. For

# other versions, see “Install from sources” below.

```
$
 sudo pip install --upgrade https://storage.googleapis.com/tensorflow/linux/gpu/tensorflow-0.8.0-cp27-none-linux_x86_64.whl
```


3. Errors

安装完成后，就可以测试，是否安装成功：


```
$ python

>>> import tensorflow as tf

>>>
```



如果输入命令后，结果跟上述的差不多，（GPU版的会有一些log信息），没有出现Error，那就说明安装好了，就可以测试使用啦。


在tensorflow/tools/pip_package/setup.py中，关于几个包（numpy , six, protobuf）的版本等都有规定，如果不满足，则编译代码就会报错：


```
#
 tensorflow version

_VERSION = '0.8.0'
```



REQUIRED_PACKAGES = [

‘numpy >= %s’ % numpy_version,

‘six >= 1.10.0’,

‘protobuf == 3.0.0b2’,


查看这些包的命令行是：


```
$
 pip show numpy protobuf six
```



出问题比较多的是protobuf和six两个包，例如：


protobuf

from google.protobuf import symbol_database as _symbol_database

ImportError: cannot import name symbol_database


protobuf如果已经安装了其他版本（例如python-protobuf或低版本），先要卸载掉：


```
$
 sudo apt-get remove python-protobuf

$ sudo pip uninstall protobuf
```



然后安装：


```
$
 sudo pip install --upgrade protobuf
```



six

AttributeError: type object ‘NewBase’ has no attribute ‘is_abstract’


出现这个问题，应该是six包安装有问题，可以卸载原有版本，重新安装：


```
$
 sudo pip uninstall six

$ sudo pip install six --upgrade
```



一般来说six包的安装位置是/usr/lib/python2.7/dist-packages，但是我在笔记本上默认安装在/usr/local/lib/python2.7/dist-packages也没问题，建议先试前者（工作站上也是前者）：


```
$
 sudo pip install six --upgrade --target="/usr/lib/python2.7/dist-packages"
```


4. Uninstall


如果想卸载掉安装的tensorflow，非常简单：


```
$
 sudo pip uninstall tensorflow
```




