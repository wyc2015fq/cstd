# 《深度卷积网络：从AlphaGo到GAN》连载：2.2.8 ~ 2.3.3 - 知乎
# 



继续在知乎的连载。这里是接近完成的草稿，正式出版时会是高清全彩色印刷。请立即关注我和本专栏吧。如果觉得有帮助，记得点个赞噢。

UPDATE: 已经在印刷啦，介绍和预订链接：
[PENG Bo：《深度卷积网络·原理与实践》，结合可读性与实用性的全彩印深度学习入门（多图）​zhuanlan.zhihu.com![图标](https://pic3.zhimg.com/v2-866e668667a6a3a8f05b5e043c752fba_ipico.jpg)](https://zhuanlan.zhihu.com/p/36451054)
**p.s. 由于版权的缘故，您可以给出到本书知乎连载的连接，但请不要在任何网站或公众号等等以任何形式直接转载或摘选本书的正文内容，非常感谢您的关注。**
![](https://pic2.zhimg.com/v2-3e61715a5d5104a694e5348ed719c699_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='1434'></svg>)![](https://pic3.zhimg.com/v2-f4731fb096eceecdfe464a405a54a212_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='1391'></svg>)![](https://pic2.zhimg.com/v2-2ae8bf4c0d9cb0e128e64afbb50fec31_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='1353'></svg>)![](https://pic1.zhimg.com/v2-e2cb6689cde1882ab0db997f78478538_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='583' height='1049'></svg>)![](https://pic4.zhimg.com/v2-e039c42d4088c63d56e1b809771e163f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='820' height='1404'></svg>)![](https://pic4.zhimg.com/v2-634d166c93a57a0b352393411d6090af_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='820' height='1430'></svg>)![](https://pic2.zhimg.com/v2-2fa349f71f5bec3b2d75841a371a6d35_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='1409'></svg>)
**【待续~ 请关注我和本专栏吧，如果觉得有帮助，记得点个赞噢。】**

深度学习的发展日新月异，笔者只是略知皮毛，书中难免有错漏之处，恳请读者不吝指正，如果您抓到bug，书的前言中会向您致谢。

p.s. 如果希望现在支持本书，请[点击我公司的主页 Blink · 禀临科技](https://link.zhihu.com/?target=http%3A//withablink.com/) ，看看我们的实体硬件。保证良心好产品，还有超赞的售后和服务（有问题直接找我）。

## **p.s. 由于版权的缘故，您可以给出到本书知乎连载的连接，但请不要在任何网站或公众号等等以任何形式直接转载或摘选本书的正文内容，非常感谢您的关注。**


