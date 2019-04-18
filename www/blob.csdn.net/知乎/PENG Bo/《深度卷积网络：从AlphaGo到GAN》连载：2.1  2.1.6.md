# 《深度卷积网络：从AlphaGo到GAN》连载：2.1 ~ 2.1.6 - 知乎
# 



继续在知乎的连载。这里是接近完成的草稿，正式出版时会是高清全彩色印刷。请立即关注我和本专栏吧。如果觉得有帮助，记得点个赞噢。

UPDATE: 已经在印刷啦，介绍和预订链接：
[PENG Bo：《深度卷积网络·原理与实践》，结合可读性与实用性的全彩印深度学习入门（多图）​zhuanlan.zhihu.com![图标](https://pic3.zhimg.com/v2-866e668667a6a3a8f05b5e043c752fba_ipico.jpg)](https://zhuanlan.zhihu.com/p/36451054)
**p.s. 由于版权的缘故，您可以给出到本书知乎连载的连接，但请不要在任何网站或公众号等等以任何形式直接转载或摘选本书的正文内容，非常感谢您的关注。**



![](https://pic4.zhimg.com/v2-b62ca505b992c9ec09467847ce7e7fe3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='778' height='1426'></svg>)![](https://pic3.zhimg.com/v2-77c9751b9b71906cad60760e3e8d563e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='778' height='1317'></svg>)![](https://pic2.zhimg.com/v2-c52ef888fca89f4b1317a31fb77f3671_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='778' height='1407'></svg>)![](https://pic1.zhimg.com/v2-224ae81f4657954ee3a2fe5a9c6cb680_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='778' height='1428'></svg>)![](https://pic1.zhimg.com/v2-8c40046a847d5ccbe41e9d139df2ef8c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='778' height='1428'></svg>)![](https://pic3.zhimg.com/v2-bd4133ce7aecdd7a1bf27e47e75b6072_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='1401'></svg>)![](https://pic2.zhimg.com/v2-c4924baa725ab41c10fa5e9451de9205_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='1328'></svg>)![](https://pic2.zhimg.com/v2-ee9222079857831de72e4777e19e12bd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='856' height='1437'></svg>)
**【待续~ 请关注我和本专栏吧，如果觉得有帮助，记得点个赞噢。】**

深度学习的发展日新月异，笔者只是略知皮毛，书中难免有错漏之处，恳请读者不吝指正，如果您抓到bug，书的前言中会向您致谢。

p.s. 如果希望现在支持本书，请[点击我公司的主页 Blink · 禀临科技](https://link.zhihu.com/?target=http%3A//withablink.com/) ，看看我们的实体硬件。保证良心好产品，还有超赞的售后和服务（有问题直接找我）。

## **p.s. 由于版权的缘故，您可以给出到本书知乎连载的连接，但请不要在任何网站或公众号等等以任何形式直接转载或摘选本书的正文内容，非常感谢您的关注。**


