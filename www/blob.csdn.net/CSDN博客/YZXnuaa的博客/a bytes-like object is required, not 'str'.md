# a bytes-like object is required, not 'str' - YZXnuaa的博客 - CSDN博客
2018年04月16日 16:37:15[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：243
# TypeError: a bytes-like object is required, not 'str'
## 问题分析
该问题主要是由于当前操作的字符串是bytes类型的字符串对象，并对该bytes类型的字符串对象进行按照str类型的操作。
如下面图所示，s 为bytes类型字符串对象。
![](https://img-blog.csdn.net/20161105204438204)
当对s进行按照str类型的操作（split）时，会弹出一下错误提示。因为split函数传入的参数是str类型的字符串，而党鞭的s为bytes编码类型的字符串对象
![](https://img-blog.csdn.net/20161105204722564)
## 解决方法
此时的解决办法是，将s转码成为str类型的或者是将对s进行str类型操作转化为对bytes类型的操作。
方法1：bytes-->str
![](https://img-blog.csdn.net/20161105205208286)
方法2：对s进行str类型操作转化为对bytes类型的操作
由于split里面用到分解字符串中的是空格‘  ’，但是该‘ ’是按照str编码的，需要转化成为bytes类型的，然后再直接对s进行对bytes类型的操作。
![](https://img-blog.csdn.net/20161105205754584)
## str和bytes类型之间的常用转码方式：
1、str to bytes:(3种方式）
![](https://img-blog.csdn.net/20161105212247419)
2、bytes to str (3种方式）
![](https://img-blog.csdn.net/20161105212657689)
