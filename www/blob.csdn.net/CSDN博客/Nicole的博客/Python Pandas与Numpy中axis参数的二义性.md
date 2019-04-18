# Python Pandas与Numpy中axis参数的二义性 - Nicole的博客 - CSDN博客
2018年04月08日 13:51:16[Nicole_Liang](https://me.csdn.net/weixin_39541558)阅读数：63标签：[axis																[pandas																[numpy](https://so.csdn.net/so/search/s.do?q=numpy&t=blog)](https://so.csdn.net/so/search/s.do?q=pandas&t=blog)](https://so.csdn.net/so/search/s.do?q=axis&t=blog)
个人分类：[python 数据处理](https://blog.csdn.net/weixin_39541558/article/category/7563702)
Stackoverflow.com是程序员的好去处，本公众号将以pandas为主题，开始一个系列，争取做到每周一篇，翻译并帮助pandas学习者一起理解一些有代表性的案例。今天的主题就是Pandas与Numpy中一个非常重要的参数：axis.(轴）
Stackoverflow问题如下：
python中的axis究竟是如何定义的呢？他们究竟代表是DataFrame的行还是列？考虑以下代码：
```
>>>df = pd.DataFrame([[1, 1, 1, 1], [2, 2, 2, 2], [3, 3, 3, 3]], \
columns=["col1", "col2", "col3", "col4"])
>>>df
   col1  col2  col3  col4
    0     1     1     1     1
    1     2     2     2     2
    2     3     3     3     3
```
如果我们调用df.mean(axis=1),我们将得到按行计算的均值
```
>>> df.mean(axis=1)
0    1
1    2
2    3
```
然而，如果我们调用 df.drop((name, axis=1),我们实际上删掉了一列，而不是一行：
```
>>> df.drop("col4", axis=1)
   col1  col2  col3
0     1     1     1
1     2     2     2
2     3     3     3
```
Can someone help me understand what is meant by an "axis" in pandas/numpy/scipy?
有人能帮我理解一下，在pandas、numpy、scipy三都当中axis参数的真实含义吗？
投票最高的答案揭示了问题的本质：
其实问题理解axis有问题，df.mean其实是在每一行上取所有列的均值，而不是保留每一列的均值。也许简单的来记就是axis=0代表往跨行（down)，而axis=1代表跨列（across)，作为方法动作的副词（译者注）
换句话说:
- 使用0值表示沿着每一列或行标签\索引值向下执行方法
- 使用1值表示沿着每一行或者列标签模向执行对应的方法
下图代表在DataFrame当中axis为0和1时分别代表的含义:
![](http://upload-images.jianshu.io/upload_images/2233157-b77105789e36c847.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
axis参数作用方向图示
另外，记住，Pandas保持了Numpy对关键字axis的用法，用法在Numpy库的词汇表当中有过解释：
轴用来为超过一维的数组定义的属性，二维数据拥有两个轴：第0轴沿着行的垂直往下，第1轴沿着列的方向水平延伸。
所以问题当中第一个列子 df.mean(axis=1)代表沿着列水平方向计算均值，而第二个列子df.drop(name, axis=1) 代表将name对应的列标签（们）沿着水平的方向依次删掉。
