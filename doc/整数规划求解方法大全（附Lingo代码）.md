# 整数规划求解方法大全（附Lingo代码）



[留德华叫兽](https://www.zhihu.com/people/ruobingshen)



全部变量限制为整数的规划问题，称为纯整数规划；部分变量限制为整数的规划问题，称为混合整数规划；变量只取0或1的规划问题，称为0-1整数规划。

整数规划问题，建议使用Lingo软件求解。常用的整数规划问题解法有：

（1）分枝定界法：可求纯或混合整数线性规划。

（2）割平面法：可求纯或混合整数线性规划。

（3）隐枚举法：用于求解0-1整数规划，有过滤法和分枝法。

（4）匈牙利法：解决指派问题（0-1规划特殊情形）。

（5）蒙特卡罗法：求解各种类型规划。



## **1、分枝定界法**

分支定界法的基本思想是：设有最大化的整数规划问题A，先解与之相应的线性规划问题B，若B的最优解不符合A的整数条件，那么B的最优目标函数必是A的最优目标函数z*的上界，记作z2, 而A的任意可行解的目标函数值将是z*的一个下界z1, 分支定界法就是将B的可行域分成子区域（称为分支）的方法，逐步减小z2和增大z1，最终求到z*。

**例1** 分枝定界法原理示例：

![\max z=5x_1+8x_2 \\](https://www.zhihu.com/equation?tex=%5Cmax+z%3D5x_1%2B8x_2+%5C%5C)

![\mathrm{s.~ t.} \quad x_1+x_2 \leq 6](https://www.zhihu.com/equation?tex=%5Cmathrm%7Bs.%7E+t.%7D+%5Cquad+x_1%2Bx_2+%5Cleq+6)![(P_0)](https://www.zhihu.com/equation?tex=%28P_0%29)

![~~~~~~~~~ 5x_1+9x_2 \leq 45](https://www.zhihu.com/equation?tex=%7E%7E%7E%7E%7E%7E%7E%7E%7E+5x_1%2B9x_2+%5Cleq+45)

![x_i \geq 0, \, x_i \in \mathbb{Z} \quad i=1,\,2](https://www.zhihu.com/equation?tex=x_i+%5Cgeq+0%2C+%5C%2C+x_i+%5Cin+%5Cmathbb%7BZ%7D+%5Cquad+i%3D1%2C%5C%2C2)



![img](https://pic3.zhimg.com/80/v2-3a347e761776e076c13ff90567c052c6_hd.jpg)



![img](https://pic3.zhimg.com/80/v2-9dd2dfa8d61cbc1af38e33f450a18e22_hd.jpg)



![img](https://pic1.zhimg.com/80/v2-74b50a27fa1a178f7e5befe9d9f987f0_hd.jpg)

**Lingo代码：**

```text
model:
max=5*x1+8*x2;
x1+x2<6;
5*x1+9*x2<45;
@gin(x1);
@gin(x2);
end
```

**运行结果：**
Global optimal solution found.

Objective value: 40.00000

Variable Value

X1 0.000000

X2 5.000000



## **2、0-1整数规划**

变量 ![x_i](https://www.zhihu.com/equation?tex=x_i) 只能取值 ![0,\,1](https://www.zhihu.com/equation?tex=0%2C%5C%2C1) , 该约束条件可表示为：

![0\leq x \leq 1, \quad x_i \in \mathbb{N} \qquad \text{或} \qquad x_i(x_i-1)=0 ](https://www.zhihu.com/equation?tex=0%5Cleq+x+%5Cleq+1%2C+%5Cquad+x_i+%5Cin+%5Cmathbb%7BN%7D+%5Cqquad+%5Ctext%7B%E6%88%96%7D+%5Cqquad+x_i%28x_i-1%29%3D0+)

**1. 隐枚举法**

**例2** 求解下列0-1规划问题：

![\max z=3x_1-2x_2+5x_3](https://www.zhihu.com/equation?tex=%5Cmax+z%3D3x_1-2x_2%2B5x_3)

![\mathrm{s.~t.} \quad x_1+2x_2-x_3 \leq 2 \qquad \qquad (a)](https://www.zhihu.com/equation?tex=%5Cmathrm%7Bs.%7Et.%7D+%5Cquad+x_1%2B2x_2-x_3+%5Cleq+2+%5Cqquad+%5Cqquad+%28a%29)

![x_1+4x_2+x_3 \leq 4 \qquad \qquad (b)](https://www.zhihu.com/equation?tex=x_1%2B4x_2%2Bx_3+%5Cleq+4+%5Cqquad+%5Cqquad+%28b%29)

![x_1+x_2 \leq 3 \qquad \qquad \qquad ~~ (c)](https://www.zhihu.com/equation?tex=x_1%2Bx_2+%5Cleq+3+%5Cqquad+%5Cqquad+%5Cqquad+%7E%7E+%28c%29)

![4x_2+x_3 \leq 6 \qquad \qquad ~~~~~~ (d)](https://www.zhihu.com/equation?tex=4x_2%2Bx_3+%5Cleq+6+%5Cqquad+%5Cqquad+%7E%7E%7E%7E%7E%7E+%28d%29)

**求解思路：**

(1) 先试探性地求一个可行解，易看出 ![(x_1,\,x_2,\,x_3)=(1,\,0,\,0)](https://www.zhihu.com/equation?tex=%28x_1%2C%5C%2Cx_2%2C%5C%2Cx_3%29%3D%281%2C%5C%2C0%2C%5C%2C0%29) 满足约束条件，故是一个可行解，相应的目标函数值为 ![z=3](https://www.zhihu.com/equation?tex=z%3D3) .

(2) 由于是求极大值，故目标值 ![z<3](https://www.zhihu.com/equation?tex=z%3C3) 的解，不必检验是否满足约束条件即可删除，于是可增加一个约束条件（称为过滤条件）：

![3x_1-2x_2+5x_3 \geq 3 \qquad \qquad ~(e)](https://www.zhihu.com/equation?tex=3x_1-2x_2%2B5x_3+%5Cgeq+3+%5Cqquad+%5Cqquad+%7E%28e%29)

(3)用全部枚举法，3个变量共 ![2^3=8](https://www.zhihu.com/equation?tex=2%5E3%3D8) 种可能的组合，用过滤条件（并计算目标函数值，不断改进过滤条件）筛选每个可能的组合，最终得到问题的最优解。

隐枚举法求解过程如下表所示：

![img](https://pic3.zhimg.com/80/v2-03eb6341ddf0be4972d49e66827893a6_hd.jpg)

从而得到最优解为 ![(1,\,0,\,1)](https://www.zhihu.com/equation?tex=%281%2C%5C%2C0%2C%5C%2C1%29) , 最优值为 ![z^*=8](https://www.zhihu.com/equation?tex=z%5E%2A%3D8) 。

**Lingo代码：**

```text
model:
max=3*x1-2*x2+5*x3;
x1+2*x2-x3<2;
x1+4*x2+x3<4;
x1+x2<3;
4*x2+x3<6;
@bin(x1);
@bin(x2);
@bin(x3);
end
```

**运行结果：**Global optimal solution found.

Objective value: 8.000000

Variable Value

X1 1.000000

X2 0.000000

X3 1.000000

**2. 指派问题**

指派问题可以描述为整数规划问题，暂略（放图论部分）

## **3、蒙特卡罗法（随机取样法）**

前面的方法，主要是针对线性整数规划而言，对于非线性整数规划没有通用的有效解法。

整数规划由于限制变量是整数，增加了求解难度，但整数解是有限个，所以可以采用枚举法。当枚举个数很多时，显性枚举是不现实的，但利用蒙特卡罗随机取样法，在一定的计算量下是可以得到满意解的。

**例3** 求解如下非线性整数规划问题：

![\max z=x_1^2 + x_2^2 +3x_3^2 +4x_4^2+2x_5^2 - 8x_1 - 2x_2 - 3x_3 -x_4 - 2x_5](https://www.zhihu.com/equation?tex=%5Cmax+z%3Dx_1%5E2+%2B+x_2%5E2+%2B3x_3%5E2+%2B4x_4%5E2%2B2x_5%5E2+-+8x_1+-+2x_2+-+3x_3+-x_4+-+2x_5)

![\mathrm{s. ~t.} \quad x_1+x+2+x_3+x_4+x_5 \leq 400](https://www.zhihu.com/equation?tex=%5Cmathrm%7Bs.+%7Et.%7D+%5Cquad+x_1%2Bx%2B2%2Bx_3%2Bx_4%2Bx_5+%5Cleq+400)

![x_1+2x_2+2x_3+x_4+6x_5 \leq 800](https://www.zhihu.com/equation?tex=x_1%2B2x_2%2B2x_3%2Bx_4%2B6x_5+%5Cleq+800)

![2x_1+x_2+6x_3 \leq 200](https://www.zhihu.com/equation?tex=2x_1%2Bx_2%2B6x_3+%5Cleq+200)

![x_3+x_4+5x_5 \leq 200](https://www.zhihu.com/equation?tex=x_3%2Bx_4%2B5x_5+%5Cleq+200)

![0 \leq x_i \leq 99, \quad i=1,\, \cdots, \, 5](https://www.zhihu.com/equation?tex=0+%5Cleq+x_i+%5Cleq+99%2C+%5Cquad+i%3D1%2C%5C%2C+%5Ccdots%2C+%5C%2C+5)

若用显枚举法，共需 ![100^5=10^{10}](https://www.zhihu.com/equation?tex=100%5E5%3D10%5E%7B10%7D) 个点，计算量非常大。但用蒙特卡罗法随机计算 ![10^6](https://www.zhihu.com/equation?tex=10%5E6) 个点，便可找到满意解。

**Matlab代码**：

目标函数：

```text
function [f,g]=fun1(x)
f=x(1)^2+x(2)^2+3*x(3)^2+4*x(4)^2+2*x(5)^2-8*x(1)-2*x(2)-3*x(3)-x(4)-2*x(5);
g(1)=sum(x)-400;
g(2)=x(1)+2*x(2)+2*x(3)+x(4)+6*x(5)-800;
g(3)=2*x(1)+x(2)+6*x(3)-200;
g(4)=x(3)+x(4)+5*x(5)-200;
```

**主程序：**

```text
rand('state',sum(clock));
p0=0;
tic
for i=1:10^6
    x=99*rand(5,1);
    x1=floor(x);
    x2=ceil(x);
    [f,g]=fun5(x1);
    if sum(g<=0)==4
        if p0<=f
            x0=x1;
            p0=f;
        end
    end
    [f,g]=fun5(x2);
    if sum(g<=0)==4
        if p0<=f
            x0=x2;
            p0=f;
        end
    end
end
x0, p0
toc
```

**运行结果（注意由于是随机取样，故每次的运行结果可能不一样）：**

x0 = 43 94 1 99 5

p0 = 49298

Elapsed time is 45.494952 seconds.



**Lingo代码：**

```text
model:
sets:
row/1..4/:b;
col/1..5/:c1,c2,x;
link(row,col):a;
endsets
data:
c1=1,1,3,4,2;
c2=-8,-2,-3,-1,-2;
a=1 1 1 1 1
1 2 2 1 6
2 1 6 0 0
0 0 1 1 5;
b=400,800,200,200;
enddata
max=@sum(col:c1*x^2+c2*x);
@for(row(i):@sum(col(j):a(i,j)*x(j))<b(i));
@for(col:@gin(x));
@for(col:@bnd(0,x,99));
end
```

**运行结果：**Local optimal solution found.

Objective value: 51568.00

Variable Value

X( 1) 50.00000

X( 2) 99.00000

X( 3) 0.000000

X( 4) 99.00000

X( 5) 20.00000



## **4、混合整数规划（配送选址问题）**



![img](https://pic3.zhimg.com/80/v2-adc92811627c1ab49b101373764277f6_hd.jpg)

物流配送中心选址问题，是在给定某一地区所有备选点的地址集合中选出一定数目的地址建立配送中心，从而建立一系列的配送区域，以实现选出点建立的配送中心与各需求点和工厂(供货点)形成的配送系统总物流费用最小。例如，如图1所示。

**总费用=运输费用+配送费用+仓储费用+固定成本**

设有 ![m](https://www.zhihu.com/equation?tex=m) 个供应工厂， ![n](https://www.zhihu.com/equation?tex=n) 个客户， ![l](https://www.zhihu.com/equation?tex=l) 个备选配送中心；

![p_k](https://www.zhihu.com/equation?tex=p_k) 表示第 ![k](https://www.zhihu.com/equation?tex=k) 个工厂的产量 (![k=1,\,\cdots,\,m](https://www.zhihu.com/equation?tex=k%3D1%2C%5C%2C%5Ccdots%2C%5C%2Cm));

![d_j](https://www.zhihu.com/equation?tex=d_j) 表示第 ![j](https://www.zhihu.com/equation?tex=j) 个客户的需求量 (![j=1,\,\cdots,\,n](https://www.zhihu.com/equation?tex=j%3D1%2C%5C%2C%5Ccdots%2C%5C%2Cn) );

![g_i](https://www.zhihu.com/equation?tex=g_i) 表示第 ![i](https://www.zhihu.com/equation?tex=i) 个配送中心的单位管理成本 ( ![i=1,\,\cdots,\,l](https://www.zhihu.com/equation?tex=i%3D1%2C%5C%2C%5Ccdots%2C%5C%2Cl) );

![f_i](https://www.zhihu.com/equation?tex=f_i) 表示建立第 ![i](https://www.zhihu.com/equation?tex=i) 个配送中心的固定成本；

![z_i](https://www.zhihu.com/equation?tex=z_i) 指示是否选中第 ![i](https://www.zhihu.com/equation?tex=i) 个配送中心， ![1](https://www.zhihu.com/equation?tex=1) 表示选中， ![0](https://www.zhihu.com/equation?tex=0) 表示未选中；

![z_{max}](https://www.zhihu.com/equation?tex=z_%7Bmax%7D) 表示配送中心的最大限制数目；

![c_{ki}](https://www.zhihu.com/equation?tex=c_%7Bki%7D) 表示第 ![k](https://www.zhihu.com/equation?tex=k) 个供应工厂到第 ![i](https://www.zhihu.com/equation?tex=i) 个配送中心的运输单价；

![h_{ij}](https://www.zhihu.com/equation?tex=h_%7Bij%7D) 表示第 ![i](https://www.zhihu.com/equation?tex=i) 个配送中心到第 ![j](https://www.zhihu.com/equation?tex=j) 个客户的配送单价；

![w_{ki}](https://www.zhihu.com/equation?tex=w_%7Bki%7D) 表示第 ![k](https://www.zhihu.com/equation?tex=k) 个供应工厂到第 ![i](https://www.zhihu.com/equation?tex=i) 个配送中心的运量；

![x_{ij}](https://www.zhihu.com/equation?tex=x_%7Bij%7D) 表示第 ![i](https://www.zhihu.com/equation?tex=i) 个配送中心到第 ![j](https://www.zhihu.com/equation?tex=j) 个客户的运量。

则配送选址问题数学模型的一般形式如下：

![\min \quad \sum_{k=1}^m \sum_{i=1}^l c_{ki}w_{ki} + \sum_{i=1}^l \sum_{j=1}^n h_{ij}x_{ij} + \sum_{i=1}^l \sum_{k=1}^m g_i w_{ki} + \sum_{i=1}^l f_i z_i](https://www.zhihu.com/equation?tex=%5Cmin+%5Cquad+%5Csum_%7Bk%3D1%7D%5Em+%5Csum_%7Bi%3D1%7D%5El+c_%7Bki%7Dw_%7Bki%7D+%2B+%5Csum_%7Bi%3D1%7D%5El+%5Csum_%7Bj%3D1%7D%5En+h_%7Bij%7Dx_%7Bij%7D+%2B+%5Csum_%7Bi%3D1%7D%5El+%5Csum_%7Bk%3D1%7D%5Em+g_i+w_%7Bki%7D+%2B+%5Csum_%7Bi%3D1%7D%5El+f_i+z_i)

![\mathrm{s.~t.} \quad \sum_{i=1}^l w_{ki} \leq p_k, \quad k=1,\, \cdots, \, m](https://www.zhihu.com/equation?tex=%5Cmathrm%7Bs.%7Et.%7D+%5Cquad+%5Csum_%7Bi%3D1%7D%5El+w_%7Bki%7D+%5Cleq+p_k%2C+%5Cquad+k%3D1%2C%5C%2C+%5Ccdots%2C+%5C%2C+m) (运出量不大于生产量)

![\sum_{j=1}^n x_{ij} = \sum_{k=1}^m w_{ki}, \quad i=1, \, \cdots, \, l](https://www.zhihu.com/equation?tex=%5Csum_%7Bj%3D1%7D%5En+x_%7Bij%7D+%3D+%5Csum_%7Bk%3D1%7D%5Em+w_%7Bki%7D%2C+%5Cquad+i%3D1%2C+%5C%2C+%5Ccdots%2C+%5C%2C+l) (运入量不等于运出量)
![\sum_{i=1}^l x_{ij} \geq d_j, \quad \, j=1, \, \cdots, \, n](https://www.zhihu.com/equation?tex=%5Csum_%7Bi%3D1%7D%5El+x_%7Bij%7D+%5Cgeq+d_j%2C+%5Cquad+%5C%2C+j%3D1%2C+%5C%2C+%5Ccdots%2C+%5C%2C+n) (运入量不小于需求量)

![\sum_{k=1}^m w_{ki} \leq a_i z_i, \, \quad i = 1, \, \cdots, \, l](https://www.zhihu.com/equation?tex=%5Csum_%7Bk%3D1%7D%5Em+w_%7Bki%7D+%5Cleq+a_i+z_i%2C+%5C%2C+%5Cquad+i+%3D+1%2C+%5C%2C+%5Ccdots%2C+%5C%2C+l) (运入量不大于仓库容量)

![\sum_{i=1}^l z_i \leq z_{max}](https://www.zhihu.com/equation?tex=%5Csum_%7Bi%3D1%7D%5El+z_i+%5Cleq+z_%7Bmax%7D) (选取配送中心不超过最大限制数目)

**例4** 设有4个备选物流配送中心地址，6个工厂为其供货，6个客户需要产品，最多设置3个物流配送中心。工厂到物流配送中心的运输价格见表2，物流配送中心到客户的运输价格见表3，工厂的总生产能力见表4，物流配送中心的固定成本、单位管理成本，及容量见表5，客户的需求量见表6：

![img](https://pic3.zhimg.com/80/v2-77a6bea58cb8143619439b0c8956e972_hd.jpg)



![img](https://pic4.zhimg.com/80/v2-7b9d36b55330d61755fa44f24277648f_hd.jpg)

![img](https://pic1.zhimg.com/80/v2-026497d06fc9f5902ab0cff11a0feb24_hd.jpg)



![img](https://pic2.zhimg.com/80/v2-b44156f089362b83bd73c780108fbfa9_hd.jpg)



![img](https://pic4.zhimg.com/80/v2-17765432b284b9884667a9805e971757_hd.jpg)

**Lingo代码：**

```text
model:
sets:
factory/p1..p6/:p;
warhouse/w1..w4/:a,f,g;
customer/c1..c6/:d;
tr/tr1..tr4/:z;
link1(factory,warhouse):c,w;
link2(warhouse,customer):h,x;
endsets
data:
p=40000,50000,60000,70000,60000,40000;
a=70000,60000,70000,50000;
f=500000,300000,400000,400000;
g=3,2,5,4;
d=10000,20000,10000,20000,30000,10000;
c = 6 5
4 2
2 3 4 9
6 8 7 5
7 4 2 3
4 2 5 1
3 4 1 7;
H = 3 2
7 4 7 5
6 1 4 2 5 3
2 4 5 3 6 8
5 6 3 7 4 6;
enddata
min=@sum(link1(k,i):c(k,i)*w(k,i))+@sum(link2(i,j):h(i,j)*x(i,j))
+@sum(link1(k,i):g(i)*w(k,i))+@sum(warhouse(i):f(i)*z(i));
@for(factory(k):@sum(link1(k,i):w(k,i))<=p(k));
@for(warhouse(i):@sum(link2(i,j):x(i,j))=@sum(link1(k,i):w(k,i)));
@for(customer(j):@sum(link2(i,j):x(i,j))>=d(j));
@for(warhouse(i):@sum(link1(k,i):w(k,i))<=(a(i)*z(i)));
@sum(tr(i):z(i))<=3;
@for(tr(i):@bin(z));
end
```



**运行结果：**Global optimal solution found.

Objective value: 1480000

Variable Value

Z(TR2) 1.000000

Z(TR4) 1.000000

W( P1,W4) 40000.00

W( P5,W2) 60000.00

X( W2,C2) 20000.00

X( W2,C3) 10000.00

X( W2,C4) 20000.00

X( W2,C6) 10000.00

X( W4,C1) 10000.00

X( W4,C5) 30000.00

结果表明，最小物流成本为1480000；最优方案是选择2号和4号备选地址作为物流配送中心；由工厂1向4号配送中心供应40000，由工厂5向2号配送中心供应60000；配送中心2分别向客户2、3、4、6供货20000、10000、20000、10000，配送中心4分别向客户1、5供货10000、30000。



## **主要参考文献：**

1. 吴刚, 张敬信 等. 数学建模与数学实验，北京: 中国商业出版社，2017
2. 卓金武 等. Matlab在数学建模中的应用，北京：北京航空航天大学出版社，2011
3. 谢金星，薛毅. 优化建模与LINDO/LINGO软件，北京：清华大学出版社，2006
4. 司守奎，孙玺菁. 数学建模算法与应用，北京：国防工业出版社，2013