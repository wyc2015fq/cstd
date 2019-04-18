# 变分推断 variational inference，写给数学系同学的介绍（带计算过程） - 知乎
# 



生成模型，绕不开 VI，这是一个在 ML 届经常出现的词。

**这个，其实只是算积分。**为了给大家节省时间，我给大家读一下。教科书 TLDL。这篇看上去不错： [请解释下variational inference？](https://www.zhihu.com/question/31032863) 这里加入计算过程。 

~~~~~~~~~~~~~~~~~~~~~~~~

我的理解，他们的意思是这样，为了逼近分布 ![p(x)](https://www.zhihu.com/equation?tex=p%28x%29) ：
- 引入一堆 ![p_\theta(x,z)](https://www.zhihu.com/equation?tex=p_%5Ctheta%28x%2Cz%29) ，![q_\theta(x,z)](https://www.zhihu.com/equation?tex=q_%5Ctheta%28x%2Cz%29) 之类。
- 还有一些先验，例如 ![q(z)](https://www.zhihu.com/equation?tex=q%28z%29) 之类。
- 选一个散度，衡量逼近的程度（选哪个散度，实际看心情）。
- 于是，得到一个最优化问题：**通过调节 **![\theta](https://www.zhihu.com/equation?tex=%5Ctheta)** ，最小化散度，就完成了分布的逼近**。
- 这个问题，很难解析解（除非做很多简化）。
- 于是，用 EM / GD / SGD / 二阶方法 等等，总之都是贪心法，暴力求解。

~~~~~~~~~~~~~~~~~~~~~~~~

下面看公式。如果，希望用 ![q_\theta(z)](https://www.zhihu.com/equation?tex=q_%5Ctheta%28z%29) 逼近 ![p(z|x)](https://www.zhihu.com/equation?tex=p%28z%7Cx%29) ，取 KL 散度，优化问题是：

![\mathop{\arg\min}_\theta \mathsf{KL}(q_\theta(z) \parallel p(z|x))](https://www.zhihu.com/equation?tex=%5Cmathop%7B%5Carg%5Cmin%7D_%5Ctheta+%5Cmathsf%7BKL%7D%28q_%5Ctheta%28z%29+%5Cparallel+p%28z%7Cx%29%29)

自然的问题是，为什么不把两项倒过来？因为 KL 散度有缺陷，选散度就是这么写意（倒过来的情况，可以用另一个 Expectation Propagation 方法处理）。

然后有个看上去好像说了什么，实际什么都没有：**最小化 KL 散度 = 最大化 ELBO，即：**

![\mathop{\arg\min}_\theta \mathsf{KL}(q_\theta(z) \parallel p(z|x)) = \mathop{\arg\max}_\theta \mathsf{ELBO(\theta)} = \mathop{\arg\max}_\theta \mathbb{E}_q(\log p(x,z) - \log q_\theta(z))](https://www.zhihu.com/equation?tex=%5Cmathop%7B%5Carg%5Cmin%7D_%5Ctheta+%5Cmathsf%7BKL%7D%28q_%5Ctheta%28z%29+%5Cparallel+p%28z%7Cx%29%29+%3D+%5Cmathop%7B%5Carg%5Cmax%7D_%5Ctheta+%5Cmathsf%7BELBO%28%5Ctheta%29%7D+%3D+%5Cmathop%7B%5Carg%5Cmax%7D_%5Ctheta+%5Cmathbb%7BE%7D_q%28%5Clog+p%28x%2Cz%29+-+%5Clog+q_%5Ctheta%28z%29%29)

~~~~~~~~~~~~~~~~~~~~~~~~

让我们看看这个有多无聊。KL 散度的定义：

![\mathsf{KL}(q \parallel p) = \int q(z) \log q(z) \, dz - \int q(z) \log p(z) \, dz = \mathbb{E}_q(\log q(z)) - \mathbb{E}_q(\log p(z))](https://www.zhihu.com/equation?tex=%5Cmathsf%7BKL%7D%28q+%5Cparallel+p%29+%3D+%5Cint+q%28z%29+%5Clog+q%28z%29+%5C%2C+dz+-+%5Cint+q%28z%29+%5Clog+p%28z%29+%5C%2C+dz+%3D+%5Cmathbb%7BE%7D_q%28%5Clog+q%28z%29%29+-+%5Cmathbb%7BE%7D_q%28%5Clog+p%28z%29%29)

于是：

![\begin{align} \mathsf{KL}(q_\theta(z) \parallel p(z|x)) &= \int q_\theta(z) \log q_\theta(z) \, dz - \int q_\theta(z) \log p(z|x) \, dz \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%5Cmathsf%7BKL%7D%28q_%5Ctheta%28z%29+%5Cparallel+p%28z%7Cx%29%29+%26%3D+%5Cint+q_%5Ctheta%28z%29+%5Clog+q_%5Ctheta%28z%29+%5C%2C+dz+-+%5Cint+q_%5Ctheta%28z%29+%5Clog+p%28z%7Cx%29+%5C%2C+dz+%5Cend%7Balign%7D)

由于：

![\log p(z|x) = \log p(x, z) - \log p(x)](https://www.zhihu.com/equation?tex=%5Clog+p%28z%7Cx%29+%3D+%5Clog+p%28x%2C+z%29+-+%5Clog+p%28x%29)

且这项和 ![\theta](https://www.zhihu.com/equation?tex=%5Ctheta) 无关，可以忽略：

![\int q_\theta(z) \log p(x) \, dz = \log p(x)](https://www.zhihu.com/equation?tex=%5Cint+q_%5Ctheta%28z%29+%5Clog+p%28x%29+%5C%2C+dz+%3D+%5Clog+p%28x%29)

因此，优化过程，等价于：

![\mathop{\arg\min}_\theta \Big[ \int q_\theta(z) \log q_\theta(z) \, dz - \int q_\theta(z) \log p(x,z) \, dz \Big]](https://www.zhihu.com/equation?tex=%5Cmathop%7B%5Carg%5Cmin%7D_%5Ctheta+%5CBig%5B+%5Cint+q_%5Ctheta%28z%29+%5Clog+q_%5Ctheta%28z%29+%5C%2C+dz+-+%5Cint+q_%5Ctheta%28z%29+%5Clog+p%28x%2Cz%29+%5C%2C+dz+%5CBig%5D)

即：

![\mathop{\arg\max}_\theta \mathbb{E}_q(\log p(x,z) - \log q_\theta(z))](https://www.zhihu.com/equation?tex=%5Cmathop%7B%5Carg%5Cmax%7D_%5Ctheta+%5Cmathbb%7BE%7D_q%28%5Clog+p%28x%2Cz%29+-+%5Clog+q_%5Ctheta%28z%29%29)

~~~~~~~~~~~~~~~~~~~~~~~~

再看优化过程，我们需要算：

![\nabla_\theta \mathsf{ELBO(\theta)} = \nabla_\theta \mathbb{E}_q(\log p(x,z) - \log q_\theta(z))](https://www.zhihu.com/equation?tex=%5Cnabla_%5Ctheta+%5Cmathsf%7BELBO%28%5Ctheta%29%7D+%3D+%5Cnabla_%5Ctheta+%5Cmathbb%7BE%7D_q%28%5Clog+p%28x%2Cz%29+-+%5Clog+q_%5Ctheta%28z%29%29)

这个也特别简单，直接带进去就可以算：

![\begin{align} &\quad\;\frac{\partial}{\partial \theta} \int q_\theta(z) \Big (\log p(x,z) - \log q_\theta(z) \Big) \, dz \\&=  \int \frac{\partial}{\partial \theta} \Big[ q_\theta(z) \Big (\log p(x,z) - \log q_\theta(z) \Big) \Big] \, dz \\&= \int \frac{\partial}{\partial \theta} \Big( q_\theta(z) \log p(x,z) \Big) - \frac{\partial}{\partial \theta} \Big( q_\theta(z) \log q_\theta(z) \Big) \, dz \\&= \int \frac{\partial q_\theta(z)}{\partial \theta} \log p(x,z) - \frac{\partial q_\theta(z)}{\partial \theta} \log q_\theta(z) - \frac{\partial q_\theta(z)}{\partial \theta} \, dz \end{align} ](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%26%5Cquad%5C%3B%5Cfrac%7B%5Cpartial%7D%7B%5Cpartial+%5Ctheta%7D+%5Cint+q_%5Ctheta%28z%29+%5CBig+%28%5Clog+p%28x%2Cz%29+-+%5Clog+q_%5Ctheta%28z%29+%5CBig%29+%5C%2C+dz+%5C%5C%26%3D++%5Cint+%5Cfrac%7B%5Cpartial%7D%7B%5Cpartial+%5Ctheta%7D+%5CBig%5B+q_%5Ctheta%28z%29+%5CBig+%28%5Clog+p%28x%2Cz%29+-+%5Clog+q_%5Ctheta%28z%29+%5CBig%29+%5CBig%5D+%5C%2C+dz+%5C%5C%26%3D+%5Cint+%5Cfrac%7B%5Cpartial%7D%7B%5Cpartial+%5Ctheta%7D+%5CBig%28+q_%5Ctheta%28z%29+%5Clog+p%28x%2Cz%29+%5CBig%29+-+%5Cfrac%7B%5Cpartial%7D%7B%5Cpartial+%5Ctheta%7D+%5CBig%28+q_%5Ctheta%28z%29+%5Clog+q_%5Ctheta%28z%29+%5CBig%29+%5C%2C+dz+%5C%5C%26%3D+%5Cint+%5Cfrac%7B%5Cpartial+q_%5Ctheta%28z%29%7D%7B%5Cpartial+%5Ctheta%7D+%5Clog+p%28x%2Cz%29+-+%5Cfrac%7B%5Cpartial+q_%5Ctheta%28z%29%7D%7B%5Cpartial+%5Ctheta%7D+%5Clog+q_%5Ctheta%28z%29+-+%5Cfrac%7B%5Cpartial+q_%5Ctheta%28z%29%7D%7B%5Cpartial+%5Ctheta%7D+%5C%2C+dz+%5Cend%7Balign%7D+)

由于：

![\int \frac{\partial q_\theta(z)}{\partial \theta} \, dz = \frac{\partial }{\partial \theta} \int q_\theta(z) \, dz =  \frac{\partial }{\partial \theta} 1 = 0](https://www.zhihu.com/equation?tex=%5Cint+%5Cfrac%7B%5Cpartial+q_%5Ctheta%28z%29%7D%7B%5Cpartial+%5Ctheta%7D+%5C%2C+dz+%3D+%5Cfrac%7B%5Cpartial+%7D%7B%5Cpartial+%5Ctheta%7D+%5Cint+q_%5Ctheta%28z%29+%5C%2C+dz+%3D++%5Cfrac%7B%5Cpartial+%7D%7B%5Cpartial+%5Ctheta%7D+1+%3D+0)

因此：

![\begin{align} \nabla_\theta \mathsf{ELBO(\theta)} &= \int \frac{\partial q_\theta(z)}{\partial \theta} \Big( \log p(x,z) - \log q_\theta(z) \Big) \,dz  \\&= \int q_\theta(z) \frac{\partial \log q_\theta(z)}{\partial \theta} \Big( \log p(x,z) - \log q_\theta(z) \Big) \,dz  \\&= \int q_\theta(z) \nabla_\theta \log q_\theta(z) \Big( \log p(x,z) - \log q_\theta(z) \Big) \,dz \\&= \mathbb{E}_q [ \nabla_\theta \log q_\theta(z)  (\log p(x,z) - \log q_\theta(z)) ] \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%5Cnabla_%5Ctheta+%5Cmathsf%7BELBO%28%5Ctheta%29%7D+%26%3D+%5Cint+%5Cfrac%7B%5Cpartial+q_%5Ctheta%28z%29%7D%7B%5Cpartial+%5Ctheta%7D+%5CBig%28+%5Clog+p%28x%2Cz%29+-+%5Clog+q_%5Ctheta%28z%29+%5CBig%29+%5C%2Cdz++%5C%5C%26%3D+%5Cint+q_%5Ctheta%28z%29+%5Cfrac%7B%5Cpartial+%5Clog+q_%5Ctheta%28z%29%7D%7B%5Cpartial+%5Ctheta%7D+%5CBig%28+%5Clog+p%28x%2Cz%29+-+%5Clog+q_%5Ctheta%28z%29+%5CBig%29+%5C%2Cdz++%5C%5C%26%3D+%5Cint+q_%5Ctheta%28z%29+%5Cnabla_%5Ctheta+%5Clog+q_%5Ctheta%28z%29+%5CBig%28+%5Clog+p%28x%2Cz%29+-+%5Clog+q_%5Ctheta%28z%29+%5CBig%29+%5C%2Cdz+%5C%5C%26%3D+%5Cmathbb%7BE%7D_q+%5B+%5Cnabla_%5Ctheta+%5Clog+q_%5Ctheta%28z%29++%28%5Clog+p%28x%2Cz%29+-+%5Clog+q_%5Ctheta%28z%29%29+%5D+%5Cend%7Balign%7D)




然后写成 SGD，就是所谓 Black Box Variational Inference (BBVI)。

![\mathbb{E}_{z \sim q_\theta(z)} [ \nabla_\theta \log q_\theta(z)  (\log p(x,z) - \log q_\theta(z)) ] \\ \approx \frac{1}{N} \sum_{i = 1}^N  \nabla_\theta \log q_\theta(z_i)  (\log p(x,z_i) - \log q_\theta(z_i)) ](https://www.zhihu.com/equation?tex=%5Cmathbb%7BE%7D_%7Bz+%5Csim+q_%5Ctheta%28z%29%7D+%5B+%5Cnabla_%5Ctheta+%5Clog+q_%5Ctheta%28z%29++%28%5Clog+p%28x%2Cz%29+-+%5Clog+q_%5Ctheta%28z%29%29+%5D+%5C%5C+%5Capprox+%5Cfrac%7B1%7D%7BN%7D+%5Csum_%7Bi+%3D+1%7D%5EN++%5Cnabla_%5Ctheta+%5Clog+q_%5Ctheta%28z_i%29++%28%5Clog+p%28x%2Cz_i%29+-+%5Clog+q_%5Ctheta%28z_i%29%29+)

其中 ![z_i \sim q_\theta(z)](https://www.zhihu.com/equation?tex=z_i+%5Csim+q_%5Ctheta%28z%29) 。
![](https://pic1.zhimg.com/v2-da1f6927801cc20830f442689db75760_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='450' height='353'></svg>)
后面还有很多提高收敛速度的方法，有空时慢慢补充。

~~~~~~~~~~~~~~~~~~~~~~~~

注意，这个过程需要 ![p(x,z)](https://www.zhihu.com/equation?tex=p%28x%2Cz%29) 。

我认为，这里的关键是，这里的 ![p(x,z)](https://www.zhihu.com/equation?tex=p%28x%2Cz%29) 可以是未归一化的。

所以说，对于 partition function 的逼近，是真正的核心问题，这还有很多其它的方法。


