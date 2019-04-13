
# python求列表交集的方法汇总 - jiahaowanhao的博客 - CSDN博客


2018年07月08日 22:03:32[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：1447


[python求列表交集的方法汇总](http://cda.pinggu.org/view/26035.html)
本文实例汇总了python求列表交集的方法。分享给大家供大家参考。具体方法如下：
交集对于给定的两个集合A 和 集合B 的交集是指含有所有既属于 A 又属于 B 的元素，而没有其他元素的集合叫交集了，下面给出几个python求列表交集例子供大家参考。
方法1
遍历b1,如果某个元素同时也存在于b2中,则返回
代码如下:
b1=[1,2,3]
b2=[2,3,4]
b3 = [val for val in b1 if val in b2]
print b3
运行结果如下
代码如下:
[2, 3]
方法2
把列表转换为集合,利用集合操作符求出交集,然后再转换回列表类型
代码如下:
b1=[1,2,3]
b2=[2,3,4]
b3=list(set(b1) & set(b2))
print b3
运行结果如下
代码如下:
[2, 3]
方法3
前面的例子中两个list都是简单的单元素列表,还有一种比较特殊的情况,就是有嵌套类型的
代码如下:
b1=[1,2,3]
b2=[[2,4],[3,5]]
b3 = [filter(lambda x: x in b1,sublist) for sublist in b2]
print b3
运行结果如下
代码如下:
[2, 3]
希望本文所述对大家的Python程序设计有所帮助。

