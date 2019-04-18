# Matlab中 pdist 函数详解(各种距离的生成) - fighting！！！ - CSDN博客
2018年11月05日 10:41:07[dujiahei](https://me.csdn.net/dujiahei)阅读数：780
转自：[http://blog.sciencenet.cn/blog-531885-589056.html](http://blog.sciencenet.cn/blog-531885-589056.html)
**一、pdist**
Pairwise distance between pairs of objects
**Syntax **
D = pdist(X)
D = pdist(X,*distance*)
**Description**
D = pdist(X)    计算 X 中各对行向量的相互距离(X是一个m-by-n的矩阵). 这里 D 要特别注意，D 是一个长为*m*(*m*–1)/2的行向量.可以这样理解 D 的生成：首先生成一个 X 的距离方阵，由于该方阵是对称的，令对角线上的元素为0，所以取此方阵的下三角元素，按照Matlab中矩阵的按列存储原则，此下三角各元素的索引排列即为(2,1), (3,1), ..., (*m*,1), (3,2), ..., (*m*,2), ..., (*m*,*m*–1).可以用命令 **squareform(D)** 将此行向量转换为原距离方阵.(squareform函数是专门干这事的，其逆变换是也是squareform。)
D = pdist(X,*distance*)     使用指定的距离.distance可以取下面圆括号中的值，用红色标出！
**Metrics**
Given an *m*-by-*n* data matrix X, which is treated as *m* (1-by-*n*) row vectors x1, x2, ..., x*m*, the various distances between the vector x*s* and x*t* are defined as follows:
- 
**欧几里德距离**Euclidean distance(**'euclidean'**)
d2s,t=(xs−xt)⋅(xs−xt)′ds,t2=(xs−xt)⋅(xs−xt)′
	Notice that the Euclidean distance is a special case of the Minkowski metric, where p = 2.
	欧氏距离虽然很有用，但也有明显的缺点。
	   一：它将样品的不同属性（即各指标或各变量）之间的差别等同看待，这一点有时不能满足实际要求。
	   二：它没有考虑各变量的数量级(量纲)，容易犯大数吃小数的毛病。所以，可以先对原始数据进行规范化处理再进行距离计算。
- **标准欧几里德距离**Standardized Euclidean distance(**'seuclidean'**)
	
d2s,t=(xs−xt)V−1(xs−xt)′ds,t2=(xs−xt)V−1(xs−xt)′
	where V is the *n*-by-*n* diagonal matrix whose *j*th diagonal element is S(*j*)2, where S is the vector of standard deviations.
	   相比单纯的欧氏距离，标准欧氏距离能够有效的解决上述缺点。注意，这里的V在许多Matlab函数中是可以自己设定的，不一定非得取标准差，可以依据各变量的重要程度设置不同的值，如knnsearch函数中的Scale属性。
- 
**马哈拉诺比斯距离**Mahalanobis distance(**'mahalanobis'**)
d2s,t=(xs−xt)C−1(xs−xt)′ds,t2=(xs−xt)C−1(xs−xt)′
	where C is the covariance matrix.
	   马氏距离是由印度统计学家马哈拉诺比斯(P. C. Mahalanobis)提出的，表示数据的协方差距离。它是一种有效的计算两个未知样本集的相似度的方法。与欧式距离不同的是它考虑到各种特性之间的联系（例如：一条关于身高的信息会带来一条关于体重的信息，因为两者是有关联的）并且是尺度无关的(scale-invariant)，即独立于测量尺度。
	如果协方差矩阵为单位矩阵,那么马氏距离就简化为欧式距离,如果协方差矩阵为对角阵,则其也可称为正规化的欧氏距离.
	马氏优缺点：
	
　　1）马氏距离的计算是建立在总体样本的基础上的，因为C是由总样本计算而来，所以马氏距离的计算是不稳定的；
　　2）在计算马氏距离过程中，要求总体样本数大于样本的维数。
　　3）协方差矩阵的逆矩阵可能不存在。  
- 
**曼哈顿距离(城市区块距离)**City block metric(**'cityblock'**)
ds,t=∑j=1n∣∣xsj−xtj∣∣ds,t=∑j=1n|xsj−xtj|
	Notice that the city block distance is a special case of the Minkowski metric, where p=1.
- 
**闵可夫斯基距离**Minkowski metric(**'minkowski'**)
ds,t=∑j=1n∣∣xsj−xtj∣∣p−−−−−−−−−−−⎷pds,t=∑j=1n|xsj−xtj|pp
	Notice that for the special case of p = 1, the Minkowski metric gives the city block metric, for the special case of p = 2, the Minkowski metric gives the Euclidean distance, and for the special case of p = ∞, the Minkowski metric gives the Chebychev distance.
	   闵可夫斯基距离由于是欧氏距离的推广，所以其缺点与欧氏距离大致相同。
- 
**切比雪夫距离**Chebychev distance(**'chebychev'**)
ds,t=maxj∣∣xsj−xtj∣∣ds,t=maxj|xsj−xtj|
	Notice that the Chebychev distance is a special case of the Minkowski metric, where p = ∞.
- 
**夹角余弦距离**Cosine distance(**'cosine'**)
ds,t=1−xsxt′∥xs∥2⋅∥xt∥2ds,t=1−xsxt′‖xs‖2⋅‖xt‖2
	   与Jaccard距离相比，Cosine距离不仅忽略0-0匹配，而且能够处理非二元向量，即考虑到变量值的大小。
- 
**相关距离**Correlation distance(**'correlation'**)
ds,t=1−xsxt′(xs−xs¯¯¯¯¯)⋅(xs−xs¯¯¯¯¯)′−−−−−−−−−−−−−−−−√⋅(xt−xt¯¯¯¯¯)⋅(xt−xt¯¯¯¯¯)′−−−−−−−−−−−−−−−−√ds,t=1−xsxt′(xs−xs¯)⋅(xs−xs¯)′⋅(xt−xt¯)⋅(xt−xt¯)′
	   Correlation距离主要用来度量两个向量的线性相关程度。
- 
**汉明距离**Hamming distance(**'hamming'**)
ds,t=(#(xsj≠xtj)n)ds,t=(#(xsj≠xtj)n)
	   两个向量之间的汉明距离的定义为两个向量不同的变量个数所占变量总数的百分比。
- 
**杰卡德距离**Jaccard distance(**'jaccard'**)
ds,t=(#[(xsj≠xtj)∩((xsj≠0)∪(xtj≠0))]#[(xsj≠0)∪(xtj≠0)])ds,t=(#[(xsj≠xtj)∩((xsj≠0)∪(xtj≠0))]#[(xsj≠0)∪(xtj≠0)])
	   Jaccard距离常用来处理仅包含非对称的二元(0-1)属性的对象。很显然，Jaccard距离不关心0-0匹配，而Hamming距离关心0-0匹配。
- 
Spearman distance(**'spearman'**)
ds,t=1−(rs−rs¯¯¯¯)(rt−rt¯¯¯¯)′(rs−rs¯¯¯¯)(rs−rs¯¯¯¯)′−−−−−−−−−−−−−−−√(rt−rt¯¯¯¯)(rt−rt¯¯¯¯)′−−−−−−−−−−−−−−√ds,t=1−(rs−rs¯)(rt−rt¯)′(rs−rs¯)(rs−rs¯)′(rt−rt¯)(rt−rt¯)′
	where
	- 
*rsj* is the rank of *xsj* taken over *x1j*, *x2j*, ...*xmj*, as computed by tiedrank
- 
*rs* and *rt* are the coordinate-wise rank vectors of *xs* and *xt*, i.e., *rs* = (*rs1*, *rs2*, ... *rsn*)
- 
rs¯¯¯¯=1n∑jrsj=n+12rs¯=1n∑jrsj=n+12
- 
rt¯¯¯¯=1n∑jrtj=n+12rt¯=1n∑jrtj=n+12
**二、pdist2**
Pairwise distance between two sets of observations
Syntax
D = pdist2(X,Y)
D = pdist2(X,Y,distance)
D = pdist2(X,Y,'minkowski',P)
D = pdist2(X,Y,'mahalanobis',C)
D = pdist2(X,Y,distance,'Smallest',K)
D = pdist2(X,Y,distance,'Largest',K)
[D,I] = pdist2(X,Y,distance,'Smallest',K)
[D,I] = pdist2(X,Y,distance,'Largest',K)
Description
这里 X 是 mx-by-n 维矩阵，Y 是 my-by-n 维矩阵，生成 mx-by-my 维距离矩阵 D。
[D,I] = pdist2(X,Y,distance,'Smallest',K)    生成 K-by-my 维矩阵 D 和同维矩阵 I，其中D的每列是原距离矩阵中最小的元素，按从小到大排列，I 中对应的列即为其索引号。注意，这里每列各自独立地取 K 个最小值。
   例如，令原mx-by-my 维距离矩阵为A，则 K-by-my 维矩阵 D 满足 D(:,j)=A(I(:,j),j).
