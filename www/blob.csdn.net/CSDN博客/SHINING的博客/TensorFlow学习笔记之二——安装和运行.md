# TensorFlow学习笔记之二——安装和运行 - SHINING的博客 - CSDN博客
2016年05月09日 13:48:09[snsn1984](https://me.csdn.net/snsn1984)阅读数：13660
所属专栏：[Tensorflow学习](https://blog.csdn.net/column/details/tensorflow.html)
通过阅读TensorFlow的论文和相关博文之后，接下来，对TensorFlow进行一个简单的初步运行。
1、安装了Ubuntu 16.04.(ubuntu-16.04-desktop-amd64.iso)
2、sudo apt-get update
3、# Ubuntu/Linux 64-bit
     $ sudo apt-get install python-pip python-dev
4、# Ubuntu/Linux 64-bit, CPU only:
     $ sudo pip install --upgrade https://storage.googleapis.com/tensorflow/linux/cpu/tensorflow-0.8.0-cp27-none-linux_x86_64.whl
5、测试安装结果：
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
>>>
```
6、测试结果运行正常，结合之前的论文，可以在这里明显的看到Session的用法，包括它的run()接口。
