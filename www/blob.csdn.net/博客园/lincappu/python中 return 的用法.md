# python中 return 的用法 - lincappu - 博客园







# [python中 return 的用法](https://www.cnblogs.com/lincappu/p/8146141.html)





return 语句就是讲结果返回到调用的地方，并把程序的控制权一起返回

程序运行到所遇到的第一个return即返回（退出def块），不会再运行第二个return。

要返回两个数值，写成一行即可：

```
def a(x,y):
    if x==y:
        return x,y
  
print a(3,3)<br><br>>>> 3,3
```

但是也并不意味着一个函数体中只能有一个return 语句，例如：

```
def test_return(x):
    if x > 0:
        return x
    else:
        return 0
```

函数没有 return，默认 return一个 None 对象。



递归函数中没有return 的情况：

```
def gcd(a,b):
     if a%b==0:
         return b
     else:
         gcd(b,a%b)
```

分析：else 中没有 return 就没有出口，这个程序是自己内部运行，程序没有返回值，　　



python 和 print 的区别：

```
x = 1
y = 2
def add (x, y):
　　z = x + y
　　return z
print （add(x,y)


x = 1
y = 2
def add (x, y):
　　z = x + y
　　print z
print （add(x,y)）
```


在交互模式下，return的结果会自动打印出来，而作为脚本单独运行时则需要print函数才能显示。




默认情况下，遇见 return 函数就会返回给调用者，但是 try，finally情况除外：


```
def func():  
    try:  
        print 98  
        return 'ok' #函数得到了一个返回值  
    finally: #finally语句块中的语句依然会执行  
        print 98  
  
print fun()
```

输出：

98
98
ok



函数作为返回值返回:
















# [python中 return 的用法](https://www.cnblogs.com/lincappu/p/8146141.html)





return 语句就是讲结果返回到调用的地方，并把程序的控制权一起返回

程序运行到所遇到的第一个return即返回（退出def块），不会再运行第二个return。

要返回两个数值，写成一行即可：

```
def a(x,y):
    if x==y:
        return x,y
  
print a(3,3)<br><br>>>> 3,3
```

但是也并不意味着一个函数体中只能有一个return 语句，例如：

```
def test_return(x):
    if x > 0:
        return x
    else:
        return 0
```

函数没有 return，默认 return一个 None 对象。



递归函数中没有return 的情况：

```
def gcd(a,b):
     if a%b==0:
         return b
     else:
         gcd(b,a%b)
```

分析：else 中没有 return 就没有出口，这个程序是自己内部运行，程序没有返回值，　　



python 和 print 的区别：

```
x = 1
y = 2
def add (x, y):
　　z = x + y
　　return z
print （add(x,y)


x = 1
y = 2
def add (x, y):
　　z = x + y
　　print z
print （add(x,y)）
```


在交互模式下，return的结果会自动打印出来，而作为脚本单独运行时则需要print函数才能显示。




默认情况下，遇见 return 函数就会返回给调用者，但是 try，finally情况除外：


```
def func():  
    try:  
        print 98  
        return 'ok' #函数得到了一个返回值  
    finally: #finally语句块中的语句依然会执行  
        print 98  
  
print fun()
```

输出：

98
98
ok



函数作为返回值返回:



```
def lazy_sum(*args):
    def sum():
        x=0
        for n in args:
            x=x+n
        return x
    return sum



lazy_sum(1,2,3,4,5,6,7,8,9) #这时候lazy_sum 并没有执行，而是返回一个指向求和的函数的函数名sum 的内存地址。
f=lazy_sum(1,2,3,4,5,6,7,8,9)
print(type(f))
print(f())  # 调用f()函数，才真正调用了 sum 函数进行求和，
```

这其实就是闭包。


返回一个函数列表：
```
def count():
    fs = []
    for i in range(1,4):
        def f():
            return i*i
        fs.append(f)
    return fs


f1, f2, f3 = count()
print(f1())
print(f2())
print(f3())
输出：
9
9
9
```


执行过程：
当i=1, 执行for循环， 结果返回函数f的函数地址，存在列表fs中的第一个位置上。
当i=2, 由于fs列表中第一个元素所指的函数中的i是count函数的局部变量，i也指向了2；然后执行for循环， 结果返回函数f的函数地址，存在列表fs中的第二个位置上。
当i=3, 同理，在fs列表第一个和第二个元素所指的函数中的i变量指向了3； 然后执行for循环， 结果返回函数f的函数地址，存在列表fs中的第三个位置上。
所以在调用f1()的时候,函数中的i是指向3的：
  f1():
     return 3*3
同理f2(), f3()结果都为9
闭包时牢记的一点就是：返回函数不要引用任何循环变量，或者后续会发生变化的变量。即包在里面的函数（本例为f()），不要引用外部函数(本例为count())的任何循环变量

如果一定要引入循环变量，方法是再创建一个函数，用该函数的参数绑定循环变量当前的值，无论该循环变量后续如何更改，已绑定到函数参数的值不变：
```
def count():
    fs=[]
    for i in range(1,4):
        def f(j):
            def g():
                return j*j
            return g
        fs.append(f(i))
    return fs

f1,f2,f3=count()
print(f1())
print(f2())
print(f3())
```




















