# Stress与Strain+Tensor - 無名黑洞 - CSDN博客
2011年10月10日 15:34:37[wangxiaojun911](https://me.csdn.net/wangxiaojun911)阅读数：2103标签：[c																[matrix](https://so.csdn.net/so/search/s.do?q=matrix&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)
个人分类：[Physics](https://blog.csdn.net/wangxiaojun911/article/category/2656641)
当材料发生形变时，内部会产生复原力，如何在数学上描述这种状况是一个有用的话题。
**Stress**
在胡克定律中使用了Stress概念，现在对Stress的定义进一步解释。Stress为单位面积上承受的作用力，它的单位与压强一样是帕斯卡。
相应的力可以通过对Stress在面积S上积分得到。
在三维空间中，物体的内部任意切一个小方块，就有三组互相垂直的面，每个面上分别有三个互相垂直的应力：Sigma_11, Sigma_12, Sigma_13, Sigma_21, Sigma_22, Sigma_23, Sigma_31, Sigma_32, Sigma_33。其中Sigma_11, Sigma_22和Sigma_33是垂直相应面的。
**Strain Tensor**
有许多Strain Tensor理论，最接近连续情况的是infinitesimal strain tensor或Cauchy's strain tensor。在这种情况下，物体的变形必须相当于本身来说必须非常小。
**数学模型**
设A为原始状态的材料，B为变化后的材料。A中有一个位置记为X，m维。B中位置x，n维。已知原有位置X：
x = phi (t, X)
phi是X的函数
delta_x = phi(t, X + delta_X) – phi(t, X)
         = p( phi(t, X) ) / p(X) * delta_X
把p( phi(t, X) ) / p(X) 记为 F( n * m 矩阵)，F是phi的函数
F = p( phi(t, X) ) / p(X)
delta_x = F * delta_X
delta_x就是要求的B中的形变。如果直接使用这个量会产生问题，因为它不是一个不变量(invariance)。如果材料发生了旋转和平移，我们希望这个量能够保持一致性，所以使用：
(delta_x)^T * delta_x = (F * delta_X)^T * F * delta_X
                             = (delta_X)^T * ( F^T * F ) * delta_X
关键在( F^T * F )这个m*m矩阵上。如果它等于单位矩阵，那就等于没有变形。
定义**Strain** C:
C是F的函数。
C = ( F^T * F ) – I。
对于C = ( F^T * F ) – I分析可知它是一个对称矩阵，其对角项等于Stretch force，非对角项是Shearing force。
定义**Stress** S:
S 为C的函数。dA 为面积改变量，_n为面积法向量。
S = f(C) * dA * _n = E(C)
Transform Matrix
p(E(C)) / p(C) = p(f(C)) / p(C) * S
p(f(C)) / p(C) = WX + b
p(x) / p(X) = W。矩阵W也叫Transform Matrix。其实它也就是F。
**例**
三维坐标x (in R3)下的一块材质, 设其二维对应的平面为X=(u, v)。
有一个二维到三维的映射phi使得
x = phi(X)
由上面分析，我们关心F, 也就是phi在X上的导数。
定义：
PhiU是在u方向的偏导数；
PhiV是在v方向的偏导数。
考虑其中任一个三角形(i, j, k),
delta_x1 = xj – xi
delta_x2 = xk – xi
delta_u1 = uj – ui
delta_u2 = uk – ui
delta_v1 = vj – vi
delta_v2 = vk – vi
delta_x1, delta_x2是3X1矩阵。
delta_u1，delta_u2，delta_v1，delta_v2是标量。
delta_x1 = PhiU * delta_u1 + PhiV * delta_v1
delta_x2 = PhiU * delta_u2 + PhiV * delta_v2
即
| delta_x1 | ^T = | PhiU | ^ T * | delta_u1 delta_u2 |
| delta_x2 |     | PhiV |     | delta_v1 delta_v2 | 
及
| delta_x1 | ^ T * | delta_u1 delta_u2 | ^ (-1)* = | PhiU | ^ T
| delta_x2 |     | delta_v1 delta_v2 |        | PhiV |
PhiU, PhiV是3X1矩阵。
F  = | PhiU | ^ T
       | PhiV |
F是3X2矩阵. 到这里，F就完全用delta的量推导了。
在一些研究中，把Stretch 和shearing分别表示为不同的方程。就本例而言
 C = ( F^T * F ) – I
中C是个2乘2矩阵，对角线上为两个Stretch 方程：
a * ( PhiU^T * PhiU - 1 ) = a ( || PhiU || - 1 ) = 0 （1）
a * ( PhiV^T * PhiV - 1 ) = a ( || PhiV || - 1 ) = 0  （2）
非对角线上为Shearing方程：
a * PhiU^T * PhiV = 0 （3）
a是一个预设的系数。保证方程(1)(2)(3)成立就可以模拟材料的形变了。
使用Stress/Strain可以十分逼近地模拟材料形变。至少强于使用弹簧系统的有限元法。嗯。
