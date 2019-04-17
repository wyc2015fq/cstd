# python cookbook 2.14 合并拼接字符串 - 别说话写代码的博客 - CSDN博客





2019年04月15日 21:04:56[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：11
所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)









```
#想将几个小的字符串合并为一个大的字符串。若想合并的字符串在一个序列或者iterable中，最好就是join()方法
parts= ['Is', 'Chicago', 'Not', 'Chicago?']
print(' '.join(parts))
print(','.join(parts))
print(''.join(parts))
```

![](https://img-blog.csdnimg.cn/20190415210306831.png)

```
#如果只有个别str，直接可以用+连接输出。其他类型可以使用,分隔
a='is china'
b='not china'
print(a+','+b)
```

![](https://img-blog.csdnimg.cn/2019041521032157.png)

```
print('{}{}'.format(a,b))
#若想在源码中将两个字面字符串合并起来，只需要简单的将他们放到一起，不用+
a='hello''world'
print(a)
```

![](https://img-blog.csdnimg.cn/20190415210338471.png)

```
#注意：+操作符去连接大量字符串是非常低效，+连接会引起内存复制以及垃圾回收，不要写下面这样：
s=''
for p in parts:
    s+=p
#这样写比使用join()运行慢很多，每次执行+=操作就会创建一个新的字符串对象。最好先收集字符串片段然后将他们连起来
#可以利用生成器表达式转换数据为字符串的同时合并字符串
data=['acm',50,91.1]
print(','.join(str(d) for d in data))
```

![](https://img-blog.csdnimg.cn/20190415210354701.png)

```
#同时注意，用优雅的方式打印字符串，用合适的方法连接字符串
c='ss'
print(a+':'+b+':'+c)  #ugly
print(':'.join([a,b,c]))  #still ugly
print(a,b,c,sep=':')  #better
```

![](https://img-blog.csdnimg.cn/20190415210410771.png)

```
#若你准备编写构建大量小字符串的输出代码，最好考虑使用生成器函数，利用yield语句产生输出片段
def sample():
    yield 'is'
    yield 'chicago'
    yield 'not'
    yield 'chichago?'
text = ''.join(sample())
print(text)
```

![](https://img-blog.csdnimg.cn/20190415210428625.png)



