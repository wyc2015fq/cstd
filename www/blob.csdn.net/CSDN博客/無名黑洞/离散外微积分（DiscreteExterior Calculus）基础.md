# 离散外微积分（DiscreteExterior Calculus）基础 - 無名黑洞 - CSDN博客
2011年10月08日 02:47:32[wangxiaojun911](https://me.csdn.net/wangxiaojun911)阅读数：3527
“若人们不相信数学简单，只因为他们未意识到生命之复杂。”
——Johnvon Neumann
DEC主要讨论离散情况下的外积分，它在计算机领域有重要用途。我们知道，使用计算机来处理几何图形的时候是不可能完全光滑的（计算机是只有0和1组成的离散化世界），利用DEC的概念也给我们提供了一种刻画离散几何的更好的工具。比如在几何分析中常用的“有限元分析(Finite Element Method)”中使用基于DEC的方法可以使用未uniform的曲面，更加方便简单。
**外代数(Exterior Algebra)**
首先谈谈外微积分（Exterior Calculus），它也叫外代数。
外代数是一类关于外积分(Exterior Product)的代数，外积分也叫楔形积分(Wedge Product)，因为它的运算符号是楔形的。传统的线性代数涉及代数加法和标量乘法，但是不足以满足实际需求，于是定义了具有特色的外积分和内积分。
外积有独特的运算法则，例如在矩阵乘法中:
A = [a1, a2, a3, a4]^T
B = [b1, b2, b3]
C是A和B的外积，则
C = [b1a1, b2a1,b3a1]
      [b1a2, b2a2, b3a2]
      [b1a3, b2a3, b3a3]
      [b1a4, b2a4, b3a4]
外积分还有其他的特点及性质，可以参阅相关教科书。外积分在不同的情况中有很多不同的用途，所以它就独自成为了一个体系，叫外代数。
虽然有外代数，但是没有内代数，只有内积，它们常常如下定义：
A = [a1, a2, a3,]^T
B = [b1, b2, b3]
C是A和B的内积，则
C = a1 * b1 + a2 * b2 + a3 * b3
**DEC的微分形态(Differential Form)**
DEC主要讨论离散情况下的外积分，它的微分形态如下：
0 形态：表示该积分在标量场(Scalar Field)中。
1 形态：表示该积分在一维场中，比如向量场(Vector Field)。
2 形态：与1形态相同。
3 形态：与0形态相同。
**牛顿-莱布尼兹法则（Newton-LeibnitzFormula）**
也叫First Fundamental theorem，是求解积分的一种常用方法。在1形态下有：
Int(a,b)(dF) = int(a,b)(f(x)dx) = F(b) –F(a)
**单纯形(Simplex)和Chain**
首先解释凸壳（Convex Hull, 也叫Convex Envelope）。给定许多点，凸壳是一个在向量空间的点集（Set），这个点集包含了所有的点而且是最小的（Minimal）。
Simplex是DEC的基本单元，有以下几种类型：
0-Simplex：点。
1-Simplex：边，或两个不同点的凸壳。
2-Simplex：三角形，或三个不同点的凸壳。
3-Simplex：四面体，或四个不同点的凸壳。
到这里我们有一个有趣的发现，一个n-Simplex必定有n+1个点组成。并且一个n-Simplex里面也包含了更小的Simplex。比如，一个三角形是2-Simplex，共有3个点，如果取其中两个点组成一个新的1-Simplex, 它也叫2-Simplex的一个1-面(1-Face)。很明显，三角形有3个0-Face，3个1-Face和1个2-Face（它自己）。
所以当我们考虑某一个n-Simplex时，要想到它其实也是许许多多更小的Simplex。
一个n-Simplex拥有很多(n-1)-Face。所有的(n-1)-Face可以组成一个(n-1)-Chain. 这所有的(n-1)-Face也可以合起来叫这个n-Simplex的(n-1)-Face。
**单纯复合体(Simplicial Complex)**
SimplicialComplex是很多Simplex的集合。这个集合中维度最高的Simplex的维度就是Simplicial Complex的维度。比如，一个2维SimplicialComplex中必须存在至少一个2-Simplex，也就是三角形，但是不能存在任何更高维度的Simplex例如四面体。
假设K是一个Simplicial Complex，它必定满足以下条件：
1、这个集合中的所有的Simplex的任何一个面都必须也同时在K中。换句话说，没有破损的Simplex；
2、集合中如果任意两个Simplex存在交集的话，这个交集必须是一个公共面而不是别的什么。也就是说，一个2维Simplicial Complex中如果存在两个三角形，这两个三角形要么不相接，如果相接只能是顶点到顶点的连接，不能其中一个的顶点直接安在另外一个的边上。
所以Simplicial Complex是一个几何完整并且合理拼接的图形，它有利于我们直接分析它的点、边、面的关系，还原它的拓扑结构。只要我们找到了一个分析方法，任何能化简为单纯复合体的复杂物体都可以适用。
刚才提到过Chain其实是一种有方向性的Simplicial Complex.
**离散流形（Discrete Manifold）**
也叫做Simplicial Manifold。一个n维的Simplicial Manifold就是一个n维的Simplicial Complex，另外还满足了以下条件：
对于其中的所有的n维Simplex，如果他们是接壤的，则必须构成一个n维“球”(Ball)，或“半球”(Half Ball)如果是边界点。如此的话，每个（n-1)-Simplex就最多与两个n-Simplex接壤。比如，一个三角形是一个2维Simplicial Manifold。如果把三角形中间的面挖去，只剩下一个三角架，就不再是Manifold了，因为每个孤立的点不再能够满足球或半球的定义。
这样的网孔(Mesh)， 也叫Manifold Mesh。通过以上的介绍，可以直观地理解为这种Mesh（通常是3维的）没有孤立的点、线、面，每条线也仅与一（边界时）或两个面（非边界时）接壤。同时也具有Simplicial Complex的所有特点（无破损，限制公共面）。非常纯净、整齐。
**同源(Homology)**
Homology是一个基本拓扑概念，当我们谈到这个概念时，设定它必须是Simplicial Complex. Homology就是用来讨论图形特性（比如相似性）的一种工具。
先看一个例子。给定三个图形：圆形、方形和圆环，我们认为圆形和方形比较相似，而圆环比较特别，因为圆形和方形可以通过变形相互转换，圆环则不可能。
如果两个Simplicial Complex有相似的结构，它们的内部就具有相同的“洞”(Hole)的数量，这两个图形就叫做“同源”(Homology)。Homology提供了研究几何不变性(Invariants)的重要方法。
**边界（Boundary）**
每个n-Simplex都是有方向的，它的方向定义如下：n-Simplex的n+1个(n-1)-Simplex（也可以叫(n-1)-Faces）可以用两种排列次序，其中的任意一种是它的方向。
比如，三角形是2-Simplex，它有3条边（1-Simplex），这三条边既可以顺时针组成这个三角形，也可以逆时针组成这个三角形。
一般而言，为了方便我们在考察一个n-Simplex的时候会假定它只有一个方向（通常是逆时针方向），本系列以后的所有内容都是按这样的规则。
所谓边界(Boundary)，就是指某个n-Simplex对应的(n-1)-Faces中的任何一个Face。如果这条Boundary的方向与规定的方向一致，记为“+”的boundary，用“+1”表示；反之则为“-”的boundary，用“-1”表示。
**边界运算符**
定义运算符P为边界运算符。当此符作用于一个n-Simplex时，取得的结果是其所有正向的边界。
例如：三角形(A,B,C)有边界关系为：A—〉B—〉C—〉A。那么P(A,B,C) = (A,B) + (B,C) + (C,A)。如果是A—〉B—〉C & A—C，则P(A,B,C) = (A,B) + (B,C) - (C,A)。读者可以自行画图理解。
值得指出的是，如果n-Simplex是空集，即0-Simplex，那么P(0-Simplex) = (-1)-Simplex。
前面说到一组Simplex是一个Chain，边界运算符其实是把一个Simplex转化成一个Chain了。如果在Chain上取边界运算，等于把Chain上的每一个Simplex取边界运算然后加起来。有趣的是如果用“+1”“-1”表示边界，那么P(Chain)一定等于零！
因为边界运算符把n-Simplex映射到(n-1)-Simplex空间中，所以可以把它看成一个(N-1)*N的矩阵。这是一个稀疏矩阵，因为边界只于向接壤的部分有关。如果用+1-1表示法，这个矩阵就只含有0, +1和-1三种元素。
参考文献：
MathieuDesbrun Eva Kanso, Yiying Tong, Discrete Differential Forms for ComputationalModeling
