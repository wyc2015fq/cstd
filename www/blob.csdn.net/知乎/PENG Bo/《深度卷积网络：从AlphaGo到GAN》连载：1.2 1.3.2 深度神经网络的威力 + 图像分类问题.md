# 《深度卷积网络：从AlphaGo到GAN》连载：1.2~1.3.2 深度神经网络的威力 + 图像分类问题 - 知乎
# 



新年好。继续在知乎的连载。这里是接近完成的草稿，正式出版时会是高清全彩色印刷。请立即关注我和本专栏吧。记得点个赞。

UPDATE: 已经在印刷啦，介绍和预订链接：
[PENG Bo：《深度卷积网络·原理与实践》，结合可读性与实用性的全彩印深度学习入门（多图）​zhuanlan.zhihu.com![图标](https://pic3.zhimg.com/v2-866e668667a6a3a8f05b5e043c752fba_ipico.jpg)](https://zhuanlan.zhihu.com/p/36451054)
**p.s. 由于版权的缘故，您可以给出到本书知乎连载的连接，但请不要在任何网站或公众号等等以任何形式直接转载或摘选本书的正文内容，非常感谢您的关注。**
![](https://pic2.zhimg.com/v2-fd68870a236004707a9aaecafba34221_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='779' height='1381'></svg>)![](https://pic3.zhimg.com/v2-bf2b2fc7586a0c43e97665636cfd5af6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='778' height='1257'></svg>)![](https://pic4.zhimg.com/v2-353999f07f60a1d06ba6010c1b47a07b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='778' height='1419'></svg>)![](https://pic4.zhimg.com/v2-dc242a1c98f07f306345058357ee19e3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='778' height='1180'></svg>)![](https://pic4.zhimg.com/v2-f0a61992b5fa5d8750ce0bcebbc744db_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='856' height='1144'></svg>)![](https://pic4.zhimg.com/v2-ecc5c80e4ee29a3f8fafedbb39f79c0b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='816' height='1437'></svg>)
## **【待续~ 请关注我和本专栏吧，记得点个赞。】**

深度学习的发展日新月异，笔者只是略知皮毛，书中难免有错漏之处，恳请读者不吝指正，如果您抓到bug，书的前言中会向您致谢。

p.s. 如果希望现在支持本书，请[点击我公司的主页 Blink · 禀临科技](https://link.zhihu.com/?target=http%3A//withablink.com/) ，看看我们的实体硬件。保证良心好产品，还有超赞的售后和服务（有问题直接找我）。

## **p.s. 由于版权的缘故，您可以给出到本书知乎连载的连接，但请不要在任何网站或公众号等等以任何形式直接转载或摘选本书的正文内容，非常感谢您的关注。**


