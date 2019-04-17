# linux的Anaconda环境下安装TensorFlow - 博客堂 - CSDN博客





2017年08月22日 20:49:59[最小森林](https://me.csdn.net/u012052268)阅读数：11834








[toc] 

现在tensorflow全面支持python3.6了，哈哈，全面拥抱python3.6~

下附官网安装方式参考： 
[https://github.com/tensorflow/tensorflow/blob/master/tensorflow/docs_src/install/install_linux.md#InstallingAnaconda](https://github.com/tensorflow/tensorflow/blob/master/tensorflow/docs_src/install/install_linux.md#InstallingAnaconda)

# 1. 建立专门的运行环境再安装

可以专门建立一个tensorflow的运行环境，好处是：1.官网建议 2.不与其他文件混乱

## （1）创建专门运行环境

为Anaconda创建一个python3.5（或者3.6都行，随便）的环境，环境名称为tensorflow ，输入下面命令：
`conda create -n tensorflow `
## （2）启动tensorflow的环境：

```bash
source activate tensorflow
```

注：当不使用tensorflow时，关闭tensorflow环境，命令为：

```bash
source deactivate tensorflow
```

## （3）安装cpu版本的TensorFlow
`pip install --upgrade --ignore-installed tensorflow tfBinaryURL`
where tfBinaryURL is the URL of the TensorFlow Python package. For example, the following command installs the CPU-only version of TensorFlow for Python 3.6:
`(tensorflow)$ pip install --ignore-installed --upgrade https://storage.googleapis.com/tensorflow/linux/cpu/tensorflow-1.3.0-cp36-cp36m-linux_x86_64.whl`
我觉的，也可以不写后面的地址。

## （4）测试是否安装完成

```python
import tensorflow as tf
hello = tf.constant('Hello, TensorFlow!')
sess = tf.Session()
print(sess.run(hello))
```

# 2. 直接在python3.6中安装

## （1）安装cpu版本的TensorFlow
`sudo pip install --upgrade --ignore-installed tensorflow`
## （2）测试是否安装完成

```python
import tensorflow as tf
hello = tf.constant('Hello, TensorFlow!')
sess = tf.Session()
print(sess.run(hello))
```

# 3 报错，权限不够

因为anaconda安装的时候用户默认为root，所以装其他的撞不上。

更改anaconda目录的用户为xqt（原来是root）否则会导致安装不上，权限不够。

```bash
sudo chown -R xqt:xqt anaconda3/
```



