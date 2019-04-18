# Caffe: Caffe的Python接口 - wishchinYang的专栏 - CSDN博客
2015年04月06日 21:35:27[wishchin](https://me.csdn.net/wishchin)阅读数：2675
官方参考：[http://caffe.berkeleyvision.org/installation.html](http://caffe.berkeleyvision.org/installation.html)
#### 官方介绍是这样的：
#### Python
The main requirements are `numpy` and `boost.python` (provided by boost).`pandas` is useful too and needed for some examples.
You can install the dependencies with：
```
for req in $(cat requirements.txt); do pip install $req; done
```
but we suggest first installing the [Anaconda](https://store.continuum.io/cshop/anaconda/) Python distribution, which provides most of the necessary packages, as well as the`hdf5` library dependency.
To import the `caffe` Python module after completing the installation, add the module directory to your`$PYTHONPATH` by`export PYTHONPATH=/path/to/caffe/python:$PYTHONPATH` or the like. You should not import the module in
 the`caffe/python/caffe` directory!
1. 对于：for req in $(cat requirements.txt); do pip install $req; done
这句shell，可能会运行无效，要把requriments 里面的软件要求顺序变更一下，安装完其他包后，依次安装ipython和h5dy；
2. 添加caffe的python包含路径：
你所添加的路径是：export PYTHONPATH=/home/wishchin/caffe-master/python:$PYTHONPATH
                        而非：export PYTHONPATH=/home/wishchin/caffe-master/python/caffe:$PYTHONPATH
修改后update一下，或者重启，可以在任一项目中import caffe
3.对于Eclipse，可以在preference-python-Interpretor里面直接添加路径
**注意事项**： 若是第一次在Eclipse中引用出现错误。Don't panic！ 重启一下eclipse就可以了。
若想在一个工程中引用Caffe，暂时没有想到可行的方法.....................................................
**后记**： 在eclipse工程中添加引用目录，直接import caffe
