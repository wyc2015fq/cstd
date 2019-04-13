
# Python的Lambda表达式 - saltriver的专栏 - CSDN博客


2016年08月12日 20:07:32[saltriver](https://me.csdn.net/saltriver)阅读数：589


Python的 Lambda 表达式，本质上仍是一个函数。通常是在**需要一个函数，**但这个函数**也许就用这一次**，或者这个函数**异常简单**，于是**不想去耗费时间精力定义一个函数**的场合下使用。由于是没有名字的函数，所以lamba也通常叫**匿名函数**。
这种场景在实际应用中其实很常见：你在某处就真的只需要一个能做一件事情的函数而已，连它叫什么名字都无关紧要，Lambda 表达式就可以用来做这件事。
看网上举的较多的Lambda配合map的例子：
a= [0,1,2,3,4,5]
print(list(map(lambdax: x **2,a)))
与下面的显式函数sq()定义功能相同。defsq(x):
returnx ** 2
a = [0,1,2,3,4,5]
print(list(map(sq,a)))
输出结果都是：[0, 1, 4, 9, 16, 25]实际上这种情况下用**列表推导式**效果更好。a= [0,1,2,3,4,5]
print([x **2forxina])
参考：[http://saltriver.blog.163.com/blog/static/226097154201652311165422](http://saltriver.blog.163.com/blog/static/226097154201652311165422)

**个人认为Lambda用在一些较复杂结构序列的****sort/sorted、max，min等****操作中更能发挥作用。示例：**
scores= [['Michael',4.0],['Lisa',4.2],['Jack',5.0],['Rose',4.5],['Tony',3.5]]scores表示每个人和相应得分的嵌套列表，需要按得分进行排序操作，用Lambda表达式：
print('max_scores=',max(scores,key=lambdax: x[1]))
scores.sort(key=lambdax: x[1])
print('scores=',scores)输出结果：
max_scores= ['Jack', 5.0]
scores= [['Tony', 3.5], ['Michael', 4.0], ['Lisa', 4.2], ['Rose', 4.5], ['Jack', 5.0]]

Lambda的使用非常方便，但Python 为了防止Lambda的滥用，造成可读性下降和程序不清晰的问题， 限制Lambda只能由1条表达式组成。也就是回归Lambda的本质，执行简单操作的无名（匿名）函数。对于简单的，易于理解的，用lambda没问题，但是对于较复杂的操作和运算，进行正常的函数定义吧。




