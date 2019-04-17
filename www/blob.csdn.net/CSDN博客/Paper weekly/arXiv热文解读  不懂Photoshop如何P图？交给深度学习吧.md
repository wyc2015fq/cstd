# arXiv热文解读 | 不懂Photoshop如何P图？交给深度学习吧 - Paper weekly - CSDN博客





2019年02月27日 08:42:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：119









![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)




人脸编辑可以在大多数图像编辑软件上得到实现，但是这都需要专业知识，例如了解在特定情况下使用哪些特定工具，以便按照想要的方式有效地修改图像，同时操作图像编辑软件也是耗时的。




基于深度学习下图像编辑得到越来越多的重视和应用，在 GAN 的推动下，图像风格转换、图像修复、图像翻译等等在近几年有了长足的发展。这篇文章将介绍**基于 GAN 损失的端到端可训练生成网络**，在人脸修复上取得了很棒的结果，同时该模型也适用于有趣的人脸编辑。




作者丨武广


学校丨合肥工业大学硕士生

研究方向丨图像生成




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkcdBwCFGTNLxtLmB6SB7eNrliaGppm2yDA5XdmtRhBvoGdLW5FVzzVeGUQIJuXQXnCzRYic6V77VWw/640?wx_fmt=png)




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkcdBwCFGTNLxtLmB6SB7eNfm5V0afu0GicicrwISfibibgiaoRUaadbjvPhRpibicR7E7fZ8BdT3SnfdpBQ/640?wx_fmt=png)




# 论文引入




提到图像编辑，一定要说的就是 Photoshop 这款软件，近乎可以处理日常所有的照片，但是 PS 不是这么容易操作的，精通 PS 更是需要专业知识了。如何让小白完成在图像上勾勾画画就能实现图像的编辑？这个任务当然可以交给深度学习来实现了。 




生成对抗网络（GAN）的发展，促进了图像生成下一系列研究的发展。图像编辑下图像修复是一个难点，这对于图像编辑软件来说也是很困难的。近几年在深度学习发展下，图像修复在不断进步，最典型的方法是使用普通（方形）掩模，然后用编码器-解码器恢复掩蔽区域，再使用全局和局部判别器来估计结果的真假。




然而，该系统限于低分辨率图像，并且所生成的图像在掩蔽区域的边缘不能很好的与原图衔接。尽管 ***Deepfillv2***[1]、***GuidedInpating***[2]、***Ideepcolor ***[3]、***FaceShop***[4] 在不断改进实现结果，但是对于深度学习处理图像修复上的难点还是依旧存在，总的来说**主要的挑战有两个：**1）图像在恢复的部分上具有不和谐的边缘；2）如果图像太多区域被覆盖，修复的图像将会不合理。 




本文要解读的论文为了解决上述限制，提出了 ***SC-FEGAN***，它具有完全卷积网络，可以进行端到端的训练。提出的网络使用 ***SN-patchGAN***[1] 判别器来解决和改善不和谐的边缘。**该系统不仅具有一般的 GAN 损失，而且还具有风格损失，即使在大面积缺失的情况下也可以编辑面部图像的各个部分。**




**这篇论文发布在 arXiv 不到10天，其源码便已标星破千。**文章的人脸编辑效果十分逼真，先一睹为快。




![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkcdBwCFGTNLxtLmB6SB7eNomAKRnMG8DgdHHPCEiaRb4YBfqjn0JhDX3hEzK6iaszlGuD6THe9R51A/640?wx_fmt=gif)




**总结一下 SC-FEGAN 的贡献：**




1. 使用类似于*** U-Net*** [5] 的网络体系结构，以及 ***gated convolutional layers***[1]。对于训练和测试阶段，这种架构更容易，更快捷，与粗糙网络相比，它产生了优越而细致的结果。 




2. 创建了解码图，颜色图和草图的自由格式域数据，该数据可以处理不完整图像数据输入而不是刻板形式输入。 




3. 应用了 SN-patchGAN 判别器，并对模型进行了额外的风格损失。该模型适应于擦除大部分的情况，并且在管理掩模边缘时表现出稳健性。它还允许生成图像的细节，例如高质量的合成发型和耳环。




# 训练数据处理




决定模型训练好坏的一个重要因素就是训练数据的处理，SC-FEGAN 采用 ***CelebA-HQ***[6] 数据集，并将图片统一处理为 512 x 512 大小。 




为了突显面部图像中眼睛的复杂性，文章使用基于眼睛位置掩模来训练网络。这主要用在掩码的提取上，当对面部图像进行训练时，随机应用一个以眼睛位置为起点的 free-form mask，以表达眼睛的复杂部分。此外，文章还使用 GFC 随机添加了人脸头发的轮廓。算法如下：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkcdBwCFGTNLxtLmB6SB7eN2AaZpNIUuVYU3TjC30gKSGFt4KzSPiczeYImHn90GDlNzZHOp0dPiaicg/640?wx_fmt=png)




文章的又一个创新是在提取图像的草图和颜色图，使用 ***HED 边缘检测器*** [7] 生成与用户输入相对应的草图数据，之后，平滑曲线并擦除了小边缘。创建颜色域数据，首先通过应用大小为 3 的中值滤波，然后应用双边滤波器来创建模糊图像。之后，使用 ***GFC***[8] 对面部进行分割，并将每个分割的部分替换为相应部分的中间颜色。最后还提取噪声表示。最终处理完得到的数据可由下图展示，上面一行作为真实标准，下面一行为处理得到的输入数据。




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkcdBwCFGTNLxtLmB6SB7eNlib5DibrB6o0lyTqEOy54MTMyL9obGorNcibag0JnWAuwLvXx1bjD7Blw/640?wx_fmt=png)

# 模型结构




SC-FEGAN 采用的网络设计结构与 U-Net 类似，上采样的思路是 U-Net 的那一套，通过 Concat 连接下采样的特征提取完成上采样，整体框架如下所示：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkcdBwCFGTNLxtLmB6SB7eNpcvQ1p9omjVJgNek3wtoCfEGMWdnZC2wRNZMEFLjnPpEVZMln4CGkg/640?wx_fmt=png)




网络的整体很好理解，图示也很清晰，我们强调一下网络的输入。 




不像传统的图像到图像的模型输入是 512 x 512 x 3 的 RGB 输入，这篇论文的输入是尺寸为 512 × 512 × 9 的张量，这个张量是由 5 张图片图片构成的。




首先是被覆盖残缺的 RGB 图像，也就是我们在图片上勾勾画画的图像，其尺寸为 512 x 512 x 3；其次是提取得到的草图 512 x 512 x 1；图像的颜色域 RGB 图 512 x 512 x 3；掩码图 512 x 512 x 1；最后就是噪声对应的 512 x 512 x 1。按照通道连接的话得到最终的模型输入 512 x 512 x 9。 




卷积层采用的是 gated convolution，使用 3 x 3 内核，下采样采取 2 个步幅内核卷积对输入进行 7 次下采样，上采样通过 U-Net 思路进行解码得到编辑好的图像。解码得到的图像依旧是 5 张图片，包括修复的 RGB 图像、草图、颜色图、掩码图、噪声图。通过判别器和真实的 5 张图片进行 SN-patchGAN 结构设计进行真假判断，优化模型。




在整个网络的卷积层后应用局部信号归一化（LRN），生成器采用 Leaky Relu，判别器通过 SN 限制，并且施加梯度惩罚。SC-FEGAN 的损失函数由 5 部分组成，像素损失![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkcdBwCFGTNLxtLmB6SB7eNLPibyelS7Pk6vfmH5jxCRdGA9J0GWTPf9GB4xnjQnkNqIDXibhNWUfOg/640?wx_fmt=png)、感知损失、感知损失![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkcdBwCFGTNLxtLmB6SB7eNa1JMChCcatrUtpJm2QmC8OhdtEGg7aMS1zNEoiafjRrX1bz9cV2j2kw/640?wx_fmt=png)、风格损失、风格损失![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkcdBwCFGTNLxtLmB6SB7eNePqWRSfpn75Rx8yAWr9lMJ5dDNxjvrta2cyIkiayFZqMR685SiaSHnFw/640?wx_fmt=png)、总方差损失、总方差损失![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkcdBwCFGTNLxtLmB6SB7eNxnJY3un2RsaJvlp46xicREq5wLK3w82IzicGHhJYVQZyktWSOwiaMM67w/640?wx_fmt=png)以及 GAN 的对抗损失。




先来看一下 GAN 的对抗损失，文章采用的 WGAN 的损失设计：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkcdBwCFGTNLxtLmB6SB7eNwftIoty62pNLnQpm0Xicjnk7OZHIo3nv1SRC4ItAYAKxWyUd0Ooqcdw/640?wx_fmt=png)




其中![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkcdBwCFGTNLxtLmB6SB7eNOABzFTGEia14oFAdaC6uLXfq6UJg8k14qwGiaEYsO1lxb2rsVoKKnZjw/640?wx_fmt=png)是生成器输出的![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkcdBwCFGTNLxtLmB6SB7eNHiajKe1DUr2FGh94z0kypEB665mfjlaRfDWktqllLC4NabB1ibGmd91A/640?wx_fmt=png)的完成图像（包括草图和颜色图以后的），也就是送入判别器判别的输入，最后判别器加上梯度惩罚项。




对于生成器希望判别器判错，故生成器损失最小值为 -1，判别器则希望真实判为真，生成判为假，上述损失还是很好理解的。生成器完整的损失为：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkcdBwCFGTNLxtLmB6SB7eNoNzUYpj7RniahjuL2MK6NokEEfV7CWSdP7tnibzcHCBaGqKicvsz2JYIw/640?wx_fmt=png)




像素损失保证图像编辑生成与真实的一致性：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkcdBwCFGTNLxtLmB6SB7eNdRUGZPc5ics5xfzBjjOE51ZSeW1yZpNWk8K8Z4ys27eX5huThMemV8Q/640?wx_fmt=png)




其中 Na 是特征 a 的数字元素，M 是二元掩模图，用于控制修复图像的像素，达到生成与真实的一致性。公式后一项给予擦除部分损失更多的权重。对于大区域下的擦除图像的修复时，风格损失和感知损失是必要的。感知损失在 GAN 中的应用已经很多了，就是对中间层进行特征差异损失，这里不做赘述。




风格损失使用 Gram 矩阵比较两个图像的内容，风格损失表示为：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkcdBwCFGTNLxtLmB6SB7eNseQbSwBPPbK2Qr1ibicbuNJUJ5SRaSqm7wHLquAoK0fTUF2vYrGicp1Vw/640?wx_fmt=png)




其中，![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkcdBwCFGTNLxtLmB6SB7eNAxlsGomTp6OibuHVSz94cPrX0Yfibku5D7dGUEVw2LJbnBU5QZD4KY2g/640?wx_fmt=png)，Θ 为网络的中间层，Gq(x) 是用于在中间的每个特征图上执行自相关的 Gram 矩阵，Gram 矩阵的形状是 Cq×Cq。总方差损失，反应的是相邻像素间是相似的，记为![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkcdBwCFGTNLxtLmB6SB7eN0zyrHwQQwlo2QVNxZw3ZKlsZGUzib1h2Bqkia5iabibLMvkWn2aKjQWUlg/640?wx_fmt=png)。其中：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkcdBwCFGTNLxtLmB6SB7eNUg0gSRhHAr8uOGe2X9zxy5fNicy17U0N75sO9FQ9RQibPoY8g1lKfR1w/640?wx_fmt=png)




对于列 row 也是一样，自此整体的损失函数构建完成，在实际实验中参数设置为 σ=0.05，β=0.001，γ=120，v=0.1，ϵ=0.001，θ=10。




# 实验




实验首先对比了使用 U-Net 框架设计的优势，对比的框架是 Coarse-Refined。论文测试了 Coarse-Refined 结构网络，并注意在一些需要精确输出的却是模糊的。




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkcdBwCFGTNLxtLmB6SB7eNobugnHxXWbyu6ib4ib7AZgBnkCBagxRyVNhaSPbxcB2yzD8P4ak2OnTg/640?wx_fmt=png)




FaceShop 已经显示出难以修改像整个头发区域那样的巨大擦除图像。由于感知和风格损失的加入，SC-FEGAN 在这方面表现更好。下图显示了有和没有 VGG 损失（感知和风格损失）的结果。




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkcdBwCFGTNLxtLmB6SB7eNAeVCLdwCA8qKZicYicsAvNXIRCgHBuo5ibGtjf8SOjlCmbibxqmT7yqz9Q/640?wx_fmt=png)




实验还与最近的研究 Deepfillv1 进行了比较，下图显示模型在使用自由形状的掩模在结构和形状的质量方面产生更好的结果。意味着没有附加信息，如草图和颜色，面部元素的形状和位置具有一定的依赖值。因此，只需提供附加信息即可在所需方向上恢复图像。




此外，即使输入图像被完全擦除，我们的 SC-FEGAN 也可以生成仅具有草图和颜色自由形式输入的人脸图像（感兴趣的可以参看论文中更多结果）。




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkcdBwCFGTNLxtLmB6SB7eNCtVKE4RZDT3Y7AkwzGDTEbzSSd99SMrCglfwO427vk0A1Wm7jGicn0Q/640?wx_fmt=png)




下图显示了草图和颜色输入的各种结果，它表明模型允许用户直观地编辑脸部图像功能，如发型，脸型，眼睛，嘴巴等。即使整个头发区域被删除，它也能够产生适当的结果。




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkcdBwCFGTNLxtLmB6SB7eN9UiaOy1fBoK4LicY9vPyztETNcBsl1SwWvs6oMmzOn7HcILKETfZuMuw/640?wx_fmt=png)




文章还做了给人物加上耳环的有趣实验，这种人脸编辑确实蛮有意思：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkcdBwCFGTNLxtLmB6SB7eNR1248PVLtGOY1G4T69BrHwaw3BJY7DiciaxmBY5DXxtu2QHQFur0EXmA/640?wx_fmt=png)




# 实验




**SC-FEGAN 提出了一种新颖的图像编辑系统**，用于自由形式的蒙版，草图，颜色输入，它基于具有新颖 GAN 损失的端到端可训练生成网络。与其他研究相比，网络架构和损失功能显着改善了修复效果。




论文基于 celebA-HQ 数据集进行了高分辨率图像的训练，并在许多情况下显示了各种成功和逼真的编辑结果。实验证明了系统能够一次性修改和恢复大区域，并且只需要用户细致的勾画就可以产生高质量和逼真的结果。




模型的框架采用 U-Net 思想，在 Decoder 端结合 Encoder 端特征实现上采样过程下精确图像的生成。引入感知损失和风格损失更是帮助模型实现了大区域擦除的修复。




在判别器采用 SN-patchGAN 思想，使得恢复的图像在边缘上更加和谐，生成器和判别器都结合了图像的草图、颜色图和掩模图进行判断，使得修复的图像更加逼真，同时也是论文的一个创新。实验上的炫彩也是使得源码得到了广泛的关注。




# 参考文献




 [1] J. Yu, Z. Lin, J. Yang, X. Shen, X. Lu, and T. S. Huang.Free-form image inpainting with gated convolution. arXiv preprint arXiv:1806.03589, 2018. 2, 3, 4, 6, 7 

[2] Y. Zhao, B. Price, S. Cohen, and D. Gurari. Guided image inpainting: Replacing an image region by pulling content from another image. arXiv preprint arXiv:1803.08435, 2018. 2 

[3] R. Zhang, J.-Y. Zhu, P. Isola, X. Geng, A. S. Lin, T. Yu, and A. A. Efros. Real-time user-guided image colorization with learned deep priors. arXiv preprint arXiv:1705.02999, 2017.2, 3 

[4] T. Portenier, Q. Hu, A. Szabo, S. Bigdeli, P. Favaro, and M. Zwicker. Faceshop: Deep sketch-based face image editing. arXiv preprint arXiv:1804.08972, 2018. 2, 3, 4, 6, 7 

[5] O. Ronneberger, P. Fischer, and T. Brox. U-net: Convolutional networks for biomedical image segmentation. In International Conference on Medical image computing andcomputer-assisted intervention, pages 234–241. Springer,2015. 2, 3, 4 

[6] T. Karras, T. Aila, S. Laine, and J. Lehtinen. Progressive growing of gans for improved quality, stability, and variation.arXiv preprint arXiv:1710.10196, 2017. 3, 4 

[7] S. ”Xie and Z. Tu. Holistically-nested edge detection. In Proceedings of IEEE International Conference on Computer Vision, 2015. 3 

[8] Y. Li, S. Liu, J. Yang, and M.-H. Yang. Generative face completion. In The IEEE Conference on Computer Vision and Pattern Recognition (CVPR), volume 1, page 3, 2017. 1,3, 4




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)







**点击以下标题查看更多往期内容：**




- 
[Airbnb实时搜索排序中的Embedding技巧](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247494488&idx=1&sn=452ac80c593a9b31252031eac38d0e01&chksm=96ea34d8a19dbdce940ed25bb93507aa6c4d118f84dd0bb965b060f232fe5d41894bbc9edcb6&scene=21#wechat_redirect)

- 
[图神经网络综述：模型与应用](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493906&idx=1&sn=15c9f18a1ce6baa15dc85ecb52e799f6&chksm=96ea3692a19dbf847c1711e6e194ad60d80d11138daf0938f90489a054d77cfd523bee2dc1d2&scene=21#wechat_redirect)

- 
[近期值得读的10篇GAN进展论文](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493987&idx=1&sn=ce1bcdce28e78f4a307743e389f42b10&chksm=96ea36e3a19dbff5cff7f4f1c9d9fc482bb2144d80566319b3d26bce4d9ab80689d38ab2e427&scene=21#wechat_redirect)

- 
[F-Principle：初探理解深度学习不能做什么](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247494694&idx=1&sn=7020fb834ce8307f27ce9c072047d37d&chksm=96ea33a6a19dbab0a6585daa00d5b5c65501dd633fa677c80541fad0e170d92baffe379315c3&scene=21#wechat_redirect)


- 
[基于深度学习和心脏影像预测生存概率](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247495241&idx=1&sn=e96b65c24c87cdf97ca35c064014fcae&chksm=96ea31c9a19db8df92dcb112d601fefc2114a0197db85e7c805e9743221749ef80fbbf6783a6&scene=21#wechat_redirect)

- 
[异构信息网络表示学习论文解读](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247495219&idx=1&sn=b3a29b833fe8438e12b600650ec0245a&chksm=96ea31b3a19db8a5a335cc445f04eb13d5b1ee5451d688544240cce27dbc953993da4e842ab6&scene=21#wechat_redirect)

- 
[小米拍照黑科技：基于NAS的图像超分辨率算法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247495166&idx=1&sn=a158e603651bc4f26836151a9113e856&chksm=96ea327ea19dbb68b8987aca041bb21579a35b1c679e91fd2368c7f2fb7acd58508cd531bdfe&scene=21#wechat_redirect)

- 
[深度思考 | 从BERT看大规模数据的无监督利用](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247494978&idx=1&sn=529b8f661b13c7b6b729e9a926b0737a&chksm=96ea32c2a19dbbd43456086a2186480fa548fa982d536564e167739ef46282eaf6fca7316ced&scene=21#wechat_redirect)

- 
[FSRNet：端到端深度可训练人脸超分辨网络](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247494929&idx=1&sn=b79ee7da68c089655bdbbd377266a9dc&chksm=96ea3291a19dbb8747841a2045e8d631389e610b0dc8e6a21bc05d9e736bf88743630fa51b57&scene=21#wechat_redirect)

- 
[超详综述：GAN在图像生成上的应用](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247494696&idx=1&sn=f393f4c2619d4a5169ddb1a6bd7620d3&chksm=96ea33a8a19dbabea7cf0c7bc84e9ea8a50aafa445a01f69bdece69dfb7b521f8471c87a759e&scene=21#wechat_redirect)











**![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)****#****投 稿 通 道#**

** 让你的论文被更多人看到 **





如何才能让更多的优质内容以更短路径到达读者群体，缩短读者寻找优质内容的成本呢？ **答案就是：你不认识的人。**



总有一些你不认识的人，知道你想知道的东西。PaperWeekly 或许可以成为一座桥梁，促使不同背景、不同方向的学者和学术灵感相互碰撞，迸发出更多的可能性。 




PaperWeekly 鼓励高校实验室或个人，在我们的平台上分享各类优质内容，可以是**最新论文解读**，也可以是**学习心得**或**技术干货**。我们的目的只有一个，让知识真正流动起来。




📝 **来稿标准：**

• 稿件确系个人**原创作品**，来稿需注明作者个人信息（姓名+学校/工作单位+学历/职位+研究方向） 

• 如果文章并非首发，请在投稿时提醒并附上所有已发布链接 

• PaperWeekly 默认每篇文章都是首发，均会添加“原创”标志



**📬 投稿邮箱：**

• 投稿邮箱：hr@paperweekly.site

• 所有文章配图，请单独在附件中发送 

• 请留下即时联系方式（微信或手机），以便我们在编辑发布时和作者沟通










🔍




现在，在**「知乎」**也能找到我们了

进入知乎首页搜索**「PaperWeekly」**

点击**「关注」**订阅我们的专栏吧







**关于PaperWeekly**





PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/640?)

▽ 点击 | 阅读原文| 下载论文 & 源码




