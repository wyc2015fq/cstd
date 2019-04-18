# 核方法kernel method - wydbyxr的博客 - CSDN博客
2018年09月30日 10:53:18[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：165
所属专栏：[经典机器学习算法](https://blog.csdn.net/column/details/28812.html)
# 核方法kernel method
  核方法(kernel method)是将数据映射到更高维的空间实现线性可分,而Kernel Function（核函数）只是一个关于特征向量的函数，本质是变换后的空间中的内积，这个函数的构造和引入的初衷只是为了提高SVM在高维的计算效率。
  Kernel 是隐式地将两个向量转换到其他形式然后求内积, 相比显式的转换可以极大的减少计算复杂度, 甚至可以将有限维的 x 转换到无限维。
  有时f(·)会把n维空间映射到无限维空间去，对此我们常常束手无策，除非是用kernel，尤其是RBF kernel（K(x,y) = exp(-||x-y||^2) ）。
## 定义
  核函数K（kernel function）就是指K(x, y) = <f(x), f(y)>，其中x和y是n维的输入值，f(·) 是从n维到m维的映射（通常而言，m>>n），<x, y>是x和y的内积，严格来说应该叫欧式空间的标准内积，也就是很多人常说的点积。
  直观上，要计算<f(x), f(y)>，我们要先分别计算f(x)和f(y)，然后再求它们的内积。但经过映射后的x和y，维数大大增加，计算内积的成本可能会非常之大，而且在高位空间费力牛劲儿地计算内积，内积又是一个scalar，相当于说又把我们的计算从高维空间拉回到一维空间！所以我们特别想要一个“简便运算法”，帮助我们不需要奔向高维空间就能在家门口计算得到想要的内积，而kernel就能帮助我们做到这一点。		
![在这里插入图片描述](https://img-blog.csdn.net/20180930105515160?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 例子
  1)令 x = (x1, x2, x3, x4); y = (y1, y2, y3, y4);
  2)令 f(x) = (x1x1, x1x2, x1x3, x1x4, x2x1, x2x2, x2x3, x2x4, x3x1, x3x2, x3x3, x3x4, x4x1, x4x2, x4x3, x4x4); f(y)亦然；
  3)令核函数 K(x, y) = (<x, y>)^2
  带入一组xy数，x = (1, 2, 3, 4); y = (5, 6, 7, 8)
<f(x), f(y)> = 4900"	好累，因为这里f(·)把四维空间的数据映射到十六维空间里。
如果用核函数,K(x, y) = (5+12+21+32)^2 = 70^2 = 4900.
## 注意
低维空间中线性不可分的数据变换到高维空间找到一个线性可分的分类的过程， 并不能与kernel trick等价，更不能与kernel等价。
