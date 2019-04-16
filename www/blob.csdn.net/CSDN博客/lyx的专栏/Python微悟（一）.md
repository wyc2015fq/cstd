# Python微悟（一） - lyx的专栏 - CSDN博客





2015年12月22日 16:12:38[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：534
所属专栏：[和python在一起的日子](https://blog.csdn.net/column/details/16842.html)









# 1.容器

python的基本容器可以分为五类，其中list、tuple、string属于序列，因为他们的index是位置，存在有顺序这样一个概念，而字典Dict则是键值对的形式存在，没有顺序的概念，他的键（Key）就相当于是序列的Index。

![my picture](http://datayes.oss-cn-hangzhou.aliyuncs.com/1.jpg)

![my picture](http://datayes.oss-cn-hangzhou.aliyuncs.com/3.jpg)


# 2.函数的参数问题



定义变参数函数


有时需要定义参数个数可变的函数，有几个方法可以做到：
- 给参数指定默认值


比如，定义参数f(a,b=1,c=’hehe’)，那么在调用的时候，后面两个参数可以定义也可以不定义，不定义的话默认为b=1，c=’hehe’，因此如下调用都可以：


F(‘dsds’);


F(‘dsds’,2);


F(‘dsds’,2,’hdasda’);
- 参数关键字


上面的方法等于固定了参数的位置，第一个值就是第一个参数的赋值。而“参数关键字”方法，其实是固定了参数关键字，比如仍然定义参数 f(a,b=1,c=’hehe’)，调用的时候可以用关键字来固定：


F(b=2,a=11)


位置可以动，只要参数关键指出来就可以了。




# 3.列表推导式


[输出值
 for 条件]


[x*x
 for x in range(10) if x%3==0]


个人感觉就像是一个Filter加上一个作用器






