# 《深度卷积网络：从AlphaGo到GAN》连载：2.1.7 ~ 2.2.8 - 知乎
# 



大家新年好，继续在知乎的连载。这里是接近完成的草稿，正式出版时会是高清全彩色印刷。请立即关注我和本专栏吧。如果觉得有帮助，记得点个赞噢。

UPDATE: 已经在印刷啦，介绍和预订链接：
[PENG Bo：《深度卷积网络·原理与实践》，结合可读性与实用性的全彩印深度学习入门（多图）​zhuanlan.zhihu.com![图标](https://pic3.zhimg.com/v2-866e668667a6a3a8f05b5e043c752fba_ipico.jpg)](https://zhuanlan.zhihu.com/p/36451054)
## **p.s. 由于版权的缘故，您可以给出到本书知乎连载的连接，但请不要在任何网站或公众号等等以任何形式直接转载或摘选本书的正文内容，非常感谢您的关注。**

**另外，由于MXNet的版本迭代，这里的安装过程有点过时了（例如其实不需要装VC2015了），到时书中会是最新方法。**
![](https://pic1.zhimg.com/v2-2ea7d76ee79c83bf93e1c361ad1a40ec_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='798' height='1437'></svg>)![](https://pic1.zhimg.com/v2-57214d741580ce4f77605d3fa2e3e6d4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='778' height='1408'></svg>)![](https://pic4.zhimg.com/v2-2b2ca1abcb8d3639e9a274c3d7fdb693_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='779' height='1382'></svg>)![](https://pic4.zhimg.com/v2-567a855c7eb8508068fab2530a4397b3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='780' height='1421'></svg>)![](https://pic2.zhimg.com/v2-1132b774ddd71f99aecb4be66f7b4261_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='792' height='1262'></svg>)![](https://pic4.zhimg.com/v2-c2ca28ce3352406c6389d43aec709c27_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='791' height='1444'></svg>)![](https://pic1.zhimg.com/v2-dec6f68936c7d51c2c12fb43793e7d0c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='1436'></svg>)![](https://pic3.zhimg.com/v2-5bf99bf950373f9bac5bedeaacc98a3e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='1438'></svg>)![](https://pic4.zhimg.com/v2-82352df1b9f0f3bb4540043e33686043_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='848' height='1420'></svg>)
**【待续~ 请关注我和本专栏吧，如果觉得有帮助，记得点个赞噢。】**

深度学习的发展日新月异，笔者只是略知皮毛，书中难免有错漏之处，恳请读者不吝指正，如果您抓到bug，书的前言中会向您致谢。

p.s. 如果希望现在支持本书，请[点击我公司的主页 Blink · 禀临科技](https://link.zhihu.com/?target=http%3A//withablink.com/) ，看看我们的实体硬件。保证良心好产品，还有超赞的售后和服务（有问题直接找我）。

## **p.s. 由于版权的缘故，您可以给出到本书知乎连载的连接，但请不要在任何网站或公众号等等以任何形式直接转载或摘选本书的正文内容，非常感谢您的关注。**


