
# 5.5 Python列表 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月02日 06:49:35[seven-soft](https://me.csdn.net/softn)阅读数：186个人分类：[Python																](https://blog.csdn.net/softn/article/category/6290759)



像字符串类型一样，列表类型也是序列式的数据类型，可以通过下标或者切片操作来访问某一个或者某一块连续的元素。然而，相同的方面也就这些，字符串只能由字符组成，而且是不可变的，而列表则是能保留任意数目的Python 对象的灵活的容器。
列表不仅可以包含Python 的标准类型，而且可以用用户定义的对象作为自己的元素。
列表可以包含不同类型的对象，而且要比C 或者Python 自己的数组类型都要灵活，因为数组类型所有的元素只能是一种类型。
列表可以执行pop、empt、sort、reverse 等操作。
列表也可以添加或者减少元素，还可以跟其他的列表结合或者把一个列表分成几个。
可以对单独一个元素或者多个元素执行insert、update或者remove 操作。
元组与列表的主要不同在于元组是不可变的，或者说是只读的，所以那些用于更新列表的操作，比如用切片操作来更新一部分元素的操作，就不能用于元组类型。
## 创建列表并赋值
列表是由方括号([ ])来定义。如下面例子所示：list1=[123,'abc',4.56,7-9j]
list2=[123,'abc',4.56,['inner','list'],7-9j]
list3=[None,'something to see here']
list4=[]
print(list1)
print(list2)
print(list3)
print(list4)
print(list('foo'))

输出：
[123, 'abc', 4.56, (7-9j)]
[123, 'abc', 4.56, ['inner', 'list'], (7-9j)]
[None, 'something to see here']
[]
['f', 'o', 'o']
## 访问列表中的值
列表的切片操作就像字符串中一样，切片操作符([ ])和索引值或索引值范围一起使用。如下所示：list1=[123,'abc',4.56,['inner','list'],7-9j]
print(list1[0])
print(list1[1:4])
print(list1[:3])
print(list1[3][1])

输出：
123
['abc', 4.56, ['inner', 'list']]
[123, 'abc', 4.56]
list
## 更新列表
你可以通过在等号的左边指定一个索引或者索引范围的方式来更新一个或几个元素，你也可以用append()方法来追加元素到列表中去。如下所示：list1=[123,'abc',4.56,['inner','list'],7-9j]
list2=[None,'something to see here']
list3=[]
print(list1[2])
list1[2]='float replacer'
list2.append("hi, i'm new here")
print(list2)
list3.append('not empty anymore')
print(list3)

输出：
4.56
float replacer
[None, 'something to see here', "hi, i'm new here"]
['not empty anymore']
## 删除列表中的元素或者列表（本身）
要删除列表中的元素，如果你确切的知道要删除元素的素引可以用del 语句，否则可以用remove()方法。如下所示：list1=[123,'abc','float replacer',['inner','list'],(7-9j)]
dellist1[1]
print(list1)
list1.remove(123)
print(list1)

输出：
[123, 'float replacer', ['inner', 'list'], (7-9j)]
['float replacer', ['inner', 'list'], (7-9j)]
你还可以通过pop()方法来删除并从列表中返回一个特定对象。
一般来说，程序员不需要去删除一个列表对象。列表对象出了作用域（比如程序结束，函数调用完成等等）后它会自动被析构，但是如果你想明确的删除一整个列表，你可以用del 语句：dellist1


