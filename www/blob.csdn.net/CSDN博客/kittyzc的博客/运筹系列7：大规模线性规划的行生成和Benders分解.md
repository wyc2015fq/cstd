# 运筹系列7：大规模线性规划的行生成和Benders分解 - kittyzc的博客 - CSDN博客
2019年04月04日 23:17:25[IE06](https://me.csdn.net/kittyzc)阅读数：2686所属专栏：[运筹学算法python实现](https://blog.csdn.net/column/details/26511.html)
# 1. 行生成算法

行生成就是指的不断添加约束的算法。

因为在求解矩阵中，一个约束条件对应一行，因此添加约束条件的方法自然叫做行生成算法（Benders分解）。相对应的，添加变量的方法就叫做列生成算法（DW分解）。

这一节先看行生成的Benders分解算法，常常用于有一部分约束条件有明显的”对角线分块“结构，可以拆下来求解的情形：
![](https://upload-images.jianshu.io/upload_images/13198105-bf70132b140a21fa.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
# 2 Benders分解

Benders分解（Benders Decomposition，BD）的基本思路是：使用子问题（primal problem）来寻找合适的约束不断添加到松弛主问题（relaxed master problem）中。对于最小化问题，子问题可以给下界（LB），松弛主问题可以给上界（UB），不断迭代就可以逐步找到最优解。具体可以参考[论文](http://www.ie.boun.edu.tr/~taskin/pdf/taskin_benders.pdf)，这里做一下简单的概述：

问题模型是：

$min⁡cx\min  cxmincx$

s.t.  $Ax=bAx = bAx=b$
$Bx=dBx = dBx=d$
$x≥0x\ge 0x≥0$
## 2.1 主问题

原问题的对偶问题为：

max $wb+vdwb+vdwb+vd$

s.t. $wA+vB≤cwA+vB ≤ cwA+vB≤c$
$w,vw,vw,v$无限制
等价于
$max⁡w{wb+max⁡v:vB≤c−wAvd}\max_{w}\{wb+\max_{v:vB\le c-wA}vd\}maxw​{wb+maxv:vB≤c−wA​vd}$

等价于
$max⁡w{wb+min⁡x:Bx=d,x≥0(c−wA)x}\max_{w}\{wb+\min_{x:Bx=d,x\ge 0}(c-wA)x\}maxw​{wb+minx:Bx=d,x≥0​(c−wA)x}$

内部的min问题是个线性规划问题，枚举可行域{$x:Bx=d,x≥0x : Bx=d,x\ge 0x:Bx=d,x≥0$}的极点（$III$）和极方向（$JJJ$）便可以求解了。我们一边是逐步添加极点和极线约束，则原问题转为松弛主问题（RMP）：

max $zzz$

s.t. $z≤wb+(c−wA)xiz\le wb+(c-wA)x_iz≤wb+(c−wA)xi​$

## 2.2 子问题

将求出来的$www$带入下式(DSP)求解$xxx$：
$min⁡(c−wA)x+wb\min  (c-wA)x+wbmin(c−wA)x+wb$

s.t.  $Bx=dBx = dBx=d$
$x≥0x\ge 0x≥0$

求出来的$xxx$是原问题的一个极点或极线，将其添加到松弛主问题上继续求解。
## 2.3. benders分解求解步骤

由于约束条件较多，直接上所有约束条件求解问题比较困难。因此从少量约束的松弛主问题开始，逐步把极点约束条件加上（由于每个极点对应的有效约束条件一般不多，因此求解松弛主问题的约束条件数目常常也不多）。对于最小化问题，benders分解步骤为：
- 求解松弛主问题RMP，得到的最优值用来更新上界UB。
- 将$w∗w^*w∗$代入对偶子问题（DSP）求解$xxx$，得到的最优值用来更新下界LB，并且给松弛主问题RMP添加约束条件$w(b−Ax∗)+cx∗≥zw(b-Ax^*)+cx^*\ge zw(b−Ax∗)+cx∗≥z$
- 求解新的松弛主问题RMP’，回到1进行迭代直至LB = UB。

# 3. Benders分解例子

举一个例子：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404200109889.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2tpdHR5emM=,size_16,color_FFFFFF,t_70)

前两个约束比较复杂，而后四个约束有明显的”对角线分块“结构，因此将前两个约束和后四个约束分开，主问题转化成下式：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404201353433.png)
## 3.1 第1轮迭代

$x=(0,0,0,0)x=(0,0,0,0)x=(0,0,0,0)$是复杂问题（前两个约束）的一个可行解，求解主问题
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404203246261.png)

得到$w=(0,0)w=(0,0)w=(0,0)$，$UB=0UB=0UB=0$，求解子问题：
$min⁡−2x1−x2−x3+x4\min -2x_1-x_2-x_3+x_4min−2x1​−x2​−x3​+x4​$

s.t.
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019040420353275.png)

分块求解($x1,x2x_1,x_2x1​,x2​$)和($x3,x4x_3,x_4x3​,x4​$)，可以得到$x=(2,1.5,3,0)x=(2,1.5,3,0)x=(2,1.5,3,0)$，最优值为$LB=−8.5LB=-8.5LB=−8.5$
## 3.2 第2轮迭代

使用新的$xxx$添加一条约束，得到新的主问题：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404203312203.png)

得到$w=(−1.7,0)w=(-1.7,0)w=(−1.7,0)$，最优值为$UB=−3.4UB=-3.4UB=−3.4$，求解子问题
$min⁡−3.4−0.3x1−x2−2.7x3+x4\min -3.4-0.3x_1-x_2-2.7x_3+x_4min−3.4−0.3x1​−x2​−2.7x3​+x4​$

s.t.
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019040420353275.png)

得到$x=(0,2.5,0,0)x=(0,2.5,0,0)x=(0,2.5,0,0)$，最优值为$LB=−5.9LB=-5.9LB=−5.9$
## 3.3 第3轮迭代

使用新的$xxx$添加一条约束，得到新的主问题：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404231235392.png)

得到$w=(−1.2,0)w=(-1.2,0)w=(−1.2,0)$，最优值为$UB=−4.9UB=-4.9UB=−4.9$，求解子问题
$min⁡−2.4−0.8x1−x2+0.2x3+x4\min -2.4-0.8x_1-x_2+0.2x_3+x_4min−2.4−0.8x1​−x2​+0.2x3​+x4​$

s.t.
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019040420353275.png)

得到$x=(2,1.5,0,0)x=(2,1.5,0,0)x=(2,1.5,0,0)$，最优值为$LB=−5.5LB=-5.5LB=−5.5$

不断迭代下去即可，直到LB=UB







