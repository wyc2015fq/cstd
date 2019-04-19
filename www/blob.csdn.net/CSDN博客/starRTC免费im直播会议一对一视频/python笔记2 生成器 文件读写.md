# python笔记2 生成器  文件读写 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年11月10日 15:48:58[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：490
生成器
一边循环一边计算的机制，称为生成器（Generator）。
把一个列表生成式的[]改成()，就创建了一个generator：
创建了一个generator后，通过for循环来迭代它。
著名的斐波拉契数列（Fibonacci），除第一个和第二个数外，任意一个数都可由前两个数相加得到：
1, 1, 2, 3, 5, 8, 13, 21, 34, ...
斐波拉契数列用列表生成式写不出来，但是，用函数把它打印出来却很容易：
**def****fib**(max): n,
 a, b = 0,
0,
1**while** n <
 max: **print**
 b a, b = b, a + b n = n + 1
上面的函数可以输出斐波那契数列的前N个数：
>>>
fib(6)112358
仔细观察，可以看出，fib函数实际上是定义了斐波拉契数列的推算规则，可以从第一个元素开始，推算出后续任意的元素，这种逻辑其实非常类似generator。
也就是说，上面的函数和generator仅一步之遥。要把fib函数变成generator，只需要把print
 b改为yield b就可以了：
**def****fib**(max): n,
 a, b = 0,
0,
1**while** n <
 max: **yield**
 b a, b = b, a + b n = n + 1
这就是定义generator的另一种方法。如果一个函数定义中包含yield关键字，那么这个函数就不再是一个普通函数，而是一个generator：
>>> fib(6)<generator
objectfibat0x104feaaa0>
最难理解的就是generator和函数的执行流程不一样。函数是顺序执行，遇到return语句或者最后一行函数语句就返回。而变成generator的函数，在每次调用next()的时候执行，遇到yield语句返回，再次执行时从上次返回的yield语句处继续执行。
举个简单的例子，定义一个generator，依次返回数字1，3，5：
>>>
**def****odd**():...
**print**'step 1'...
**yield**1...
**print**'step 2'...
**yield**3...
**print**'step 3'...
**yield**5...>>>
o = odd()>>>
o.next()step
11>>>
o.next()step
23>>>
o.next()step
35>>>
o.next()Traceback (most recent call last): File
"<stdin>", line
1,
**in** <module>StopIteration
可以看到，odd不是普通函数，而是generator，在执行过程中，遇到yield就中断，下次又继续执行。执行3次yield后，已经没有yield可以执行了，所以，第4次调用next()就报错。
回到fib的例子，我们在循环过程中不断调用yield，就会不断中断。当然要给循环设置一个条件来退出循环，不然就会产生一个无限数列出来。
同样的，把函数改成generator后，我们基本上从来不会用next()来调用它，而是直接使用for循环来迭代：
>>>**for** n
**in** fib(6):...
**print**
 n...112358
列表用[ ]标识。
元组用"()"标识。
- mylist = [x*x **for** x **in** range(3)]  
- **print** mylist  
- mygen=(x*x **for** x **in** range(3))  
- **print** mygen  
行1生成一个列表list，这个列表的每个元素由x的平方组成，x的取值为range(3)，也就是0,1,2。因此这个列表有三个元素：0的平方(0)，1的平方(1)，2的平方(4)。
行2打印出这个列表的内容，显示结果果真是[0,1,4]
行3生成的是一个生成器generator，它和行1唯一的不同就是它用的小括号。但是产生的返回值并不再是一个列表了。
行4想要打印出来这个mygen生成器，但结果显示是这样的: <generator object <genexpr> at 0x022F8030> 一个内存地址。
其实这个mygen生成器就是用来生成x的平方的东西。这个结果呢就存在上面显示的内存地址里。但是由于你还没说你到底要谁的平方，所以只能看到个地址不能看到答案。mylist则不同，它是把所有答案穷举列在内存里了，你需要哪个就从里面找出来即可，比较耗费资源。而mygen则还没生成，你需要哪个我现制造一个出来放在一个内存空间显示，节省了资源。
怎么用这个生成器涅?
- mygen=(x*x **for** x **in** range(3))  
- **for** i **in** mygen:  
- **print** (i)  
这样就可以把mygen可以生成的所有平方数拿出来了。结果显示：
0
1
4
只要有yield这个词出现，你在用def定义函数的时候，系统默认这就不是一个函数啦，是一个生成器啦!!

文件读写
read()方法可以一次读取文件的全部内容
由于文件读写时都有可能产生IOError，一旦出错，后面的f.close()就不会调用。所以，为了保证无论是否出错都能正确地关闭文件，我们可以使用try
 ... finally来实现：
**try**:
 f = open('/path/to/file',
'r')
**print** f.read()**finally**:
**if** f: f.close()
但是每次都这么写实在太繁琐，所以，Python引入了with语句来自动帮我们调用close()方法：
**with**
 open('/path/to/file',
'r')
**as** f:
**print** f.read()
这和前面的try ... finally是一样的，但是代码更佳简洁，并且不必调用f.close()方法。
调用read()会一次性读取文件的全部内容，如果文件有10G，内存就爆了，所以，要保险起见，可以反复调用read(size)方法，每次最多读取size个字节的内容。另外，调用readline()可以每次读取一行内容，调用readlines()一次读取所有内容并按行返回list。因此，要根据需要决定怎么调用。
如果文件很小，read()一次性读取最方便；如果不能确定文件大小，反复调用read(size)比较保险；如果是配置文件，调用readlines()最方便：
**for**
 line **in** f.readlines():
 print(line.strip()) *# 把末尾的'\n'删掉*
