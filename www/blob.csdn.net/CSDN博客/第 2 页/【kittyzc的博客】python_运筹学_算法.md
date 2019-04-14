
# 【kittyzc的博客】python_运筹学_算法 - 第 2 页 - CSDN博客
# [kittyzc的博客](https://blog.csdn.net/kittyzc)


只看原创排序：[默认](javascript:void(0);)[按更新时间](https://blog.csdn.net/kittyzc/article/list?orderby=UpdateTime)[按访问量](https://blog.csdn.net/kittyzc/article/list?orderby=ViewCount)[](https://blog.csdn.net/kittyzc/rss/list)[RSS订阅](https://blog.csdn.net/kittyzc/rss/list)
#### [](https://blog.csdn.net/yoyo_liyy/article/details/82762601)
#### 原
#### [帝都的凛冬](https://blog.csdn.net/yoyo_liyy/article/details/82762601)
[各种AI、人工智能、大数据如秋日凉爽的风，杳然erzhi；区块链的风头得到短暂的下降。
                此次山竹台风造成了多少伤亡和破坏？人民的生命和财产遭受重大损失](https://blog.csdn.net/yoyo_liyy/article/details/82762601)
2019-04-14 12:10:46
阅读数：13
评论数：2

#### [](https://blog.csdn.net/kittyzc/article/details/84318934)
#### 原
#### [运筹系列18：routing模型之VRPTW问题](https://blog.csdn.net/kittyzc/article/details/84318934)
[1. 问题模型
当VRP问题有到达时间的约束条件时，问题变为VRPTW（VRP with Time Windows）。
2. 例子
每个点右下角的数字表示需求，上方的两个数字表示开始服务的时间窗，车辆最大允许服务时间为120。单位服务时间=5*需求量，网格长度114，高度80，车辆速度83.33，...](https://blog.csdn.net/kittyzc/article/details/84318934)
2018-11-21 14:18:15
阅读数566
评论数3

#### [](https://blog.csdn.net/kittyzc/article/details/84314956)
#### 原
#### [运筹系列17：routing模型之CVRP问题](https://blog.csdn.net/kittyzc/article/details/84314956)
[1. CVRP模型
CVRP指的是有容量（capacity）限制的VRP模型，是最常见的VRP模型。
2. 示例
依旧用上一章节的点，不同的是每个点多了一个送货需求。每辆车的最大容量是15，最小化总运输距离。
dimension可以使用AddDimensionWithVehicleCapacity...](https://blog.csdn.net/kittyzc/article/details/84314956)
2018-11-21 10:42:59
阅读数303
评论数0

#### [](https://blog.csdn.net/kittyzc/article/details/84309295)
#### 原
#### [运筹系列16：routing模型之VRP问题](https://blog.csdn.net/kittyzc/article/details/84309295)
[1. 问题模型
VRP问题是车辆路径问题的缩写。问题是：有N辆车，都从原点出发，每辆车访问一些点后回到原点，要求所有的点都要被访问到，求最短的车辆行驶距离或最少需要的车辆数或最小化最长行驶距离。
常见的限制要求包括：车辆容量限制、时间窗限制、点访问顺序要求等。
2. RoutingModel的一些...](https://blog.csdn.net/kittyzc/article/details/84309295)
2018-11-20 21:28:33
阅读数610
评论数2

#### [](https://blog.csdn.net/kittyzc/article/details/84306786)
#### 原
#### [运筹系列15：routing模型之TSP问题](https://blog.csdn.net/kittyzc/article/details/84306786)
[1. 问题模型
TSP问题是旅行商问题的简写，问题非常简单：从原点出发经过所有需求点并回到原点，使得途经的距离最短。
ortools可以使用RoutingModel来进行求解。
2. 求解代码
首先建立RoutingModel模型：
routing = pywrapcp.RoutingModel(...](https://blog.csdn.net/kittyzc/article/details/84306786)
2018-11-20 17:42:34
阅读数230
评论数0

#### [](https://blog.csdn.net/kittyzc/article/details/84302911)
#### 原
#### [运筹系列19：scheduling模型与python代码求解](https://blog.csdn.net/kittyzc/article/details/84302911)
[1. 问题模型
scheduling问题比assignment问题又要复杂很多。在调度问题中，除了要考虑任务分配外，还要考虑时间约束。
来看一个官方例子：job shop problem。数据如下：
job 0 = [(0, 3), (1, 2), (2, 2)]
job 1 = [(0, 2),...](https://blog.csdn.net/kittyzc/article/details/84302911)
2018-11-20 15:02:24
阅读数301
评论数0

#### [](https://blog.csdn.net/kittyzc/article/details/84262042)
#### 原
#### [运筹系列13：Network Flows模型与python代码求解](https://blog.csdn.net/kittyzc/article/details/84262042)
[1. 网络流问题
网络流问题一般首先定义一张由点和边构成的图。
最大流问题（maximum flow problem）中每条边有一个容量限制，要求最大化起点到终点可以通过的流量。
最小费用流问题（minimum cost flows）中，除了容量限制，每条边还对应着一个单位流量的费用。要求在满足终...](https://blog.csdn.net/kittyzc/article/details/84262042)
2018-11-20 09:22:27
阅读数148
评论数0

#### [](https://blog.csdn.net/kittyzc/article/details/84260593)
#### 原
#### [运筹系列12：约束规划和python求解](https://blog.csdn.net/kittyzc/article/details/84260593)
[1. 从算式谜问题说起
SEND + MORE = MONEY，每一个字母代表一个数字，如何求解？
这个问题显然不是传统意义上的整数规划问题，并没有需要进行优化的目标函数，因此使用约束规划进行求解，问题建模如下：
from ortools.sat.python import cp_model
\# ...](https://blog.csdn.net/kittyzc/article/details/84260593)
2018-11-19 22:50:23
阅读数279
评论数0

#### [](https://blog.csdn.net/kittyzc/article/details/84145684)
#### 原
#### [运筹系列14：Assignment问题模型与python代码求解](https://blog.csdn.net/kittyzc/article/details/84145684)
[1. 问题描述
分配问题可以简单描述为：有数个人和数个任务，人做任务有不同的费用。每个人最多只能做一项任务，每个任务只能由一个人做。如何将任务分配给人可以使总费用最小？
用数学语言表示为：
minminmin Σi∈I,j∈Jcijxij\Sigma_{i\in I,j\in J}c_{ij}x_...](https://blog.csdn.net/kittyzc/article/details/84145684)
2018-11-16 20:49:29
阅读数152
评论数0

#### [](https://blog.csdn.net/kittyzc/article/details/84143766)
#### 原
#### [运筹系列11：google的OR-Tools简介](https://blog.csdn.net/kittyzc/article/details/84143766)
[1. 介绍
google的开源优化算法包ortools，支持线性规划、整数规划，可以方便的求解Routing、Bin packing、Network flows、Assignment、Scheduling等问题。官网地址为：https://developers.google.com/optimiz...](https://blog.csdn.net/kittyzc/article/details/84143766)
2018-11-16 18:07:42
阅读数947
评论数0

#### [](https://blog.csdn.net/kittyzc/article/details/83054988)
#### 原
#### [python数据绘图pyecharts](https://blog.csdn.net/kittyzc/article/details/83054988)
[pyecharts的绘图非常漂亮，这里总结一下学习内容。
1. 安装相关：
使用python2.7前加入中文支持：
\#coding=utf-8
from future import unicode_literals
目前pyecharts不带地图文件，因此如果要使用地图的话必须自己下载：...](https://blog.csdn.net/kittyzc/article/details/83054988)
2018-10-15 10:14:22
阅读数234
评论数0

#### [](https://blog.csdn.net/kittyzc/article/details/83054250)
#### 原
#### [python调用soa服务](https://blog.csdn.net/kittyzc/article/details/83054250)
[公司项目需要调用基于soa接口的服务，几个技术人员用java配置了一整天都没有搞定，最后一怒之后我自己把要调用的功能重写了一遍。最近用python试了一下，结果一行代码就搞定了，真是感慨万千。
1. SOA
SOA是service-oriented architecture（面向服务的架构）的...](https://blog.csdn.net/kittyzc/article/details/83054250)
2018-10-15 09:32:21
阅读数210
评论数0

#### [](https://blog.csdn.net/kittyzc/article/details/83022577)
#### 原
#### [python操作redis](https://blog.csdn.net/kittyzc/article/details/83022577)
[1. redis介绍
redis是一个运行在内存中的key-value存储系统，主要优势在于读写性能非常棒。
reids的安装非常简单，到这里下载最新版本，解压缩后进入文件夹，执行make命令即可。
make完后 redis-2.8.17目录下会出现编译后的redis服务程序redis-se...](https://blog.csdn.net/kittyzc/article/details/83022577)
2018-10-12 10:01:55
阅读数31
评论数0

#### [](https://blog.csdn.net/kittyzc/article/details/83006403)
#### 原
#### [强化学习系列2：Open AI的gym框架、baselines和Spinning Up](https://blog.csdn.net/kittyzc/article/details/83006403)
[1. gym介绍
Openai gym 是一个用于开发和比较 强化学习算法的工具包，github地址点击这里。gym安装十分简单：
pip install gym
gym 的核心接口是 Env，作为统一的环境接口。Env 包含下面几个核心方法：
1、reset(self):重置环境的状...](https://blog.csdn.net/kittyzc/article/details/83006403)
2018-10-11 22:57:59
阅读数236
评论数0

#### [](https://blog.csdn.net/kittyzc/article/details/82789439)
#### 原
#### [运筹系列10：线性规划开源软件GLPK和PyMathProg](https://blog.csdn.net/kittyzc/article/details/82789439)
[1. GLPK
GLPK全称GNU Linear Programming Kit，是一个开源的求解线性规划问题的工具套件，由c写成，可以求解大规模线性规划问题、混合整数规划问题。GLPK是免费的，在大规模问题上的性能要逊色于商用软件，求解的性能可以参考安装文件doc目录下的miplib2.txt...](https://blog.csdn.net/kittyzc/article/details/82789439)
2018-09-21 19:52:44
阅读数947
评论数0

#### [](https://blog.csdn.net/kittyzc/article/details/82504886)
#### 原
#### [大数据学习笔记](https://blog.csdn.net/kittyzc/article/details/82504886)
[1. 基本概念
Docker：就是操作系统中的chroot。可以理解为你在写一篇论文的时候新建了一个文件夹，所有的东西都在这个文件夹里面。 
Host：主机，运行容器的机器。比如我们自己的电脑。 
Image：镜像，文件的层次结构，包含如何运行容器的元数据。可以比成一个论文模板。 
Contai...](https://blog.csdn.net/kittyzc/article/details/82504886)
2018-09-09 14:30:43
阅读数58
评论数0

#### [](https://blog.csdn.net/kittyzc/article/details/82147425)
#### 原
#### [自然语言处理核心内容](https://blog.csdn.net/kittyzc/article/details/82147425)
[1. 现代自然语言处理
一般我们学习语言的时候，都是从语法规则+字/词汇开始学习的，这也是传统的语言处理方法。现代方式则是基于统计学的，简单来说，就是：说的人多了，就是正确的话（可以是词汇，可以是句子，可以是文章）。换种方式表达，就是：出现的概率多了，就是正确的话。用数学语言表达，则是：SSS出...](https://blog.csdn.net/kittyzc/article/details/82147425)
2018-08-28 14:32:38
阅读数126
评论数0

#### [](https://blog.csdn.net/kittyzc/article/details/81744545)
#### 原
#### [PySpark学习笔记](https://blog.csdn.net/kittyzc/article/details/81744545)
[读入文件：
spam = sc.textFile(&amp;amp;quot;spam.txt&amp;amp;quot;)
normal = sc.textFile(&amp;amp;quot;normal.txt&amp;amp;quot;)
构建词频向量：...](https://blog.csdn.net/kittyzc/article/details/81744545)
2018-08-16 16:39:55
阅读数376
评论数0

#### [](https://blog.csdn.net/kittyzc/article/details/81712485)
#### 原
#### [运筹系列9：混合整数非线性规划扩展割平面法](https://blog.csdn.net/kittyzc/article/details/81712485)
[1. 混合整数非线性规划
混合整数非线性规划，英文为Mixed-Integer NonLinear Programming (MINLP) ，指的是既包含整数约束，也包含非线性约束的优化问题。

2. 扩展割平面法
扩展割平面（Extended cutting plane，ECP）的思路是...](https://blog.csdn.net/kittyzc/article/details/81712485)
2018-08-15 22:36:00
阅读数623
评论数0

#### [](https://blog.csdn.net/kittyzc/article/details/81712448)
#### 原
#### [运筹系列8：大规模线性规划的列生成方法](https://blog.csdn.net/kittyzc/article/details/81712448)
[1. 应用场景
列生成算法是不断添加求解变量的算法，可参考论文。列生成算法常常用于如下的场景：使用set-covering构建的模型，变量非常多，约束相对较少。 
具体来说，场景如下：有nnn个0-1变量z1...znz1...znz_1...z_n，每个ziziz_i带着很多中间变量xi,jx...](https://blog.csdn.net/kittyzc/article/details/81712448)
2018-08-15 22:32:47
阅读数4467
评论数5

#### [](https://blog.csdn.net/kittyzc/article/details/81708924)
#### 原
#### [运筹系列6：凸优化梯度下降法python代码](https://blog.csdn.net/kittyzc/article/details/81708924)
[1. 梯度下降法
梯度下降法与现在火热的机器学习、深度学习息息相关，相关的文章非常多。其基本思路可以理解为一种启发式搜索算法，每次迭代都寻找附近一定范围内的最小值。 
其基本步骤是： 
使用拉格朗日法对一般的凸优化问题进行转化，假设问题为min L(x)L(x)L(x)，从一个初始解x0x0x_...](https://blog.csdn.net/kittyzc/article/details/81708924)
2018-08-15 21:28:55
阅读数385
评论数0


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
