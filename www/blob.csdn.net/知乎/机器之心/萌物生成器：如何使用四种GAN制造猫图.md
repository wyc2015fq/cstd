# 萌物生成器：如何使用四种GAN制造猫图 - 知乎
# 



> *生成对抗生成网络（GAN）被认为是近年来机器学习界的一大发现，在它的基础上开发各种新算法是很多科研人员努力的方向。近日，毕业于魁北克大学的 Alexia Jolicoeur-Martineau 开始尝试使用猫咪图片来训练不同的对抗生成网络，并把代码和结果公开。目前他已经测试了四种不同 GAN 的效果，Meow Generator 引起了众多学者的兴趣。有人打趣地评论道：由于网络中存在大量猫图，这种训练的数据集简直是最容易获得的。*
![](https://pic4.zhimg.com/v2-33ab6209ccd2d73b98a4662ceb752527_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='880' height='459'></svg>)



项目 GitHub：[https://github.com/AlexiaJM/Deep-learning-with-cats](https://link.zhihu.com/?target=https%3A//github.com/AlexiaJM/Deep-learning-with-cats)


我尝试使用几种对抗生成网络（GAN）来生成猫脸，其中包括 DCGAN、WGAN 和 WGAN-GP，以及低和高分辨率。训练模型则使用 CAT 数据集（是的，真的有这么个东西）。这一数据集包含 10000 余张猫的图片。在训练之前，我们必须让图片集中于正面猫脸，剔除数据集中其他方向的图片（作者是手动挑选的，这花了好几个小时...）。最后，用于训练的图片为 9304 张大于 64×64 像素的图片，以及 6445 张大于 128×128 的图片。



CAT 数据集链接：[https://web.archive.org/web/20150703060412/http://137.189.35.203/WebUI/CatDatabase/catData.html](https://link.zhihu.com/?target=https%3A//web.archive.org/web/20150703060412/http%3A//137.189.35.203/WebUI/CatDatabase/catData.html)

**DCGAN**


DCGAN 生成器在两到三个小时后就可以收敛出非常逼真的图像，只用了 209 个 epoch，但为了让收敛效果更佳，我们需要做适当的调整。你必须为鉴别器与生成器选择单独的学习速率，以避免它们有一方表现过于出色——这需要非常小心的平衡，不过一旦成功，你就会得到收敛。在 64×64 像素的图片中，鉴别器的最佳学习率是 0.00005，生成器的则是 0.0002。这样就不会出现模型崩溃，得到真正可爱的图片了！




![](https://pic2.zhimg.com/v2-bfc7e26648f75ca9b1ae8341c129b24d_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='530' height='530'></svg>)



**高分辨率 DCGAN 和 SELU**


初步尝试 DCGAN 生成 128×128 像素猫图的所有努力都失败了。但是，把批量规范化（batch normalization）的方法和 ReLU 用 SELU 代替问题就轻松地解决了，这让我可以缓慢（需要六个小时以上）但稳定地收敛和此前相同的学习速率。SELU 是自标准化的，因此不需要进行批量规范化。SELU 论文是在 2017 年 6 月 8 日出现的，目前还没有太多使用它的 GAN 研究，这种方法看起来可以大幅度提高 GAN 的稳定性。



在结果中我们可以看到，猫不像之前那么好看了，还会出现大量类似面孔的黑猫。这主要是因为样本大小为 N = 6445 而非原来的 N = 9304（这次实验只对大于 128×128 的图片进行了训练）。不过，其中的一些结果看起来质量很高，而且因为分辨率的提升，该结果仍可以被视为有意义。 SELU 论文《Self-Normalizing Neural Networks》链接：[https://arxiv.org/abs/1706.02515](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1706.02515)



![](https://pic2.zhimg.com/v2-521f3967806784adb917d7d3fb2d753d_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1024' height='1024'></svg>)
**WGAN**


WGAN 生成器收敛的速度非常慢（需要花费四五个小时，600 以上个 epoch），且只会在使用 64 个隐藏节点的情况下收敛。我无法让生成器在 128 个隐藏节点时收敛。使用 DCGAN 意味着必须进行大量学习速率的调整，但你可以快速看到是否收敛的结果（鉴别器损失为 0，或生成器损失为 0）；但在 WGAN 里，你需要运行完大量 epoch 之后才能得到结果。



看起来，WGAN 的结果有些崩坏，很多猫的两只眼睛都呈现异色，只睁一只眼睛，有些还长着奇怪的鼻子。总体而言，这样的结果并不如 DCGAN，但考虑到 WGAN 的网络复杂性更低，这样的比较或许不太公平。另外，这些结果看起来也陷入了局部最佳的状态，总体而言，WGAN 令人失望。



WGAN-GP（一种改进版本的 WGAN，用正则化代替了权重裁剪）或许可以解决这些问题。在 Gulrajani 等人（2017）的一篇论文中，他们成功训练了 101 层的神经网络来生成图片！所以我怀疑训练一个有 5 层和 128 个隐藏节点的猫生成器还差得很远。Adam 优化器的一些特性也可以降低模型崩溃并陷入错误局部最佳陷阱的可能性。这些方法可能会帮助 WGAN，因为 DCGAN 和 WGAN-GP 都使用了 Adam。




![](https://pic3.zhimg.com/v2-8222180f2243debfb015da7c234f1fce_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='530' height='530'></svg>)
**WGAN-GP**


WGAN-GP 生成器收敛的速度非常慢（超过 6 个小时），但它几乎已经帮你完成了所有设置。它是开箱即用的，无需任何调试。你也可以增加或减少学习速率，这不会造成太多问题。所以在这个任务中，WGAN-GP 赢得了我的青睐。



如下图所示，WGAN-GP 生成的猫图具有多样性，也没有明显的模型崩溃，这是与 WGAN 相比最大的提升。但另一方面，这些猫看起来十分粗糙，就像是用高分辨率的屏幕在看低分辨率的图片，我也不知道这是为什么。或许这是 Wasserstein 损失函数的特点？我认为使用不同的学习率和体系结构将可以提升一下性能。不过，这还需要进一步的尝试，肯定还有很大的潜力。



WGAN-GP 论文《Improved Training of Wasserstein GANs》链接：[https://arxiv.org/abs/1704.00028](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1704.00028)



![](https://pic4.zhimg.com/v2-648b557dbb463b3ea0b663ca77189413_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='530' height='530'></svg>)
**LSGAN（Least Squares GAN）**


LSGAN 是一种略为不同的方法，在这里我们得试图最小化鉴别器输出和分配标签的平方距离；研究者建议：对于真实图像为 1，对于鉴别器更新时的假图像使用 0。Hejlm 等人（2017）的研究则认为：对于真实图像为 1，对于鉴别器更新时的假图像使用 0，而生成器更新时的假图像用 0.5 以寻找边界。



作者还没有时间运行整个流程，但它看起来非常稳定，而且可以输出漂亮的猫图。尽管它总体稳定，有一次损失函数和梯度还是爆炸了，猫变成了无意义的图片。如下图所示。




![](https://pic4.zhimg.com/v2-e3321e800a2f817ff5d6be70e0616d9b_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='982' height='495'></svg>)
所以，LSGAN 不是完全稳定的。通过 Adam 选择更好的超参数可以帮助解决这个问题。在失败时（很少见），它可以让你无须像 DCGAN 那样调整学习速率，这个方法可以让你生成漂亮的猫图。



LSGAN 论文《Least Squares Generative Adversarial Networks》链接：[https://arxiv.org/abs/1611.04076](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1611.04076)


Update：LSGAN 论文的第一作者毛旭东（香港城市大学）给我发来使用 LSGAN 生成的 128×128 分辨率猫图（如下图），结果证明了这种方法至少可以生成和 DCGAN 一样质量的结果。
![](https://pic3.zhimg.com/v2-02ce8cd6556cc1de569d56aea0da5986_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1024' height='1024'></svg>)



选自Word Press  **机器之心编译**


