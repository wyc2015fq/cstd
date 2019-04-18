# 【数据平台】Centos下仅CPU安装TensorFlow - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年12月11日 16:47:51[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：525
个人分类：[Big data](https://blog.csdn.net/fjssharpsword/article/category/6309934)

所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









1、安装命令：



```python
# Ubuntu/Linux 64-bit, CPU only, Python 2.7:
$ sudo pip install --upgrade https://storage.googleapis.com/tensorflow/linux/cpu/tensorflow-0.8.0-cp27-none-linux_x86_64.whl
```


![](https://img-blog.csdn.net/20171211164658954?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




2、简易测试：



```python
$ python

>>> import tensorflow as tf
>>> hello = tf.constant('Hello, TensorFlow!')
>>> sess = tf.Session()
>>> print sess.run(hello)
Hello, TensorFlow!
>>> a = tf.constant(10)
>>> b = tf.constant(32)
>>> print sess.run(a+b)
42
>>>
```

![](https://img-blog.csdn.net/20171211164740643?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



