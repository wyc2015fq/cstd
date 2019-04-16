# Python random模块sample、randint、shuffle、choice随机函数概念和应用 - 虚生 - 博客园







# [Python random模块sample、randint、shuffle、choice随机函数概念和应用](https://www.cnblogs.com/dylancao/p/8202888.html)





[Python标准库](http://www.iplaypy.com/module/)中的random函数，可以生成随机浮点数、[整数](http://www.iplaypy.com/jichu/int.html)、[字符串](http://www.iplaypy.com/jichu/str.html)，甚至帮助你随机选择[列表](http://www.iplaypy.com/jichu/list.html)序

列中的一个元素，打乱一组数据等。

　　random中的一些重要函数的用法：

1 )、random() 返回0<=n<1之间的随机实数n；
2 )、choice(seq) 从序列seq中返回随机的元素；
3 )、getrandbits(n) 以长整型形式返回n个随机位；
4 )、shuffle(seq[, random]) 原地指定seq序列；
5 )、sample(seq, n) 从序列seq中选择n个随机且独立的元素；

　详细介绍：

**random.random()**函数是这个模块中最常用的方法了，它会生成一个随机的浮点数，范围是在0.0~1.0之间。

**random.uniform()**正好弥补了上面函数的不足，它可以设定浮点数的范围，一个是上限，一个是下限。

**random.randint()**随机生一个整数int类型，可以指定这个整数的范围，同样有上限和下限值，python random.randint。

**random.choice()**可以从任何序列，比如list列表中，选取一个随机的元素返回，可以用于字符串、列表、[元组](http://www.iplaypy.com/jichu/tuple.html)等。

**random.shuffle()**如果你想将一个序列中的元素，随机打乱的话可以用这个函数方法。

**random.sample()**可以从指定的序列中，随机的截取指定长度的片断，不作原地修改。

　　使用例子：

```
# -*- coding: utf-8 -*-   
import random

print "\n\t"
print "start test choice:"
foo = ['a', 'b', 'c', 'd', 'e']
print random.choice(foo)

print "\n\t"
# -*- coding: utf-8 -*-   
import random

print "\n\t"
print "start test choice:"
foo = ['a', 'b', 'c', 'd', 'e']
print random.choice(foo)

print "\n\t"
print "start test slice:"
list = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
slice = random.sample(list, 5)  #从list中随机获取5个元素，作为一个片断返回  
print slice  
print list #原有序列并没有改变
             
print "\n\t"
print "start test uniform:"
print random.uniform(10, 20)  
print random.uniform(20, 10) 
    
print "\n\t"
print "start test randint:"
print random.randint(10, 20)  
print random.randint(0, 1)

print "\n\t"
print "start test random:"
print random.random()*1000
print random.random()

print "\n\t"
print "start test shuffle:"
li=range(20)
print random.shuffle(li)
print li
```

```
start test choice:
a

    
start test slice:
[1, 4, 10, 8, 5]
[1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

    
start test uniform:
10.4775179432
10.5882871067

    
start test randint:
20
0

    
start test random:
483.103200723
0.430725744563

    
start test shuffle:
None
[13, 6, 5, 2, 10, 18, 0, 7, 3, 16, 4, 11, 15, 12, 9, 8, 17, 19, 1, 14]
```














