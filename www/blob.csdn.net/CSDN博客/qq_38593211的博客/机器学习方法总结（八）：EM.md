# 机器学习方法总结（八）：EM - qq_38593211的博客 - CSDN博客





2018年08月02日 19:35:13[小屁猴多看书](https://me.csdn.net/qq_38593211)阅读数：58








# EM算法（Expectation-Maximum）

## 1.引入

    这个算法的过程我认为李航的统计学习方法讲的很好，所以我就顺着他的思路理一遍然后讲清楚中间的过程。EM算法是一个含有隐变量和观测变量用极大似然估计法估计参数并进行迭代改进参数的生成方法，每次迭代分为两步，E步（Expectation）用极大似然估计得到参数的期望值，M步（Maximum）求出此时参数新的估计值，最大化似然函数。

## 2.例子

      假设有三枚硬币，分别记作A,B,C，这些硬币正面出现的概率为Π、p和q，进行如下实验：先掷硬币A，根据其结果选出硬币B和C，正面选B，反面选C；然后重复掷硬币，记录结果，正面为1，反面为0.独立重复进行十次实验，观测结果如下：

                                                                   1，1，0，1，0，0，0，1，0，1，1

     三硬币模型可写为：

![P(y|\theta )=\sum P(y,z|\theta)=\sum P(z|\theta)P(y|z,\theta)](https://private.codecogs.com/gif.latex?P%28y%7C%5Ctheta%20%29%3D%5Csum%20P%28y%2Cz%7C%5Ctheta%29%3D%5Csum%20P%28z%7C%5Ctheta%29P%28y%7Cz%2C%5Ctheta%29)

![=\pi p^{y}(1-p)^{1-y}+(1-\pi)q^{y}(1-q)^{1-y}](https://private.codecogs.com/gif.latex?%3D%5Cpi%20p%5E%7By%7D%281-p%29%5E%7B1-y%7D&plus;%281-%5Cpi%29q%5E%7By%7D%281-q%29%5E%7B1-y%7D)

    这里的![Y={y_{1}......y_{10}}](https://private.codecogs.com/gif.latex?Y%3D%7By_%7B1%7D......y_%7B10%7D%7D)是观测变量，![Z={z_{1}......z_{10}}](https://private.codecogs.com/gif.latex?Z%3D%7Bz_%7B1%7D......z_%7B10%7D%7D)是隐变量，一般把Y,Z联合在一起叫做完全数据，上述公式按条件概率和联合概率公式得出的，![\theta= {\pi,p,q}](https://private.codecogs.com/gif.latex?%5Ctheta%3D%20%7B%5Cpi%2Cp%2Cq%7D)是模型的参数。那观测数据的似然函数是否可以写出来呢？（就是对于每个y的概率的乘积最大嘛）：

![P(Y|\theta)=\prod_{j=1}^{10}\pi p^{y_{j}}(1-p)^{1-y_{j}}+(1-\pi)q^{y_{j}}(1-q)^{1-y_{j}}](https://private.codecogs.com/gif.latex?P%28Y%7C%5Ctheta%29%3D%5Cprod_%7Bj%3D1%7D%5E%7B10%7D%5Cpi%20p%5E%7By_%7Bj%7D%7D%281-p%29%5E%7B1-y_%7Bj%7D%7D&plus;%281-%5Cpi%29q%5E%7By_%7Bj%7D%7D%281-q%29%5E%7B1-y_%7Bj%7D%7D)

    根据似然函数估计模型的参数就要满足似然函数最大是不是？问题的目标函数就出来了：

![\widehat{\theta}=argmaxlogP(Y|\theta)](https://private.codecogs.com/gif.latex?%5Cwidehat%7B%5Ctheta%7D%3DargmaxlogP%28Y%7C%5Ctheta%29)

    这个问题如何求解，书中给出迭代法，也就是我们的EM过程：
- 第一步初始化我们的![\theta= {\pi,p,q}](https://private.codecogs.com/gif.latex?%5Ctheta%3D%20%7B%5Cpi%2Cp%2Cq%7D)，初始化的值比较重要，它会参数决定最终取值。
- E步：计算在模型初始参数的情况下观测数据yj(这里指第一次的硬币结果)来自B的概率（为什么是B呢?其实都可以，只要能求三个参数就行，即![logP(Y,Z|\theta)|Y,\theta^{(i)})](https://private.codecogs.com/gif.latex?logP%28Y%2CZ%7C%5Ctheta%29%7CY%2C%5Ctheta%5E%7B%28i%29%7D%29)）
- M步：Π的新值是不是就是E部算出的概率的期望。（10次结果的均值），p,q能求吗？自己可以算，很好求。
- 此时求得的新![\theta= {\pi,p,q}](https://private.codecogs.com/gif.latex?%5Ctheta%3D%20%7B%5Cpi%2Cp%2Cq%7D)参数是不是影响了E步的概率结果，继续迭代下去直到三个值不再变化或者达到停止条件。

    由上述步骤可以看出EM算法的关键在二三步，在第二步中完全数据的似然函数在给定观测数据Y和当前参数![\theta^{(i)}](https://private.codecogs.com/gif.latex?%5Ctheta%5E%7B%28i%29%7D)下对未观测数据Z的条件概率![P(Z|Y,\theta^{(i)})](https://private.codecogs.com/gif.latex?P%28Z%7CY%2C%5Ctheta%5E%7B%28i%29%7D%29)的期望也叫做Q函数，每次迭代实际上是让Q函数最大化:

![Q(\theta,\theta^{(i)})=E_{z}[logP(Y,Z|\theta)|Y,\theta^{(i)}]](https://private.codecogs.com/gif.latex?Q%28%5Ctheta%2C%5Ctheta%5E%7B%28i%29%7D%29%3DE_%7Bz%7D%5BlogP%28Y%2CZ%7C%5Ctheta%29%7CY%2C%5Ctheta%5E%7B%28i%29%7D%5D)

    通过上述例子很清楚的理清了EM算法思路，但是会有一个问题，为什么我们保证Q会不断变大就可以近似表达出似然函数不断变大呢？

## 3.导出

    目标：![\widehat{\theta}=argmaxlogP(Y|\theta)=argmaxL(\theta)](https://private.codecogs.com/gif.latex?%5Cwidehat%7B%5Ctheta%7D%3DargmaxlogP%28Y%7C%5Ctheta%29%3DargmaxL%28%5Ctheta%29)

    过程：

          由jensen不等式得：

![L(\theta)-L(\theta^{(i)})\geq \sum P(Z|Y,\theta^{(i)})log\frac{P(Y|Z,\theta)P(Z,\theta)}{P(Z|Y,\theta^{(i)})P(Y,\theta^{(i)})}](https://private.codecogs.com/gif.latex?L%28%5Ctheta%29-L%28%5Ctheta%5E%7B%28i%29%7D%29%5Cgeq%20%5Csum%20P%28Z%7CY%2C%5Ctheta%5E%7B%28i%29%7D%29log%5Cfrac%7BP%28Y%7CZ%2C%5Ctheta%29P%28Z%2C%5Ctheta%29%7D%7BP%28Z%7CY%2C%5Ctheta%5E%7B%28i%29%7D%29P%28Y%2C%5Ctheta%5E%7B%28i%29%7D%29%7D)

          令![L(\theta)](https://private.codecogs.com/gif.latex?L%28%5Ctheta%29)的一个下界函数![B(\theta,\theta^{(i)})=L(\theta^{(i)})+ \sum P(Z|Y,\theta^{(i)})log\frac{P(Y|Z,\theta)P(Z,\theta)}{P(Z|Y,\theta^{(i)})P(Y,\theta^{(i)})}](https://private.codecogs.com/gif.latex?B%28%5Ctheta%2C%5Ctheta%5E%7B%28i%29%7D%29%3DL%28%5Ctheta%5E%7B%28i%29%7D%29&plus;%20%5Csum%20P%28Z%7CY%2C%5Ctheta%5E%7B%28i%29%7D%29log%5Cfrac%7BP%28Y%7CZ%2C%5Ctheta%29P%28Z%2C%5Ctheta%29%7D%7BP%28Z%7CY%2C%5Ctheta%5E%7B%28i%29%7D%29P%28Y%2C%5Ctheta%5E%7B%28i%29%7D%29%7D)

          即只要每一次迭代让这个下界增大即可，而下界函数的后项值就是Q函数，这里推导略过。

          同样这种办法也是可以保证收敛的，由于我对这些偏实用性，没有去推导。

          这里不用梯度下降的办法是因为：梯度下降算法会收敛，但是可能会收敛到局部最优，而且求和的项数会随着隐变量的数         目指数上升，会给梯度计算带来麻烦。相对来说，EM算法是一种非梯度优化算法。

## 4.应用
- 高斯混合模型：用EM算法估计高斯模型中的方差和期望
- 贝叶斯混合模型：EM算法是一种迭代算法，它通常用于缺失数据的填补问题，首先对缺失值有一个初始的估计，然后进行EM迭代，第一步在变量和当前参数估计的情况，给出缺失值相应的估计，第二步在承认第一步正确的情况下重新对参数进行估计。朴素贝特斯的缺点就在于它的属性相互依赖，同时对缺失值敏感，在朴素贝叶斯中缺失值的初始估计用未缺失的众数来替代，第一步在给定的初始值和给定的完整数据的基础上求得对数似然函数的条件期望（即缺失值补全成这样之后结果的取值概率的乘积），第二步最大化条件期望。







