# 最优化方法：梯度下降法、SGD - wishchinYang的专栏 - CSDN博客
2016年04月01日 16:04:54[wishchin](https://me.csdn.net/wishchin)阅读数：2633
**前言：**
       机器学习从目标函数到模型构建到特征提取，需要模型依据目标函数约束，根据回归方式进行调整数学模型，最基本最常用的一种方法是梯度下降法，以梯度下降为指导准则，优化目标函数到最优解。**  原文地址：**[**再谈最速/梯度下降法**](https://www.codelast.com/%E5%8E%9F%E5%88%9B-%E5%86%8D%E8%B0%88-%E6%9C%80%E9%80%9F%E4%B8%8B%E9%99%8D%E6%B3%95%E6%A2%AF%E5%BA%A6%E6%B3%95steepest-descent/)
**一、算法过程**
最速下降法（又称梯度法，或Steepest Descent），是无约束[最优化](http://zh.wikipedia.org/wiki/%E6%9C%80%E4%BC%98%E5%8C%96)领域中最简单的算法，单独就这种算法来看，属于早就“过时”了的一种算法。但是，它的理念是其他某些算法的组成部分，或者说是在其他某些算法中，也有最速下降法的“影子”。因此，我们还是有必要学习一下的。
我很久以前已经写过一篇关于最速下降法的文章了，但是这里我还打算再写一篇，提供更多一些信息，让大家可以从更简单生动的方面去理解它。
『1』名字释义
最速下降法只使用目标函数的一阶导数信息——从“梯度法”这个名字也可见一斑。并且，它的本意是取目标函数值“最快下降”的方向作为搜索方向。于是我们就想知道这个问题的答案：沿什么方向，目标函数 的值下降最快呢？
『2』函数值下降最快的方向
先说结论：沿负梯度方向   ，函数值下降最快。
下面就来推导一下。
将目标函数  在点  处泰勒展开（这是我们惯用的“伎俩”了）——
高阶无穷小  可忽略，由于我们定义了步长 0 ，因此，当 0 时，  ，即函数值是下降的。此时  就是一个下降方向。
但是  具体等于什么的时候，可使目标函数值下降最快呢？
文章来源：[http://www.codelast.com/](http://www.codelast.com/)
由[Cauchy-Schwartz不等式（柯西-许瓦兹不等式）](http://www.codelast.com/?p=8022)可得：
当且仅当  时，等号成立，  最大（>0）。
所以  时，  最小（<0），  下降量最大。
所以  是最快速下降方向。
『3』缺点
它真的“最快速”吗？答案是否定的。
事实是，它只在局部范围内具有“最速”性质。
对整体求解过程而言，它的下降非常缓慢。
『4』感受一下它是如何“慢”的
先来看一幅图（直接从维基百科上弄过来的，感谢Wiki）：
![](http://www.codelast.com/wp-content/uploads/ckfinder/images/Rosenbrock_function.png)
文章来源：[http://www.codelast.com/](http://www.codelast.com/)
这幅图表示的是对一个目标函数的寻优过程，图中锯齿状的路线就是寻优路线在二维平面上的投影。
这个函数的表达式是：
121121002122
它叫做Rosenbrock function（罗森布罗克方程），是个非凸函数，在最优化领域，它通常被用来作为一个最优化算法的performance test函数。
我们来看一看它在三维空间中的图形：
![Rosenbrock function 3D](http://www.codelast.com/wp-content/uploads/ckfinder/images/Rosenbrock_function_3d.jpg)
文章来源：[http://www.codelast.com/](http://www.codelast.com/)
它的全局最优点位于一个长长的、狭窄的、抛物线形状的、扁平的“山谷”中。
找到“山谷”并不难，难的是收敛到全局最优解（全局最优解在 (1,1) 处）。
正所谓：世界上最遥远的距离，不是你离我千山万水，而是你就在我眼前，我却要跨越千万步，才能找到你。
文章来源：[http://www.codelast.com/](http://www.codelast.com/)
我们再来看另一个目标函数 122142321 的寻优过程：
![](http://www.codelast.com/wp-content/uploads/ckfinder/images/function_find_opt_process.jpg)
和前面的Rosenbrock function一样，它的寻优过程也是“锯齿状”的。
它在三维空间中的图形是这样的：
![](http://www.codelast.com/wp-content/uploads/ckfinder/images/function_find_opt_process_3d.jpg)
总而言之就是：当目标函数的等值线接近于圆(球)时，下降较快；等值线类似于扁长的椭球时，一开始快，后来很慢。
文章来源：[http://www.codelast.com/](http://www.codelast.com/)
『5』为什么“慢”的分析
上面花花绿绿的图确实很好看，我们看到了那些寻优过程有多么“惨烈”——太艰辛了不是么？
但不能光看热闹，还要分析一下——为什么会这样呢？
由[精确line search满足的一阶必要条件](http://www.codelast.com/?p=7838)，得：
0 ，即 10
故由最速下降法的  得：
1111010
即：相邻两次的搜索方向是相互直交的（投影到二维平面上，就是锯齿形状了）。
文章来源：[http://www.codelast.com/](http://www.codelast.com/)
如果你非要问，为什么 10 就表明这两个向量是相互直交的？那么我就耐心地再解释一下：
由两向量夹角的公式：
![](http://www.codelast.com/wp-content/uploads/ckfinder/images/two_vector_angle.png)
=>2
两向量夹角为90度，因此它们直交。
『6』优点
这个被我们说得一无是处的最速下降法真的就那么糟糕吗？其实它还是有优点的：程序简单，计算量小；并且对初始点没有特别的要求；此外，许多算法的初始/再开始方向都是最速下降方向（即负梯度方向）。
文章来源：[http://www.codelast.com/](http://www.codelast.com/)
『7』收敛性及收敛速度
最速下降法具有整体收敛性——对初始点没有特殊要求。
采用[精确线搜索](http://www.codelast.com/?p=2348)的最速下降法的收敛速度：线性。
**二、梯度下降算法的代码**
```cpp
//梯度下降法
float gsdFindArc(std::vector<cv::Point2f>  & inlierPs,cv::Point2f  ¢er, float radius)
{
	//弧的残差函数为 f = A - 2nXiX - 2nYiY
	
	//double matrix[4][2]={{1,4},{2,5},{5,1},{4,2}};
	Eigen::MatrixXf  M(inlierPs.size(),2);
	for (int i=0;i< M.rows();++i )
	{
		M(i,0) = inlierPs[i].x;
		M(i,1) = inlierPs[i].y;
	}
	//初始化三个优化参数
	std::vector<float >  xi(3);
	xi[0] = center.x;
	xi[1] = center.y;
	xi[2] =   radius;
	//初始化result
	//double result[4]={19,26,19,20};
	std::vector<float >  result(inlierPs.size() );
	float r =  xi[2];
	for (int i=0;i< M.rows();++i )
	{
		float x = M(i,0);
		float y = M(i,1);
		result[i] = x*x + y*y - 2*x*xi[0] - 2*y*xi[1] + xi[0]*xi[0] + xi[1]*xi[1] - r*r;
	}
	//double      w[2]={0,0};//初始为零向量
	double    w[3] = {0,0,0};
	double    loss =    10.0;
	const double n = 0.01; //步长 
	int numIter = 100*inlierPs.size();
	for(int i=0;i< numIter && loss>0.001; i++)
	{
		double error_sum=0;
		int j = i % inlierPs.size();
		{ 
			double h = 0;
			for(int k=0; k<xi.size() ; k++)
				h += M(j,k)* w[k];
			error_sum = h - result[j];
			for(int k=0; k<xi.size(); k++)
				w[k] -= n * (error_sum) * M(j,k);//更新权值,权值更新过程为整个关键过程
		}
		double loss=0;
		for(int j=0; j< M.rows() ;j++)
		{
			double sum = 0;
			for( int k=0; k<xi.size() ; k++)
				sum += M(j,k) * w[k];
			loss += (sum - result[j]) * (sum-result[j]);
		}
		std::cout<< "Loss!"<< loss << std::endl;
	}
	return 1.0;
}
```
**延伸**
      GD方法，从复杂方程解的高原区向梯度的负方向进行迭代，遇到了鞍点该怎么走，走错了怎么办？随机走下去吗？随机走下去的恰好是局部最优又怎么办？
       还有 一个，GD算法的学习率一般不能太大，必须小步小步，否则极易落入局部最优解，再也走不出来或者在局部最优解处反复震荡。
![](https://img-blog.csdn.net/20180604144115284)
     这就引入了SGD。两大缺陷竟然可以用同一个方法解决, 就是Stochastic Gradient Descent (SGD) 算法. 
SGD 算法的表达式和GD差不多:
![x_{t+1}=x_t-\eta_t g_t](https://www.zhihu.com/equation?tex=x_%7Bt%2B1%7D%3Dx_t-%5Ceta_t+g_t)
这里 ![g_t](https://www.zhihu.com/equation?tex=g_t+)就是所谓的Stochastic Gradient，它满足 ![E[g_t]=\nabla f(x_t)](https://www.zhihu.com/equation?tex=E%5Bg_t%5D%3D%5Cnabla+f%28x_t%29)
也就是说，虽然包含一定的随机性，但是从期望上来看，它是等于正确的导数的．用一张图来表示，其实SGD就像是喝醉了酒的GD，它依稀认得路，最后也能自己走回家，但是走得歪歪扭扭．（红色的是GD的路线，偏粉红的是SGD的路线）．
![](https://img-blog.csdn.net/20180604144658829)
仔细看的话，其实SGD需要更多步才能够收敛的，毕竟它喝醉了．可是，由于它对导数的要求非常低，可以包含大量的噪声，只要期望正确就行（有时候期望不对都是可以的．．），所以导数算起来**非常快**．
SGD的引入带来ML界一个非常本质的变化，训练模型开始依赖经验怎样指导去调参，越深的模型越像在炼丹.....
