# 变分自编码器 VAE 在近两年的新进展 Variational Autoencoder - 知乎
# 



本专栏之前介绍了 VAE 的推导：[PENG Bo：快速推导 VAE 变分自编码器，多种写法，和重要细节 Variational Autoencoder](https://zhuanlan.zhihu.com/p/51589440) ，在此介绍 VAE 在 2017/18 年的部分新进展。

## **1. VAE 的“编码坍塌”**

如果思考过 VAE 的原理，会发现它有一个矛盾之处：如果每个 ![x](https://www.zhihu.com/equation?tex=x) 的 ![z](https://www.zhihu.com/equation?tex=z) 的每维都是 ![N(0,1)](https://www.zhihu.com/equation?tex=N%280%2C1%29) ，那么这个 ![z](https://www.zhihu.com/equation?tex=z) 实际就没有编码能力，因为不同 ![x](https://www.zhihu.com/equation?tex=x) 的编码都一样。网络最终会试图忽略 ![z](https://www.zhihu.com/equation?tex=z)，直接去还原 ![p_x](https://www.zhihu.com/equation?tex=p_x)。
![](https://pic4.zhimg.com/v2-073d6e6c70511b7fd1c194bb148b425f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='818' height='85'></svg>)![](https://pic4.zhimg.com/v2-0bfb69b4a1ae72ed0c73d001dcf435cf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='738' height='444'></svg>)
如下图所示：
![](https://pic1.zhimg.com/v2-4843d50bee65a16ef8931947b94974a0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1782' height='726'></svg>)
对于左边的普通 VAE，它会要求 ![z](https://www.zhihu.com/equation?tex=z) （三角标记）都往目标分布（中间的三角标记）靠拢（减少 KL），但是，中间的三角标记只会还原出一个固定的 ![x](https://www.zhihu.com/equation?tex=x) （中间的正方标记）。

假如我们加入一个新网络，可以判断 ![z](https://www.zhihu.com/equation?tex=z) 是否属于 ![p_z](https://www.zhihu.com/equation?tex=p_z)（而不是预先限制 ![z](https://www.zhihu.com/equation?tex=z) 为 Gaussian 然后只看 KL），就可以改善这种问题。于是在 [[1711.01558] Wasserstein Auto-Encoders](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1711.01558) 提出用一个 WGAN 或 MMD 去判断，例如这样的全连接 WGAN：
![](https://pic4.zhimg.com/v2-bad74ba023ad3f5d9df62215b1e2767f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='519' height='211'></svg>)
他们发现 WGAN 的效果比 MMD 更好。

类似的例子是 [Information Maximizing Variational Autoencoders](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1706.02262) 其中只用了 MMD 判断：
![](https://pic4.zhimg.com/v2-726bc5cde0828b280deeebefd79420b7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='738' height='394'></svg>)
这类思想最早的来源是 [Adversarial Autoencoders](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1511.05644) ，大同小异。

## **2. VAE 为什么能实现 Disentanglement？**

考虑 SVHN，令编码为 ![(y,z)](https://www.zhihu.com/equation?tex=%28y%2Cz%29) ，其中 ![y](https://www.zhihu.com/equation?tex=y) 是 10 个数字类别， ![z](https://www.zhihu.com/equation?tex=z) 是额外的“风格”，例如数字的角度，颜色，等等。
![](https://pic4.zhimg.com/v2-2c0639199581ec75deca27cc189b6cab_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1229' height='321'></svg>)
如果用 VAE 训练这个问题，我们会发现成功实现 disentanglement，类似于无监督的分类：
![](https://pic2.zhimg.com/v2-ca6122b3df9273e927b7de8e4fdac4b5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='441' height='227'></svg>)
但如果仔细想想，这并不显然。如 [Rethinking Style and Content Disentanglement in Variational Autoencoders](https://link.zhihu.com/?target=https%3A//openreview.net/forum%3Fid%3DB1rQtwJDG) 所述，模型没有动力去必须完成这个任务：
![](https://pic4.zhimg.com/v2-1670a195afeaefd7a165933be5713f47_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1215' height='319'></svg>)
经过实验，关键还是在于 DCNN + SGD 过程。在编码 disentangle 的情况下，E 和 G 网络都训练更快（因为对于 DCNN 而言，这样的任务“更简单”）。所以梯度下降时网络会倾向于走这个最快的道路。
![](https://pic3.zhimg.com/v2-65efd913bae0c61cfcd27589a766bbc2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='982' height='325'></svg>)
我的看法：关键是在网络架构本身，在于 Deep Image Prior，此前我也说过很多次了。

## **3. 对 VAE loss 的拆分**

本文见 [[1804.02086] Structured Disentangled Representations](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1804.02086)，考虑 G 网络和 E 网络：
![](https://pic4.zhimg.com/v2-b8eeeafb2436a812152d89888c74baaf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='900' height='576'></svg>)
如我在前文的 VAE 推导所述，VAE 用这样的看法更直接：
![](https://pic3.zhimg.com/v2-1df5c9ae6c44f1123f09b6e3ea4e137a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='707' height='146'></svg>)
可进一步拆分：
![](https://pic2.zhimg.com/v2-e1e4eecb3cf770e42bcc108fb5dd2ad9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1203' height='281'></svg>)
每个部分的作用如图：
![](https://pic1.zhimg.com/v2-13125e245666394816a148de0a534eec_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1512' height='669'></svg>)
还可以继续拆 KL：
![](https://pic3.zhimg.com/v2-c1a171b6071b75a4fb1bca146890931a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='906' height='534'></svg>)
每个项目都可以加因子，许多论文只是调整因子而已：
![](https://pic2.zhimg.com/v2-85946842c4529f2ab7206a2f3596c031_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='854' height='530'></svg>)
## **4. IntroVAE：为 VAE 植入 GAN 思想的一种方法**

论文见[Introspective Variational Autoencoders for Photographic Image Synthesis](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1807.06358)。通过修改 VAE 的 LOSS，在不引入新网络的情况下，用 E 网络承担了 GAN 中 D 网络的功能。
![](https://pic4.zhimg.com/v2-2add40502bb218ed154cf976ed67d94f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1321' height='466'></svg>)
效果如下图，和 nVidia 的 PGGAN 相当，而且不需要分层训练，可以直接训练：
![](https://pic3.zhimg.com/v2-fedadcaf62c47a688eac9be6b5d00cce_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1139' height='845'></svg>)
训练过程，其实思想非常简单：
![](https://pic4.zhimg.com/v2-d93092b2be8283034bfcabaaf1dfade7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1103' height='891'></svg>)
思想是，在原有 VAE 的基础上，要求 E 网络在输入假图像【包括 G(z) 和 G(E(x))】时，给出和 ![p_z](https://www.zhihu.com/equation?tex=p_z) 分布尽量不同的编码（增大此时 ![z](https://www.zhihu.com/equation?tex=z) 和 ![p_z](https://www.zhihu.com/equation?tex=p_z) 的 KL）。这样 E 网络就有了“鉴别能力”。而 G 网络加一个目标去骗过 E 网络。

有个细节是，为了防止 E 网络把假图像的 z 分布推得和 ![p_z](https://www.zhihu.com/equation?tex=p_z) 过远，在两个分布间的 KL 大于超参数 ![m](https://www.zhihu.com/equation?tex=m) 时，就不再推了。这个 ![m](https://www.zhihu.com/equation?tex=m) 可以取得比正常 VAE 中的 KL 大一些即可。

**还有更多进展，会在本专栏未来的文章介绍，欢迎关注。**

最后发一下我在调的网络，类似 VAE，不过更简化，只有生成 G 网络。例如，编码转移效果，点击看 GIF 动画（**噪点是知乎的压缩，原图很流畅，毕竟是 VAE**）：
![](https://pic3.zhimg.com/v2-e0ef4a6be28f0643a9fdc9139f0dc31a_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='512' height='512'></svg>)![](https://pic3.zhimg.com/v2-b95fd4373e8e6b6e55bdd03e597587d2_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='512' height='512'></svg>)![](https://pic3.zhimg.com/v2-883d33a38f1b4f558fc9e4d06609294a_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='512' height='512'></svg>)
训练非常快。在 GTX1070 上 1 个小时就很清晰。稍后再调清晰一些写写做法。

数据集是 Trillion Pair，其中图片有点旧，也似乎少了一些人（例如，缺杨幂，有迪丽热巴，缺范冰冰，有李冰冰，缺梅西，有C罗，缺费德勒，有纳达尔...）。


