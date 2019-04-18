# 概率神经网络实现Parzen窗方法 - 这是个无聊的世界 - CSDN博客





2016年05月24日 17:10:00[lancelot_vim](https://me.csdn.net/lancelot_vim)阅读数：1077








# 概率神经网络实现Parzen窗方法

标签（空格分隔）： 模式分类

假设我们要实现一个Parzen估计，共有n个d维度的样本，都是随机从c个类别中选取的，这种情况下，输入层由d个输入单元组成，每个输入单元都与n个模式单元相连。每一个模式单元都与c个类别中的其中之一相连。 

从输入层到模式层的连线表示为可以修改的权系数，这些系数都可以通过训练得到。这些权重可以用一个向量$\theta$表示，但为了与神经网络术语保持一致，统一改用向量w来表示，而一个类别单元都计算与之相连的各模式单元的输出结果和。

首先我们先把样本$x$进行归一化处理，即$\sum\limits_i^dx_i^2=1$,经过归一化后的样本置于输入层单元上。同时，连接输入单元和第一个模式层单元的那些连接被初始化为$w_1 = x_1$，然后从模式层的第一个单元到类别层中代表$x_1$的那个类别建立了一个连接，如图所示

![PNN初始化.png-93.4kB](http://static.zybuluo.com/lancelot-vim/85k9xc8gslqsu2a2iyq0wj7e/PNN%E5%88%9D%E5%A7%8B%E5%8C%96.png)

如此，当n个随机样本遍历完成之后，可以得到一个输入层到模式层的全连接网络，和一个模式层到类别层的一个稀疏连接网络

> 
PNN训练算法 

  begin initialize j <- 0, n, $a_{ji}$ <- 0, j = 1, … n; i = 1, …, c 
$\quad $do j <- j + 1 
$\qquad x_{jk}$ <- $x_{jk}/(\sum\limits_i^dx^2)^{1/2}$
$\qquad w_{jk}$ <- $x{jk}$
$\qquad$ if $x \in \omega_i$ then $a_{ji}$ <- 1 
$\quad$ until j = n 

  end
训练完成之后，对于测试样本，首先对测试样本进行归一化处理，然后供给输入节点，之后计算”净激活(net activation)” 


$ net_k = w_k^Tx $

然后把净激活输入一个非线性函数$\exp(\frac{net_k - 1}{\sigma^2})$, 其中$\sigma$为用户设置的参数，表示高斯窗的有效高度，最后再把这些估计和与相连接的权重相乘相加得到类别层的估计
> 
PNN分类算法 

  begin initialize k <- 0, x<-测试数据 
$\quad$ do k <- k + 1 
$\qquad net_k$ <- $w_k^Tx$
$\qquad $ if $a_{ki} = 1$ then $g_i$ <- $g_i + \exp((net_k - 1)/\sigma^2)$
$\qquad $ until k = n 
$\quad $return class <- $argmax (g_i)$

  end
PNN的好处之一就是学习速度很快，因为规则简单，而且新的训练样本容易被加入以前训练好的分类器中，但是它的关键问题实际上和Parzen窗一样，就是选取体积序列（在这里是选取$\sigma$），如果过大，会导致平滑效果太过明显，太小会导致变化特别剧烈，分类效果都不会太好，实际上可以使用交叉验证方法来确定一个比较恰当的$\sigma$值








