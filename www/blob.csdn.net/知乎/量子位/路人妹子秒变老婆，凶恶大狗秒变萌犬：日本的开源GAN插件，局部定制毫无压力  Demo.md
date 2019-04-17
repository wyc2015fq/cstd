# 路人妹子秒变老婆，凶恶大狗秒变萌犬：日本的开源GAN插件，局部定制毫无压力 | Demo - 知乎
# 



> 花栗子 发自 凹非寺
量子位 出品 | 公众号 QbitAI

这是一只面露凶光的哈士奇。
![](https://pic3.zhimg.com/v2-a3a8b4cac1f9f37b0129841a31aafbb6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='656' height='652'></svg>)



但它凶不过三秒，只要在眼睛上刷一刷：



![](https://pic4.zhimg.com/v2-44323eae9a8091a1fc63f085c68d6eff_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='575' height='366'></svg>)



就回归了呆萌的本性。

机智的你可能已经发现，是左下角那只 (有点像博美的) 小狗，帮它找到了自我。

既然眼睛圆润了，耳朵就没必要再挣扎，也刷一下吧：
![](https://pic3.zhimg.com/v2-59407549890bdf628e197c93d0bc27f2_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='571' height='364'></svg>)



一键换上梗犬的耳朵，更加温柔了。

还有，只要让忧郁的小猎犬保持皱眉，再把嘴张开，便可以获得**职业假笑**。



![](https://pic3.zhimg.com/v2-4773a77dc8504a5044ebab4d422c35ba_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='850' height='428'></svg>)



想P哪里点哪里，这是东京大学和日本独角兽Preferred Networks开源的“插件”，可调教各种GAN：包括**BigGAN**和**StyleGAN**，也不用重新训练。

另外，既然是来自东瀛的AI，当然可以定制老婆啦：
![](https://pic2.zhimg.com/v2-1ff1c9844ad427c39936da78c413f421_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='750' height='1186'></svg>)
已为您生成，棕瞳的猫女。

**有Demo可以玩，还有开源代码。**

不过为什么，修改**局部**也可以这样天衣无缝？

## 想修哪里点哪里

团队用了两种方法。

一种是**标签拼贴** (Label Collage) 。拼贴大法名叫**sCBN**。

通俗地说，图上的各种色彩代表不同的标签，共同组成一张**标签图** (Label Map) 。比如，红色是狐狸的头：



![](https://pic2.zhimg.com/v2-54fb13208b49fb98421fa3b31effff7d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='596' height='292'></svg>)



修改一个标签，把图上的某个部分，标成自己想要的样子：**比如把狐狸的头，标成狗头**。它就真的变成了狗：



![](https://pic2.zhimg.com/v2-d005ca9ef71835831fefce73fbe659e5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='592' height='292'></svg>)△“绿色”的后背部分也修改了



这就是从标签图上生成的结果。

做法很简单，只要在归一化参数上，应用一下位置相关的仿射变换 (Affine-Transformations) ，就能在空间上改变语义信息，**不用重新训练一只GAN**，直接用预训练网络就可以。

这样，把金毛变成狮子，人类依然笑得灿烂：



![](https://pic3.zhimg.com/v2-fd7d281f551658400f2b06ef18d7f84a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='972' height='480'></svg>)



就算不是动物，两岸青山相对出，中间的绿草如茵，也能变成碧波粼粼：



![](https://pic1.zhimg.com/v2-8475edebad9fca284f3884b193af8a40_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='672' height='680'></svg>)



但有时候，要修改的部分，对应的并不是单一的标签。

这时，便有了名叫**特征混合** (Feature Blending) 的第二种方法。

它不需要用标签图，直接把几张图拼起来就可以了。

一张是源图：



![](https://pic1.zhimg.com/v2-0e3f68b7fda058a46b4ee5420be75560_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='542' height='609'></svg>)



两张作为样本，老爷爷的胡子，和小朋友的刘海：




![](https://pic2.zhimg.com/v2-8b5e2b869325d172bb181334b4127799_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1036' height='604'></svg>)



然后，最初的男生就有了花白的胡子和棕色的头帘：



![](https://pic2.zhimg.com/v2-ec9226db5714187de8f20329d69e2751_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='544' height='624'></svg>)



原理是，虽然想要修改的部位，对应的不是某一个标签；**但图上的一个区域，依然可以对应到一组神经元**。

前段时间轰动的GAN Dissection算法，就是给网络做解剖，找到一组神经元，对应某个**中间特征** (Intermediate Feature) 。

特征混合，就是要加工各种中间特征，把它们无缝融合到一起。

你看，丹凤眼的面瘫少年：



![](https://pic2.zhimg.com/v2-eab3396aecb7d01f6260851ea56d6825_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='494' height='494'></svg>)



有了棕色的大眼睛，再加上愉快的嘴：




![](https://pic4.zhimg.com/v2-9800879dc25685090b4cdd728c24f8ab_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1032' height='512'></svg>)



画风就全然不一样了。




![](https://pic2.zhimg.com/v2-7984d4940e20ad9698e6dd45cde1dae9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='484' height='504'></svg>)



你一定还记得，开头的老婆也是这样生成的：



![](https://pic4.zhimg.com/v2-d7547c56799a346b69b7db1af9fe106b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='988' height='954'></svg>)



从前，想要红瞳的妹子，只能截取稍纵即逝的缘分：



![](https://pic1.zhimg.com/v2-84cdd941874d5b40c9da8c55963fb238_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='400' height='400'></svg>)△ 这是英伟达StyleGAN的作品
现在，想要怎样的发型、瞳色、表情的老婆，都可以任意定制了。

只要把“插件”放到各种预训练的GAN上。

## **开源了**

上周，新版本的论文已经发布了。

算法也开源了，近期可能有更新。



![](https://pic2.zhimg.com/v2-6e51ec4c483e8722ab2adf470ce2a98d_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='623' height='211'></svg>)



想定制老婆的同学，可以使用Demo：

[https://colab.research.google.com/github/quolc/neural-collage/blob/master/NeuralCollage_demo.ipynb](https://link.zhihu.com/?target=https%3A//colab.research.google.com/github/quolc/neural-collage/blob/master/NeuralCollage_demo.ipynb)

代码传送门：
[https://github.com/quolc/neural-collage](https://link.zhihu.com/?target=https%3A//github.com/quolc/neural-collage)

论文传送门：
[https://arxiv.org/pdf/1811.10153v2.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1811.10153v2.pdf)

— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤


