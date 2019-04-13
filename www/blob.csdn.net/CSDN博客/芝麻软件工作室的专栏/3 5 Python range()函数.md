
# 3.5 Python range()函数 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月01日 07:31:12[seven-soft](https://me.csdn.net/softn)阅读数：2220


如果你需要一个数值序列，使用内建函数range() 会很方便，它产生等差级数序列。
以下代码生成长度为5的数列：foriinrange(5):
print(i)

输出：
0
1
2
3
4
注意：默认情况下，range() 的起始值是 0。
当然，可以让range() 的起始值定为某一个数，也可以指定一个不同的增量（甚至可以为负；有时这被称为“步长”）。例如：range(5,10)\# 5, 6, 7, 8, 9
range(0,10,3)\# 0, 3, 6, 9
range(-10,-100,-30)\# -10 , -40 , -70

要对一个序列的索引进行迭代的话，组合使用range() 和len()：a=['Mary ','had','a','little ','lamb']
foriinrange(len(a)):
print(i,a[i])

输出：
0 Mary
1 had
2 a
3 little
4 lamb

