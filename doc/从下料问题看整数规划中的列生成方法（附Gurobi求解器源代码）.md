# 从下料问题看整数规划中的列生成方法（附Gurobi求解器源代码）

[![留德华叫兽](https://pic4.zhimg.com/v2-9f62cb7cf5fd6abb06893e96e631eb10_xs.jpg)](https://www.zhihu.com/people/ruobingshen)

[留德华叫兽](https://www.zhihu.com/people/ruobingshen)



海德堡大学 交叉学科计算中心离散与组合优化实验室研究员





## **1. 前言**

“列生成方法（Column Generation，CG）用来解决大型线性规划问题”，这是许多书中对列生成算法的描述最多的一句话，接下来的D-W分解（Dantzig-Wolfe decomposition），可能就让初学者迷迷糊糊，从理论到理论的证明并不是工科生的强项，所以我换了一个角度来简单描述一下列生成方法的一个初步使用。旅行商问题（Travelling salesman problem,TSP），车辆路径问题（Vehicle Routing Problem,VRP），车间调度问题（scheduling）等一系列组合优化问题的下界（lower bound）求解方向的论文可以查得到许多，其中也有人使用拉格朗日松弛（lagrangian relaxation）等不同的求解技术。



## **2. 优化对象：一维下料问题**

**一维下料问题**的 一般描述是：钢材厂（也许是木材厂，铝材厂，造纸厂，emmm..）有一批钢管数量为 ![N](https://www.zhihu.com/equation?tex=N) ，长度都为 ![L](https://www.zhihu.com/equation?tex=L) ；假设有 ![m](https://www.zhihu.com/equation?tex=m) 个顾客，来钢厂下订单:每位顾客需要长度为 ![l_{i}](https://www.zhihu.com/equation?tex=l_%7Bi%7D) （ ![l_{i}\preceq L](https://www.zhihu.com/equation?tex=l_%7Bi%7D%5Cpreceq+L)，也就是下文切割规格 ）的钢管 ![d_{i}](https://www.zhihu.com/equation?tex=d_%7Bi%7D) 根，优化的目标是:如何切割钢管使得我既能满足客户的需求又使得的消耗的最少呢？

显然，在不加附加约束的情况下，这是一个典型的**0-1整数规划(IP)，**可以参见 

 

相关介绍，例如：



[【学界】离散/整数/组合/非凸优化概述及其在AI的应用](https://zhuanlan.zhihu.com/p/27429666)

而且我们知道最差的情况是用料 ![\sum_{i=1}^{m}{d_{i}}](https://www.zhihu.com/equation?tex=%5Csum_%7Bi%3D1%7D%5E%7Bm%7D%7Bd_%7Bi%7D%7D) ，也就是我每一根都拿来切，每一根只切成 ![l_{i}](https://www.zhihu.com/equation?tex=l_%7Bi%7D) 的长度。这样是很可耻的，剩下的拿来做金箍棒吗？？所以我们得利用数学的力量！建模啊



- **模型1：一种直观的建模方式**

第一步：确定我们的**决策变量为** **![y_{i}](https://www.zhihu.com/equation?tex=y_%7Bi%7D)** 表示是否写切割钢管i**,** 既然是整数规划， ![y_{i}](https://www.zhihu.com/equation?tex=y_%7Bi%7D) 的取值只能是整数。

![y_i = \left\{ \begin{array}{ll} 1\, & \text{if cut;}\\ 0\, & \text{otherwise;}\\ \end{array} \right. ](https://www.zhihu.com/equation?tex=y_i+%3D+%5Cleft%5C%7B+%5Cbegin%7Barray%7D%7Bll%7D+1%5C%2C+%26+%5Ctext%7Bif+cut%3B%7D%5C%5C+0%5C%2C+%26+%5Ctext%7Botherwise%3B%7D%5C%5C+%5Cend%7Barray%7D+%5Cright.+)

在每一根上我们不可能只切割一种 ![l_{i}](https://www.zhihu.com/equation?tex=l_%7Bi%7D) ；为确保最小浪费，所以要尽可能安排多的切割规格在一根钢管上，所以还需要添加**变量** ![x_{ij}](https://www.zhihu.com/equation?tex=x_%7Bij%7D) ,表示在钢管i上切 ![x_{ij}](https://www.zhihu.com/equation?tex=x_%7Bij%7D) 次来满足客户j的需求。到此大家可能自己也把约束想出来了：

优化目标：最少的切割钢管数

约束1：不管怎样咋切，顾客的要求一定满足（他们是上帝）

约束2：我在选中的每根钢管上切割，不管安排几个切割规格，加和总不能大于我的钢管长度 ![L](https://www.zhihu.com/equation?tex=L)（巧妇难为无米之炊）

这种方式emmmmm...........不太数学，换一下

![\begin{align} &\min_{x_{ij}, y_{i}} \, && \sum_{i=1}^{N} y_i \\ &\text{s.t.}\, && \sum_{i=1}^{N} x_{ij} \ge d_j \, && j=1,2,\dots, m\\ &&& \sum_{j=1}^{m}l_j x_{ij}\le Ly_i \, && i=1,2,\dots,N\\ &&& x_{ij} \in \mathbb{Z} \, && i=1,\dots,N, \, j=1,\dots,m\\ &&& y_i \in \{0,1\} \, && i=1,\dots,N \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%26%5Cmin_%7Bx_%7Bij%7D%2C+y_%7Bi%7D%7D+%5C%2C+%26%26+%5Csum_%7Bi%3D1%7D%5E%7BN%7D+y_i+%5C%5C+%26%5Ctext%7Bs.t.%7D%5C%2C+%26%26+%5Csum_%7Bi%3D1%7D%5E%7BN%7D+x_%7Bij%7D+%5Cge+d_j+%5C%2C+%26%26+j%3D1%2C2%2C%5Cdots%2C+m%5C%5C+%26%26%26+%5Csum_%7Bj%3D1%7D%5E%7Bm%7Dl_j+x_%7Bij%7D%5Cle+Ly_i+%5C%2C+%26%26+i%3D1%2C2%2C%5Cdots%2CN%5C%5C+%26%26%26+x_%7Bij%7D+%5Cin+%5Cmathbb%7BZ%7D+%5C%2C+%26%26+i%3D1%2C%5Cdots%2CN%2C+%5C%2C+j%3D1%2C%5Cdots%2Cm%5C%5C+%26%26%26+y_i+%5Cin+%5C%7B0%2C1%5C%7D+%5C%2C+%26%26+i%3D1%2C%5Cdots%2CN+%5Cend%7Balign%7D)

还是抽象？那具体到一个例子来看：

假设我的钢管长度为120米，总共有240根，现在有10个顾客，顾客的要求用Python随机生成如下：

demands = [10, 11, 11, 12, 12, 12, 10, 11, 12, 10]

lengths = [92, 59, 97, 32, 38, 55, 80, 75, 108, 57]

把上边约束写开：

![\begin{align} &\min && y_1 + y_2 + \dots + y_{240} \\ &\text{s.t.} && x_{1,1} + x_{2,1} + \dots + x_{240,1} \ge 10\\ &&& x_{1,2} + x_{2,2} + \dots + x_{240,2} \ge 11 \\ &&& \vdots \ddots \\ &&& x_{1,10} + x_{2,10} + \dots + x_{240,10} \ge 10\\ &&& 92x_{1,1} + 59x_{1,2} + \dots +57x_{1,10} \le 120 y_1 \\ &&& 92x_{2,1} + 59x_{2,2} + \dots +57x_{2,10} \le 120 y_2 \\ &&& \vdots \ddots \\ &&& 92x_{240,1} + 59x_{240,2} + \dots +57x_{240,10} \le 120y_{240} \\ &&& x_{ij} \in \mathbb{Z},\, i = 1,2,\dots,240 \, j= 1,2,\dots,10\\ &&& y_i \in \{0,1\}, j = 1,2,\dots,240 \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%26%5Cmin+%26%26+y_1+%2B+y_2+%2B+%5Cdots+%2B+y_%7B240%7D+%5C%5C+%26%5Ctext%7Bs.t.%7D+%26%26+x_%7B1%2C1%7D+%2B+x_%7B2%2C1%7D+%2B+%5Cdots+%2B+x_%7B240%2C1%7D+%5Cge+10%5C%5C+%26%26%26+x_%7B1%2C2%7D+%2B+x_%7B2%2C2%7D+%2B+%5Cdots+%2B+x_%7B240%2C2%7D+%5Cge+11+%5C%5C+%26%26%26+%5Cvdots+%5Cddots+%5C%5C+%26%26%26+x_%7B1%2C10%7D+%2B+x_%7B2%2C10%7D+%2B+%5Cdots+%2B+x_%7B240%2C10%7D+%5Cge+10%5C%5C+%26%26%26+92x_%7B1%2C1%7D+%2B+59x_%7B1%2C2%7D+%2B+%5Cdots+%2B57x_%7B1%2C10%7D+%5Cle+120+y_1+%5C%5C+%26%26%26+92x_%7B2%2C1%7D+%2B+59x_%7B2%2C2%7D+%2B+%5Cdots+%2B57x_%7B2%2C10%7D+%5Cle+120+y_2+%5C%5C+%26%26%26+%5Cvdots+%5Cddots+%5C%5C+%26%26%26+92x_%7B240%2C1%7D+%2B+59x_%7B240%2C2%7D+%2B+%5Cdots+%2B57x_%7B240%2C10%7D+%5Cle+120y_%7B240%7D+%5C%5C+%26%26%26+x_%7Bij%7D+%5Cin+%5Cmathbb%7BZ%7D%2C%5C%2C+i+%3D+1%2C2%2C%5Cdots%2C240+%5C%2C+j%3D+1%2C2%2C%5Cdots%2C10%5C%5C+%26%26%26+y_i+%5Cin+%5C%7B0%2C1%5C%7D%2C+j+%3D+1%2C2%2C%5Cdots%2C240+%5Cend%7Balign%7D)

这样就比较清楚了，接下来，我们用Python2.7写调用gurobi 求解器进行求解(这个模型这么完美，一定很快得出答案！千万不要这么想，因为我们还没用列生成)，我们把程序录入

```text
#-*- coding: cp936 -*-
import numpy as np
import random
from gurobipy import *


L = 120 #最大长
M =240 #总数
demands =[ 10 , 11,  11 , 12 , 12 , 12 , 10 , 11 , 12 , 10]
lengths = [ 92,  59  ,97 , 32,  38 , 55 , 80 , 75 ,108,  57]
demands=np.array(demands )
lengths =np.array(lengths)
c = {}
x={}
mol = Model("XP")
for i in range(M):
    c[i] = mol.addVar(obj=1,  vtype=GRB.BINARY,name="c[%s]"%i ) #添加决策变量
for i in range(10):
    for j in range(M):#添加变量
        x[j,i]=mol.addVar(lb=0,obj=0,  vtype=GRB.INTEGER,name="c[%s][%s]"%(j,i))

for k in range(10):        
    coef =[1 for j in range(M) ]
    var = [x[j,k] for j in range(M) ]
    mol.addConstr(LinExpr(coef, var), ">=", demands[k], name="demands" )


for l in range(M):        
    coef = [lengths[i] for i in range(10)]
    var = [x[l, i] for i in range(10) ]
    mol.addConstr(LinExpr(coef, var), "<=", L*c[l], name="withlimit" )


mol.optimize()

mol.printAttr('x')
obj = mol.getObjective()
print obj
for v in mol.getVars():
    print('%s %g' % (v.varName, v.x))
```

waiting。。。。

waiting。。。。

。。。。。。



![img](https://pic3.zhimg.com/80/v2-de28f61cf572453165f0f850b3b03672_hd.jpg)gurobi的求解过程

![img](https://pic4.zhimg.com/80/v2-128b8645b13a5451aea0687ac545e1b3_hd.jpg)输出的最后的结果

以上截图是gurobi的log部分截图，我们找到最优了！只需要切割72根，没错，时间就是这么长1295s，我们的规模也不大，10客户都满足不了？？工厂恐怕每天不止10个客户............不要想：一定是Python的问题！你可以用C++,C, 那就是求解器gurobi的问题，你可以选用cplex,(有用的，可以对比结果)。此模型在1939 年，由前苏联经济学家 Kantorovich 给出[1]



- **对模型1的反思**

只要思想不滑坡，方法总比困难多！

如果我们这样思考：在模型1中我们算的是最小的钢管被切割数量，但是我们同时也知道了**每一种切割规格被切割的次数**和**每一根钢管上的切割规格。**我们可以把每一种钢管的切割看着成一种模式，再次建模。

**举例说明：**假设有4个顾客和其要求来看，其要求的长度lengths=[92, 59, 97, 32]数量是demands=[10, 11, 11, 12]我们可以列出其切割模式：

![M_{1}](https://www.zhihu.com/equation?tex=M_%7B1%7D) ：长度59，切割2次。

![M_{2}](https://www.zhihu.com/equation?tex=M_%7B2%7D) ：长度92，切割1次。

![M_{3}](https://www.zhihu.com/equation?tex=M_%7B3%7D) ：长度32，切割3次。

![M_{4}](https://www.zhihu.com/equation?tex=M_%7B4%7D) : 长度59，切割1次，长度38，切割2次。

。。。

可以看出我们只要从中找出最好的组合，问题就迎刃而解。换成数学表达形式：

用 ![y_{j}](https://www.zhihu.com/equation?tex=y_%7Bj%7D) 此时用来表示切割模式 ![M_{j}](https://www.zhihu.com/equation?tex=M_%7Bj%7D) 使用的次数;

![m_{ij}](https://www.zhihu.com/equation?tex=m_%7Bij%7D) 表示在第j种模式下，第i个要求切割次数。

表示成矩阵形式：

![\left( \begin{array}{ccc} m_{11} & \cdots & m_{1n} \\ \vdots& \ddots& \vdots \\ m_{m1} & \cdots & m_{mn} \end{array} \right)](https://www.zhihu.com/equation?tex=%5Cleft%28+%5Cbegin%7Barray%7D%7Bccc%7D+m_%7B11%7D+%26+%5Ccdots+%26+m_%7B1n%7D+%5C%5C+%5Cvdots%26+%5Cddots%26+%5Cvdots+%5C%5C+m_%7Bm1%7D+%26+%5Ccdots+%26+m_%7Bmn%7D+%5Cend%7Barray%7D+%5Cright%29)

以上四种模式写成矩阵： ![\begin{bmatrix} 0 & 1 & 0 & 0 & \cdots\\ 2 & 0 & 0 & 1 & \dots \\ 0 & 0 & 0 & 0 &\\ 0 & 0 & 3 & 2 & \cdots \end{bmatrix}](https://www.zhihu.com/equation?tex=%5Cbegin%7Bbmatrix%7D+0+%26+1+%26+0+%26+0+%26+%5Ccdots%5C%5C+2+%26+0+%26+0+%26+1+%26+%5Cdots+%5C%5C+0+%26+0+%26+0+%26+0+%26%5C%5C+0+%26+0+%26+3+%26+2+%26+%5Ccdots+%5Cend%7Bbmatrix%7D)

其第一列表示的含义第二种要求切割两次，其余的不切取0。**那每一列就代表一个可行的切割方式，把所有的的列找出来，那问题就解决了。但是我们面临的问题是当我的要求是几百个客户时，它可行的列的个数是成千上万个，穷举找出所有的列是不可行的，**这就是我们面临的组合爆炸问题。但还是要给出这个问题的模型



- **模型2 ：列生成初探**

![\begin{align} &\min_{y_j}\, && \sum_{j=1}^{n} y_j\\ &\text{s.t.} \, && \sum_{j=1}^{n} m_{ij} y_j \ge n_i && i=1,2,\dots,m\\ &&& y_j \in \mathbb{Z} && j = 1,\dots, n \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%26%5Cmin_%7By_j%7D%5C%2C+%26%26+%5Csum_%7Bj%3D1%7D%5E%7Bn%7D+y_j%5C%5C+%26%5Ctext%7Bs.t.%7D+%5C%2C+%26%26+%5Csum_%7Bj%3D1%7D%5E%7Bn%7D+m_%7Bij%7D+y_j+%5Cge+n_i+%26%26+i%3D1%2C2%2C%5Cdots%2Cm%5C%5C+%26%26%26+y_j+%5Cin+%5Cmathbb%7BZ%7D+%26%26+j+%3D+1%2C%5Cdots%2C+n+%5Cend%7Balign%7D)

当**![y_{j}](https://www.zhihu.com/equation?tex=y_%7Bj%7D)** **取正整数的时候，又是一个整数规划**，这种表现形式就是我们熟知的set covering形式（参见孙小玲《整数规划》）。这个模型比上一个更为简单，但是面临的 问题是当规模较大的时候无法找到所有的列。

当我们把 ![y_{i}](https://www.zhihu.com/equation?tex=y_%7Bi%7D) 松弛以后，即让其取值为正数。模型就变成一个**线性规划，**对于线性规划，我们想到的就是**单纯形法，**从一个顶点解开始，求其检验数，迭代....思路是对的。但是单纯形法要求知道所有列，这又矛盾！所以我们要找到一种方法使得，**不需要列出所有列的迭代方式就能求解。**

**在单纯形法和其对偶形式中：**单纯形法及其对偶中，其非基变量检验数最终的表达形式 ![\sigma=c_{N}^{T}-c_{B}^{T}B^{-1}N](https://www.zhihu.com/equation?tex=%5Csigma%3Dc_%7BN%7D%5E%7BT%7D-c_%7BB%7D%5E%7BT%7DB%5E%7B-1%7DN) ,如果 ![\sigma\geq0](https://www.zhihu.com/equation?tex=%5Csigma%5Cgeq0) ，此时便找到最优解，否则我们选择进基列和出基列进行换基，再次迭代（检验数有两种的表现形式，其中差一个负号，判断方式也就相反）。

所以列生成的思想：单纯形法要找到所有的N！我们想能不能不用找出所有列**，我们先找到其中的一小部分可行解，计算其检验数，如果小于零，再添加一列，再求解新问题及其检验数，如果还小于零，再添加一列，往复循环，直至求到最优。我们把这种方法成为列生成方法。**当只有部分可行解构成的模型时，如下所示，我们把此模型称之为*限制主问题（RMP），*其此时检验数的形式 ![\sigma=c_{j}^{T}-c_{B}^{T}B^{-1}m_{j}](https://www.zhihu.com/equation?tex=%5Csigma%3Dc_%7Bj%7D%5E%7BT%7D-c_%7BB%7D%5E%7BT%7DB%5E%7B-1%7Dm_%7Bj%7D) ,令 ![c_{B}^{T}B^{-1}](https://www.zhihu.com/equation?tex=c_%7BB%7D%5E%7BT%7DB%5E%7B-1%7D) = ![\pi^{T}](https://www.zhihu.com/equation?tex=%5Cpi%5E%7BT%7D) ,检验数的形式变为 ![\sigma=c_{j}^{T}-\pi^{T}m_{j}](https://www.zhihu.com/equation?tex=%5Csigma%3Dc_%7Bj%7D%5E%7BT%7D-%5Cpi%5E%7BT%7Dm_%7Bj%7D) ，在此问题中 ![c_{j}](https://www.zhihu.com/equation?tex=c_%7Bj%7D) 为1， ![\pi](https://www.zhihu.com/equation?tex=%5Cpi) 可以通过gurobi 等求解器求出。那么剩下的就是解决怎么找到可行的一列m，我们把寻找找求解可行的一列m称为求解*子问题（sub-problem）。*



![\begin{alignat}{4} &\min_{y_j}\, && \sum_{j=1}^{n} y_j &&&&\text{} \\ &\text{s.t.} \, && \sum_{j=1}^{n} m_{ij} y_j \ge n_i && i=1,2,\dots,m \qquad&& (\pi^i)\\ &&& y_j \ge 0 && j = 1,\dots, n \end{alignat}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balignat%7D%7B4%7D+%26%5Cmin_%7By_j%7D%5C%2C+%26%26+%5Csum_%7Bj%3D1%7D%5E%7Bn%7D+y_j+%26%26%26%26%5Ctext%7B%7D+%5C%5C+%26%5Ctext%7Bs.t.%7D+%5C%2C+%26%26+%5Csum_%7Bj%3D1%7D%5E%7Bn%7D+m_%7Bij%7D+y_j+%5Cge+n_i+%26%26+i%3D1%2C2%2C%5Cdots%2Cm+%5Cqquad%26%26+%28%5Cpi%5Ei%29%5C%5C+%26%26%26+y_j+%5Cge+0+%26%26+j+%3D+1%2C%5Cdots%2C+n+%5Cend%7Balignat%7D)

其中m<n，以上的形式就是限制主问题，当我们求解限制主问题时，我们会得到乘子 ![\pi](https://www.zhihu.com/equation?tex=%5Cpi) ，子问题是什么呢？既然当我们的检验数是负的才可以添加一列，等价于我要找一个最大的 ![m_{j}](https://www.zhihu.com/equation?tex=m_%7Bj%7D) ( ![m_{j}](https://www.zhihu.com/equation?tex=m_%7Bj%7D) 代表每一根钢管上的切割模式及其切割次数),这一列要满足的情况就是我切割要求的长度 ![l_{i}](https://www.zhihu.com/equation?tex=l_%7Bi%7D) 时，不能大于我的钢管固定长度 ![L](https://www.zhihu.com/equation?tex=L) ,写出子问题

![\begin{align} &\max_{m_j} \, && \sum_{j=1}^{m} \pi^{j} m_j \\ &\text{s.t.} \, && \sum_{i=1}^{m} l_i m_j \le L \\ &&& m_j \ge 0 && i = 1, \dots, m \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%26%5Cmax_%7Bm_j%7D+%5C%2C+%26%26+%5Csum_%7Bj%3D1%7D%5E%7Bm%7D+%5Cpi%5E%7Bj%7D+m_j+%5C%5C+%26%5Ctext%7Bs.t.%7D+%5C%2C+%26%26+%5Csum_%7Bi%3D1%7D%5E%7Bm%7D+l_i+m_j+%5Cle+L+%5C%5C+%26%26%26+m_j+%5Cge+0+%26%26+i+%3D+1%2C+%5Cdots%2C+m+%5Cend%7Balign%7D)

**求出新的一列然后添加到限制主规划中，重新求解，再更新子问题，求解新的一列。**

**我们仍然通过一个小栗子来看一下列生成算法：**

假设钢管的长度是120米,有三个顾客来下单lengths=[56,78,30];demands=[50,65,80],首先，我们要建模并且给出一个可行解，最笨的可行解莫过于每一种模式只取一种 ![l_{i}](https://www.zhihu.com/equation?tex=l_%7Bi%7D) 那在RLMP模型下， ![m_{11}](https://www.zhihu.com/equation?tex=m_%7B11%7D) ，![m_{22}](https://www.zhihu.com/equation?tex=m_%7B22%7D) ，![m_{33}](https://www.zhihu.com/equation?tex=m_%7B33%7D) =1，其余取值为0建立模型如下：

![\begin{align} &\min && y_1 + y_2 +y_3\\ &\text{s.t.} && y_1 \ge 50\\ &&& y_2 \ge 65 \\ &&& y_3 \ge 80 \\ &&& y_1,y_2,y_3 \ge 0 \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%26%5Cmin+%26%26+y_1+%2B+y_2+%2By_3%5C%5C+%26%5Ctext%7Bs.t.%7D+%26%26+y_1+%5Cge+50%5C%5C+%26%26%26+y_2+%5Cge+65+%5C%5C+%26%26%26+y_3+%5Cge+80+%5C%5C+%26%26%26+y_1%2Cy_2%2Cy_3+%5Cge+0+%5Cend%7Balign%7D)

还是使用Python建模，gurobi求解，并且gurobi 可以通过getAttr(GRB.Attr.Pi)来获得乘子 ![\pi](https://www.zhihu.com/equation?tex=%5Cpi)

```text
================= RESTART: C:\gurobi752\win64\python27\m.py =================
Optimize a model with 3 rows, 3 columns and 3 nonzeros
Iteration    Objective       Primal Inf.    Dual Inf.      Time
       0    1.9500000e+02   0.000000e+00   0.000000e+00      0s

Solved in 0 iterations and 0.06 seconds
Optimal objective  1.950000000e+02
y1 = 50
y2 = 65
y3 = 80
Obj: 195
pi: [1.0, 1.0, 1.0]
>>> 
```

接下来我们要求解子问题，看能否找到一列使得检验数为负，从而添加一列，继续优化

![\begin{align} &\max && 1.0 m_1 + 1.0m_2 +1.0m_3 \\ &\text{s.t.} && 56m_1 +78m_2 + 30m_3 \le 120\\ &&& m_1, m_2, m_3 \ge 0 \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%26%5Cmax+%26%26+1.0+m_1+%2B+1.0m_2+%2B1.0m_3+%5C%5C+%26%5Ctext%7Bs.t.%7D+%26%26+56m_1+%2B78m_2+%2B+30m_3+%5Cle+120%5C%5C+%26%26%26+m_1%2C+m_2%2C+m_3+%5Cge+0+%5Cend%7Balign%7D)

结果为：

```text
Optimize a model with 1 rows, 3 columns and 3 nonzeros
Iteration    Objective       Primal Inf.    Dual Inf.      Time
       0    4.0000000e+00   0.000000e+00   0.000000e+00      1s

Solved in 0 iterations and 1.15 seconds
Optimal objective  4.000000000e+00
m1 = 0
m2 = 0
m3 = 4
Obj: 4
```

我们找到一列新的 ![[0,0,4]^{T}](https://www.zhihu.com/equation?tex=%5B0%2C0%2C4%5D%5E%7BT%7D) ,然后计算检验数 ![\sigma](https://www.zhihu.com/equation?tex=%5Csigma) =1-4=-3，可以将这一列选中，添加

RMP变为：

![\begin{align} &\min && y_1 + y_2 +y_3 + y_4\\ &\text{s.t.} && y_1 \ge 50\\ &&& y_2 \ge 65 \\ &&& y_3 + 4y_4\ge 80 \\ &&& y_1,y_2,y_3, y_4 \ge 0 \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%26%5Cmin+%26%26+y_1+%2B+y_2+%2By_3+%2B+y_4%5C%5C+%26%5Ctext%7Bs.t.%7D+%26%26+y_1+%5Cge+50%5C%5C+%26%26%26+y_2+%5Cge+65+%5C%5C+%26%26%26+y_3+%2B+4y_4%5Cge+80+%5C%5C+%26%26%26+y_1%2Cy_2%2Cy_3%2C+y_4+%5Cge+0+%5Cend%7Balign%7D)

结果为：

```text
Optimize a model with 3 rows, 4 columns and 4 nonzeros

Iteration    Objective       Primal Inf.    Dual Inf.      Time
       0    1.3500000e+02   0.000000e+00   0.000000e+00      0s

Solved in 0 iterations and 0.03 seconds
Optimal objective  1.350000000e+02
y1 = 50
y2 = 65
y3 = 0
y4 = 20
Obj: 135
pi: [1.0, 1.0, 0.25]
```



再求解子问题：

![\begin{align} &\max && 1.0 m_1 + 1.0m_2 + 0.25m_3 \\ &\text{s.t.} && 56m_1 +78m_2 + 30m_3 \le 120\\ &&& m_1, m_2, m_3 \ge 0 \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%26%5Cmax+%26%26+1.0+m_1+%2B+1.0m_2+%2B+0.25m_3+%5C%5C+%26%5Ctext%7Bs.t.%7D+%26%26+56m_1+%2B78m_2+%2B+30m_3+%5Cle+120%5C%5C+%26%26%26+m_1%2C+m_2%2C+m_3+%5Cge+0+%5Cend%7Balign%7D)

结果为：

```text
Optimize a model with 1 rows, 3 columns and 3 nonzeros
Iteration    Objective       Primal Inf.    Dual Inf.      Time
       0    2.1428571e+00   0.000000e+00   0.000000e+00      0s

Solved in 0 iterations and 0.07 seconds
Optimal objective  2.142857143e+00
m1 = 2.14286
m2 = 0
m3 = 0
Obj: 2.14286
```

显然，检验数仍为负数，可以选择一列再迭代

Continue。。。

注意到我们的子问题没有让变量取整数，是因为在列生成算法中，通常需要自己设计分枝策略，所以未规定其变量为整数。

第二种模型由Gilmore 和 Gomory 分别在文章[2]、[3]、[4]中提出并使用。

现简单给出列生成算法步骤：

**step1：**给出模型的初始解，其解代表可行列

**step2:** 求解主规划（RMP）并获取乘子 ![\pi](https://www.zhihu.com/equation?tex=%5Cpi)

**step3:** 求解子问题 ，并计算 ![\sigma](https://www.zhihu.com/equation?tex=%5Csigma) ，如果小于0，向限制主问题中添加一列求解，返回step2

***step4:*** 若 ![\sigma](https://www.zhihu.com/equation?tex=%5Csigma) 大于等于零，停止算法，输出最优解



## **3. 结论**

但是当我们去翻看SCI中关于这方面的论文，会发现**列生成方法通常结合动态规划，分支定界算法结合使用，称之为分支定价算法（branch and price）。**这里我们通过一个下料问题小栗子，来从另一个角度阐述了一下列生成算法的思想，绕过了DW分解的理论，但是如果想要深入研究，请从线性规划，凸优化，闵可夫斯基定理等开始看起，本文的指导思想是孙小玲教授《整数规划》，章学仁《线性规划》，钱颂迪《运筹学》参考写成，参考论文：

[1] Haessler R W. Sweeney P E. Cutting Stock Problem and Solution Procedures [J]. European Journal of Operation of Research, 1991, 54: 141-150

[2] Gilmore P C, Gomory R E. A linear programming approach to the cutting stock problem [J] Operations Research,1961, 9: 849-859.

[3] Gilmore P C, Gomory R E. A linear programming approach to the cutting stock problem [J]. Operations Research, 1963, 1:863-888.

[4] Gilmore P C, Gomory R E. Multistage cutting stock problems of two and more dimensions [J].Operations Research, 1965, 13: 94-120.



由于匆忙，难免有不足之处，恳请赐教。

对于车间调度的问题求解最优解的问题，大家使用最多的是智能算法比如遗传，模拟退火，果蝇，蝙蝠，花粉，粒子群，EDA算法等等，但是通过列生成得到的优良的下界可以对智能算法的上界进行一个良好的估计，这也是价值所在。

关于这部分内容，可以参考：

[【学界】整数规划精确算法/近似算法/(元)启发算法/神经网络反向传播等算法的区别与关联](https://zhuanlan.zhihu.com/p/30140008)

------

审稿人 

[@PhilFWu](https://www.zhihu.com/people/7d42b5485ad6dffc8a2d97fca443638f)

 

系美国俄亥俄州立大学运筹学博士现为LLamasoft的应用科学家，专注于研究并编写路径优化问题和供应链网络优化问题的求解器；博士期间专注于随机优化，整数规划，最优控制理论，以及电力市场及电网运行策略；曾在洛斯·阿拉莫斯国家试验室研究天然气管网的控制与优化，天然气市场的重构。





[@吐服](https://www.zhihu.com/people/6fbe6f0115acc6bd425e3cc61d5e7462)

 

大连海事大学物流实验室：杨敏，大连海事物流工程与管理研究生在读，研究方向为港口仿真，航线优化等。





[@浊清风](https://www.zhihu.com/people/a845c8dbbf4ec954bea826d8c98a4d57)

 

孙卓，大连海事大学物流系教授，博士生导师，从事交通物流网络优化、计算机仿真等研究。曾获由阿里巴巴、香港科技大学、美国INFORMS协会举办的菜鸟网络全球算法大赛季军。开源空间建模框架

MicroCity

。