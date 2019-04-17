# 10 列表生成式 / list的for生成式 / for的扩展应用 - 博客堂 - CSDN博客





2017年03月10日 09:32:13[最小森林](https://me.csdn.net/u012052268)阅读数：571








## 10 列表生成式 / list的for生成式 / for的扩展应用



- - [列表生成式  list的for生成式  for的扩展应用](#10-列表生成式-list的for生成式-for的扩展应用)

- [10-1 生成列表](#10-1-生成列表)
- [10-2 复杂表达式](#10-2-复杂表达式)
- [10-3 条件过滤](#10-3-条件过滤)
- [10-4 多层表达式](#10-4-多层表达式)




# 10-1 生成列表

生成列表

要生成list [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]，我们可以用range(1, 11)：

```
>>> range(1, 11)
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
```

但如果要生成[1x1, 2x2, 3x3, …, 10x10]怎么做？方法一是循环：

```
>>> L = []
>>> for x in range(1, 11):
...    L.append(x * x)
... 
>>> L
[1, 4, 9, 16, 25, 36, 49, 64, 81, 100]
```

但是循环太繁琐，而**列表生成式**则可以用一行语句代替循环生成上面的list：

```python
>>> [x * x for x in range(1, 11)]
[1, 4, 9, 16, 25, 36, 49, 64, 81, 100]
```

这种写法就是Python特有的列表生成式。利用列表生成式，可以以非常简洁的代码生成 list。

写列表生成式时，把要生成的元素 x * x 放到前面，后面跟 for 循环，就可以把list创建出来，十分有用，多写几次，很快就可以熟悉这种语法。

任务

请利用列表生成式生成列表 [1x2, 3x4, 5x6, 7x8, …, 99x100]

提示：range(1, 100, 2) 可以生成list [1, 3, 5, 7, 9,…]

答案
`print([i*(i+1) for i in range(1,101,2)])`
# 10-2 复杂表达式

复杂表达式

使用**for循环**的迭代不仅可以迭代普通的list，还可以迭代dict。

假设有如下的dict：

```bash
d = { 'Adam': 95, 'Lisa': 85, 'Bart': 59 }
```

完全可以通过一个复杂的列表生成式把它变成一个 HTML 表格：

```
tds = ['<tr><td>%s</td><td>%s</td></tr>' % (name, score) for name, score in d.iteritems()]
print '<table>'
print '<tr><th>Name</th><th>Score</th><tr>'
print '\n'.join(tds)
print '</table>'
```

**注**：字符串可以通过 % 进行格式化，用指定的参数替代 %s。字符串的join()方法可以把一个 list 拼接成一个字符串。

把打印出来的结果保存为一个html文件，就可以在浏览器中看到效果了：

```xml
<table border="1">
<tr><th>Name</th><th>Score</th><tr>
<tr><td>Lisa</td><td>85</td></tr>
<tr><td>Adam</td><td>95</td></tr>
<tr><td>Bart</td><td>59</td></tr>
</table>
```

![image](http://img.mukewang.com/540fcd2a0001ff4600940104.jpg)

任务

在生成的表格中，对于没有及格的同学，请把分数标记为红色。

提示：红色可以用  实现。 
![image](http://img.mukewang.com/540fcd610001be6800940104.jpg)
答案

```python
d = { 'Adam': 95, 'Lisa': 85, 'Bart': 59 }
def generate_tr(name, score):
    if score < 60:
        return '<tr><td style="color:red">%s</td><td>%s</td></tr>' % (name, score)
    return '<tr><td>%s</td><td>%s</td></tr>' % (name, score)

tds = [generate_tr(name, score) % for name, score in d.iteritems()]
print '<table border="1">'
print '<tr><th>Name</th><th>Score</th><tr>'
print '\n'.join(tds)
print '</table>'
```

# 10-3 条件过滤

条件过滤

**列表生成式的 for 循环后面还可以加上 if 判断**。例如：

```python
>>> [x * x for x in range(1, 11)]
[1, 4, 9, 16, 25, 36, 49, 64, 81, 100]
```

如果我们只想要偶数的平方，不改动 range()的情况下，可以加上 if 来筛选：

```python
>>> [x * x for x in range(1, 11) if x % 2 == 0]
[4, 16, 36, 64, 100]
```

有了 if 条件，只有 if 判断为 True 的时候，才把循环的当前元素添加到列表中。

任务

请编写一个函数，它接受一个 list，然后把list中的所有字符串变成大写后返回，非字符串元素将被忽略。

**提示**：
- 
isinstance(x, str) 可以判断变量 x 是否是字符串；

- 
字符串的 upper() 方法可以返回大写的字母。


答案

```python
def toUppers(L):
    return [x.upper() for x in L if isinstance(x, str)]

print toUppers(['Hello', 'world', 101])
```

# 10-4 多层表达式

多层表达式

for循环可以嵌套，因此，在列表生成式中，也可以用多层 for 循环来生成列表。

对于字符串 ‘ABC’ 和 ‘123’，可以使用两层循环，生成全排列：

```python
>>> [m + n for m in 'ABC' for n in '123']
['A1', 'A2', 'A3', 'B1', 'B2', 'B3', 'C1', 'C2', 'C3']
```

翻译成循环代码就像下面这样：

```bash
L = []
for m in 'ABC':
    for n in '123':
        L.append(m + n)
```

任务

利用 3 层for循环的列表生成式，找出对称的 3 位数。例如，121 就是对称数，因为从右到左倒过来还是 121。

答案
`print [100*x+10*y+z for x in range(1,10) for y in range(0,10) for z in range(0,10) if x==z]`



