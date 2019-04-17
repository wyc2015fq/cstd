# Python 程序控制语句 - Alex_McAvoy的博客 - CSDN博客





2018年08月11日 21:03:25[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：76








# 【条件控制语句】

## 1.if 语句

1）形式：

```python
if condition_test:
    do something
```

2）特点：如果条件为真，则执行 if 语句后所有缩进的代码，否则忽略它们

3）常用条件测试运算符：比较运算符、成员运算符等

## 2.if-else 语句

1）形式：

```python
if condition_test:
    do something
else：
    do something
```

2）特点：类似 if 语句，但其中的 else 语句可以指定条件测试未通过时要执行的操作

## 3.if-elif-else 语句

1）形式：

```python
if condition_test1:
    do something
elif condition_test2：
    do something
else：
    do something
```

2）特点：依次检查每个条件测试，直到遇到通过了的条件测试，测试通过后，执行其后紧跟的代码块，并跳过余下的测试

## 4.多个 elif 语句

1）形式：

```python
if condition_test1:
    do something
elif condition_test2：
    do something
elif condition_test3：
    do something
elif condition_test4：
    do something
else：
    do something
```

2）特点：可根据需要，使用任意数量的 elif 代码块，且不要求最后一定要有 else 语句

3）与 if 语句区别

if-elif-else 语句功能强大，但仅适合用于只有一个条件满足的情况，遇到通过了的测试后，就跳过余下的测试，但有时需要检查所有条件，在这种可能有多个条件为 True，且需要对每个条件为 True 时进行相应操作时，需要使用多次 if 语句。

总而言之，如果想只执行一个代码块，就用 if-elif-else 语句；如果要执行多个代码块，就要使用一系列独立的 if 语句。

# 【循环语句】

## 1.while 语句

### 1）一般格式：

```python
while condition_test1:
    do something
```

例如：

```python
n = 100
 
sum = 0
counter = 1
while counter <= n:
    sum = sum + counter
    counter += 1
 
print("1 到 %d 之和为: %d" % (n,sum))
```

### 2）无限循环

通过设置条件表达式，可令其永远不为 false，从而来实现无限循环。

例如：

```python
var = 1
while var == 1 :  # 表达式永远为 true
   num = int(input("输入一个数字  :"))
   print ("你输入的数字是: ", num)
```

**注：如果出现了无限循环，可以使用 Ctrl+C 来中断循环。**

## 2.for 语句

### 1）一般格式

for循环可以遍历任何序列的项目，如一个列表或者一个字符串。

```python
for <variable> in <sequence>:
    <statements>
```

例如：

```python
languages = ["C", "C++", "Perl", "Python"] 
for x in languages:
    print (x)
```

### 2）range()函数

如果需要遍历数字序列，可以使用内置的 range()函数，它会生成一个数列。

例如：

```python
for i in range(5):
    print(i)
```

此外，range() 函数还可以指定区间的值，并指定不同的增量

例如：

```python
for i in range(0, 10, 3) :
    print(i)
```

因此，range() 函数用途很广

例如：

```python
#可用 range() 函数来创建一个列表
list(range(5))

#可用 range()、len()函数遍历一个序列的索引
a = ['Google', 'Baidu', 'Runoob', 'Taobao', 'QQ']
for i in range(len(a)):
    print(i, a[i])
```

## 3.break 与 continue 语句

### 1）break 语句

break 语句可以跳出 for 和 while 的循环体。

如果从 for 或 while 循环中终止，任何对应的循环 else 块将不再执行。

例如：

```python
for letter in 'Runoob':
   if letter == 'b':    #字母为 b 时终止循环
      break
   print ('当前字母为 :', letter)
```

### 2）continue 语句

continue 语句被用于跳过当前循环块中的剩余语句，然后继续进行下一轮循环。

例如：

```python
for letter in 'Runoob':     
   if letter == 'o':        # 字母为 o 时跳过输出
      continue
   print ('当前字母 :', letter)
```

## 4.else 子句

对于循环语句的 else 子句，其在穷尽列表 (for 循环) 或条件变为 false (while 循环)导致循环终止时被执行，但循环被 break 语句终止时，不再执行。

例如：

```python
for n in range(2, 10):
    for x in range(2, n):
        if n % x == 0:
            print(n, '等于', x, '*', n//x)
            break
    else:    #循环中没有找到元素
        print(n, '是质数')
```

# 【占位语句】

pass 语句称为空语句，其不做任何事情，因此也叫占位语句，其被设计的目的是为了保持程序结构的完整性。

当编写一个程序时，如果还没有完成执行语句部分的思路，此时可以用 pass 语句来占位，当做一个用于过后来完成的代码的标记。

例如：

```python
for letter in 'Runoob': 
   if letter == 'o':
      pass
   print ('当前字母 :', letter)
```





