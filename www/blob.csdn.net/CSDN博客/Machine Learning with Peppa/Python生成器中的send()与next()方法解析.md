# Python生成器中的send()与next()方法解析 - Machine Learning with Peppa - CSDN博客





2018年04月09日 12:56:14[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：1344








在异步IO的使用时，generator是最基本的一个实现方法。python生成器有两个主要方法，一个是send一个是next。今天我们来看看两者的用法和联系。在每段代码中，第一个next调用，相当于启动生成器，会从生成器函数的第一行代码开始执行，直到第一次执行完yield语句（第4行）后，跳出生成器函数。然后第二个next调用，进入生成器函数后，从yield语句的下一句语句（第5行）开始执行，然后重新运行到yield语句，执行后，跳出生成器函数后面再次调用next，依次类推。

这里我们用经典的生产者-消费者模型举个例子（摘自[廖雪峰的博客](https://www.liaoxuefeng.com/wiki/0014316089557264a6b348958f449949df42a6d3a2e542c000/001432090954004980bd351f2cd4cc18c9e6c06d855c498000)）

```python
1 def consumer():
2     r = 0
3     for i in xrange(3):
4         yield r
5         r = '200 OK'+ str(i)
6
7 c = consumer()
8 n1 = c.next()
9 n2 = c.next()
10 n3 = c.next()
```


实际上next()和send()在一定意义上作用是相似的，区别是send()可以传递yield表达式的值进去，而next()不能传递特定的值，只能传递None进去。因此，我们可以看做c.next() 和 c.send(None) 作用是一样的。

第一次调用时，请使用next()语句或是send(None)，不能使用send发送一个非None的值，否则会出错的，因为没有Python yield语句来接收这个值。

下面说明下send执行的顺序。先记住，**n1 = yield r这句话是从右往左执行的**。当第一次send（None）（对应11行）时，启动生成器，从生成器函数的第一行代码开始执行，直到第一次执行完yield（对应第4行）后，跳出生成器函数。这个过程中，n1一直没有定义。
运行到send（1）时，进入生成器函数，此时，**将yield r看做一个整体，赋值给它并且传回**。此时即相当于把1赋值给n1，但是并不执行yield部分。下面继续从yield的下一语句继续执行，然后重新运行到yield语句，执行后，跳出生成器函数。即send和next相比，只是开始多了一次赋值的动作，其他运行流程是相同的。


```python
1 def consumer():
2     r = 'here'
3     while True:
4         n1 = yield r   #这里的等式右边相当于一个整体，接受回传值
5         if not n1:
6             return
7         print('[CONSUMER] Consuming %s...' % n1)
8         r = '%d00 OK' % n1
9
10 def produce(c):
11     aa = c.send(None)
12     n = 0
13     while n < 5:
14         n = n + 1
15         print('[PRODUCER] Producing %s...' % n)
16         r1 = c.send(n)
17         print('[PRODUCER] Consumer return: %s' % r1)
18     c.close()
19
20 c = consumer()
21 produce(c)
```
运行结果：

```python
[PRODUCER] Producing 1...
[CONSUMER] Consuming 1...
[PRODUCER] Consumer return: 100 OK
[PRODUCER] Producing 2...
[CONSUMER] Consuming 2...
[PRODUCER] Consumer return: 200 OK
[PRODUCER] Producing 3...
[CONSUMER] Consuming 3...
[PRODUCER] Consumer return: 300 OK
[PRODUCER] Producing 4...
[CONSUMER] Consuming 4...
[PRODUCER] Consumer return: 400 OK
[PRODUCER] Producing 5...
[CONSUMER] Consuming 5...
[PRODUCER] Consumer return: 500 OK
```



