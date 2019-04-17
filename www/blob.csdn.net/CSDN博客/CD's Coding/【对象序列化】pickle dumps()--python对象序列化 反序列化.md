# 【对象序列化】pickle.dumps()--python对象序列化/反序列化 - CD's Coding - CSDN博客





2015年08月19日 11:18:27[糖果天王](https://me.csdn.net/okcd00)阅读数：4748








## 前言

在python编程中，我们往往能够发现，作为数据容器的某个对象每次都需要载入进缓存，然后繁复的程序运行之后，这个对象需要记录下来作下次使用。那么，存成文件，自然是大家想到最轻便简易的方法，但是文件读写那“相对较慢”的速度，令我们着实感受到了使用大型工具时“等待预热”的感觉。

然而，我们是可以通过dump（对对对你们这些爱显示隐藏文件的，是不是经常看见dump什么什么的呀，有可能其中就有那么一个两个用的是这样的机制呢）~

序列化：序列化 (Serialization)将对象的状态信息转换为可以存储或传输的形式的过程。在序列化期间，对象将其当前状态写入到临时或持久性存储区。以后，可以通过从存储区中读取或反序列化对象的状态，重新创建该对象。

[Python-Wiki](https://wiki.python.org/moin/UsingPickle)上看了不少Demo，确实是相当利于理解的解释。传送门：https://wiki.python.org/moin/UsingPickle




## 例程



```python
# Save a dictionary into a pickle file.
import pickle
favorite_color = { "lion": "yellow", "kitty": "red" }
pickle.dump( favorite_color, open( "save.p", "wb" ) )
```
这段代码的意思是，创建了一个存有（lion,yellow）和（kitty,red）两个键值对的字典，被序列化后存储为save.p文件中。




```python
# Load the dictionary back from the pickle file.
import pickle
favorite_color = pickle.load( open( "save.p", "rb" ) )
# favorite_color is now { "lion": "yellow", "kitty": "red" }
```
读取的时候像这样操作即可




往往用作存储dict，当然用来存储各种各样的对象应该都是可行的，不妨来试试吧~








