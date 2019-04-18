# Python入门--7--元祖：列表的顽固亲戚 - 默槑 - 博客园







# [Python入门--7--元祖：列表的顽固亲戚](https://www.cnblogs.com/modaidai/p/6802171.html)





一、创建和访问一个元祖

zheshiyige_yuanzu=(1,2,3,4,5,6)　　#创建一个元祖

zheshiyige_yuanzu[1]　　　　　　　　#打印第二个元素

zheshiyige_yuanzu[:5]　　　　　　　　#打印第一个元素到第五个元素　　

亦可以：zheshiyige_yuanzu=1,2,3,4,5,6　　#也能创建

但注意：zheshiyige_yuanzu=(1)　　　　#虽然能创建，蛋是他不是元祖，它的类型的int

zheshiyige_yuanzu=(1,)　　　　　　　　#这是元祖了

zheshiyige_yuanzu= ()　　　　　　　　　#创建一个空元祖

二、更新、删除元祖

temp = (kk,yy,tt,ff)

temp=temp[:2]+(ww,)+temp[2:]　　　　#用这种方式更新，以前的元祖也没有消失，一会才会被py删除

del temp　　　　　　　　　　　　　　　　#删除元祖

三、元祖相关的操作法

+：拼接，但两边必须是同一类型

*：

in：

<、>:

等等
四、补充

1、什么时候元祖需要加上括号，又要加上逗号

答：在元祖只有一个元素的时候

2、所有的多对象的、逗号分隔的、没有明确用符号定义的这些集合默认的类型都是元组

x, y, z = 1, 2, 3
print type(x)　　　　　　#整数
h = x, y, z
print type(h)　　　　　　#元祖












