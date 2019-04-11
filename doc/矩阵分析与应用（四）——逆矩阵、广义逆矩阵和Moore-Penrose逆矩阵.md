# 矩阵分析与应用（四）——逆矩阵、广义逆矩阵和Moore-Penrose逆矩阵

2017年08月07日 17:02:20 [NirHeavenX](https://me.csdn.net/qsczse943062710) 阅读数：3320



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/qsczse943062710/article/details/76851454

### **逆矩阵**

  [逆矩阵的定义](https://blog.csdn.net/qsczse943062710/article/details/76851454)：如果对于一个**方阵**A，存在一个方阵B，使得AB=BA=I，那么我们称B为A的逆矩阵，记做：$A^{-1}=B=\frac{1}{\vert A\vert}A^*$，这里$A^∗$代表伴随矩阵。 
  一个n∗n的方阵存在逆矩阵的充要条件等价于：

1. A为非奇异矩阵
2. rank(A)=n
3. A的行向量线性无关
4. A的列向量线性无关
5. det(A)≠0，即行列式不为0
6. Ax=0只有唯一平凡解x=0
7. Ax=b为一致方程，且有唯一解
8. A的零空间的维度为0

  如果对于方程Ax=b，当其中的某些线性约束成立的情况下，其他的线性约束不可能成立，则称该方程为**非一致方程**。 
  [矩阵的零空间](https://blog.csdn.net/qsczse943062710/article/details/76851454)是指线性方程组Ax=0的解向量张成的空间的。

  一些基本的性质这里不赘述，值得一提的是[两个矩阵之和的求逆运算](https://blog.csdn.net/qsczse943062710/article/details/76851454)，它不同于转置等运算（$(A+B)^T = A^T+B^T$）。

#### **Sherman-Morrison公式：**

$$
(A+xy^T)^{-1}=A^{-1}+\frac{A^{-1}xy^HA^{-1}}{1+y^HA^{-1}x}
$$

  这个公式还有很多形式的变体。



#### **分块矩阵的求逆公式：**

$$
当A可逆时，\begin{bmatrix}A&U\\V&D\end{bmatrix}^{-1}=\begin{bmatrix}A^{-1}+A^{-1}U(D-VA^{-1}U)^{-1}VA^{-1})&-A^{-1}U(D-VA^{-1}U)^{-1}\\-(D-VA^{-1}U)^{-1}VA^{-1}&(D-VA^{-1}U)^{-1}
\end{bmatrix}^{-1}\\
当A，D可逆时，\begin{bmatrix}A&U\\V&D\end{bmatrix}^{-1}化简为：\begin{bmatrix}(A-UD^{-1V})^{-1}&-A^{-1}U(D-VA^{-1}U)^{-1}\\-D^{-1}V(A-UD^{-1}V)^{-1}&(D-VA^{-1}U)^{-1}\end{bmatrix}^{-1}
$$



### **广义逆矩阵**

  我们看到，逆矩阵的定义仅仅针对方阵而言，但是实际应用中，我们遇到的很多问题并不满足这个条件，将矩阵的逆的定义扩展到任意矩阵，得到我们的广义逆矩阵： 
  如果一个矩阵L满足$LA=I，A\in R^{m*n}$，则我们称L为A的广义逆矩阵，特别地，对于LA=I，我们称为[左逆矩阵](https://blog.csdn.net/qsczse943062710/article/details/76851454)，只有当m≥n时，A才**可能**有左逆矩阵；对于AL=I，我们称为[右逆矩阵](https://blog.csdn.net/qsczse943062710/article/details/76851454)，只有当m≤n时，A才**可能**有右逆矩阵。 
  证明如下，考虑m≥n： 
$$
A=\begin{bmatrix}B\\C\end{bmatrix}，其中B\in R^{n*n}，令L=[X,Y]满足LA=I，\\
我们有XB+YC=I，如B非奇异（这是可能的），只需要令X=B^{-1}，Y=O即可
$$


  一个矩阵的广义逆矩阵往往不是唯一的，特别地，有以下形式的广义逆矩阵：
$$
左逆：L=(A^HA)^{-1}A^H唯一，且称为左伪逆矩阵\\
右逆：L=A^H(AA^H)^{-1}唯一，且称为右伪逆矩阵
$$


是不是很眼熟？对了，这就是和最小二乘密切相关的两个广义逆矩阵！，左逆对应于**超定问题（非一致方程）的最小二乘解**，右逆对应于**欠定问题（一致方程）的最小范数解**。



------

  现在，我们将左逆和右逆统一起来，用线性方程组的解的形式来描述： 
$$
对于A\in R^{m*n}，秩任意，则A的广义逆矩阵是一个n*m矩阵G\\
并使得当Ax=y(y \ne 0)为一致方程时，有解x=Gy，当且仅当G满足AGA=A
$$
  最后一个等式也是广义逆的定义式：$A^-存在\leftrightarrow AA^-A=A$



  **定理**：

- $A^-存在\leftrightarrow A^-A和AA^-皆为幂等矩阵，即(AA^-)^2=AA^-,(A^-A)^2=A^-A$
- $A^-存在\leftrightarrow rank(A)=rank(AA^-)或rank(A)=rank(A^-A)$

#### **广义逆矩阵的计算**

  **定理**：对于任意的秩为rr的矩阵AA，都可以分解为： 
$$
A=F_{m*r}G_{r*n},F和G分别为列满秩和行满秩
$$
  称为矩阵的**满秩分解**，求解步骤如下

1. 将AA通过行初等变换化为阶梯矩阵，得到$A=\begin{bmatrix}G\\O\end{bmatrix}$
2. 对单位矩阵执行上述变换的逆变换，得到$I\to P^{-1}$
3. A=FG，其中F为$P^{-1}$前r列组成的子矩阵

------

  则A的广义逆矩阵可以用以下公式求解： 
$$
A^-=G^T(F^TAG^T)^{-1}F^T
$$
  容易验证，它满足广义逆矩阵的定义式$AA^-A=A$ 

  且F和G分别为列满秩和行满秩，所以$(F^TAG^T)^{-1}=(F^TFGG^T)^{-1}=(F^TF)^{-1}(GG^T)^{-1}$一定存在。



  回过头来，我们看看用广义逆矩阵来定义线性方程的解会有什么结论：

  **定理1**：齐次方程Ax=0的一个通解为$x=(I-A^-A)z$，其中zz为任意的$n*1$向量。 
  **定理2**：非齐次方程Ax=y为一致方程的充要条件为：$AA^-y=y$。 
  **定理3**：非齐次方程Ax=y的一个通解为$x=A^-y+(I-A^-A)z$，其中z为任意的$n*1​$向量。 
  上述三个定理可以通过直接验证广义逆矩阵的定义式得证。

### **Moore-Penrose逆矩阵**

  由前面定义的逆矩阵求解超定问题（非一致方程）的最小二乘解和欠定问题（一致方程）的最小范数解时，解是不唯一的。因此将广义逆矩阵做进一步的约束，便得到Moore-Penrose逆矩阵（平时说的伪逆就是它），它能保证解的唯一性。

  定义满足下列性质的矩阵G为矩阵A的Moore-Penrose逆矩阵，记做$A^+$：

1. AGA=A
2. GAG=G
3. $(AG)^H=AG$
4. $(GA)^H=GA$

  Moore-Penrose逆矩阵是由Moore在1935年提出的，由于原始定义十分晦涩，于是Penrose于1955年提出了上述的四个条件，因此名为[Moore-Penrose逆矩阵](https://blog.csdn.net/qsczse943062710/article/details/76851454)。

Moore-Penrose逆矩阵又根据满足上述条件的个数，分为以下几种： 
①只满足条件1,2，称为[自反广义逆矩阵](https://blog.csdn.net/qsczse943062710/article/details/76851454) 
②只满足条件1,2,3，称为[正则化广义逆矩阵](https://blog.csdn.net/qsczse943062710/article/details/76851454) 
③只满足条件1,2,4，称为[弱广义逆矩阵](https://blog.csdn.net/qsczse943062710/article/details/76851454)

注意，对于只满足某些条件的逆矩阵，它的秩总是大于等于原矩阵的秩。即： 
$$
rank(A^g)\ge rank(A)=rank(AA^g)=rank(A^gA)，当A^g为自反逆矩阵时取等。
$$
我们前面提到的**左伪逆矩阵**和**右伪逆矩阵**都是Moore-Penrose矩阵，满足四个条件。



#### **Moore-Penrose逆矩阵的计算**

**1.方程求解法：**

![img](https://img-blog.csdn.net/20170807192059433?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXNjenNlOTQzMDYyNzEw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**算法1.9.1**

步骤1 计算矩阵$B=AA^H$。
步骤2 求解矩阵方程$B^2X^H=B$得到矩阵$X^H$。
步骤3 计算B的Moore-Penrose逆矩阵$B^+ =(AA^H)^+=XBX^H$。
步骤4 计算矩阵A的Moore-Penrose逆矩阵$A^+=A^H（AA^H）^+=A^HB^+$。

**算法1.9.2**

步骤1计算矩阵$B=A^HA$。
步骤2求解矩阵方程$B^2X^H=B$得到矩阵$X^H$。
步骤3计算B的Moore-Penrose逆矩阵$B^+=（A^HA）^+=XBX^H$。
步骤4计算矩阵A的Moore-Penrose 逆矩阵 $A^+=（A^HA）^+A^H=B^+A^H$。
若矩阵$A_{m \times n}$的列数大于行数，则矩阵乘积$AA^H$的维数比$A^HA$的维数小，故选择算法1.9.1可花费较少的计算量。反之，若A的行数大于列数，则选择算法1.9.2。





**2.KL分解法：**

  即通过矩阵的满秩分解求解，求解方式同上述的广义逆矩阵，只不过将转置运算换成共轭转置，容易验证，该法求解得到的结果满足上述四个条件。

PS:当使用Moore-Penrose逆矩阵求解超定问题（非一致方程）的最小二乘解时，不仅解唯一，且是[最小二乘最小范数解](https://blog.csdn.net/qsczse943062710/article/details/76851454)