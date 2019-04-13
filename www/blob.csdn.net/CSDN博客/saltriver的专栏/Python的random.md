
# Python的random - saltriver的专栏 - CSDN博客


2016年08月21日 23:37:55[saltriver](https://me.csdn.net/saltriver)阅读数：7289标签：[python																](https://so.csdn.net/so/search/s.do?q=python&t=blog)[random																](https://so.csdn.net/so/search/s.do?q=random&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=python&t=blog)个人分类：[Python																](https://blog.csdn.net/saltriver/article/category/6363186)



随机数生成是日常工作中经常使用的功能。下面简要介绍下Python的随机数生成标准库random。
**一、常用函数**
**（1）生成0和1之间的浮点数，random()**
生成区间为[0,1)
import random
print(random.random())
输出结果：0.8092116913076974
**（2）生成a和b之间的整数，randint(a,b)**
生成区间为[a,b]
print(random.randint(-10,10))
输出结果：9
a必须小于等于b，如果a==b，则输出a。
print(random.randint(10,10))
输出结果：10
**（3）生成a和b之间的浮点数，unifrom(a,b)，也是均匀分布，因为非常常用放在这里。**
生成区间为[a,b]
print(random.uniform(-10,10))
输出结果：-4.109021075631352
这个函数中，a可以大于b，生成的是min(a,b)和max(a,b)之间的浮点数。
**（4）生成a和b之间某个范围的整数，randrange(a,b,step)**
生成区间为：在[a,b]范围内，从a开始（包含a），每隔step的数形成的集合。
print(random.randrange(0,10,2))
输出结果：生成[0,10]之前的偶数
print(random.randrange(1,10,2))
输出结果：生成[0,10]之前的奇数
**二、操作序列的函数**
**（1）从一个序列seq中随机选取一个元素，choice(seq)**
seq1 = [1,2,3,4,5]
seq2 = ((1,2),(3,4),(5,6))
seq3 ="abcdef"
print(random.choice(seq1))
print(random.choice(seq2))
print(random.choice(seq3))
输出结果：
5
(3, 4)
a
**（2）将一个列表list中的元素随机打乱，shuffle(list)**
注意，shuffle函数需要修改原序列，因此输入的序列seq必须是可更改的，所以只支持list类型。shuffle有个很传神的名字：洗牌。
seq = [1,2,3,4,5]
print(random.shuffle(seq),seq,sep='\n')
输出结果：
None
[2, 5, 1, 4, 3]
**（3）从一个序列中随机取出k个元素，sample(seq,k)**
seq = [1,2,3,4,5]
print(random.sample(seq,3),seq,sep='\n')
输出结果：
[4, 5, 2]
[1, 2, 3, 4, 5]
注意几个方面：一是sample并不改变原序列，因此输入序列seq可以是元组、字符串、set等数据类型。二是取出k个元素是从不同位置取值的，因此如果原序列无重复元素，形成的新序列也不会存在重复元素。三是不管输入序列seq是什么类型，sample返回的是list类型，看下例。
seq = "abcdef"
print(random.sample(seq,3),seq,sep='\n')
输出结果：
['c', 'a', 'd']
abcdef
**三、随机生成符合某种分布的数据**
**（1）均匀分布，uniform(a,b)**
见第一节描述。非常常用。
**（2）正态分布，normalvariate(mu,sigma)**
mu：均值
sigma：标准差
mu=0，sigma=1为标准正态分布。
除了均匀分布，正态分布用的是最多的。
import random
import numpy as np
import time
st = time.clock()
N =1000000
n = []
for i in range(N):
n.append(random.normalvariate(0,1))
print("均值=",np.mean(n))
print("标准差=",np.std(n))
print("耗时=",time.clock() - st)
输出结果：
均值= -7.42643413388e-05
标准差= 1.00049689373
耗时= 5.702438655147374
可以看出，随机生成1000000个数，这些数符合正态分布。
**（3）高斯分布，gauss(mu,sigma)**
就是正态分布，采用了不同的实现方式，据说运行速度更快。
import random
import numpy as np
import time
st = time.clock()
N =1000000
n = []
for i in range(N):
n.append(random.gauss(0,1))
print("均值=",np.mean(n))
print("标准差=",np.std(n))
print("耗时=",time.clock() - st)
输出结果
均值= 0.000604319020838
标准差= 0.999634159673
耗时= 4.827287158657131
1000000个数快了不到1秒，也没快太多。
**（4）还有生成三角形分布、对数分布、指数分布、β分布、伽马分布等的函数**
triangular(low, high, mode)三角形分布
lognormvariate(mu, sigma)对数分布
expovariate(lambd)指数分布
gammavariate(alpha, beta)伽马分布
等等。实际工作中，这些分布比均匀分布和正态分布用的都少的多。

