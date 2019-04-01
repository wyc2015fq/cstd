# 语音的线性预测系数（Linear Prediction Coefficient，LPC）

2017年01月06日 11:05:42 [Lebronze](https://me.csdn.net/Lebronze) 阅读数：7392



 版权声明：本文为博主原创文章，未经博主允许不得转载。

​	https://blog.csdn.net/Lebronze/article/details/54135664

线性预测技术很早（1967年）就已经被应用与语音处理领域，**基本概念**是：一个语音的采样值可以通过过去若干语音采样值的线性组合来逼近（最小均方误差），能够决定唯一的一组预测系数，而这个**预测系数**就是题目中的LPC，可以当作是该语音的一个特征。

## 1.优势

线性预测最重要的优势在于其可以较为精确的估计语音的参数，而这些极少的参数可以正确的表现语音信号的时域和频域特性。

## 2.基本原理

线性预测分析的基本原理是把信号用一个模型来表示，即将信号看作某一个模型（系统）的输出，这样就可以用该模型的参数来描述信号。

假设模型的输入为u(n),输出为x(n)，模型传递函数为H(z)，则求模型的参数则可用传递函数的方法来求解，即： 
传递函数：![频域方程](https://img-blog.csdn.net/20170106111902486?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTGVicm9uemU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

对应到时域： 
![时域关系](https://img-blog.csdn.net/20170106112409880?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTGVicm9uemU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 
上式是一个线性常系数差分方程，表示模型的输出是由模型过去的输入u(n-l)、当前的输入u(n)和过去输入x(n-i)的线性组合。当模型参数确定后就可以用模型过去的输入以及过去的信号值来估计当前的信号值。 
而为了方便求解，我们对模型进行了简化，采用了自回归模型：当前输出=当前输入+过去输出进行线性组合： 
![自回归模型](https://img-blog.csdn.net/20170106112510192?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTGVicm9uemU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 3.求解方法

由于上面已经得到了模型的差分方程，接下来我们就是想如何求解这个方程，考虑使用下面的现行预测器来估计： 
![这里写图片描述](https://img-blog.csdn.net/20170106114310560?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTGVicm9uemU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 
其中x^是原始信号x的估计值，是由过去的p个输出线性组合而成，其中ai即为线性预测系数（LPC）。所以我们只要保证预测信号与原始信号误差（最小均方误差）最小，来求解出系数ai即可。 
经过对误差函数的一系列变形我们可以得到： 
![这里写图片描述](https://img-blog.csdn.net/20170106170519315?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTGVicm9uemU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) ……………..(a) 
其中![这里写图片描述](https://img-blog.csdn.net/20170106154156355?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTGVicm9uemU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

所以只要求出![这里写图片描述](https://img-blog.csdn.net/20170106154336658?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTGVicm9uemU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，就可解出系数ai。

又由自相关的定义，我们可以得到 
![这里写图片描述](https://img-blog.csdn.net/20170106171248450?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTGVicm9uemU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 
所以可以联想到![这里写图片描述](https://img-blog.csdn.net/20170106171314287?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTGVicm9uemU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)可以得出： 
![这里写图片描述](https://img-blog.csdn.net/20170106171338912?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTGVicm9uemU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 
所以(a)式可以化为 
![这里写图片描述](https://img-blog.csdn.net/20170106171408444?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTGVicm9uemU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 
因为r(j)可以求出，则上式中求解ai即为求解线性方程组， 
求解方法Levinson-Durbin法： 
![这里写图片描述](https://img-blog.csdn.net/20170106152826231?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTGVicm9uemU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
重复上述步骤即可得到系数![这里写图片描述](https://img-blog.csdn.net/20170106154846609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTGVicm9uemU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 
至此，即求出了语音信号的p阶LPC特征。

文中很多公式、思想来自于北航出版社宋知用老师编著的**《MATLAB在语音分析与合成中的应用》**，十分感谢。