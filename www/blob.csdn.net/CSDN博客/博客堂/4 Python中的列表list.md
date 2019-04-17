# 4 Python中的列表list - 博客堂 - CSDN博客





2017年03月08日 22:13:10[最小森林](https://me.csdn.net/u012052268)阅读数：1883标签：[python																[列表																[list](https://so.csdn.net/so/search/s.do?q=list&t=blog)
个人分类：[Python](https://blog.csdn.net/u012052268/article/category/6625160)





## 4 Python中的列表list



- - [Python中的列表list](#4-python中的列表list)

- [4-1 Python创建list](#4-1-python创建list)
- [4-2 Python按照索引访问list](#4-2-python按照索引访问list)
- [4-3 Python之倒序访问list](#4-3-python之倒序访问list)
- [4-4 Python之添加新元素](#4-4-python之添加新元素)
- [4-5 Python从list删除元素](#4-5-python从list删除元素)
- [4-6 Python中替换元素](#4-6-python中替换元素)




# 4-1 Python创建list

Python内置的一种数据类型是**列表**：**list**。list是一种有序的集合，可以随时添加和删除其中的元素。

比如，列出班里所有同学的名字，就可以用一个list表示：

```python
>>> ['Michael', 'Bob', 'Tracy']
['Michael', 'Bob', 'Tracy']
```

list是数学意义上的有序集合，也就是说，list中的元素是按照顺序排列的。

构造list非常简单，按照上面的代码，直接用 [ ] 把list的所有元素都括起来，就是一个list对象。

> 
通常，我们会把list赋值给一个变量，这样，就可以通过变量来引用list：


```python
>>> classmates = ['Michael', 'Bob', 'Tracy']
>>> classmates # 打印classmates变量的内容
['Michael', 'Bob', 'Tracy']
```

由于Python是动态语言，所以list中包含的元素并不要求都必须是同一种数据类型，我们完全可以在list中包含各种数据：

```python
>>> L = ['Michael', 100, True]
```

一个元素也没有的list，就是空list：
`>>> empty_list = []`
**任务**

假设班里有3名同学：Adam，Lisa和Bart，他们的成绩分别是 95.5，85 和 59，请按照 名字, 分数, 名字, 分数… 的顺序按照分数从高到低用一个list表示，然后打印出来。

**解答：**

```
L = ['adam',95.5,'lisa',85,'bart',59]
print L
```

# 4-2 Python按照索引访问list

由于list是一个有序集合，所以，我们可以用一个list按分数从高到低表示出班里的3个同学：

```python
>>> L = ['Adam', 'Lisa', 'Bart']
```

那我们如何从list中获取指定第 N 名的同学呢？方法是通过索引来获取list中的指定元素。

需要特别注意的是，索引从 0 开始，也就是说，第一个元素的索引是0，第二个元素的索引是1，以此类推。

因此，要打印第一名同学的名字，用 L[0]:

```python
>>> print L[0]
Adam
```

要打印第二名同学的名字，用 L[1]:

```python
>>> print L[1]
Lisa
```

要打印第三名同学的名字，用 L[2]:

```python
>>> print L[2]
Bart
```

要打印第四名同学的名字，用 L[3]:

```
>>> print L[3]
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
IndexError: list index out of range
```

报错了！IndexError意思就是索引超出了范围，因为上面的list只有3个元素，有效的索引是 0，1，2。

所以，使用索引时，千万注意**不要越界**。

**任务**

三名同学的成绩可以用一个list表示：

L = [95.5, 85, 59]

请按照索引分别打印出第一名、第二名、第三名、第四名的分数。

**答案**

```
L = [95.5,85,59]
print (L[0])
print (L[1])
print (L[2])
print (L[-1])
```

# 4-3 Python之倒序访问list

我们还是用一个list按分数从高到低表示出班里的3个同学：

```python
>>> L = ['Adam', 'Lisa', 'Bart']
```

这时，老师说，请分数最低的同学站出来。

要写代码完成这个任务，我们可以先数一数这个 list，发现它包含3个元素，因此，最后一个元素的索引是2：

```python
>>> print L[2]
Bart
```

有没有更简单的方法？

有！

Bart同学是最后一名，俗称倒数第一，所以，我们可以用 -1 这个索引来表示最后一个元素：

```python
>>> print L[-1]
Bart
```

Bart同学表示躺枪。

类似的，倒数第二用 -2 表示，倒数第三用 -3 表示，倒数第四用 -4 表示：

```python
>>> print L[-2]
Lisa
>>> print L[-3]
Adam
>>> print L[-4]
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
IndexError: list index out of range
```

L[-4] 报错了，因为倒数第四不存在，一共只有3个元素。

使用倒序索引时，也要注意**不要越界**。

**任务**

三名同学的成绩可以用一个list表示：

L = [95.5, 85, 59]

请按照倒序索引分别打印出倒数第一、倒数第二、倒数第三。

**答案：**

```
L = [95.5, 85, 59]
print (L[-1])
print (L[-2])
print (L[-3])
```

# 4-4 Python之添加新元素

```
append() 方法
insert()方法
```

现在，班里有3名同学：

```python
>>> L = ['Adam', 'Lisa', 'Bart']
```

今天，班里转来一名新同学 Paul，如何把新同学添加到现有的 list 中呢？

第一个办法是用 list 的 append() 方法，把新同学追加到 list 的末尾：

```python
>>> L = ['Adam', 'Lisa', 'Bart']
>>> L.append('Paul')
>>> print L
['Adam', 'Lisa', 'Bart', 'Paul']
```

append()总是把新的元素添加到 list 的尾部。

如果 Paul 同学表示自己总是考满分，要求添加到第一的位置，怎么办？

方法是用list的 insert()方法，它接受两个参数，第一个参数是索引号，第二个参数是待添加的新元素：

```python
>>> L = ['Adam', 'Lisa', 'Bart']
>>> L.insert(0, 'Paul')
>>> print L
['Paul', 'Adam', 'Lisa', 'Bart']
```

**L.insert(0, ‘Paul’)** 的意思是，’Paul’将被添加到索引为 0 的位置上（也就是第一个），而原来索引为 0 的Adam同学，以及后面的所有同学，都自动向后移动一位。

**任务**

假设新来一名学生Paul，Paul 同学的成绩比Bart好，但是比Lisa差，他应该排到第三名的位置，请用代码实现。

**解**

```
L = ['Adam', 'Lisa', 'Bart']
L.insert(2,'Paul')
print L
```

# 4-5 Python从list删除元素

Paul同学刚来几天又要转走了，那么我们怎么把Paul 从现有的list中删除呢？

如果Paul同学排在最后一个，我们可以用list的**pop()方法**删除：

```python
>>> L = ['Adam', 'Lisa', 'Bart', 'Paul']
>>> L.pop()
'Paul'
>>> print L
['Adam', 'Lisa', 'Bart']
```

pop()方法总是删掉list的最后一个元素，并且它还返回这个元素，所以我们执行 L.pop() 后，会打印出 ‘Paul’。

如果Paul同学不是排在最后一个怎么办？比如Paul同学排在第三：

```python
>>> L = ['Adam', 'Lisa', 'Paul', 'Bart']
```

要把Paul踢出list，我们就必须先定位Paul的位置。由于Paul的索引是2，因此，用 pop(2)把Paul删掉：

```python
>>> L.pop(2)
'Paul'
>>> print L
['Adam', 'Lisa', 'Bart']
```

**任务**

注意右边编辑器代码中 list 如下：

L = [‘Adam’, ‘Lisa’, ‘Paul’, ‘Bart’]

Paul的索引是2，Bart的索引是3，如果我们要把Paul和Bart都删掉，请解释下面的代码为什么不能正确运行：

L.pop(2)

L.pop(3)

怎样调整代码可以把Paul和Bart都正确删除掉？

**答：**

```
L = ['Adam', 'Lisa', 'Paul', 'Bart']
L.pop(3)
L.pop(2)
print L
```

# 4-6 Python中替换元素

list是可变的。

假设现在班里仍然是3名同学：

```python
>>> L = ['Adam', 'Lisa', 'Bart']
```

现在，Bart同学要转学走了，碰巧来了一个Paul同学，要更新班级成员名单，我们可以先把Bart删掉，再把Paul添加进来。

另一个办法是直接用Paul把Bart给替换掉：

```python
>>> L[2] = 'Paul'
>>> print L
L = ['Adam', 'Lisa', 'Paul']
```

对list中的某一个索引赋值，就可以**直接用新的元素替换掉**原来的元素，list包含的元素个数保持不变。

由于Bart还可以用 -1 做索引，因此，下面的代码也可以完成同样的替换工作：

```python
>>> L[-1] = 'Paul'
```

**任务**

班里的同学按照分数排名是这样的：

L = [‘Adam’, ‘Lisa’, ‘Bart’]

但是，在一次考试后，Bart同学意外取得第一，而Adam同学考了倒数第一。

请通过对list的索引赋值，生成新的排名。

**答案**

```
L = ['Adam', 'Lisa', 'Bart']
L.append(L.pop(0)) 
L.insert(0,L.pop(1))
print L
```](https://so.csdn.net/so/search/s.do?q=列表&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)




