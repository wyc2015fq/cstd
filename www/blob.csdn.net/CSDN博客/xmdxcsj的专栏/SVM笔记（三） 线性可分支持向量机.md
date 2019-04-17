# SVM笔记（三） 线性可分支持向量机 - xmdxcsj的专栏 - CSDN博客





2016年05月26日 22:44:13[xmucas](https://me.csdn.net/xmdxcsj)阅读数：433标签：[svm](https://so.csdn.net/so/search/s.do?q=svm&t=blog)
个人分类：[机器学习](https://blog.csdn.net/xmdxcsj/article/category/3074739)









## 预备

### 适合场景

如果训练输入线性可分，通过应间隔最大化学习得到的线性分类器称为线性可分支持向量机。 

假设特征空间上的训练数据集： 


$T=\{(x_1,y_1),(x_2,y_2),...,(x_N,y_N)\}$

其中$x_i$表示第i个特征向量，$y_i\in\{+1,-1\}$为$x_i$的类标记。 

学习目标是在特征空间找到一个分离超平面： 


$w\cdot x+b=0$

可以将正负样例分开，即正负样例分布在超平面的两侧。
### 函数间隔和几何间隔

定义超平面关于训练数据集T的函数间隔为超平面$(w,b)$关于T中所有样本点的函数间隔最小值 


$\gamma'=\min_{1,2,...,N}\gamma'_i $

其中$\gamma'_i=y_i(w\cdot x_i+b)$

定义超平面关于训练数据集T的几何间隔为超平面$(w,b)$关于T中所有样本点的几何间隔最小值 


$\gamma=\min_{1,2,...,N}\gamma_i $

其中$\gamma_i=y_i(\frac{w}{||w||}\cdot x_i+\frac{b}{||w||})$

几何间隔的物理意义是指点到超平面的距离，函数间隔则会随着$w$和$b$成比例的缩放而改变。
## 构建目标函数

目标是找到能够正确划分训练数据集并且几何间隔最大的分离超平面。直观上面理解就是距离分界面最近的距离最大化。可以表示成如下形式： 


$\max_{w,b}\gamma$


$s.t. y_i(\frac{w}{||w||}\cdot x_i+\frac{b}{||w||})\ge\gamma , i=1,2,...,N$

基于几何间隔和函数间隔的关系，上式等价于 


$\max_{w,b}\frac{\gamma'}{||w||}$


$s.t. y_i(w\cdot x_i+b)\ge\gamma' , i=1,2,...,N$

考虑到以$\lambda$等比例改变$w$和$b$，对于上式没有影响，这里选择固定一个$\lambda$使得$\gamma'=1$，那么上式等价于： 


$\min_{w,b}\frac{1}{2}||w||^2$


$s.t. y_i(w\cdot x_i+b)-1\ge 0, i=1,2,...,N$

根据上式的最优解$w^*$和$b^*$可以构建出分离超平面和分类决策函数如下： 


$w^*\cdot x+b^*=0$


$f(x)=sign(w^*\cdot x+b^*)$

训练数据集中的样本点中与分离超平面距离最近的样本点称为支持向量，对应于约束条件中的等号，即 


$y_i(w\cdot x_i+b)-1= 0$

对应$y=\{-1,+1\}$，支持向量分布在两条超平面上面： 


$(w\cdot x_i+b)-1= 0$


$(w\cdot x_i+b)+1= 0$
![Alt text](https://img-blog.csdn.net/20160526225351454)

所以SVM分类中只有支持向量对应的实例在分类中起作用，其他实例点并没有作用。
## 求解的对偶算法

使用拉格朗日对偶性对原始问题求解。首先引入拉格朗日乘子$\alpha_i\ge0,i=1,2,...N$,构建拉格朗日函数: 


$L(w,b,\alpha)=\frac{1}{2}||w||^2-\sum_{i=1}^{N}\alpha_iy_i(w\cdot x_i+b)+\sum_{i=1}^{N}\alpha_i$

其中，$\alpha=(\alpha_1,\alpha_2,...,\alpha_N)^T$称为拉格朗日乘子向量。 

原始问题对应的对偶问题如下： 


$\max_{\alpha}\min_{w,b}L(w,b,\alpha)$

根据$L(w,b,\alpha)$对于$w$和$b$的偏导为0，可将原始问题转化为： 


$\min_{\alpha}\frac{1}{2}\sum_{i=1}^{N}\sum_{j=1}^{N}\alpha_{i}\alpha_{j}y_{i}y_{j}(x_i\cdot x_j)-\sum_{i=1}^{N}\alpha_i$


$\sum_{i=1}^{N}\alpha_iy_i=0$


$\alpha_i\ge0,i=1,2,...,N$

假设$\alpha^*=(\alpha_1^*,\alpha_2^*,...,\alpha_N^*)^T$是上面问题的最优解，那么: 


$w^*=\sum_{i=1}^{N}\alpha_i^*y_ix_i$

选择一个下标j，使得$\alpha_j^*\gt0$，可得： 


$b^*=y_j-\sum_{i=1}^{N}\alpha_i^*y_i(x_i\cdot x_j)$

根据KKT互补条件可知，$\alpha_i^*\gt0$对应的实例为支持向量。






