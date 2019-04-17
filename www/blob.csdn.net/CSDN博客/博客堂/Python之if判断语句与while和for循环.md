# Python之if判断语句与while和for循环 - 博客堂 - CSDN博客





2017年03月08日 22:30:12[最小森林](https://me.csdn.net/u012052268)阅读数：5450标签：[python																[for																[while																[if](https://so.csdn.net/so/search/s.do?q=if&t=blog)
个人分类：[Python](https://blog.csdn.net/u012052268/article/category/6625160)





## Python之if判断语句与while和for循环



- - [Python之if判断语句与while和for循环](#python之if判断语句与while和for循环)

- [5-1 Python之if语句](#5-1-python之if语句)
- [5-2 Python之 if-else](#5-2-python之-if-else)
- [5-3 Python之 if-elif-else](#5-3-python之-if-elif-else)
- [5-4 Python之 for循环](#5-4-python之-for循环)
- [5-5 Python之 while循环](#5-5-python之-while循环)
- [5-6 Python之 break退出循环](#5-6-python之-break退出循环)
- [5-7 Python之 continue继续循环](#5-7-python之-continue继续循环)
- [5-8 Python之 多重循环](#5-8-python之-多重循环)




# 5-1 Python之if语句

计算机之所以能做很多自动化的任务，因为它可以自己做条件判断。

比如，输入用户年龄，根据年龄打印不同的内容，在Python程序中，可以用**if语句**实现：

```
age = 20
if age >= 18:
    print 'your age is', age
    print 'adult'
print 'END'
```

**注意**: ==Python代码的缩进规则==。具有相同缩进的代码被视为代码块，上面的3，4行 print 语句就构成一个代码块（但不包括第5行的print）。如果 if 语句判断为 True，就会执行这个代码块。

缩进请严格按照Python的习惯写法：==4个空格==，不要使用Tab，更不要混合Tab和空格，否则很容易造成因为缩进引起的语法错误。

注意: if 语句后接表达式，然后用:表示代码块开始。

如果你在Python交互环境下敲代码，还要特别留意缩进，并且退出缩进需要多敲一行回车：

```
>>> age = 20
>>> if age >= 18:
...     print 'your age is', age
...     print 'adult'
...
your age is 20
adult
```

**任务**

如果成绩达到60分或以上，视为passed。

假设Bart同学的分数是75，请用if语句判断是否能打印出 passed:

答案：

```
score = 75
if score >= 60:
    print 'passed'
```

# 5-2 Python之 if-else

当 if 语句判断表达式的结果为 True 时，就会执行 if 包含的代码块：

```
if age >= 18:
    print 'adult'
```

如果我们想判断年龄在18岁以下时，打印出 ‘teenager’，怎么办？

方法是再写一个 if:

```
if age < 18:
    print 'teenager'
```

或者用 not 运算：

```
if not age >= 18:
    print 'teenager'
```

细心的同学可以发现，这两种条件判断是“非此即彼”的，要么符合条件1，要么符合条件2，因此，完全可以用一个 if … else … 语句把它们统一起来：

```
if age >= 18:
    print 'adult'
else:
    print 'teenager'
```

利用 if … else … 语句，我们可以根据条件表达式的值为 True 或者 False ，分别执行 if 代码块或者 else 代码块。

注意: else 后面有个“:”。

**任务**

如果成绩达到60分或以上，视为passed，否则视为failed。

假设Bart同学的分数是55，请用if语句打印出 passed 或者 failed:

**答案**

```
score = 55
if score >= 60:
    print 'passed'
else:
    print 'failed'
```

# 5-3 Python之 if-elif-else

有的时候，一个 if … else … 还不够用。比如，根据年龄的划分：

```
条件1：18岁或以上：adult
条件2：6岁或以上：teenager
条件3：6岁以下：kid
```

我们可以用一个 if age >= 18 判断是否符合条件1，如果不符合，再通过一个 if 判断 age >= 6 来判断是否符合条件2，否则，执行条件3：

```
if age >= 18:
    print 'adult'
else:
    if age >= 6:
        print 'teenager'
    else:
        print 'kid'
```

这样写出来，我们就得到了一个两层嵌套的 if … else … 语句。这个逻辑没有问题，但是，如果继续增加条件，比如3岁以下是 baby：

```
if age >= 18:
    print 'adult'
else:
    if age >= 6:
        print 'teenager'
    else:
        if age >= 3:
            print 'kid'
        else:
            print 'baby'
```

这种缩进只会越来越多，代码也会越来越难看。

要避免嵌套结构的 if … else …，我们可以用 if … 多个elif … else … 的结构，一次写完所有的规则：

```python
if age >= 18:
    print 'adult'
elif age >= 6:
    print 'teenager'
elif age >= 3:
    print 'kid'
else:
    print 'baby'
```

elif 意思就是 else if。这样一来，我们就写出了结构非常清晰的一系列条件判断。

特别注意: 这一系列条件判断会从上到下依次判断，如果某个判断为 True，执行完对应的代码块，后面的条件判断就直接忽略，不再执行了。

请思考下面的代码：

```python
age = 8
if age >= 6:
    print 'teenager'
elif age >= 18:
    print 'adult'
else:
    print 'kid'
```

当 age = 8 时，结果正确，但 age = 20 时，为什么没有打印出 adult？

如果要修复，应该如何修复？

**任务**

如果按照分数划定结果：

```
90分或以上：excellent

80分或以上：good

60分或以上：passed

60分以下：failed
```

请编写程序根据分数打印结果。

**答案**

```python
score = 85

if score >= 90:
    print 'excellent'
elif score >= 80:
    print 'good'
elif score >= 60:
    print 'passed'
else :
    print 'failed'
```

# 5-4 Python之 for循环

list或tuple可以表示一个有序集合。如果我们想依次访问一个list中的每一个元素呢？比如 list：

```
L = ['Adam', 'Lisa', 'Bart']
print L[0]
print L[1]
print L[2]
```

如果list只包含几个元素，这样写还行，如果list包含1万个元素，我们就不可能写1万行print。

这时，循环就派上用场了。

Python的 for 循环就可以依次把list或tuple的每个元素迭代出来：

```
L = ['Adam', 'Lisa', 'Bart']
for name in L:
    print name
```

注意:  name 这个变量是在 for 循环中定义的，意思是，依次取出list中的每一个元素，并把元素赋值给 name，然后执行for循环体（就是缩进的代码块）。

这样一来，遍历一个list或tuple就非常容易了。

**任务**

班里考试后，老师要统计平均成绩，已知4位同学的成绩用list表示如下：

L = [75, 92, 59, 68]

请利用for循环计算出平均成绩。

**答案**

```
L = [75, 92, 59, 68]
sum = 0.0
for x in L:
   sum += x
print sum / 4
```

# 5-5 Python之 while循环

Python之 while循环

和 for 循环不同的另一种循环是 while 循环，while 循环不会迭代 list 或 tuple 的元素，而是根据表达式判断循环是否结束。

比如要从 0 开始打印不大于 N 的整数：

```
N = 10
x = 0
while x < N:
    print x
    x = x + 1
```

while循环每次先判断 x < N，如果为True，则执行循环体的代码块，否则，退出循环。

在循环体内，x = x + 1 会让 x 不断增加，最终因为 x < N 不成立而退出循环。

如果没有这一个语句**，while循环在判断 x < N 时总是为True**，就会无限循环下去，变成死循环，所以要特别留意while循环的退出条件。

**任务**

利用while循环计算100以内奇数的和。

**答案**

```
sum = 0
x = 1
while x <100:
    sum += x
    x += 2
print sum
```

# 5-6 Python之 break退出循环

Python之 break退出循环

用 for 循环或者 while 循环时，如果要在循环体内直接退出循环，可以使用 break 语句。

比如计算1至100的整数和，我们用while来实现：

```
sum = 0
x = 1
while True:
    sum = sum + x
    x = x + 1
    if x > 100:
        break
print sum
```

咋一看， while True 就是一个死循环，但是在循环体内，我们还判断了 x > 100 条件成立时，用break语句退出循环，这样也可以实现**循环的结束**。

**任务**

利用 while True 无限循环配合 break 语句，计算 1 + 2 + 4 + 8 + 16 + … 的前20项的和。

**答案**：

```
sum = 0
x = 1
n = 1
while True:
    sum += x
    x = x * 2
    n += 1
    if n > 20:
        break
print sum
```

# 5-7 Python之 continue继续循环

Python之 continue继续循环

在循环过程中，可以用**break**退出当前循环，还可以用**continue**跳过后续循环代码，继续下一次循环。

假设我们已经写好了利用for循环计算平均分的代码：

```
L = [75, 98, 59, 81, 66, 43, 69, 85]
sum = 0.0
n = 0
for x in L:
    sum = sum + x
    n = n + 1
print sum / n
```

现在老师只想统计及格分数的平均分，就要把 x < 60 的分数剔除掉，这时，利用 continue，可以做到当 x < 60的时候，不继续执行循环体的后续代码，直接进入下一次循环：

```
for x in L:
    if x < 60:
        continue
    sum = sum + x
    n = n + 1
```

**任务**

对已有的计算 0 - 100 的while循环进行改造，通过增加 continue 语句，使得只计算奇数的和：

```
sum = 0
x = 1
while True:
    sum = sum + x
    x = x + 1
    if x > 100:
        break
print sum
```

**答案**

```
sum = 0
x = 0
while True:
    x = x + 1
    if x > 100:
        break
    if x % 2 == 0:
        continue
    sum += x
print sum
```

# 5-8 Python之 多重循环

Python之 多重循环

在循环内部，还可以嵌套循环，我们来看一个例子：

```
for x in ['A', 'B', 'C']:
    for y in ['1', '2', '3']:
        print x + y
```

x 每循环一次，y 就会循环 3 次，这样，我们可以打印出一个全排列：

```
A1
A2
A3
B1
B2
B3
C1
C2
C3
```

**任务**

对100以内的两位数，请使用一个两重循环打印出所有十位数数字比个位数数字小的数，例如，23（2 < 3）。

**答案**

```
for x in [1,2,3,4,5,6,7,8,9]:
    for y in [0,1,2,3,4,5,6,7,8,9]:
        if x < y:
            print(10 * x + y)
```](https://so.csdn.net/so/search/s.do?q=while&t=blog)](https://so.csdn.net/so/search/s.do?q=for&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)




