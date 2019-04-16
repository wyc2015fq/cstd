# 一行python代码 - 我相信...... - CSDN博客





2016年10月02日 17:40:43[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：12235








Life is short,  just use Python. 

自从08年接触Python，就有爱不释手的感觉，逐渐地，有些不忍地疏远了Perl 和Shell编程，因为python 的优雅么？ 不全是，主要是可以高效开发吧。

那一行代码可以干什么呢？

## 有趣

我孩子的英文名叫andy，也许当初教他写程序的时候，如果先秀一下这行代码，可能就更能激起他对代码的兴趣了。
`>>> print'\n'.join([''.join([('AndyLove'[(x-y)%8]if((x*0.05)**2+(y*0.1)**2-1)**3-(x*0.05)**2*(y*0.1)**3<=0 else' ')for x in range(-30,30)])for y in range(15,-15,-1)])`
再python 里执行它，会输出一个字符拼出的心形。

![LoveAndy](https://img-blog.csdn.net/20160930213648405)

字符图形还是很有趣的，有一个著名的图像叫mandelbrot。Mandelbrot图像中的每个位置都对应于公式N=x+y＊i 中的一个复数，高中学过复数的都还应该有印象。每个位置用参数N来表示，它是x＊x+y＊y的平方根。如果这个值大于或等于2，则这个数字对应的位置值是0。如果参数N的值小于2，就把N的值改为N*N- 

 N（N=（x*x-y*y-x）+（2*x*y-y）*i）），并再次测试这个新N值。wiki百科给出的图像是这样的： 
![Mandelbrot](https://img-blog.csdn.net/20160930214911152)
让我们用一行代码画一个Mandelbrot：  

```python
>>> print'\n'.join([''.join(['*'if abs((lambda a:lambda z,c,n:a(a,z,c,n))(lambda s,z,c,n:z if n==0else s(s,z*z+c,c,n-1))(0,0.02*x+0.05j*y,40))<2 else' 'for x in range(-80,20)])for y in range(-20,20)])
```

![这里写图片描述](https://img-blog.csdn.net/20160930215515946)

## 高效

对于随手小工具而言，更是Python的拿手好戏。

一行代码打印九九乘法表:
`print '\n'.join([' '.join(['%s*%s=%-2s' % (y,x,x*y) for y in range(1,x+1)]) for x in range(1,10)])`
输出： 
![乘法表](https://img-blog.csdn.net/20160930221655942)
一行代码计算出1-1000之间的素数
`print(*(i for i in range(2, 1000) if all(tuple(i%j for j in range(2, int(i**.5))))))  `
一行代码可以输出前100项斐波那契数列的值：
`print [x[0] for x in [  (a[i][0], a.append((a[i][1], a[i][0]+a[i][1]))) for a in ([[1,1]], ) for i in xrange(100) ]]`
一行代码实现阶乘，而且还带交互：

```python
>>> reduce ( lambda x,y:x*y,  range(1,input()+1))
10
3628800
```

一行代码实现摄氏度与华氏度之间的转换器：

```python
>>> print((lambda i:i not in [1,2] and "Invalid input!" or i==1 and (lambda f:f<-459.67 and "Invalid input!" or f)(float(input("Please input a Celsius temperature:"))*1.8+32) or i==2 and (lambda c:c<-273.15 and "Invalid input!" or c)((float(input("Please input a Fahrenheit temperature:"))-32)/1.8))(int(input("1,Celsius to Fahrenheit\n2,Fahrenheit to Celsius\nPlease input 1 or 2\n"))))
1,Celsius to Fahrenheit
2,Fahrenheit to Celsius
Please input 1 or 2
1
Please input a Celsius temperature:28
82.4
>>>
```

至于字符串排序和快速排序更是手到擒来。

```
"".join((lambda x:(x.sort(),x)[1])(list(‘string’)))

qsort = lambda arr: len(arr) > 1 and  qsort(filter(lambda x: x<=arr[0], arr[1:] )) + arr[0:1] + qsort(filter(lambda x: x>arr[0], arr[1:] )) or arr
```

## 内涵

看一看下面一行python代码，可能就要晕了：

![猜数游戏](https://img-blog.csdn.net/20161002122249750)

这是原来为了激发孩子编程兴趣，让孩子练习的代码，它的真实面貌是大致这样的：

```python
def guess_my_number(n):
    while True:
        user_input = raw_input("Enter a positive integer to guess: ")
        if len(user_input)==0 or not user_input.isdigit():
            print "Not a positive integer!"
        else:
            user_input = int(user_input)
            if user_input > n:
                print "Too big ! Try again!"
            elif user_input < n:
                print "Too small ! Try again!"
            else:
                print "You win!"
                return True
guess_my_number(42)
```

实际上，只要理解了函数式编程，使用神奇的Lambda，配合列表推导以及复杂一点的判断语句，任何的python 代码都可以转换成一行代码的。

例如，取一个列表里的随机数

```python
import random as rnd
print rnd.choice([2,3, 5,7, 11,13,17])
```

转换成Lambda 可以是：

```python
print (lambda rnd: rnd.choice([1, 2, 3, 10]))(__import__('random'))
```

这些代码出了觉得好玩，主要是可以**帮助我们了解某些Python的雕虫小记，尤其是神奇的Lambda 用法**。

## 延展

当然，还有其他好玩的地方，输入下面这一行

```java
import antigravity
```

它打开了浏览器，展示网站上的漫画和相关内容： 
![python 打开浏览器](https://img-blog.csdn.net/20161002130253753)
我们可以把python的文件打包，做成库的形式，然后import进来，是一种偷换概念和前提的一行代码。例如，为了与windows 传输文件，再Mac上临时搭个ftp：
`$ python -m pyftpdlib`
这当然要依赖pyftpdlib 这个库了，机器上没有，`pip  install pyftpdlib` 就可以了。

如果一行代码中允许分号存在，那就只是牺牲可读性而已了，那就基本上无所不能。

在连网的前提下，获取公网IP地址
`python -c "import socket; sock=socket.create_connection(('ns1.dnspod.net',6666)); print sock.recv(16); sock.close()"`
一行代码就可以轻易写个小游戏了，来模拟一下golf击球。

```
python -c "import math as m;a,v=eval(input());[print('%03d'%x+' '*m.floor(0.5+x*m.tan(a)-x*x/(v*m.cos(a)))+'o') for x in range(102)]"
```

输入角度和力量大小如（0.8,80)，就能得到一条字符描画的抛物线了。 

增加上while 等语句，画一个没完没了的`python -c "while 1:import random;print(random.choice('╱╲'), end='')"`。 
![maze wall](https://img-blog.csdn.net/20161002174945740)
最后，一行代码以python 的哲学结束吧。

```
$ python -c "import this" 
The Zen of Python, by Tim Peters

Beautiful is better than ugly.
Explicit is better than implicit.
Simple is better than complex.
Complex is better than complicated.
Flat is better than nested.
Sparse is better than dense.
Readability counts.
Special cases aren't special enough to break the rules.
Although practicality beats purity.
Errors should never pass silently.
Unless explicitly silenced.
In the face of ambiguity, refuse the temptation to guess.
There should be one-- and preferably only one --obvious way to do it.
Although that way may not be obvious at first unless you're Dutch.
Now is better than never.
Although never is often better than *right* now.
If the implementation is hard to explain, it's a bad idea.
If the implementation is easy to explain, it may be a good idea.
Namespaces are one honking great idea -- let's do more of those!
```

参考资料： 
[http://www.zhihu.com/question/37046157](http://www.zhihu.com/question/37046157)
[https://onelinepy.herokuapp.com/](https://onelinepy.herokuapp.com/)
[https://github.com/csvoss/onelinerizer](https://github.com/csvoss/onelinerizer)







