
# Python几个简单好用的基础功能 - saltriver的专栏 - CSDN博客


2016年08月12日 20:07:15[saltriver](https://me.csdn.net/saltriver)阅读数：1141


Python是一门简洁、优雅的语言，希望你能在下面的代码片段中感受到。Python是一座宝库，而实践就是开启这座宝库的钥匙。
（1）三元表达式
看一个示例：
**if**a >= 0:
value = a + b
**else**:
value = a - b
用Python的三元表达式是如下的写法：
value = a + b**if**a >= 0**else**a - b
四行浓缩成了一行，更简洁。但需要注意，如果条件以及表达式非常复杂，可能会牺牲可读性。

（2）数据交换
看2个示例：
1.初始化3个变量
a = 0
b = 10
c = 100可以写成一行：
a, b, c = 0, 10, 100
2.交换数组中两个位置的值
arr = [9, 2, 3, 4, 5, 6, 7, 8, 1]
temp = arr[0]
arr[0] = arr[8]
arr[8] = temp
print(arr)[1, 2, 3, 4, 5, 6, 7, 8, 9]可以写成这样：
arr[0], arr[8] = arr[8], arr[0]
（3）级联比较
示例：
**if**a < b**and**b < c**and**b < d**and**d < e:
**pass**写成如下形式，更简洁，可读性更强。
**if**a < b < c**and**b < d < e:
**pass**
（4）函数返回多个值
简单例子：
**def fun**():
a = 10
b = 3.14
c = "Hello"
**return**a, b, c
a, b, c = fun()不同于C++/Java，Python的函数可以返回多个值，这可以说是个非常有用的功能，因为实际应用中许多函数都可能会有多个输出。此外还有一个非常有吸引力的多值返回方式：返回字典。
**def fun**():
a = 10
b = 3.14
c = "Hello"
**return**{"a": a, "b": b, "c": c}
d = fun()
print(d){'a': 10, 'c': 'Hello', 'b': 3.14}

**（5）in检查**
示例：检查n是否是某些值
**if**n == 1**or**n == 3**or**n == 5**or**n == 7**or**n == 11**or**n == 13**or**n == 17:
**pass**可以这么做：
lst = [1, 3, 5, 7, 11, 13, 17]
**if**n**in**lst:
**pass**



