
# A Note on Kaldi's PLDA Implementation - 丁丁的博客 - CSDN博客


2018年04月05日 16:24:05[MoussaTintin](https://me.csdn.net/JackyTintin)阅读数：268标签：[Kaldi																](https://so.csdn.net/so/search/s.do?q=Kaldi&t=blog)[EM																](https://so.csdn.net/so/search/s.do?q=EM&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=Kaldi&t=blog)个人分类：[原创																](https://blog.csdn.net/JackyTintin/article/category/1194487)[机器学习																](https://blog.csdn.net/JackyTintin/article/category/6788744)[概率统计																](https://blog.csdn.net/JackyTintin/article/category/6788746)[语音技术																](https://blog.csdn.net/JackyTintin/article/category/8012303)[
							](https://blog.csdn.net/JackyTintin/article/category/6788746)
[
																								](https://blog.csdn.net/JackyTintin/article/category/6788744)
[
				](https://blog.csdn.net/JackyTintin/article/category/1194487)
[
			](https://blog.csdn.net/JackyTintin/article/category/1194487)

[Kaldi](https://github.com/kaldi-asr/kaldi)’s PLDA  implementation is based on [1], the so-called two-covariance PLDA by [2]. The authors derive a clean update formula for the EM training and give a detailed comment in the[source code](https://github.com/kaldi-asr/kaldi/blob/master/src/ivector/plda.cc#L427). Here we add some explanations to make formula derivation more easy to catch.
> A pdf version of this note can be found
> [here](https://arxiv.org/abs/1804.00403)

# 1. Background
Recall that PLDA assume a two stage generative process:
1) generate the class center according to

$$
y \sim \mathcal{N}(\mu, \Phi_b)
$$
2) then, generate the observed data by:

$$
x \sim \mathcal{N}(y, \Phi_w)
$$
Here,$\mu$is estimated by the global mean value:

$$
\mu = \sum_{k=1}^K \sum_{i=1}^{n_k} z_{ki}
$$
here$z_{ki}$depicts the$i$-th sample of the$k$-th class.
So let’s to the estimation of$\Phi_b$and$\Phi_w$.
Note that, as$\mu$is fixed, we remove it from all samples. Hereafter, we assume all samples have pre-processed by removing$mu$from them.
The prior distribution of an arbitrary sample$z$is:

$$
p(z) \sim \mathcal{N}(0,  \Phi_w + \Phi_w)
$$
Let’s suppose the mean of a particular class is$m$, and suppose that that class had$n$examples.

$$
m = \frac{1}{n}\sum_{i=1}^n z_i \sim \mathcal{N}(0, \Phi_w + \frac{\Phi_w}{n} )
$$
i.e.$m$is Gaussian-distributed with zero mean and variance equal to the between-class variance plus$1/n$times the within-class variance. Now,$m$is observed (average of all observed samples).
# 2. EM
We’re doing an E-M procedure where we treat$m$as the sum of two variables:

$$
m = x + y
$$
where$x \sim N(0, \Phi_b)$,$y \sim N(0, \Phi_w/n)$.
The distribution of$x$will contribute to the stats of$\Phi_b$, and$y$to$\Phi_w$.
## 2.1 E Step
Note that given$m$, there’s only one latent variable in effect. Observe the$y = m - x$, so we can focus on working out the distribution of$x$and then we can very simply get the distribution of$y$.
Given$m$, the posterior distribution of$x$is:

$$
p(x|m) = \int_y p_x(x|m, y)p_y(y) = p_x(x|m)p_y(m-x|m)
$$
Hereafter, we drop the condition on$m$for brevity.

$$
p(x) = p_x(x)p_y(m-x) =\mathcal{N}(x|0, \Phi_b) \mathcal{N}(x|m, \Phi_w/n)
$$
Since two Gaussian’s product is Gaussian as well, we get.

$$
p(x) = \mathcal{N}(w, \hat \Phi)
$$
where$\hat \Phi = (\Phi_b^{-1} + n \Phi_w^{-1}) ^{-1}$and$w = \hat \Phi  n\Phi_w^{-1} m$.
---
$\hat \Phi$and$w$can be inferred by comparing the one and two order coefficients to the standard form of log Gaussian. As Kaldi’s comment does:
Note: the C is different from line to line.

$$
\ln p(x) = C - 0.5 (x^T \Phi_b^{-1} x + (m-x)^T n\Phi_w^{-1}(m-x)) = C - 0.5 x^T (\Phi_b^{-1} + n\Phi_w^{-1}) x + x^T z
$$
where$z = n \Phi_w^{-1} m$, and we can write this as:

$$
\ln p(x) = C - 0.5 (x-w)^T (\Phi_b^{-1} + n \Phi_w^{-1}) (x-w)
$$
where$x^T (\Phi_b^{-1} + n \Phi_w^{-1}) w = x^T z$, i.e.

$$
(\Phi_b^{-1} + n \Phi_w^{-1}) w = z = n \Phi_w^{-1} m
$$
,
so

$$
w = (\Phi_b^{-1} + n \Phi_w^{-1})^{-1} * n \Phi_w^{-1} m
$$

$$
\hat \Phi = (\Phi_b^{-1} + n \Phi_w^{-1}) ^{-1}
$$

---

## 2.2 M Step
The objective function of EM update is:

$$
\begin{eqnarray}
\nonumber  Q = \mathbb{E}_x \ln p_x(x) = \mathbb{E}_x -0.5 \ln|\Phi_b| -0.5 x^T (\Phi_b)^{-1} x\\  
\nonumber = -0.5 \ln|\Phi_b| -0.5 \mathrm{tr}( xx^T (\Phi_wb)^{-1})
\end{eqnarray}
$$
derivative w.r.t$\Phi_w/n$is as follows:

$$
\frac{\partial }{\partial (\Phi_b)} = -0.5 (\Phi_b)^{-1} + 0.5  (\Phi_b)^{-1} \mathbb{E}[xx^T]  (\Phi_b)^{-1}
$$
to zero it, we have:

$$
\hat \Phi_b = \mathbb{E}_x[xx^T] = \hat \Phi + \mathbb{E}_x[x]  \mathbb{E}_x[x] ^T = \hat \Phi +ww^T
$$
Similarly, we have:

$$
\hat \Phi_w/n = \mathbb{E}_y[yy^T] = \hat \Phi + \mathbb{E}_y[y]  \mathbb{E}_y[y] ^T = \hat \Phi +(w-m)(w-m)^T
$$

# 3. Summary
recap that given samples of certain class, we can calculate the following statistics:

$$
\hat \Phi = (\Phi_b^{-1} + n \Phi_w^{-1}) ^{-1}
$$

$$
w =\hat  \Phi  \Phi_w^{-1}n m
$$

$$
\hat \Phi_w = n( \Phi +ww^T)
$$

$$
\hat \Phi_b =  \hat \Phi +(w-m)(w-m)^T
$$
Given$K$classes, updated estimation via EM will be:

$$
\Phi_w = \frac{1}{N}\sum_k n_k(\hat  \Phi_k +w_kw_k^T)
$$

$$
\Phi_b = \frac{1}{K}\sum_k (\hat \Phi_k + (w_k-m_k)(w_k-m_k)^T)
$$
Finally, Kaldi use the following update formula for$\Phi_w$:

$$
\Phi_w = \frac{1}{2N-K} (S + \sum_k n_k(\hat  \Phi_k +w_kw_k^T))
$$
where$S$is the scatter matrix$S = \sum_k \sum_i (z_{ki} - c_k)$, and$c_k = \frac{1}{n_k}\sum_i z_{ki}$is the mean  of samples of the$k$-th class.
> Note that
> S
> is the result of EM used here, since
> m = x + y
> only take pooling of data into consideration.
For other EM training, see [2] and the references therein.
# References
Ioffe.[Probabilistic Linear Discriminant Analysis]().
Sizov et al.[Unifying Probabilistic Linear Discriminant Analysis Variants in Biometric Authentication](http://cs.uef.fi/~sizov/pdf/unifying_PLDA_ssspr2014.pdf).

