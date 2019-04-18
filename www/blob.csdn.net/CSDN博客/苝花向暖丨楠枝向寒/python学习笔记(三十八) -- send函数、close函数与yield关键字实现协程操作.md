# python学习笔记(三十八) -- send函数、close函数与yield关键字实现协程操作 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年09月16日 15:50:22[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：147
所属专栏：[python学习笔记](https://blog.csdn.net/column/details/26223.html)



# 前言

我们之前接触过多线程和多进程，他们可以完成两个甚至多个线程或进程通知执行不同得函数，并且可以通过函数来控制让其中一个线程等待另一个线程执行一部分代码以后再去执行，比如有两个A和B，线程1 执行一部分A，然后停止执行，等待线程2执行一部分B，然后线程2停止执行，再等待线程1执行A，执行完以后线程2再执行函数B

体现如下：

```python
def A():
    print('1')
    print('2')
    print('3')

def B():
    print('x')
    print('y')
    print('z')
```

结果

```
1
2
x
y
3
z
```

这种操作其实可以用协程来实现，什么是协程？ 协程就是只有一个线程来实现上面的效果。协程相比与多线程有什么好处呢？

最大的优势就是协程极高的执行效率。因为子程序切换不是线程切换，而是由程序自身控制，因此，没有线程切换的开销，和多线程比，线程数量越多，协程的性能优势就越明显。

第二大优势就是不需要多线程的锁机制，因为只有一个线程，也不存在同时写变量冲突，在协程中控制共享资源不加锁，只需要判断状态就好了，所以执行效率比多线程高很多。

协程是通过generator来实现的，其中涉及的就是yield关键字和send()函数的使用。

在讲解具体的使用之前，我们先了解一下send()函数和yield关键字。

# generator的send()函数和yield关键字

以之前对yield关键字的了解，无非就是yield关键字可以给调用者返回消息，同时当函数通过yield关键字返回消息以后函数会暂停在当前位置。

如下:

```python
>>> def test():
	number = 1
	while True:
		number *= 2
		yield number
		print('我是yield关键字下面一行代码')

		
>>> t = test()
>>> next(t)
2
>>> next(t)
我是yield关键字下面一行代码
4
>>> next(t)
我是yield关键字下面一行代码
8
```

 但其实 yield关键字 的作用是 函数与调用者的通信，它不光能够将值返回给调用者，它还可以接收调用者传过来的值，那么怎么实现呢？ 调用者通过send()函数将值传递给generator，generator通过 yield 前面的变量来接收，如下：

```python
>>> def test():
	number = 0
	while True:
		n = yield number
		print('我是yield关键字下面一行代码')
		print('调用者传递过来的值为:', n)

		
>>> t = test()
>>> t.send(None)
0
>>> t.send(1)
我是yield关键字下面一行代码
调用者传递过来的值为: 1
0
>>> t.send(2)
我是yield关键字下面一行代码
调用者传递过来的值为: 2
0
>>> next(t)
我是yield关键字下面一行代码
调用者传递过来的值为: None
0
```

可以发现，send函数和next函数的区别就是 send函数可以在执行generator的过程中，给generator发送消息。而next仅仅是接收yield右边的变量值。

好了，我们现在知道了send函数与yield关键字配合使用可以实现调用者与generator的通信了，那么我们就可以通过他们来实现单一线程切换执行不同函数了。这种操作就叫做协程。

# generator的close函数

由于下面还涉及到一个 close函数，这里也演示一下，方便下面理解

```python
>>> def test():
	number = 1
	while True:
		number *= 2
		yield number
		print('我是yield关键字下面一行代码')

		
>>> t = test()
>>> 
>>> next(t)
2
>>> next(t)
我是yield关键字下面一行代码
4
>>> next(t)
我是yield关键字下面一行代码
8
>>> t.close()
>>> next(t)    # 关闭generator后再调用，会出现StopIteration异常，即后面没值了
Traceback (most recent call last):
  File "<pyshell#105>", line 1, in <module>
    next(t)
StopIteration
>>>
```

# 协程

下面是一个生产者与消费者的例子，代码如下：

```python
# encoding=utf8

def consumer():
    r = ''
    while True:
        n = yield r  # yield 后面的r是返回给调用者的值，yield前面的 n 用来接收调用者通过send函数传过来的值
        if not n:  # 如果n是None，就结束该函数，因为None代表还没有生产出来产品
            print('consumer即将结束')  # 实际上该代码中这行永远不会被执行
            return
        print('[CONSUMER] Consuming %s...' % n)
        r = '200 OK'

def produce(c):
    # send函数让线程停止执行当前函数，从而去执行generator
    print('send返回值为: ', c.send(None))  # send语句实际上是给 yield左边的变量n赋值，send返回的值就是 yield后面的r，返回的是空字符
    n = 0
    while n < 5:
        n = n + 1
        print('[PRODUCER] Producing %s...' % n)
        r = c.send(n)  # 生产出来产品以后告诉消费者，我这里有货了
        print('[PRODUCER] Consumer return: %s' % r)  # 消费者传递过来消息，它已经收到了
    c.close()

c = consumer()
produce(c)
```

运行结果如下：

第一行 send 返回值 是一个空 字符

```python
send返回值为:  
[PRODUCER] Producing 1...
[CONSUMER] Consuming 1...
[PRODUCER] Consumer return: 200 OK
[PRODUCER] Producing 2...
[CONSUMER] Consuming 2...
[PRODUCER] Consumer return: 200 OK
[PRODUCER] Producing 3...
[CONSUMER] Consuming 3...
[PRODUCER] Consumer return: 200 OK
[PRODUCER] Producing 4...
[CONSUMER] Consuming 4...
[PRODUCER] Consumer return: 200 OK
[PRODUCER] Producing 5...
[CONSUMER] Consuming 5...
[PRODUCER] Consumer return: 200 OK
```

