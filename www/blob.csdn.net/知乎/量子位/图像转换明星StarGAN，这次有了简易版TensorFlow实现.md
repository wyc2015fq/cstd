# 图像转换明星StarGAN，这次有了简易版TensorFlow实现 - 知乎
# 



> 安妮 编译整理
量子位 出品 | 公众号 QbitAI

半年前AI圈内红红火火的StarGAN模型有新实现出炉。

昨天，网友taki0112公布了一个简易版StarGAN TensorFlow实现，不到一天时间就在Reddit论坛上热度暴增到700。

因为实现的效果，看起来真的还不错：
![](https://pic3.zhimg.com/v2-f2b79de914de740c64e1136335a23c9a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='578'></svg>)△ 女性头像上的实现效果![](https://pic1.zhimg.com/v2-4a48fac3e99926b1ae3f0818c53e08ec_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='587'></svg>)△ 男性头像上的实现效果
仅仅给出一张原始头像，这个简易实现的GAN就能合成出染发、变性、衰老的模样。“性别和年龄的变化是微妙的，但结果相当不错。”网友Valiox评价。

## **本就是Star的StarGAN**

StarGAN由高丽大学、韩国Clova AI研究院Yunjey Choi、Minje Choi等人在去年年底提出，在多领域的图像迁移学习研究中确实是个效果不错的明星。



![](https://pic4.zhimg.com/v2-215c07d25e0b925dce9ab00f5df7bcb7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='277'></svg>)



在图像翻译领域，无论是Pix2Pix还是CycleGAN都只能解决一对一的问题，即从一个领域到另一个领域的转换。

若要实现多领域的转换，则每个领域都需重新训练模型，而星型结构的StarGAN实现了同一生成网络学习多个域之间映射关系，高效解决多领域间图像转换问题。



![](https://pic1.zhimg.com/v2-5fa3a25fff18d5751f6e30f77b52526c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='519' height='272'></svg>)△ 左：传统迁移模型结构；右：StarGAN结构
StarGAN的转换效果非常逼真，输入一张头像，就能看到这个喜怒哀乐的样子。



![](https://pic1.zhimg.com/v2-25c6ecf76e5ac31091c97cd9e09b8d8c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='293'></svg>)△ StarGAN转换效果
## **你要的资料来了**

关于这个简易版的TensorFlow具体实现，可在作者GitHub中查看：

[https://github.com/taki0112/StarGAN-Tensorflow](https://link.zhihu.com/?target=https%3A//github.com/taki0112/StarGAN-Tensorflow)

此外，附StarGAN论文地址：

[https://arxiv.org/pdf/1711.09020.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1711.09020.pdf)

论文原作的实现：

[https://github.com/yunjey/StarGAN](https://link.zhihu.com/?target=https%3A//github.com/yunjey/StarGAN)

预训练权重地址：

[https://drive.google.com/open?id=1ezwtU1O_rxgNXgJaHcAynVX8KjMt0Ua-](https://link.zhihu.com/?target=https%3A//drive.google.com/open%3Fid%3D1ezwtU1O_rxgNXgJaHcAynVX8KjMt0Ua-)

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


