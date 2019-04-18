# python中的随机函数random的用法示例 - 心纯净，行致远 - CSDN博客





2018年09月08日 21:56:02[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：142








这篇文章主要介绍了python中的随机函数random的用法示例，详细的介绍了python 随机函数random的用法和示例，具有一定的参考价值，感兴趣的小伙伴们可以参考一下

**一、random模块简介**

Python标准库中的random函数，可以生成随机浮点数、整数、字符串，甚至帮助你随机选择列表序列中的一个元素，打乱一组数据等。

**二、random模块重要函数**

1 )、random() 返回0<=n<1之间的随机实数n；

2 )、choice(seq) 从序列seq中返回随机的元素；

import random
a = random.choice([1, 2, 3, 4])
print(a)
3 )、getrandbits(n) 以长整型形式返回n个随机位；

4 )、shuffle(seq[, random]) 原地指定seq序列；

5 )、sample(seq, n) 从序列seq中选择n个随机且独立的元素；

**三、random模块方法说明**
- random.random()函数是这个模块中最常用的方法了，它会生成一个随机的浮点数，范围是在0.0~1.0之间。
- random.uniform()正好弥补了上面函数的不足，它可以设定浮点数的范围，一个是上限，一个是下限。
- random.randint()随机生一个整数int类型，可以指定这个整数的范围，同样有上限和下限值，python random.randint。
- random.choice()可以从任何序列，比如list列表中，选取一个随机的元素返回，可以用于字符串、列表、元组等。
- random.shuffle()如果你想将一个序列中的元素，随机打乱的话可以用这个函数方法。
- random.sample()可以从指定的序列中，随机的截取指定长度的片断，不作原地修改。

**四、需要导入random模块**

1、random.random

random.random()用于生成一个0到1的随机符小数: 0 <= n < 1.0

>>> random.random()  # Random float x,
2、random.uniform

random.uniform的函数原型为：random.uniform(a, b)，用于生成一个指定范围内的随机符点数，两个参数其中一个是上限，一个是下限。如果a > b，则生成的随机数n: a <= n <= b。如果 a <b， 则 b <= n <= a。

>>> random.uniform(1, 10) # Random float x,
3、random.randint

random.randint()的函数原型为：random.randint(a, b)，用于生成一个指定范围内的整数。其中参数a是下限，参数b是上限，生成的随机数n: a <= n <= b

>>> random.randint(10, 100)
4、random.randrange

random.randrange的函数原型为：random.randrange([start], stop[, step])，从指定范围内，按指定基数递增的集合中 获取一个随机数。如：random.randrange(10, 100, 2)，结果相当于从[10, 12, 14, 16, ... 96, 98]序列中获取一个随机数。random.randrange(10, 100, 2)在结果上与 random.choice(range(10, 100, 2) 等效。

随机选取0到100间的偶数： 

>>> import random 
>>> random.randrange(0, 101, 2)
5、random.choice

random.choice从序列中获取一个随机元素。其函数原型为：random.choice(sequence)。参数sequence表示一个有序类型。这里要说明 一下：sequence在python不是一种特定的类型，而是泛指一系列的类型。list, tuple, 字符串都属于sequence。有关sequence可以查看python手册数据模型这一章。

>>> random.choice('abcdefg&#%^*f') #随机字符 
'd'

>>> random.choice ( ['apple', 'pear', 'peach', 'orange', 'lemon'] ) #随机选取字符串： 
'lemon'

6、random.shuffle

random.shuffle的函数原型为：random.shuffle(x[, random])，用于将一个列表中的元素打乱。如:

p = ["Python", "is", "powerful", "simple", "and so on..."] 
random.shuffle(p) 
print p 
#结果（因为随机，所以你的结果可能不一样。） 
#['powerful', 'simple', 'is', 'Python', 'and so on...'] 
p = ["Python", "is", "powerful", "simple", "and so on..."]
7、random.sample

random.sample的函数原型为：random.sample(sequence, k)，从指定序列中随机获取指定长度的片断。sample函数不会修改原有序列。 如果k大于sequence元素个数的话会报错。 



