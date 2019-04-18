# VAE 的细节：p(x|z) 的噪音，与 p(z|x) 的编码坍塌 - 知乎
# 



变分自编码器 VAE 的常见错误理解，是认为 VAE 模糊。

VAE 并不模糊，VAE 有噪音。真正的 VAE 输出是一个每点固定 stdev 的分布 ![p(x|z)](https://www.zhihu.com/equation?tex=p%28x%7Cz%29)，有很多噪音（由于常用的 Gaussian 模型是每点独立，因此噪音也是每点独立的噪音）。

许多论文显示的模糊图像，是 Gaussian 情况下的"平均图像" ![\mathrm{E}[p(x|z)]](https://www.zhihu.com/equation?tex=%5Cmathrm%7BE%7D%5Bp%28x%7Cz%29%5D) ，并非真正的输出采样 ![\tilde{x} \sim p(x|z)](https://www.zhihu.com/equation?tex=%5Ctilde%7Bx%7D+%5Csim+p%28x%7Cz%29)。

这在 VAE 的推导中很清楚，例如本专栏之前的介绍： 
[PENG Bo：快速推导 VAE 变分自编码器，多种写法，和常见错误 Variational Autoencoder​zhuanlan.zhihu.com![图标](https://pic1.zhimg.com/v2-138a414057730d0f0ddd31506e71f4d8_180x120.jpg)](https://zhuanlan.zhihu.com/p/51589440)
如下所述：

> Vanilla VAEs with Gaussian posteriors / priors and factorized pixel distributions aren't blurry, they're noisy. People tend to show the mean value of p(x|z) rather than drawing samples from it. Hence the reported blurry samples aren't actually samples from the model, and they don't reveal the extent to which variability is captured by pixel noise. Real samples would typically demonstrate salt and pepper noise due to independent samples from the pixel distributions.

VAEs are poor models of data whenever insufficiently flexible posterior / prior / decoder distributions are used. These issues are much improved when more expressive choices are used as in IAF, pixelvae, variational lossy autoencoder.

我的观点，两全其美的方法，是用 VAE 生成图像的分布，再用 GAN 从中采样（降噪），以同时解决 GAN 的模式缺失，和 VAE 的噪音。关于图像分布，之前的实验【训练出来，面部皮肤的方差就小，五官边缘和头发的方差就大，背景的方差也大，可见网络有能力判断这些】：
[PENG Bo：DGN v2：生成器应该输出分布，清晰图像并不是 GAN 的特权​zhuanlan.zhihu.com![图标](https://pic2.zhimg.com/v2-08d4415c191a42b2ced9bc30643d2cb5_180x120.jpg)](https://zhuanlan.zhihu.com/p/51496961)
关于 beta-VAE，我也更倾向于认为它其实相当于反向调 ![p(x|z)](https://www.zhihu.com/equation?tex=p%28x%7Cz%29) 每点的 stdev。

我用一个例子，说明 VAE 的噪音问题，以及编码坍塌。

这里的目标，是学会 ![x \sim N(0, t^2)](https://www.zhihu.com/equation?tex=x+%5Csim+N%280%2C+t%5E2%29) 。这可谓是最简单的任务。

**然而，如果了解 VAE，会意识到，常见的 VAE 是学不会这个 ![x](https://www.zhihu.com/equation?tex=x) 的。**




令 E 网络输入 ![x](https://www.zhihu.com/equation?tex=x) 的输出为 ![\{\mu_x, \sigma_x^2\}](https://www.zhihu.com/equation?tex=%5C%7B%5Cmu_x%2C+%5Csigma_x%5E2%5C%7D) 。

为得到解析解，不妨令 ![\mu_x = a \cdot x](https://www.zhihu.com/equation?tex=%5Cmu_x+%3D+a+%5Ccdot+x) ， ![\sigma_x = b](https://www.zhihu.com/equation?tex=%5Csigma_x+%3D+b) ，其中 ![a](https://www.zhihu.com/equation?tex=a) 和 ![b](https://www.zhihu.com/equation?tex=b) 是待训练的参数。

那么 ![x](https://www.zhihu.com/equation?tex=x) 对应的编码是 ![z \sim N(\mu_x, \sigma_x^2)](https://www.zhihu.com/equation?tex=z+%5Csim+N%28%5Cmu_x%2C+%5Csigma_x%5E2%29) 。

令 G 网络的输出为 ![G(z) = c \cdot z](https://www.zhihu.com/equation?tex=G%28z%29+%3D+c+%5Ccdot+z)，其中 ![c](https://www.zhihu.com/equation?tex=c) 是待训练的参数。




如果 ![t=1](https://www.zhihu.com/equation?tex=t%3D1) ，![a=1](https://www.zhihu.com/equation?tex=a%3D1) ， ![b=0](https://www.zhihu.com/equation?tex=b%3D0) ， ![c=1](https://www.zhihu.com/equation?tex=c%3D1) ，那么 E 和 G 对于 ![x](https://www.zhihu.com/equation?tex=x) 的还原是完美的，而且 ![z](https://www.zhihu.com/equation?tex=z) 的实际分布 ![p(z)](https://www.zhihu.com/equation?tex=p%28z%29) 也是完美的 ![N(0,1)](https://www.zhihu.com/equation?tex=N%280%2C1%29) 。

## **但是，常见的 VAE 不能看到 ![p(z)](https://www.zhihu.com/equation?tex=p%28z%29) ，它只能看到 **![p(z|x)](https://www.zhihu.com/equation?tex=p%28z%7Cx%29) ，**此时 E 网络的输出是 ![N(x,0)](https://www.zhihu.com/equation?tex=N%28x%2C0%29) ，与 ![N(0,1)](https://www.zhihu.com/equation?tex=N%280%2C1%29) 相差甚远，于是 KL 散度会将它推过去，令还原变差。**




在常见的 VAE 中，LOSS 为：

![\mathop{\arg\min}_{a,\, b,\, c} \mathbb{E}_{x \sim N(0,t^2)} \Big[ \mathbb{E}_{z \sim N(a \cdot x, b^2)} \vert c \cdot z - x \vert^2 + \beta \cdot  \frac{1}{2}\big(a^2x^2 + b^2 - \log b^2 -1 \big) \Big]](https://www.zhihu.com/equation?tex=%5Cmathop%7B%5Carg%5Cmin%7D_%7Ba%2C%5C%2C+b%2C%5C%2C+c%7D+%5Cmathbb%7BE%7D_%7Bx+%5Csim+N%280%2Ct%5E2%29%7D+%5CBig%5B+%5Cmathbb%7BE%7D_%7Bz+%5Csim+N%28a+%5Ccdot+x%2C+b%5E2%29%7D+%5Cvert+c+%5Ccdot+z+-+x+%5Cvert%5E2+%2B+%5Cbeta+%5Ccdot++%5Cfrac%7B1%7D%7B2%7D%5Cbig%28a%5E2x%5E2+%2B+b%5E2+-+%5Clog+b%5E2+-1+%5Cbig%29+%5CBig%5D)

积分得到解析解：

![\mathop{\arg\min}_{a,\, b,\, c} \frac{1}{2} (2 b^2 c^2 + 2 t^2 - 4 a c t^2 + 2 a^2 c^2 t^2 - \beta +   b^2 \beta + a^2 t^2 \beta - \beta \log[b^2])](https://www.zhihu.com/equation?tex=%5Cmathop%7B%5Carg%5Cmin%7D_%7Ba%2C%5C%2C+b%2C%5C%2C+c%7D+%5Cfrac%7B1%7D%7B2%7D+%282+b%5E2+c%5E2+%2B+2+t%5E2+-+4+a+c+t%5E2+%2B+2+a%5E2+c%5E2+t%5E2+-+%5Cbeta+%2B+++b%5E2+%5Cbeta+%2B+a%5E2+t%5E2+%5Cbeta+-+%5Cbeta+%5Clog%5Bb%5E2%5D%29)




如果 ![t=1](https://www.zhihu.com/equation?tex=t%3D1) ， ![\beta=1](https://www.zhihu.com/equation?tex=%5Cbeta%3D1) ，最小化 LOSS 的参数为：

![a=\frac{\sqrt{2}}{2},\quad b=\frac{\sqrt{2}}{2},\quad c=\frac{\sqrt{2}}{2}](https://www.zhihu.com/equation?tex=a%3D%5Cfrac%7B%5Csqrt%7B2%7D%7D%7B2%7D%2C%5Cquad+b%3D%5Cfrac%7B%5Csqrt%7B2%7D%7D%7B2%7D%2C%5Cquad+c%3D%5Cfrac%7B%5Csqrt%7B2%7D%7D%7B2%7D)

这是有偏（biased） ，且有噪音。下图是 ![b=\frac{\sqrt{2}}{2}](https://www.zhihu.com/equation?tex=b%3D%5Cfrac%7B%5Csqrt%7B2%7D%7D%7B2%7D) 时 ![ LOSS(a,b,c) ](https://www.zhihu.com/equation?tex=+LOSS%28a%2Cb%2Cc%29+) 的情况：
![](https://pic4.zhimg.com/v2-c26bdc15a839469a50767f12d0b5d837_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='449' height='447'></svg>)
如果 ![\beta \geq 2](https://www.zhihu.com/equation?tex=%5Cbeta+%5Cgeq+2) ，问题就更严重：

![a=0,\quad b=1,\quad c=0](https://www.zhihu.com/equation?tex=a%3D0%2C%5Cquad+b%3D1%2C%5Cquad+c%3D0)

彻底放弃了还原 ![x](https://www.zhihu.com/equation?tex=x) ，只最小化 KL 散度。这就是编码坍塌。




只有 ![\beta = 0](https://www.zhihu.com/equation?tex=%5Cbeta+%3D+0)，去掉 VAE 的 KL 散度，才能得到 ![a \cdot c = 1](https://www.zhihu.com/equation?tex=a+%5Ccdot+c+%3D+1) ， ![b=0](https://www.zhihu.com/equation?tex=b%3D0) 的完美还原。


