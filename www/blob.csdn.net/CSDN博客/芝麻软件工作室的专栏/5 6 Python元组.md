
# 5.6 Python元组 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月02日 06:50:24[seven-soft](https://me.csdn.net/softn)阅读数：175个人分类：[Python																](https://blog.csdn.net/softn/article/category/6290759)



实际上元组是跟列表非常相近的另一种容器类型。元组和列表看起来不同的一点是元组用的是圆括号而列表用的是方括号。而功能上，元组和列表相比有一个很重要的区别，元组是一种不可变类型。
由于元组类型跟列表类型有着如此多的共同之处，为了避免太多重复信息，我们会讲解元组和列表在应用时的区别，然后讨论一下元组的不变性以及其他独特的特性。
## 创建一个元组并给它赋值
创建一个元组并给他赋值实际上跟创建一个列表并给它赋值完全一样，除了一点，只有一个元素的元组需要在元组分割符里面加一个逗号(,)用以防止跟普通的分组操作符混淆。aTuple=(123,'abc',4.56,['inner','tuple'],7-9j)
anotherTuple=(None,'something to see here')
print(aTuple)
print(anotherTuple)
emptiestPossibleTuple=(None,)
print(emptiestPossibleTuple)
print(tuple('bar'))

输出：
(123, 'abc', 4.56, ['inner', 'tuple'], (7-9j))
(None, 'something to see here')
(None,)
('b', 'a', 'r')
## 访问元组中的值
元组的切片操作跟列表一样，用方括号作为切片操符([])，里面写上索引值或者索引范围。aTuple=(123,'abc',4.56,['inner','tuple'],7-9j)
print(aTuple[1:4])
print(aTuple[:3])
print(aTuple[3][1])

输出：
('abc', 4.56, ['inner', 'tuple'])
(123, 'abc', 4.56)
tuple
## 更新元组
跟数字和字符串一样，元组也是不可变类型，就是说你不能更新或者改变元组的元素，前面，我们是通过现有字符串的片段再构造一个新字符串的方式解决的，对元组同样需要这样。aTuple=(123,'abc',4.56,['inner','tuple'],7-9j)
aTuple=aTuple[0],aTuple[1],aTuple[-1]
print(aTuple)
tup1=(12,34.56)
tup2=('abc','xyz')
tup3=tup1+tup2
print(tup3)

输出：
(123, 'abc', (7-9j))
(12, 34.56, 'abc', 'xyz')
## 移除一个元组的元素以及元组本身
删除一个单独的元组元素是不可能的，当然，把不需要的元素丢弃后，重新组成一个元组是没有问题的。
要显示地删除一整个元组，只要用del 语句减少对象引用计数。当这个引用计数达到0 的时候，该对象就会被析构。记住，大多数时候，我们不需要显式的用del 删除一个对象，一出它的作用域它就会被析构，Python 编程里面用到显式删除元组的情况非常之少。delaTuple


