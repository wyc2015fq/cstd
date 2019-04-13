
# 机器学习之EM算法 - Magician的博客 - CSDN博客


2018年09月17日 20:34:16[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：174所属专栏：[自学机器学习之路](https://blog.csdn.net/column/details/26597.html)




### 文章目录
[1、知道先验概率和后验概率](#1_1)
[2、了解高斯混合模型GMM](#2GMM_5)
[3、通过最大似然估计推导EM算法的过程的实例](#3EM_16)
[4、EM算法](#4EM_39)
[5、知道pLSA模型](#5pLSA_53)

# 1、知道先验概率和后验概率
先验概率（prior probability）是指根据以往经验和分析得到的概率（理解为自定义概率）。而后验概率是在考虑了一个事实之后的条件概率。
# 2、了解高斯混合模型GMM
EM是K-means的推广
以下的两个问题都是属于无监督学习（**对于不知道样本数的问题，采用EM算法**）
![EM算法场景](https://img-blog.csdn.net/20180917200413642?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
高斯混合模型GMM：随机变量x由k个高斯分布混合而成。
GMM参数估计的理解（相当于由变量x的一部分样本进而去估计对应的x发生的概率以及均值μ和方差）
![在这里插入图片描述](https://img-blog.csdn.net/20180917200642346?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![在这里插入图片描述](https://img-blog.csdn.net/20180917200730242?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 3、通过最大似然估计推导EM算法的过程的实例
![在这里插入图片描述](https://img-blog.csdn.net/20180917200743972?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![在这里插入图片描述](https://img-blog.csdn.net/20180917201258863?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
首先知道最大似然估计：多个事件同事发生的概率。
**由身高体重推测男女的例子（参考下图理解）**
1、先对给定的一组数据Xi假设初始自定义的**均值μ和方差σ**，以及对应的**分类概率**（即使男性、女性的概率是多大）
2、代入对应的高斯密度函数得到对应的**概率密度值**
3、由对应的概率密度值以及对应的分类概率求的**条件概率**（即如1.88是男性的概率是多少）
4、重复以上步骤**求其他组对应的条件概率的值**。（高斯混合分布）
5、将所有组得到的数据做条件概率的值乘以数据的操作（如1.88乘以0.67）得到整个样本对应的均值μ和方差σ（注意计算均值时对应的除数N代表的是条件概率0.67等的加和）
6、将计算得到的均值和方差不断的迭代，直到稳定为止。
![在这里插入图片描述](https://img-blog.csdn.net/2018091720151992?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![在这里插入图片描述](https://img-blog.csdn.net/20180917201610439?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**不断迭代最终直到μ和σ达到稳定值**
![在这里插入图片描述](https://img-blog.csdn.net/20180917201742383?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
下图中所说的概率的加和代表的是0.67+0.4+…即对应身高是男性的概率得到最终的N男
![在这里插入图片描述](https://img-blog.csdn.net/20180917201919668?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**掌握每个组份的参数公式对应的含义**
![在这里插入图片描述](https://img-blog.csdn.net/20180917202009262?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 4、EM算法
**x对应的是数据，z代表的是隐变量如类别（男、女性）。**
![在这里插入图片描述](https://img-blog.csdn.net/20180917202155587?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
θ代表未知变量如π、μ、σ
除了想估计x还想估计z（这个z代表的是隐随机变量，p代表估计1.88是男性的概率；x代表的是1.88，男性代表的z）
![在这里插入图片描述](https://img-blog.csdn.net/20180917202214243?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
整个过程其实就由求f(θ)转化为求r函数（**下界函数**）的极值（减小了参数个数），直到r函数极值等于f函数的值。
当θ0处两个函数相等，这时候求r函数的极值处θ1，这时候若r函数小，则再固定z选取一条新的函数r1函数在θ1处等于p函数，接着再求r1函数的极值…以此类推。（固定θ找z，再固定z找θ依次类推直到找到稳定的值）
![在这里插入图片描述](https://img-blog.csdn.net/20180917202329318?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
E步骤：Q对应于实例已知θ求1.88条件下是男性的条件概率的值。
M步骤：θ代表的加权后求得的均值μ和方差σ。
整个过程就相当于固定θ（初始给定的值）求Q（Q代表对应的1.88条件下是男性的概率），固定Q（知道这组数据对应的条件概率的值）求θ（求的对应的μ和σ2）…不断重复直到结果稳定（Q可以看做是对应的下界函数）
![在这里插入图片描述](https://img-blog.csdn.net/20180917202511577?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 5、知道pLSA模型
![在这里插入图片描述](https://img-blog.csdn.net/20180917202735662?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![在这里插入图片描述](https://img-blog.csdn.net/20180917202745554?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![在这里插入图片描述](https://img-blog.csdn.net/20180917202802707?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![在这里插入图片描述](https://img-blog.csdn.net/20180917202817229?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![在这里插入图片描述](https://img-blog.csdn.net/2018091720282542?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![在这里插入图片描述](https://img-blog.csdn.net/20180917203628221?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![在这里插入图片描述](https://img-blog.csdn.net/20180917202847410?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
[
						](https://img-blog.csdn.net/20180917203628221?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
[
	](https://img-blog.csdn.net/20180917203628221?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
[
  ](https://img-blog.csdn.net/2018091720282542?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)