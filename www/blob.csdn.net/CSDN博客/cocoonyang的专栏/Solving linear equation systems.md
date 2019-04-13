
# Solving linear equation systems - cocoonyang的专栏 - CSDN博客


2017年11月17日 19:35:59[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：133


现代数值计算方法一般都会转化为求解线性方程组. 线性方程组的矩阵形式如下所示:

$$
[A][x]=[b]
$$
其中$A$是$m \times n$矩阵,$m \ge n$,$x \in R^{n}$,$b\in R^{m}$. 由于求解线性方程组在数值分析中基础性核心地位, 线性方程组求解算法和技术一直层出不穷: 直接法,迭代法,并行算法,分块技术,超线程技术……
如果矩阵$A$是$n  \times n$阶方阵,  线性方程组的最直接的求解方法是:

$$
[x]=[A]^{-1}[b]
$$
Calculating the inverse of matrix$[A]$is easy to be implemented, however it requires approximately$n!$operations.  例如下列4X4矩阵求逆算法[3]:
```python
bool
```
```python
gluInvertMatrix(
```
```python
const
```
```python
double
```
```python
m[
```
```python
16
```
```python
],
```
```python
double
```
```python
invOut[
```
```python
16
```
```python
])
{
```
```python
double
```
```python
inv[
```
```python
16
```
```python
], det;
```
```python
int
```
```python
i;
    inv[
```
```python
0
```
```python
] = m[
```
```python
5
```
```python
]  * m[
```
```python
10
```
```python
] * m[
```
```python
15
```
```python
] - 
             m[
```
```python
5
```
```python
]  * m[
```
```python
11
```
```python
] * m[
```
```python
14
```
```python
] - 
             m[
```
```python
9
```
```python
]  * m[
```
```python
6
```
```python
]  * m[
```
```python
15
```
```python
] + 
             m[
```
```python
9
```
```python
]  * m[
```
```python
7
```
```python
]  * m[
```
```python
14
```
```python
] +
             m[
```
```python
13
```
```python
] * m[
```
```python
6
```
```python
]  * m[
```
```python
11
```
```python
] - 
             m[
```
```python
13
```
```python
] * m[
```
```python
7
```
```python
]  * m[
```
```python
10
```
```python
];
    inv[
```
```python
4
```
```python
] = -m[
```
```python
4
```
```python
]  * m[
```
```python
10
```
```python
] * m[
```
```python
15
```
```python
] + 
              m[
```
```python
4
```
```python
]  * m[
```
```python
11
```
```python
] * m[
```
```python
14
```
```python
] + 
              m[
```
```python
8
```
```python
]  * m[
```
```python
6
```
```python
]  * m[
```
```python
15
```
```python
] - 
              m[
```
```python
8
```
```python
]  * m[
```
```python
7
```
```python
]  * m[
```
```python
14
```
```python
] - 
              m[
```
```python
12
```
```python
] * m[
```
```python
6
```
```python
]  * m[
```
```python
11
```
```python
] + 
              m[
```
```python
12
```
```python
] * m[
```
```python
7
```
```python
]  * m[
```
```python
10
```
```python
];
    inv[
```
```python
8
```
```python
] = m[
```
```python
4
```
```python
]  * m[
```
```python
9
```
```python
] * m[
```
```python
15
```
```python
] - 
             m[
```
```python
4
```
```python
]  * m[
```
```python
11
```
```python
] * m[
```
```python
13
```
```python
] - 
             m[
```
```python
8
```
```python
]  * m[
```
```python
5
```
```python
] * m[
```
```python
15
```
```python
] + 
             m[
```
```python
8
```
```python
]  * m[
```
```python
7
```
```python
] * m[
```
```python
13
```
```python
] + 
             m[
```
```python
12
```
```python
] * m[
```
```python
5
```
```python
] * m[
```
```python
11
```
```python
] - 
             m[
```
```python
12
```
```python
] * m[
```
```python
7
```
```python
] * m[
```
```python
9
```
```python
];
    inv[
```
```python
12
```
```python
] = -m[
```
```python
4
```
```python
]  * m[
```
```python
9
```
```python
] * m[
```
```python
14
```
```python
] + 
               m[
```
```python
4
```
```python
]  * m[
```
```python
10
```
```python
] * m[
```
```python
13
```
```python
] +
               m[
```
```python
8
```
```python
]  * m[
```
```python
5
```
```python
] * m[
```
```python
14
```
```python
] - 
               m[
```
```python
8
```
```python
]  * m[
```
```python
6
```
```python
] * m[
```
```python
13
```
```python
] - 
               m[
```
```python
12
```
```python
] * m[
```
```python
5
```
```python
] * m[
```
```python
10
```
```python
] + 
               m[
```
```python
12
```
```python
] * m[
```
```python
6
```
```python
] * m[
```
```python
9
```
```python
];
    inv[
```
```python
1
```
```python
] = -m[
```
```python
1
```
```python
]  * m[
```
```python
10
```
```python
] * m[
```
```python
15
```
```python
] + 
              m[
```
```python
1
```
```python
]  * m[
```
```python
11
```
```python
] * m[
```
```python
14
```
```python
] + 
              m[
```
```python
9
```
```python
]  * m[
```
```python
2
```
```python
] * m[
```
```python
15
```
```python
] - 
              m[
```
```python
9
```
```python
]  * m[
```
```python
3
```
```python
] * m[
```
```python
14
```
```python
] - 
              m[
```
```python
13
```
```python
] * m[
```
```python
2
```
```python
] * m[
```
```python
11
```
```python
] + 
              m[
```
```python
13
```
```python
] * m[
```
```python
3
```
```python
] * m[
```
```python
10
```
```python
];
    inv[
```
```python
5
```
```python
] = m[
```
```python
0
```
```python
]  * m[
```
```python
10
```
```python
] * m[
```
```python
15
```
```python
] - 
             m[
```
```python
0
```
```python
]  * m[
```
```python
11
```
```python
] * m[
```
```python
14
```
```python
] - 
             m[
```
```python
8
```
```python
]  * m[
```
```python
2
```
```python
] * m[
```
```python
15
```
```python
] + 
             m[
```
```python
8
```
```python
]  * m[
```
```python
3
```
```python
] * m[
```
```python
14
```
```python
] + 
             m[
```
```python
12
```
```python
] * m[
```
```python
2
```
```python
] * m[
```
```python
11
```
```python
] - 
             m[
```
```python
12
```
```python
] * m[
```
```python
3
```
```python
] * m[
```
```python
10
```
```python
];
    inv[
```
```python
9
```
```python
] = -m[
```
```python
0
```
```python
]  * m[
```
```python
9
```
```python
] * m[
```
```python
15
```
```python
] + 
              m[
```
```python
0
```
```python
]  * m[
```
```python
11
```
```python
] * m[
```
```python
13
```
```python
] + 
              m[
```
```python
8
```
```python
]  * m[
```
```python
1
```
```python
] * m[
```
```python
15
```
```python
] - 
              m[
```
```python
8
```
```python
]  * m[
```
```python
3
```
```python
] * m[
```
```python
13
```
```python
] - 
              m[
```
```python
12
```
```python
] * m[
```
```python
1
```
```python
] * m[
```
```python
11
```
```python
] + 
              m[
```
```python
12
```
```python
] * m[
```
```python
3
```
```python
] * m[
```
```python
9
```
```python
];
    inv[
```
```python
13
```
```python
] = m[
```
```python
0
```
```python
]  * m[
```
```python
9
```
```python
] * m[
```
```python
14
```
```python
] - 
              m[
```
```python
0
```
```python
]  * m[
```
```python
10
```
```python
] * m[
```
```python
13
```
```python
] - 
              m[
```
```python
8
```
```python
]  * m[
```
```python
1
```
```python
] * m[
```
```python
14
```
```python
] + 
              m[
```
```python
8
```
```python
]  * m[
```
```python
2
```
```python
] * m[
```
```python
13
```
```python
] + 
              m[
```
```python
12
```
```python
] * m[
```
```python
1
```
```python
] * m[
```
```python
10
```
```python
] - 
              m[
```
```python
12
```
```python
] * m[
```
```python
2
```
```python
] * m[
```
```python
9
```
```python
];
    inv[
```
```python
2
```
```python
] = m[
```
```python
1
```
```python
]  * m[
```
```python
6
```
```python
] * m[
```
```python
15
```
```python
] - 
             m[
```
```python
1
```
```python
]  * m[
```
```python
7
```
```python
] * m[
```
```python
14
```
```python
] - 
             m[
```
```python
5
```
```python
]  * m[
```
```python
2
```
```python
] * m[
```
```python
15
```
```python
] + 
             m[
```
```python
5
```
```python
]  * m[
```
```python
3
```
```python
] * m[
```
```python
14
```
```python
] + 
             m[
```
```python
13
```
```python
] * m[
```
```python
2
```
```python
] * m[
```
```python
7
```
```python
] - 
             m[
```
```python
13
```
```python
] * m[
```
```python
3
```
```python
] * m[
```
```python
6
```
```python
];
    inv[
```
```python
6
```
```python
] = -m[
```
```python
0
```
```python
]  * m[
```
```python
6
```
```python
] * m[
```
```python
15
```
```python
] + 
              m[
```
```python
0
```
```python
]  * m[
```
```python
7
```
```python
] * m[
```
```python
14
```
```python
] + 
              m[
```
```python
4
```
```python
]  * m[
```
```python
2
```
```python
] * m[
```
```python
15
```
```python
] - 
              m[
```
```python
4
```
```python
]  * m[
```
```python
3
```
```python
] * m[
```
```python
14
```
```python
] - 
              m[
```
```python
12
```
```python
] * m[
```
```python
2
```
```python
] * m[
```
```python
7
```
```python
] + 
              m[
```
```python
12
```
```python
] * m[
```
```python
3
```
```python
] * m[
```
```python
6
```
```python
];
    inv[
```
```python
10
```
```python
] = m[
```
```python
0
```
```python
]  * m[
```
```python
5
```
```python
] * m[
```
```python
15
```
```python
] - 
              m[
```
```python
0
```
```python
]  * m[
```
```python
7
```
```python
] * m[
```
```python
13
```
```python
] - 
              m[
```
```python
4
```
```python
]  * m[
```
```python
1
```
```python
] * m[
```
```python
15
```
```python
] + 
              m[
```
```python
4
```
```python
]  * m[
```
```python
3
```
```python
] * m[
```
```python
13
```
```python
] + 
              m[
```
```python
12
```
```python
] * m[
```
```python
1
```
```python
] * m[
```
```python
7
```
```python
] - 
              m[
```
```python
12
```
```python
] * m[
```
```python
3
```
```python
] * m[
```
```python
5
```
```python
];
    inv[
```
```python
14
```
```python
] = -m[
```
```python
0
```
```python
]  * m[
```
```python
5
```
```python
] * m[
```
```python
14
```
```python
] + 
               m[
```
```python
0
```
```python
]  * m[
```
```python
6
```
```python
] * m[
```
```python
13
```
```python
] + 
               m[
```
```python
4
```
```python
]  * m[
```
```python
1
```
```python
] * m[
```
```python
14
```
```python
] - 
               m[
```
```python
4
```
```python
]  * m[
```
```python
2
```
```python
] * m[
```
```python
13
```
```python
] - 
               m[
```
```python
12
```
```python
] * m[
```
```python
1
```
```python
] * m[
```
```python
6
```
```python
] + 
               m[
```
```python
12
```
```python
] * m[
```
```python
2
```
```python
] * m[
```
```python
5
```
```python
];
    inv[
```
```python
3
```
```python
] = -m[
```
```python
1
```
```python
] * m[
```
```python
6
```
```python
] * m[
```
```python
11
```
```python
] + 
              m[
```
```python
1
```
```python
] * m[
```
```python
7
```
```python
] * m[
```
```python
10
```
```python
] + 
              m[
```
```python
5
```
```python
] * m[
```
```python
2
```
```python
] * m[
```
```python
11
```
```python
] - 
              m[
```
```python
5
```
```python
] * m[
```
```python
3
```
```python
] * m[
```
```python
10
```
```python
] - 
              m[
```
```python
9
```
```python
] * m[
```
```python
2
```
```python
] * m[
```
```python
7
```
```python
] + 
              m[
```
```python
9
```
```python
] * m[
```
```python
3
```
```python
] * m[
```
```python
6
```
```python
];
    inv[
```
```python
7
```
```python
] = m[
```
```python
0
```
```python
] * m[
```
```python
6
```
```python
] * m[
```
```python
11
```
```python
] - 
             m[
```
```python
0
```
```python
] * m[
```
```python
7
```
```python
] * m[
```
```python
10
```
```python
] - 
             m[
```
```python
4
```
```python
] * m[
```
```python
2
```
```python
] * m[
```
```python
11
```
```python
] + 
             m[
```
```python
4
```
```python
] * m[
```
```python
3
```
```python
] * m[
```
```python
10
```
```python
] + 
             m[
```
```python
8
```
```python
] * m[
```
```python
2
```
```python
] * m[
```
```python
7
```
```python
] - 
             m[
```
```python
8
```
```python
] * m[
```
```python
3
```
```python
] * m[
```
```python
6
```
```python
];
    inv[
```
```python
11
```
```python
] = -m[
```
```python
0
```
```python
] * m[
```
```python
5
```
```python
] * m[
```
```python
11
```
```python
] + 
               m[
```
```python
0
```
```python
] * m[
```
```python
7
```
```python
] * m[
```
```python
9
```
```python
] + 
               m[
```
```python
4
```
```python
] * m[
```
```python
1
```
```python
] * m[
```
```python
11
```
```python
] - 
               m[
```
```python
4
```
```python
] * m[
```
```python
3
```
```python
] * m[
```
```python
9
```
```python
] - 
               m[
```
```python
8
```
```python
] * m[
```
```python
1
```
```python
] * m[
```
```python
7
```
```python
] + 
               m[
```
```python
8
```
```python
] * m[
```
```python
3
```
```python
] * m[
```
```python
5
```
```python
];
    inv[
```
```python
15
```
```python
] = m[
```
```python
0
```
```python
] * m[
```
```python
5
```
```python
] * m[
```
```python
10
```
```python
] - 
              m[
```
```python
0
```
```python
] * m[
```
```python
6
```
```python
] * m[
```
```python
9
```
```python
] - 
              m[
```
```python
4
```
```python
] * m[
```
```python
1
```
```python
] * m[
```
```python
10
```
```python
] + 
              m[
```
```python
4
```
```python
] * m[
```
```python
2
```
```python
] * m[
```
```python
9
```
```python
] + 
              m[
```
```python
8
```
```python
] * m[
```
```python
1
```
```python
] * m[
```
```python
6
```
```python
] - 
              m[
```
```python
8
```
```python
] * m[
```
```python
2
```
```python
] * m[
```
```python
5
```
```python
];
    det = m[
```
```python
0
```
```python
] * inv[
```
```python
0
```
```python
] + m[
```
```python
1
```
```python
] * inv[
```
```python
4
```
```python
] + m[
```
```python
2
```
```python
] * inv[
```
```python
8
```
```python
] + m[
```
```python
3
```
```python
] * inv[
```
```python
12
```
```python
];
```
```python
if
```
```python
(det ==
```
```python
0
```
```python
)
```
```python
return
```
```python
false
```
```python
;
    det =
```
```python
1.0
```
```python
/ det;
```
```python
for
```
```python
(i =
```
```python
0
```
```python
; i <
```
```python
16
```
```python
; i++)
        invOut[i] = inv[i] * det;
```
```python
return
```
```python
true
```
```python
;
}
```
当$A$是低阶矩阵时,这种直接求解算法简易,可靠. 计算量较小.对于大规模线性方程组,$n!$是一个天文数字. 为了提高求解效率, 利用变换等多种方法简化求解线性方程组, 是降低计算量的重要途径. 常用的算法主要有: 高斯消元法(Gaussian elimination), Gauss-Jordan 法, LU分解法(LU decomposition), QR分解法(QR decomposition)等.
[8]In Gaussian elimination is unstable when meeting zero pivots.  As small pivots operate similar to zeros during Gaussian elimination, some LU decompositions become numerically unstable if relatively small pivots are used.  In order to prevent this instability, realtively large entries are selected as pivot entries.  This prevents large factors from appearing in the computed L and U, which reduces roundoff errors during compuation. [9] provided some case studies of pivoting in LU decomposition. There are three kinds of pivoting: partial pivoting, complete pivoting and rook pivoting.
Using QR factorization is much better to solve$Ax=b$than using LU factorization even with partial pivoting[10].
[1] 列出了几种主要算法的简要对比：
|Method|Computing Complexity|Notes|
|---|---|---|
|Laplace expansion|n!|numerical errors|
|Gaussian elimination|n^{3}/3 + n^{2}/2|numerical errors|
|LU decomposition|n^{3}/3 + n^{2} -n/3|numerical errors|
|QR decomposition|2n^{3}+ 3n^{3}|Good Precision|
如果矩阵$A$具有特殊的结构，例如是稀疏矩阵,或者是对角阵, 利用矩阵$A$的结构特点, 波前法,共轭法等可以高效求解大规模线性方程组.
然而要做一个高效的线性方程组求解器是一件非常艰难的事。这需要多年的时间来学习，研究矩阵理论等相关知识。编写代码不仅需要多年磨练，也需要花费多年时间对代码进行调整、测试，还需要多年的经验来探究求解过程中的一些技巧和窍门(这已经属于商业公司的不传之密了)
下面是几个著名的稀疏矩阵求解器：
Harwell subroutines
[http://www.hsl.rl.ac.uk/catalogue/](http://www.hsl.rl.ac.uk/catalogue/)
PARDISO:
[http://www.pardiso-project.org/](http://www.pardiso-project.org/)
MUMPS
[http://graal.ens-lyon.fr/MUMPS/](http://graal.ens-lyon.fr/MUMPS/)
SuperLU
[http://crd-legacy.lbl.gov/~xiaoye/SuperLU/](http://crd-legacy.lbl.gov/~xiaoye/SuperLU/)
> [1]
> [http://qucs.sourceforge.net/tech/node99.html](http://qucs.sourceforge.net/tech/node99.html)

> [2]
> [https://en.wikipedia.org/wiki/System_of_linear_equations](https://en.wikipedia.org/wiki/System_of_linear_equations)

> [3]
> [https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix](https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix)

> [4]
> [http://fourier.eng.hmc.edu/e176/lectures/NM/node10.html](http://fourier.eng.hmc.edu/e176/lectures/NM/node10.html)

> [5]
> [http://www.math.iit.edu/~fass/477577_Chapter_4.pdf](http://www.math.iit.edu/~fass/477577_Chapter_4.pdf)

> [6]
> [http://www.aaronschlegel.com/qr-decomposition-householder-reflections/](http://www.aaronschlegel.com/qr-decomposition-householder-reflections/)

> [7]
> [http://www.math.usm.edu/lambers/mat610/sum10/lecture9.pdf](http://www.math.usm.edu/lambers/mat610/sum10/lecture9.pdf)

> [8]
> [http://buzzard.ups.edu/courses/2014spring/420projects/math420-UPS-spring-2014-reid-LU-pivoting.pdf](http://buzzard.ups.edu/courses/2014spring/420projects/math420-UPS-spring-2014-reid-LU-pivoting.pdf)

> [9]
> [http://www.cnblogs.com/lacozhang/p/3693203.html](http://www.cnblogs.com/lacozhang/p/3693203.html)

> [10]
> [https://www-old.math.gatech.edu/academic/courses/core/math2601/Web-notes/3num.pdf](https://www-old.math.gatech.edu/academic/courses/core/math2601/Web-notes/3num.pdf)


