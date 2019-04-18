# Python入门--14--字典 - 默槑 - 博客园







# [Python入门--14--字典](https://www.cnblogs.com/modaidai/p/6913822.html)





基础知识：

0、python对值没有要求，但对key有些要求，必须是非变量（变量，列表，字典）

1、在列表中使用成员资格操作符，和在字典中使用成员资格操作符，后者会更快

2、fromkeys这个函数会生成一个新的字典，如果以前存在就会直接覆盖

3、字典拷贝：dict2=dict1.copy()

4、







一、创建一个字典

dict1={'李宁':'一起皆有可能','耐克',}



字典其实就是perl里面的哈希

元祖、列表都是序列类型，字典是映射类型



用[]的数据都叫列表，用{}的数据我们叫做字典，这句话对吗？

答：不对

xx={1,2,3,4,5}

type(xx)　　　　　　#输出的是<class 'set'>



创建字典有几种方式？

a=dict(one=1,two=2,three=3)

b={'one':1,'two':2,'three':3}

c=dict(zip(['one','two',three'],[1,2,3]))

d=dict([('two',2),('three',3),('one',1)])

e=dict({'three':3,'one':1,'two':2})



还有一个函数也可以

dict={}

dict.fromkeys((1,2,3),'shuzi')　　　　#会生成一个新的字典：    {1: 'shuzi', 2: 'shuzi', 3: 'shuzi'}



字典用到的函数

key()、value()、item()

dict1=dict.fromkeys(range(32),'赞')

for each in dict1.keys():
    　　print each
for each in dict1.values():
    　　print each
for each in dict1.items():
    　　print each

想看这个键有没有对应的值，又不想看到报错信息怎么办

dict.get(3)　　　　

dict.get(34)　　　　　　　　#括号里面是key的值，想查看那个就看那个，如果没有就返回None



还可以使用成员关系符号：in  和 not in 。来查看这个键是否在这个字典当中

举个栗子：

30 in dict1　　　　　　#返回TRUE

33 in dict1　　　　　　#返回FLASE



怎么清空一个字典？　　　　#clear

dict1.clear()

当然也可以这样：dict1={}

蛋是有下面这样的弊端：

dict1.fromkeys((1,2,3),'shuzi')

dict2=dict1

dict1={}

print dict2　　　　　　#这里还是之前dict1的值 ， 所以有些时候这种方法慎用

　　　　　　　　　　　#用clear是两个字典全部清空



浅拷贝

a={1:'gg',2:'mm',3:'biantai'}

b=a.copy()　　　　#这就是浅拷贝

c=a　　　　　　　　#这只是在相同的数据上加上不同的标签

id(a)　　　　id(b)　　　　　　id(c)　　　　#第一个和第三个是一样的，id(b)跟他们不一样

　　　　　　　　　　#改变a的值后，c会变，b不会变



还有一些函数：

a.pop(3)　　　　#删除“3”这个键和值，并返回“3”对应的值

a.popitem()　　　#删除最后一个键值对，因为字典是无序的，所以你也不知道会删除那个

a.setdefault(4)　　#增加一个键 蛋是这个键没有值

a.setdefault(5,'chengxuyuan')　　#有键有值了

最后一个：update 　　#用一个字典更新另外一个

举个栗子就知道了

b={'ergou':'dog'}

a.update(b)　　　　　　#a就会添加b中的键值对












