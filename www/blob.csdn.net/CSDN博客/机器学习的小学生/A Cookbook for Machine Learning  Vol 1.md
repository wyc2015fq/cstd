
# A Cookbook for Machine Learning: Vol 1 - 机器学习的小学生 - CSDN博客


2017年11月23日 09:46:17[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：209


通常的情况是：你拥有某个模型参数$\theta$。你试图优化某个客观标准，但是采用下面列的方式，优化问题不可行或者很难。如果你可以的话，那么你可以应用相应的转换到你的问题上。如果现在这个问题你可以有效的优化，那么很好。如果不能，你可以递归的应用这些转换直到它可以（优化）。
对于Vol1,我们首先陈述下面的问题转换：
**变分边界**（variational bound）
**对抗博弈**（Adversarial game）
**进化策略**（Evolution Strategy）
**凸松弛法**（convex relaxation）
变分边界：
典型问题：通常因为损失函数$f(\theta)$涉及到很难处理的边缘化，因此它（损失函数）很难计算。我很难计算它，更别说最小化它了。
解决方案：
让我们构造一系列的通常可微的上界：

$$
f(\theta) \leq \inf_{\psi} g(\theta,\psi)
$$
并且求解优化问题：

$$
\theta^*,\psi^* \rightarrow \text{argmin}_{\theta,\psi} g(\theta,\psi)
$$
从技术上讲，一旦你完成这个优化，那么可以抛弃辅助参数$\psi^*$，虽然这个辅助变量通常本身有意义和有用，并且通常是针对近似推断（例如VAE的识别模型）（有意义和有用的）
变换技巧:
Jensen’s 不等式：一个凸函数的均值永远不会低于均值对应的函数值。 Jensen不等式通常出现在下面的标准证据下界(ELBO)推导的一些变体中：

$$
\begin{align}
-\log p(x) &= - \log \int p(x,y) dy \\
&= - \log \int q(y|x) \frac{p(y,x)}{q(y|x)} dy \\
&\leq - \int q(y|x) \log \frac{p(y,x)}{q(y|x)} dy
\end{align}
$$
重参数化技巧，在变分推断中，我们通常碰到下面形式的梯度：

$$
\frac{\partial }{\partial \theta_i} \mathbb E_{x \sim q_{\theta}}[f(x,q_{\theta}(x))]
$$
其中变量(x)的概率密度函数(pdf)在积分中。如果我们可以找到一个函数$h:(\mathcal{E}, \Theta)\mapsto \mathcal{X}$,它关于第二个参数是可微的，并且$\mathcal{E}$上的概率分布$p_{\mathcal{E}}$很容易采样，那么下式成立：

$$
x = h(\epsilon, \theta), \epsilon \sim p_\epsilon \iff x \sim q_{\theta},
$$
我们可以使用下面（关于）积分的重新表示，这在变分上界中经常碰到：

$$
\frac{\partial}{\partial \theta_i}\mathbb{E}_{x \sim q_\theta}\left[f(x, q_\theta(x))\right] = \mathbb{E}_{\epsilon \sim p_\epsilon}\left[ \frac{\partial}{\partial \theta_i} f(h(\epsilon, \theta), q_\epsilon(h(\epsilon, \theta)))\right]
$$
A Monte Carlo estimators to this expectation typically have substantially lower variance than REINFORCE estimators to the same quantity.
参考文献：
1.[http://www.inference.vc/design-patterns/](http://www.inference.vc/design-patterns/)
2.[http://www.sohu.com/a/205103359_465975](http://www.sohu.com/a/205103359_465975)
3.[https://blog.csdn.net/weixin_40132961/article/details/78572169](https://blog.csdn.net/weixin_40132961/article/details/78572169)

