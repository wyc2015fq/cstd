
# 【kittyzc的博客】python_运筹学_算法 - 第 3 页 - CSDN博客
# [kittyzc的博客](https://blog.csdn.net/kittyzc)


只看原创排序：[默认](javascript:void(0);)[按更新时间](https://blog.csdn.net/kittyzc/article/list?orderby=UpdateTime)[按访问量](https://blog.csdn.net/kittyzc/article/list?orderby=ViewCount)[](https://blog.csdn.net/kittyzc/rss/list)[RSS订阅](https://blog.csdn.net/kittyzc/rss/list)
#### [](https://blog.csdn.net/yoyo_liyy/article/details/82762601)
#### 原
#### [帝都的凛冬](https://blog.csdn.net/yoyo_liyy/article/details/82762601)
[各种AI、人工智能、大数据如秋日凉爽的风，杳然erzhi；区块链的风头得到短暂的下降。
                此次山竹台风造成了多少伤亡和破坏？人民的生命和财产遭受重大损失](https://blog.csdn.net/yoyo_liyy/article/details/82762601)
2019-04-14 12:10:55
阅读数：13
评论数：2

#### [](https://blog.csdn.net/kittyzc/article/details/81708873)
#### 原
#### [运筹系列5：凸优化、拉格朗日乘子法、KKT条件](https://blog.csdn.net/kittyzc/article/details/81708873)
[1. 从线性规划到凸优化
线性规划相对比较简单，比如：

min y = x1 + x2
s.t. x1 + 3*x2 ≥ 5
     2*x1 + x2 ≥ 6
    x1,x2≥0
求解步骤嘛，首先添加剩余变量x3消除不等式约束，将问题转化为：

max y = x1 + x2...](https://blog.csdn.net/kittyzc/article/details/81708873)
2018-08-15 18:18:13
阅读数399
评论数0

#### [](https://blog.csdn.net/kittyzc/article/details/81708729)
#### 原
#### [运筹系列4：整数规划割平面法python代码](https://blog.csdn.net/kittyzc/article/details/81708729)
[1. 从分支定界（branch and cut）到割平面（cutting plane）
割平面简单来说，就是添加约束条件。在分支定界算法中，添加的x≤floor[xs]和x≥ceil[xs]便是两个用来割平面的约束条件。 
分支定界法最终生成一颗树，当整数变量非常多时，求解节点会指数速度增加，因...](https://blog.csdn.net/kittyzc/article/details/81708729)
2018-08-15 18:06:44
阅读数487
评论数0

#### [](https://blog.csdn.net/kittyzc/article/details/81708457)
#### 原
#### [运筹系列3：整数规划分支定界法python代码](https://blog.csdn.net/kittyzc/article/details/81708457)
[1. 模型
整数规划的模型与线性规划基本相同，只是额外的添加了部分变量为整数的约束。

2. 求解步骤
整数规划求解的基本框架是分支定界法（Branch and bound，BnB）。首先去除整数约束得到“松弛模型”，使用线性规划的方法求解。若有某个变量不是整数，在松弛模型上分别添加约束：...](https://blog.csdn.net/kittyzc/article/details/81708457)
2018-08-15 17:51:27
阅读数1669
评论数6

#### [](https://blog.csdn.net/kittyzc/article/details/81708282)
#### 原
#### [运筹系列2：线性规划两阶段法python代码](https://blog.csdn.net/kittyzc/article/details/81708282)
[1. 补充问题
上一节中的代码在运行时还有很多细节没有处理，这里补充两个比较重要的情况： 
1. 存在等式约束 
如果有等式约束，那么就没法通过添加松弛变量直接给出初始可行解，需要用大M法或者两阶段法求初始可行解。计算机求解一般使用二阶段法，即首先给等式约束条件添加人工变量，使得问题有一个初始可...](https://blog.csdn.net/kittyzc/article/details/81708282)
2018-08-15 17:42:34
阅读数575
评论数3

#### [](https://blog.csdn.net/kittyzc/article/details/81707464)
#### 原
#### [运筹系列1：线性规划单纯形法python代码](https://blog.csdn.net/kittyzc/article/details/81707464)
[1. 模型
常见的线性规划模型如下： 
max z = cx 
s.t. Ax = b

2. 求解步骤
假设B是基变量集合，通过矩阵的线性变换，基变量可由非基变量表示： 
x’i = ci + Σj∉Bmi,jx’j （i∈B） 
目标函数z也可以完全由非基变量表示： 
z = z0 +...](https://blog.csdn.net/kittyzc/article/details/81707464)
2018-08-15 17:06:32
阅读数1558
评论数2


[](https://me.csdn.net/kittyzc)![](https://avatar.csdn.net/7/A/0/3_kittyzc.jpg)[](https://avatar.csdn.net/7/A/0/3_kittyzc.jpg)[](https://me.csdn.net/kittyzc)
[IE06](https://me.csdn.net/kittyzc)
关注
[原创](https://blog.csdn.net/kittyzc?t=1)44粉丝71喜欢18评论34
等级：[](https://blog.csdn.net/home/help.html#level)[](https://blog.csdn.net/home/help.html#level)访问：2万+积分：674排名：10万+
勋章：
专栏达人
授予成功创建个人博客专栏的用户。专栏中添加五篇以上博文即可点亮！撰写博客专栏浓缩技术精华，专栏达人就是你！


持之以恒
授予每个自然月内发布4篇或4篇以上原创或翻译IT博文的用户。不积跬步无以至千里，不积小流无以成江海，程序人生的精彩需要坚持不懈地积累！




### 博主专栏
[](https://blog.csdn.net/kittyzc/column/info/26511)![](//img-blog.csdn.net/20180823102429681?imageView2/5/w/120/h/120)[](//img-blog.csdn.net/20180823102429681?imageView2/5/w/120/h/120)[](https://blog.csdn.net/kittyzc/column/info/26511)
[运筹学算法python实现](https://blog.csdn.net/kittyzc/column/info/26511)
文章数：21 篇访问量：3471

[](https://blog.csdn.net/kittyzc/column/info/31935)![](https://img-blog.csdnimg.cn/20181227145040661.png?imageView2/5/w/120/h/120)[](https://img-blog.csdnimg.cn/20181227145040661.png?imageView2/5/w/120/h/120)[](https://blog.csdn.net/kittyzc/column/info/31935)
[python小tips](https://blog.csdn.net/kittyzc/column/info/31935)
文章数：6 篇访问量：31

[](https://blog.csdn.net/kittyzc/column/info/33845)![](http://img-blog.csdn.net/column?imageView2/5/w/120/h/120)[](http://img-blog.csdn.net/column?imageView2/5/w/120/h/120)[](https://blog.csdn.net/kittyzc/column/info/33845)
[强化学习系列](https://blog.csdn.net/kittyzc/column/info/33845)
文章数：13 篇访问量：28


个人分类
[](https://blog.csdn.net/kittyzc/article/category/7940149)python[](https://blog.csdn.net/kittyzc/article/category/7940149)35篇[](https://blog.csdn.net/kittyzc/article/category/7940149)
[](https://blog.csdn.net/kittyzc/article/category/7940150)运筹学[](https://blog.csdn.net/kittyzc/article/category/7940150)23篇[](https://blog.csdn.net/kittyzc/article/category/7940150)
[](https://blog.csdn.net/kittyzc/article/category/7940151)算法[](https://blog.csdn.net/kittyzc/article/category/7940151)27篇[](https://blog.csdn.net/kittyzc/article/category/7940151)
[](https://blog.csdn.net/kittyzc/article/category/7942472)spark[](https://blog.csdn.net/kittyzc/article/category/7942472)1篇[](https://blog.csdn.net/kittyzc/article/category/7942472)
[](https://blog.csdn.net/kittyzc/article/category/7946931)大数据[](https://blog.csdn.net/kittyzc/article/category/7946931)1篇[](https://blog.csdn.net/kittyzc/article/category/7946931)
[](https://blog.csdn.net/kittyzc/article/category/8425273)强化学习[](https://blog.csdn.net/kittyzc/article/category/8425273)8篇[](https://blog.csdn.net/kittyzc/article/category/8425273)
展开
归档
[2019年4月](https://blog.csdn.net/kittyzc/article/month/2019/04)3篇[](https://blog.csdn.net/kittyzc/article/month/2019/04)
[2019年3月](https://blog.csdn.net/kittyzc/article/month/2019/03)2篇[](https://blog.csdn.net/kittyzc/article/month/2019/03)
[2019年2月](https://blog.csdn.net/kittyzc/article/month/2019/02)3篇[](https://blog.csdn.net/kittyzc/article/month/2019/02)
[2019年1月](https://blog.csdn.net/kittyzc/article/month/2019/01)3篇[](https://blog.csdn.net/kittyzc/article/month/2019/01)
[2018年12月](https://blog.csdn.net/kittyzc/article/month/2018/12)7篇[](https://blog.csdn.net/kittyzc/article/month/2018/12)
[2018年11月](https://blog.csdn.net/kittyzc/article/month/2018/11)11篇[](https://blog.csdn.net/kittyzc/article/month/2018/11)
[2018年10月](https://blog.csdn.net/kittyzc/article/month/2018/10)4篇[](https://blog.csdn.net/kittyzc/article/month/2018/10)
[2018年9月](https://blog.csdn.net/kittyzc/article/month/2018/09)2篇[](https://blog.csdn.net/kittyzc/article/month/2018/09)
[2018年8月](https://blog.csdn.net/kittyzc/article/month/2018/08)10篇[](https://blog.csdn.net/kittyzc/article/month/2018/08)
展开
热门文章
[运筹系列8：大规模线性规划的列生成方法](https://blog.csdn.net/kittyzc/article/details/81712448)阅读数4433
[运筹系列7：大规模线性规划的行生成和Benders分解](https://blog.csdn.net/kittyzc/article/details/81712257)阅读数2649
[运筹系列3：整数规划分支定界法python代码](https://blog.csdn.net/kittyzc/article/details/81708457)阅读数1663
[运筹系列1：线性规划单纯形法python代码](https://blog.csdn.net/kittyzc/article/details/81707464)阅读数1530
[运筹系列10：线性规划开源软件GLPK和PyMathProg](https://blog.csdn.net/kittyzc/article/details/82789439)阅读数930

最新评论
[运筹系列1：线性规划单纯形法pyt...](https://blog.csdn.net/kittyzc/article/details/81707464#comments)[qq_36959251：](https://my.csdn.net/qq_36959251)list.remove(x): x not in list 为啥会报这个错呀
[运筹系列3：整数规划分支定界法py...](https://blog.csdn.net/kittyzc/article/details/81708457#comments)[weixin_44193909：](https://my.csdn.net/weixin_44193909)[reply]weixin_44193909[/reply]
&lt;是小于号，发出来不知怎么变成乱码了
[运筹系列3：整数规划分支定界法py...](https://blog.csdn.net/kittyzc/article/details/81708457#comments)[weixin_44193909：](https://my.csdn.net/weixin_44193909)[code=python]
min -x1-5x2
s.t -x1+x2&lt;=2
     5x1+6x2&lt;=30
     x2>=4
     x1,x2>=0
[/code]
运行结果：(-55340232221128654842, [9223372036854775807, 9223372036854775807]) 实际答案：（-17，[2,3]） 兄弟，换个数据出了点问题帮忙解决一下。下面是我修改地方的代码，其它地方没变。
[code=python]
res = linprog(c, A_ub=A, b_ub=b, A_eq=Aeq, b_eq=beq,bounds=((0,None),(0,4)))\#加了bounds
"""我的数据"""
c = [-1,-5]
A = [[-1,1],[5,6]]
b = [2,30]
Aeq = [[0,0]]
beq = [0]
[/code]
[运筹系列3：整数规划分支定界法py...](https://blog.csdn.net/kittyzc/article/details/81708457#comments)[weixin_44193909：](https://my.csdn.net/weixin_44193909)[reply]kittyzc[/reply]
多个括号。。。互相关注下，以后多交流
[运筹系列3：整数规划分支定界法py...](https://blog.csdn.net/kittyzc/article/details/81708457#comments)[kittyzc：](https://my.csdn.net/kittyzc)[reply]weixin_44193909[/reply]
第七行需要添加求解状态的判断：if (type(res.x) is float or res.status != 0): 
感谢指正！




提示
确定要删除当前文章？
取消删除
关闭
关闭
