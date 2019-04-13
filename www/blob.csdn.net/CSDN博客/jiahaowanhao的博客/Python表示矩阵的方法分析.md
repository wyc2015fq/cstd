
# Python表示矩阵的方法分析 - jiahaowanhao的博客 - CSDN博客


2018年04月27日 19:23:19[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：1264


[Python表示矩阵的方法分析](http://cda.pinggu.org/view/25385.html)
本文实例讲述了Python表示矩阵的方法。分享给大家供大家参考，具体如下：
在c语言中，表示个“整型3行4列”的矩阵，可以这样声明：int  a[3][4];在python中一不能声明变量int，二不能列出维数。可以利用列表中夹带列表形式表示。例如：
表示矩阵![](http://files.jb51.net/file_images/article/201705/2017526122157141.png?2017426122243)，可以这样：
count = 1
a = []
for i in range(0, 3):
tmp = []
for j in range(0, 3):
tmp.append(count)
count += 1
a.append(tmp)
print a
结果：
[[1, 2, 3], [4, 5, 6], [7, 8, 9]]
但是注意一点：初始化（赋值全部为0时），下面是错误的！！
tmp = []
for j in range(0, 3):
tmp.append(0)
a = []
for i in range(0, 3):
a.append(tmp)
print a
结果：
[[0, 0, 0], [0, 0, 0], [0, 0, 0]]
原因：这样的列表tmp为同一个，改变任意行，其他行都会给随着改变，千万注意！！，下面正确：
a = []
for i in range(0, 3):
tmp = []
for j in range(0, 3):
tmp.append(0)
a.append(tmp)
print a

