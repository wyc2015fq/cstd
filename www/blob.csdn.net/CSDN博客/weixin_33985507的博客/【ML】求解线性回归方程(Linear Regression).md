# 【ML】求解线性回归方程(Linear Regression) - weixin_33985507的博客 - CSDN博客
2014年11月06日 13:35:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
参考资料：[openclassroom](http://openclassroom.stanford.edu/MainFolder/CoursePage.php?course=MachineLearning)
线性回归(Linear Regression)
为了拟合10岁以下儿童年龄(x1)与身高(y)之间的关系，我们假设一个关于x的函数h(x)：
h(x) = Θ0+Θ1*x1 = Θ0*x0+Θ1*x1 = ΘT*x(其中x0=1, x=[x0, x1])
我们的目的是求出Θ，使得h(x)接近真实的y。
因此我们需要在m个训练样本(x,y)上使得h(x)与y的平方误差最小。
也就是最小化J(Θ) =1/(2*m) * ∑i(h(x(i))-y(i))2
分母上2的作用是抵消求导时平方项产生的2.
解法一：Gradient Descent(梯度下降)
Θ朝着J(Θ)的梯度方向（即J(Θ)关于Θ的偏导）前进，直到J(Θ)达到极小点（线性回归中J(Θ)为碗状，极小点即最小点）
α为步长，由于J(Θ)关于Θ的偏导会逐渐变小，因此α无需调整。
同时执行以下两个更新公式，直到收敛。
注意：同时执行。而不是求出一个代入另一个的迭代执行。
Θ0 = Θ0-α/m*∑i(h(x(i))-y(i))x0(i)
Θ1 = Θ1-α/m*∑i(h(x(i))-y(i))x1(i)
解法二：Normal Equations
J(Θ)关于Θ求导为0，联列方程组求解得：
Θ = (XTX)-1XTY (其中X的行向量为x(i)，Y每个元素为y(i))
注意：(XTX)-1不一定有意义
case 1: 每个x(i)样本的维度为n。当m <= n时，XTX 非满秩，为奇异矩阵，无逆元。
case 2: x(i)特征线性相关，即X列向量线性相关时，XTX 非满秩，为奇异矩阵，无逆元。
