# 介绍一篇今年NIPS 的 spotlight （Optimization 方向） - 知乎
# 

今年我们组侥幸中了一篇NIPS的spotlight，最近刚好有点空，我还蛮想介绍一下我们这篇文章具体研究了什么的。今年中的paper是 “Convergence of Cubic Regularization for Nonconvex Optimization under KŁ Property” 链接在这（[http://papers.nips.cc/paper/7633-convergence-of-cubic-regularization-for-nonconvex-optimization-under-kl-property.pdf](https://link.zhihu.com/?target=http%3A//papers.nips.cc/paper/7633-convergence-of-cubic-regularization-for-nonconvex-optimization-under-kl-property.pdf)）

我们研究了一个二阶算法 Cubic Regularization 在 Local的表现 under KL property。下面想说一下我们研究这个的 motivation.

## Motivation 1:

因为我们很早就感觉一个算法在非凸情况的分析，单单分析它的 global收敛可能不是那么全面，因为很多全局非凸的函数，其实局部有特别好的性质，比如局部可以是 strongly convex, 可以是 gradient dominate, 可以是满足regularity condition，可以是符合 KL property. 这些较好的局部特性会使得算法在局部有着异常优秀的收敛行为。所以我们觉得对一个算法在非凸情况比较彻底的研究，其实应该分成两个阶段，
- 第一个阶段是从出发点到陷入一个良好局部特性的坑里。 这就是我们指的 Global Behaviour
- 第二个阶段是一个性质比较好的坑里收敛到最小值的表现。这就是我们指的 Local Behaviour

**所以我们的第一个motivation是：全面研究一个算法在非凸函数下表现，应该分成两个阶段，将Global 和Local 的性质都给与刻画**。

## Motivation 2:

第二个motivation其实源于对牛顿法的局部收敛的认知，因为我们知道牛顿法这种二阶算法在局部strongly convex的情况下是quadratic收敛的，而一阶算法比如 GD 在局部strongly convex的情况下是 linearly 收敛的 （我之前介绍过不同收敛的定义，不熟的同学们可以在这里回顾一下 [Zeap：当我们谈论收敛速度时，我们都在谈什么？](https://zhuanlan.zhihu.com/p/27644403)）。 所以我们当时怀疑 Cubic Regularization 这种算法是不是应该具有和牛顿法一样优于一阶算法的收敛，因为 Cubic Regularization 也是一种二阶算法，它的更新步骤如下：
![](https://pic3.zhimg.com/v2-daedb04ccdc169b1adf977f7b89b5b66_b.png)
为了简单起见，我们简称Cubic Regularization 为CR, 我们发现 CR 其实相当于对原函数做了二阶泰勒展开，然后加了一个cubic term regularized， 而且 Nesterov 给过证明在Hessian Lipschitz 的情况下这样展开可以保证上面的model一定在原函数的上方 （至于为什么一定在原函数上方，我在这个地方写过介绍，感兴趣的同学可以看一看：[非凸优化基石：Lipschitz Condition](https://zhuanlan.zhihu.com/p/27554191)），从而保证了 CR的收敛效果。 这里有趣的在于和 Newton's Method的对比， 其实就是少了这个 Cubic term, 从而导致了牛顿法只有局部收敛证明，没有global收敛证明。 

**所以我们的第二个motivation是: 既然牛顿法有良好的局部特性，那么CR应该也有，如果有比较好的结果，那么也可以在长久争论的一阶算法和二阶算法谁好的问题上给与二阶算法一个支撑。**

## Motivation 3

第三个motivation是因为，我们发现当大家研究算法收敛的时候，其实研究的收敛对象是不一样的，比如 convex下大家一般给的是函数值 ![f(x_k) - f^*](https://www.zhihu.com/equation?tex=f%28x_k%29+-+f%5E%2A) 的收敛，而非凸函数的话，一阶算法会研究梯度 ![\|\nabla f(x_k)\|](https://www.zhihu.com/equation?tex=%5C%7C%5Cnabla+f%28x_k%29%5C%7C) 的收敛，而二阶算法还会研究 Hessian 最小特征值![\lambda_{\min}(\nabla^2 f(x_k))](https://www.zhihu.com/equation?tex=%5Clambda_%7B%5Cmin%7D%28%5Cnabla%5E2+f%28x_k%29%29) 的收敛。但是大家很少能集齐所有的收敛指标。**所以我们的第三个motivation就是：既然局部特征比较好，是不是所以的收敛行为都能研究一遍呢？于是我们给与一个比较完整的收敛证明，从以下四个角度给与了收敛研究**
- 点的收敛： ![\|x_k - x^*\|](https://www.zhihu.com/equation?tex=%5C%7Cx_k+-+x%5E%2A%5C%7C)
- 函数值的收敛： ![f(x_k) - f^*](https://www.zhihu.com/equation?tex=f%28x_k%29+-+f%5E%2A)
- 梯度的收敛： ![\|\nabla f(x_k)\|](https://www.zhihu.com/equation?tex=%5C%7C%5Cnabla+f%28x_k%29%5C%7C)
- Hessian 的收敛： ![\lambda_{\min}(\nabla^2 f(x_k))](https://www.zhihu.com/equation?tex=%5Clambda_%7B%5Cmin%7D%28%5Cnabla%5E2+f%28x_k%29%29)

下面放一下我们在这四个方向的结果：

**第一个是点的收敛：**
![](https://pic4.zhimg.com/v2-aadf23dc6f6bed7c6e4eab03d0983b3f_b.jpg)
**第二个是函数值的收敛：**
![](https://pic4.zhimg.com/v2-9c68ce65e9f7a4c334d4368bc9db62ef_b.jpg)
**第三个是梯度和Hessian的收敛，我们放一块了：**
![](https://pic2.zhimg.com/v2-02fd03d5f375cfb544a71df339c17f79_b.png)![](https://pic2.zhimg.com/v2-d67a1ab449976953c842394ad326b135_b.jpg)
从证明的结果来看，CR这种二阶算法相比于GD这种一阶算法在更大的范围内会有更好的收敛速度。但是另一方面，我们也要注意到，CR这个算法每一步是需要求解一个三次的sub problem，而且还涉及到 Hessian 的计算，所以在实际中一阶算法和二阶段算法到底谁快，其实还需要更多的实验和理论才行。

## 总结

行文至此，我总结一下我们这篇文章的三个motivation吧，因为对optimization的了解还不深，如果说的不对的地方希望大牛轻拍。
- 如果想更全面研究一个算法在非凸函数下表现，应该分成两个阶段，将Global 和Local 的性质都给与刻画。
- 既然牛顿法有良好的局部特性，那么CR应该也有，如果有比较好的结果，那么也可以在长久争论的一阶算法和二阶算法谁好的问题上给与二阶算法一个支撑。
- 既然局部特征比较好，是不是所有的收敛指标都能研究一遍呢？于是我们给与一个比较完整的收敛证明，从四个角度给与了收敛研究。

如果觉得写的还行，点个赞呗

欢迎大家关注我 [@Zeap](https://www.zhihu.com/people/08b0dcf08389c7e2ca1676442ef45079) 和非凸优化专栏 [非凸优化学习之路](https://zhuanlan.zhihu.com/optimization)，一起努力呗，少年。

最后，我们组最近还做了一篇paper 叫 ** SpiderBoost: **[A Class of Faster Variance-reduced Algorithms for Nonconvex Optimization](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1810.10690)，改进了今年 NIPS 的一篇Spotlight: **SPIDER:   (**[Near-Optimal Non-Convex Optimization via Stochastic Path Integrated Differential Estimator](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1807.01695)**)**，SPIDER 这篇paper很厉害，将variance reduce 算法的复杂度提升到了 ![O（n^{1/2}\epsilon^{-2}）](https://www.zhihu.com/equation?tex=O%EF%BC%88n%5E%7B1%2F2%7D%5Cepsilon%5E%7B-2%7D%EF%BC%89), 这是一个接近optimal的结果，然而我们发现这个算法用的learning rate 比较局限，我们将算法的learning rate 从 ![O（L^{-1}\|\nabla f(x)\|^{-1}\epsilon）](https://www.zhihu.com/equation?tex=O%EF%BC%88L%5E%7B-1%7D%5C%7C%5Cnabla+f%28x%29%5C%7C%5E%7B-1%7D%5Cepsilon%EF%BC%89) 改进成了 ![O（L^{-1}）](https://www.zhihu.com/equation?tex=O%EF%BC%88L%5E%7B-1%7D%EF%BC%89) 并且保持了原有的优秀的复杂度。有空了我会详细介绍一下我们这篇paper，一点小小的改进，但是觉得还蛮有意思的。

