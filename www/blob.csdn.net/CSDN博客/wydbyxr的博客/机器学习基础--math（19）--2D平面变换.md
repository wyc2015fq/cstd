# 机器学习基础--math（19）--2D平面变换 - wydbyxr的博客 - CSDN博客
2018年06月26日 21:41:56[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：238
# 2D平面变换
　　2D平面变换包括仿射变换、射影变换等所有2D平面变化 
　　参考资料：[https://blog.csdn.net/myarrow/article/details/53709275](https://blog.csdn.net/myarrow/article/details/53709275)
# 仿射变换
　　仿射变换（affine），即“线性变换”+“平移” 
　　仿射变换（Affine Transformation）是空间直角坐标系的变换，从一个二维坐标变换到另一个二维坐标，仿射变换是一个线性变换，他保持了图像的“平行性”和“平直性”，即图像中原来的直线和平行线，变换后仍然保持原来的直线和平行线。可以理解为：经过对坐标轴的放缩，旋转，平移后原坐标在在新坐标域中的值. 
　　一个仿射变换对应于一个矩阵和一个向量的乘法，而仿射变换的复合对应于普通的矩阵乘法，只要加入一个额外的行到矩阵的底下，这一行全部是0除了最右边是一个1，而列向量的底下要加上一个1。 
　　参考资料：[https://blog.csdn.net/myarrow/article/details/53709275](https://blog.csdn.net/myarrow/article/details/53709275)
### 几种典型的仿射变换
　　仿射变换比较常用的特殊变换有平移(Translation)、缩放（Scale）、翻转（Flip）、旋转（Rotation）和错切(Shear)。 
　　1）平移变换（译注：平移变换是一种“刚体变换”，rigid-body transformation，中学学过的物理，都知道啥叫“刚体”吧，就是不会产生形变的理想物体，平移当然不会改变二维图形的形状。同理，下面的“旋转变换”也是刚体变换，而“缩放”、“错切”都是会改变图形形状的。）  
　　2）缩放变换，将每一点的横坐标放大（缩小）至sx倍，纵坐标放大（缩小）至sy倍 
　　[ sx 0 0 ] 
　　[ 0 sy 0 ] 
　　[ 0 0 1 ] 
　　当sx=sy时，称为尺度缩放，sx不等于sy时，这就是我们平时所说的拉伸变换。
　　3）“剪切变换”又称“错切变换”，指的是类似于四边形不稳定性那种性质，街边小商店那种铁拉门都见过吧？想象一下上面铁条构成的菱形拉动的过程，那就是“错切”的过程。） 
### 仿射变换的矩阵
　　仿射变换用到的是2×3矩阵”    “图像的二维仿射变换包括图像的平移（Translation）、缩放（Scale）、旋转（Rotation）等变换，实现这些变换只需要一个2*3维的变换矩阵。 
![这里写图片描述](https://img-blog.csdn.net/20180622233556428?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180622233612267?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 透视变换（投影变换）
　　透视变换与仿射变换（仿射变换是透视变换的子集）   “透视变换的函数和得到透视映射矩阵的函数同仿射变换没有太大区别，主要是仿射变换用到的是2×3矩阵，而透视变换用到的是3×3矩阵。 
　　仿射变换后平行四边形的各边仍操持平行，透视变换结果允许是梯形等四边形，所以仿射变换是透视变换的子集。”   仿射变换是透视变换的一种特殊情况．它把二维转到三维，变换后，再转映射回之前的二维空间（而不是另一个二维空间）
### 用途
　　将2D矩阵图像变换成3D的空间显示效果，全景拼接． 
　　射影变换：是最一般的线性变换。有8个自由度。 
　　射影变换保持重合关系和交比不变。但不会保持平行性（仿射变换会有平行性）。” 
### 方法
　　透视变换是将图片投影到一个新的视平面，也称作投影映射．它是二维（x,y）到三维(X,Y,Z)，再到另一个二维(x’,y’)空间的映射． 
　　相对于仿射变换，它提供了更大的灵活性，将一个四边形区域映射到另一个四边形区域（不一定是平行四边形）．它不止是线性变换．但也是通过矩阵乘法实现的，使用的是一个3x3的矩阵，矩阵的前两行与仿射矩阵相同(m11,m12,m13,m21,m22,m23)，也实现了线性变换和平移，第三行用于实现透视变换． 
### 参考资料
[https://blog.csdn.net/xieyan0811/article/details/71106539](https://blog.csdn.net/xieyan0811/article/details/71106539)
[https://blog.csdn.net/kinbosong/article/details/64923831](https://blog.csdn.net/kinbosong/article/details/64923831)
