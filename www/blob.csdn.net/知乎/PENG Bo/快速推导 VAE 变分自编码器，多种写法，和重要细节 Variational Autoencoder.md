# 快速推导 VAE 变分自编码器，多种写法，和重要细节 Variational Autoencoder - 知乎
# 



这里用 ![p_\theta(x|z)](https://www.zhihu.com/equation?tex=p_%5Ctheta%28x%7Cz%29) 代表生成模型， ![q_\phi(z|x)](https://www.zhihu.com/equation?tex=q_%5Cphi%28z%7Cx%29) 代表编码模型。

首先：![\begin{align} \mathop{\arg\min}_\theta \mathsf{KL}(p \parallel p_\theta) &= \mathop{\arg\min}_\theta \int p(x) \log \frac{p(x)}{p_\theta(x)}  \, dx \\&= \mathop{\arg\min}_\theta \Big[ \int p(x) \log p(x)  \, dx  - \int p(x) \log p_\theta(x)  \, dx \Big] \\&= \mathop{\arg\min}_\theta \Big[H(p(x))  - \int p(x) \log p_\theta(x)  \, dx \Big] \\&= \mathop{\arg\max}_\theta \int p(x) \log p_\theta(x)  \, dx  \\&=  \mathop{\arg\max}_\theta \mathbb{E}_{x \sim p(x)}\log p_\theta(x) \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%5Cmathop%7B%5Carg%5Cmin%7D_%5Ctheta+%5Cmathsf%7BKL%7D%28p+%5Cparallel+p_%5Ctheta%29+%26%3D+%5Cmathop%7B%5Carg%5Cmin%7D_%5Ctheta+%5Cint+p%28x%29+%5Clog+%5Cfrac%7Bp%28x%29%7D%7Bp_%5Ctheta%28x%29%7D++%5C%2C+dx+%5C%5C%26%3D+%5Cmathop%7B%5Carg%5Cmin%7D_%5Ctheta+%5CBig%5B+%5Cint+p%28x%29+%5Clog+p%28x%29++%5C%2C+dx++-+%5Cint+p%28x%29+%5Clog+p_%5Ctheta%28x%29++%5C%2C+dx+%5CBig%5D+%5C%5C%26%3D+%5Cmathop%7B%5Carg%5Cmin%7D_%5Ctheta+%5CBig%5BH%28p%28x%29%29++-+%5Cint+p%28x%29+%5Clog+p_%5Ctheta%28x%29++%5C%2C+dx+%5CBig%5D+%5C%5C%26%3D+%5Cmathop%7B%5Carg%5Cmax%7D_%5Ctheta+%5Cint+p%28x%29+%5Clog+p_%5Ctheta%28x%29++%5C%2C+dx++%5C%5C%26%3D++%5Cmathop%7B%5Carg%5Cmax%7D_%5Ctheta+%5Cmathbb%7BE%7D_%7Bx+%5Csim+p%28x%29%7D%5Clog+p_%5Ctheta%28x%29+%5Cend%7Balign%7D)

写成 MLE，比 KL 散度少一个常数，形式更简单。没必要时刻写出 KL 散度。

让我们 1 秒钟推导出 VAE。

思想是，匹配 ![p(x,z)](https://www.zhihu.com/equation?tex=p%28x%2Cz%29) 和 ![p_\theta(x,z)](https://www.zhihu.com/equation?tex=p_%5Ctheta%28x%2Cz%29) ，就可以同时匹配 ![x](https://www.zhihu.com/equation?tex=x) 和 ![z](https://www.zhihu.com/equation?tex=z) 的边缘分布。




MLE 如下：

![\mathop{\arg\max}_\theta \mathbb{E}_{(x,z) \sim p(x,z)}\log p_\theta(x,z)](https://www.zhihu.com/equation?tex=%5Cmathop%7B%5Carg%5Cmax%7D_%5Ctheta+%5Cmathbb%7BE%7D_%7B%28x%2Cz%29+%5Csim+p%28x%2Cz%29%7D%5Clog+p_%5Ctheta%28x%2Cz%29)

显然等价于：

![\mathop{\arg\max}_\theta \mathbb{E}_{x \sim p(x)} \Big[ \mathbb{E}_{z \sim p(z|x)} \Big[ \log p_\theta(x|z) + \log p_\theta(z) \Big]  \Big]](https://www.zhihu.com/equation?tex=%5Cmathop%7B%5Carg%5Cmax%7D_%5Ctheta+%5Cmathbb%7BE%7D_%7Bx+%5Csim+p%28x%29%7D+%5CBig%5B+%5Cmathbb%7BE%7D_%7Bz+%5Csim+p%28z%7Cx%29%7D+%5CBig%5B+%5Clog+p_%5Ctheta%28x%7Cz%29+%2B+%5Clog+p_%5Ctheta%28z%29+%5CBig%5D++%5CBig%5D)

恭喜，推导出了 VAE。




此外，这个 MLE 显然也等价于：

![\mathop{\arg\min}_\theta \mathsf{KL}(p(x,z) \parallel p_\theta(x,z))](https://www.zhihu.com/equation?tex=%5Cmathop%7B%5Carg%5Cmin%7D_%5Ctheta+%5Cmathsf%7BKL%7D%28p%28x%2Cz%29+%5Cparallel+p_%5Ctheta%28x%2Cz%29%29)

加 10 秒钟，把它变成更常见的样子。

展开：

![\mathop{\arg\max}_\theta \mathbb{E}_{x \sim p(x)} \Big[ \mathbb{E}_{z \sim p(z|x)}  \log p_\theta(x|z) + \mathbb{E}_{z \sim p(z|x)} \log p_\theta(z) \Big]](https://www.zhihu.com/equation?tex=%5Cmathop%7B%5Carg%5Cmax%7D_%5Ctheta+%5Cmathbb%7BE%7D_%7Bx+%5Csim+p%28x%29%7D+%5CBig%5B+%5Cmathbb%7BE%7D_%7Bz+%5Csim+p%28z%7Cx%29%7D++%5Clog+p_%5Ctheta%28x%7Cz%29+%2B+%5Cmathbb%7BE%7D_%7Bz+%5Csim+p%28z%7Cx%29%7D+%5Clog+p_%5Ctheta%28z%29+%5CBig%5D)

改变符号，显然等价于：

![\mathop{\arg\min}_\theta \mathbb{E}_{x \sim p(x)} \Big[ \mathbb{E}_{z \sim p(z|x)} - \log p_\theta(x|z) + \mathsf{KL}\big( p(z|x) \parallel  p_\theta(z)\big) \Big]](https://www.zhihu.com/equation?tex=%5Cmathop%7B%5Carg%5Cmin%7D_%5Ctheta+%5Cmathbb%7BE%7D_%7Bx+%5Csim+p%28x%29%7D+%5CBig%5B+%5Cmathbb%7BE%7D_%7Bz+%5Csim+p%28z%7Cx%29%7D+-+%5Clog+p_%5Ctheta%28x%7Cz%29+%2B+%5Cmathsf%7BKL%7D%5Cbig%28+p%28z%7Cx%29+%5Cparallel++p_%5Ctheta%28z%29%5Cbig%29+%5CBig%5D)




再加上先验：

![p(z|x) = N(\mu_x, \sigma_x^2)](https://www.zhihu.com/equation?tex=p%28z%7Cx%29+%3D+N%28%5Cmu_x%2C+%5Csigma_x%5E2%29)

![p_\theta(z) = N(0, I)](https://www.zhihu.com/equation?tex=p_%5Ctheta%28z%29+%3D+N%280%2C+I%29)

并令 ![p_\theta(x|z) = N(G_\theta(z), \sigma^2)](https://www.zhihu.com/equation?tex=p_%5Ctheta%28x%7Cz%29+%3D+N%28G_%5Ctheta%28z%29%2C+%5Csigma%5E2%29) 是固定 stdev 的 Gaussian 以造出 MSE：

![- \log p_\theta(x|z) = \frac{1}{2 \sigma^2} \cdot |G_\theta(z)-x|^2 + \log(\sqrt{2\pi \sigma^2})](https://www.zhihu.com/equation?tex=-+%5Clog+p_%5Ctheta%28x%7Cz%29+%3D+%5Cfrac%7B1%7D%7B2+%5Csigma%5E2%7D+%5Ccdot+%7CG_%5Ctheta%28z%29-x%7C%5E2+%2B+%5Clog%28%5Csqrt%7B2%5Cpi+%5Csigma%5E2%7D%29)

忽略常数，就和实际用的一模一样了。

## **注意：由此可见，VAE 一点儿也不模糊，真正的 VAE **![p_\theta(x|z)](https://www.zhihu.com/equation?tex=p_%5Ctheta%28x%7Cz%29)**有很多噪音（由于这里的概率模型是每点独立，因此噪音也是每点独立的噪音）。许多论文显示的模糊图像，是"平均图像"。**

## **注意：我们完全可以用更复杂的先验，例如用 PixelXNN 生成图像，这样就完全没有"模糊"。**

## **注意：似乎没人实验可变 stdev 的 Gaussian。所以我做了一些实验，见本文末尾。**

最终结果（ ![\beta-\text{VAE}](https://www.zhihu.com/equation?tex=%5Cbeta-%5Ctext%7BVAE%7D) ）：

![\mathop{\arg\min}_{\theta,\, \mu_i,\, \sigma_i} \mathbb{E}_{x \sim p(x)} \Big[ \mathbb{E}_{z_i \sim N(\mu_i,\sigma_i^2)} \vert G_\theta(\{z_i\}) - x \vert^2 + \beta \cdot  \frac{1}{2} \sum_i \big(\mu_i^2 + \sigma_i^2 - \log \sigma_i^2 -1 \big) \Big]](https://www.zhihu.com/equation?tex=%5Cmathop%7B%5Carg%5Cmin%7D_%7B%5Ctheta%2C%5C%2C+%5Cmu_i%2C%5C%2C+%5Csigma_i%7D+%5Cmathbb%7BE%7D_%7Bx+%5Csim+p%28x%29%7D+%5CBig%5B+%5Cmathbb%7BE%7D_%7Bz_i+%5Csim+N%28%5Cmu_i%2C%5Csigma_i%5E2%29%7D+%5Cvert+G_%5Ctheta%28%5C%7Bz_i%5C%7D%29+-+x+%5Cvert%5E2+%2B+%5Cbeta+%5Ccdot++%5Cfrac%7B1%7D%7B2%7D+%5Csum_i+%5Cbig%28%5Cmu_i%5E2+%2B+%5Csigma_i%5E2+-+%5Clog+%5Csigma_i%5E2+-1+%5Cbig%29+%5CBig%5D)

其中每个 ![z_i](https://www.zhihu.com/equation?tex=z_i) 来自独立的 ![N(\mu_i,\sigma_i^2)](https://www.zhihu.com/equation?tex=N%28%5Cmu_i%2C%5Csigma_i%5E2%29) 采样。

定义 ![t_i=\log \sigma_i^2](https://www.zhihu.com/equation?tex=t_i%3D%5Clog+%5Csigma_i%5E2) ，并加入重参数化 trick：

![\mathop{\arg\min}_{\theta,\, \mu_i,\, t_i} \mathbb{E}_{x \sim p(x)} \Big[ \mathbb{E}_{\epsilon_i \sim N(0,1)} \vert G_\theta(\{\mu_i + \epsilon_i \cdot \exp(t_i/2)\}) - x \vert^2 + \beta \cdot  \frac{1}{2} \sum_i \big(\mu_i^2 + \exp(t_i) - t_i -1 \big) \Big]](https://www.zhihu.com/equation?tex=%5Cmathop%7B%5Carg%5Cmin%7D_%7B%5Ctheta%2C%5C%2C+%5Cmu_i%2C%5C%2C+t_i%7D+%5Cmathbb%7BE%7D_%7Bx+%5Csim+p%28x%29%7D+%5CBig%5B+%5Cmathbb%7BE%7D_%7B%5Cepsilon_i+%5Csim+N%280%2C1%29%7D+%5Cvert+G_%5Ctheta%28%5C%7B%5Cmu_i+%2B+%5Cepsilon_i+%5Ccdot+%5Cexp%28t_i%2F2%29%5C%7D%29+-+x+%5Cvert%5E2+%2B+%5Cbeta+%5Ccdot++%5Cfrac%7B1%7D%7B2%7D+%5Csum_i+%5Cbig%28%5Cmu_i%5E2+%2B+%5Cexp%28t_i%29+-+t_i+-1+%5Cbig%29+%5CBig%5D)

于是可求 LOSS 对 ![\theta,\, \mu_i,\, t_i](https://www.zhihu.com/equation?tex=%5Ctheta%2C%5C%2C+%5Cmu_i%2C%5C%2C+t_i) 的导数，进行 SGD。

补充传统的推导过程。如前所述，用 ![p_\theta(x|z)](https://www.zhihu.com/equation?tex=p_%5Ctheta%28x%7Cz%29) 代表生成模型， ![q_\phi(z|x)](https://www.zhihu.com/equation?tex=q_%5Cphi%28z%7Cx%29) 代表编码模型。

我们的目标是边缘分布的 MLE：

![\mathop{\arg\max}_\theta \mathbb{E}_{x \sim p(x)}\log p_\theta(x)](https://www.zhihu.com/equation?tex=%5Cmathop%7B%5Carg%5Cmax%7D_%5Ctheta+%5Cmathbb%7BE%7D_%7Bx+%5Csim+p%28x%29%7D%5Clog+p_%5Ctheta%28x%29)

注意到这里有 ELBO：

![\begin{align} \log p_\theta(x) &= \log \int p_\theta(x,z) \,dz \\&=  \log \int q_\phi(z|x) \frac{p_\theta(x,z)}{q_\phi(z|x)} \,dz \\&= \log E_{ z \sim q_\phi(z|x)} \frac{p_\theta(x,z)}{q_\phi(z|x)} \\&\geq  E_{ z \sim q_\phi(z|x)} \log \frac{p_\theta(x,z)}{q_\phi(z|x)} \\&=  E_{ z \sim q_\phi(z|x)} \log \frac{p_\theta(x|z) \, p_\theta(z)}{q_\phi(z|x)} \\&= E_{ z \sim q_\phi(z|x)} \log p_\theta(x|z) - E_{ z \sim q_\phi(z|x)} \log \frac{q_\phi(z|x)}{p_\theta(z)} \\&=E_{ z \sim q_\phi(z|x)} \log p_\theta(x|z) - \mathsf{KL}(q_\phi(z|x) \parallel p_\theta(z)) \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%5Clog+p_%5Ctheta%28x%29+%26%3D+%5Clog+%5Cint+p_%5Ctheta%28x%2Cz%29+%5C%2Cdz+%5C%5C%26%3D++%5Clog+%5Cint+q_%5Cphi%28z%7Cx%29+%5Cfrac%7Bp_%5Ctheta%28x%2Cz%29%7D%7Bq_%5Cphi%28z%7Cx%29%7D+%5C%2Cdz+%5C%5C%26%3D+%5Clog+E_%7B+z+%5Csim+q_%5Cphi%28z%7Cx%29%7D+%5Cfrac%7Bp_%5Ctheta%28x%2Cz%29%7D%7Bq_%5Cphi%28z%7Cx%29%7D+%5C%5C%26%5Cgeq++E_%7B+z+%5Csim+q_%5Cphi%28z%7Cx%29%7D+%5Clog+%5Cfrac%7Bp_%5Ctheta%28x%2Cz%29%7D%7Bq_%5Cphi%28z%7Cx%29%7D+%5C%5C%26%3D++E_%7B+z+%5Csim+q_%5Cphi%28z%7Cx%29%7D+%5Clog+%5Cfrac%7Bp_%5Ctheta%28x%7Cz%29+%5C%2C+p_%5Ctheta%28z%29%7D%7Bq_%5Cphi%28z%7Cx%29%7D+%5C%5C%26%3D+E_%7B+z+%5Csim+q_%5Cphi%28z%7Cx%29%7D+%5Clog+p_%5Ctheta%28x%7Cz%29+-+E_%7B+z+%5Csim+q_%5Cphi%28z%7Cx%29%7D+%5Clog+%5Cfrac%7Bq_%5Cphi%28z%7Cx%29%7D%7Bp_%5Ctheta%28z%29%7D+%5C%5C%26%3DE_%7B+z+%5Csim+q_%5Cphi%28z%7Cx%29%7D+%5Clog+p_%5Ctheta%28x%7Cz%29+-+%5Cmathsf%7BKL%7D%28q_%5Cphi%28z%7Cx%29+%5Cparallel+p_%5Ctheta%28z%29%29+%5Cend%7Balign%7D)

因此目标等价于：

![\mathop{\arg\max}_{\theta, \, \phi} \mathbb{E}_{x \sim p(x)} \Big[ E_{ z \sim q_\phi(z|x)} \log p_\theta(x|z) - \mathsf{KL}(q_\phi(z|x) \parallel p_\theta(z)) \Big]](https://www.zhihu.com/equation?tex=%5Cmathop%7B%5Carg%5Cmax%7D_%7B%5Ctheta%2C+%5C%2C+%5Cphi%7D+%5Cmathbb%7BE%7D_%7Bx+%5Csim+p%28x%29%7D+%5CBig%5B+E_%7B+z+%5Csim+q_%5Cphi%28z%7Cx%29%7D+%5Clog+p_%5Ctheta%28x%7Cz%29+-+%5Cmathsf%7BKL%7D%28q_%5Cphi%28z%7Cx%29+%5Cparallel+p_%5Ctheta%28z%29%29+%5CBig%5D)

即：

![\mathop{\arg\min}_{\theta,\, \phi} \mathbb{E}_{x \sim p(x)} \Big[E_{ z \sim q_\phi(z|x)} - \log p_\theta(x|z) + \mathsf{KL}(q_\phi(z|x) \parallel p_\theta(z)) \Big]](https://www.zhihu.com/equation?tex=%5Cmathop%7B%5Carg%5Cmin%7D_%7B%5Ctheta%2C%5C%2C+%5Cphi%7D+%5Cmathbb%7BE%7D_%7Bx+%5Csim+p%28x%29%7D+%5CBig%5BE_%7B+z+%5Csim+q_%5Cphi%28z%7Cx%29%7D+-+%5Clog+p_%5Ctheta%28x%7Cz%29+%2B+%5Cmathsf%7BKL%7D%28q_%5Cphi%28z%7Cx%29+%5Cparallel+p_%5Ctheta%28z%29%29+%5CBig%5D)

这与我们之前的推导相同，只是多了一个 ![q_\phi(z|x)](https://www.zhihu.com/equation?tex=q_%5Cphi%28z%7Cx%29) 去逼近之前的 ![p(z|x)](https://www.zhihu.com/equation?tex=p%28z%7Cx%29) 。

这里还有一种写法，注意到：

![E_{ z \sim q_\phi(z|x)} \log \frac{p_\theta(x,z)}{q_\phi(z|x)} = E_{ z \sim q_\phi(z|x)} \log \frac{p_\theta(x) \, p_\theta(z|x)}{q_\phi(z|x)} = \log p_\theta(x) - \mathsf{KL}( q_\phi(z|x) \parallel  p_\theta(z|x))](https://www.zhihu.com/equation?tex=E_%7B+z+%5Csim+q_%5Cphi%28z%7Cx%29%7D+%5Clog+%5Cfrac%7Bp_%5Ctheta%28x%2Cz%29%7D%7Bq_%5Cphi%28z%7Cx%29%7D+%3D+E_%7B+z+%5Csim+q_%5Cphi%28z%7Cx%29%7D+%5Clog+%5Cfrac%7Bp_%5Ctheta%28x%29+%5C%2C+p_%5Ctheta%28z%7Cx%29%7D%7Bq_%5Cphi%28z%7Cx%29%7D+%3D+%5Clog+p_%5Ctheta%28x%29+-+%5Cmathsf%7BKL%7D%28+q_%5Cphi%28z%7Cx%29+%5Cparallel++p_%5Ctheta%28z%7Cx%29%29)

因此目标等价于：

![\mathop{\arg\min}_{\theta,\, \phi} \mathbb{E}_{x \sim p(x)} \Big[ - \log p_\theta(x) + E_{ z \sim q_\phi(z|x)} \mathsf{KL}(q_\phi(z|x) \parallel p_\theta(z|x)) \Big]](https://www.zhihu.com/equation?tex=%5Cmathop%7B%5Carg%5Cmin%7D_%7B%5Ctheta%2C%5C%2C+%5Cphi%7D+%5Cmathbb%7BE%7D_%7Bx+%5Csim+p%28x%29%7D+%5CBig%5B+-+%5Clog+p_%5Ctheta%28x%29+%2B+E_%7B+z+%5Csim+q_%5Cphi%28z%7Cx%29%7D+%5Cmathsf%7BKL%7D%28q_%5Cphi%28z%7Cx%29+%5Cparallel+p_%5Ctheta%28z%7Cx%29%29+%5CBig%5D)

我在另一篇文章，简单实验了 ![p_\theta(x|z)](https://www.zhihu.com/equation?tex=p_%5Ctheta%28x%7Cz%29) 有可变 stdev 的情况：
[PENG Bo：DGN v2：生成器应该输出分布，清晰图像并不是 GAN 的特权​zhuanlan.zhihu.com![图标](https://pic2.zhimg.com/v2-08d4415c191a42b2ced9bc30643d2cb5_180x120.jpg)](https://zhuanlan.zhihu.com/p/51496961)



