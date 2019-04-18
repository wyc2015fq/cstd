# 干货|MIT线性代数课程精细笔记6-子空间与零空间 - 算法与数学之美 - CSDN博客
2017年11月29日 00:00:00[算法与数学之美](https://me.csdn.net/FnqTyr45)阅读数：787
#### MIT线性代数课程精细笔记[第六课]
**前言**
**MIT线性代数课程精细笔记[第一课]笔记见**[**MIT线性代数课程精细笔记**](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554640&idx=3&sn=f475c7e76b9dc736be25f97cb0a82dc0&chksm=8b7e34cbbc09bddd2f2d95666941f7b3e93a959903aae44d717d6c32d0329716177b23cb1e7b&scene=21#wechat_redirect)**1。**
**MIT线性代数课程精细笔记[第二课]笔记见**[**MIT线性代数课程精细笔记2**](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554716&idx=2&sn=1e575fdf8a0f2fd03026a0e9c087d9c1&chksm=8b7e3487bc09bd91d320e55a4cb9965da30ef61734dc7ae102484481c83dec08239cde4cf3c3&scene=21#wechat_redirect)**。**
**MIT线性代数课程精细笔记[第三课]笔记见**[**干货|MIT线性代数课程精细笔记3**](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652555019&idx=3&sn=db8bffcca94617c5e7af8ee62283f753&chksm=8b7e3650bc09bf46bd08e0c481f9275ee0cfa1c9a47007bda99badb62a0808d9df6d17e95ee4&scene=21#wechat_redirect)**。**
**MIT线性代数课程精细笔记[第四课]笔记见****[干货|MIT线性代数课程精细笔记4](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652555368&idx=2&sn=291dc8786b3498bae295c3473dbf4043&chksm=8b7e3933bc09b025682180833166f1874841d465f80ecd3649c9791a21e9881f74ea0f398d41&scene=21#wechat_redirect)。**
[**MIT线性代数课程精细笔记[第四课]笔记见干货|MIT线性代数课程精细笔记5**](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652555626&idx=2&sn=82423b7a44942a9c67ceb79d8c962d07&chksm=8b7e3831bc09b12725a6fe43ae372e73582a84a72f6fbd684296e756f9f781d36955ea68e663&scene=21#wechat_redirect)
该笔记是连载笔记，希望对大家有帮助。
1 知识概要
本节从之前学习的子空间开始，介绍了子空间的部分性质。并重点介绍了列 空间与方程 Ax = b 之间的联系。并由此引出了零空间，根据 Ax = b 这个方程给 出了两种构建子空间的方法。
2 子空间
## **2.1 子空间回顾**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/nJZZib3qIQW7D526KKqhV5lSXfCGoM43cXnjtN9SkmqDmNTuOFRWZ6b7gFg5uBGkcQDII8EF7ZQoiahXj9QUJOkw/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/nJZZib3qIQW7D526KKqhV5lSXfCGoM43cqLrFelWYCpw883BbmeL9NJF3BPxZiaibpNa6jov3uhKtRqaHtBSaqKKQ/640?wx_fmt=png)
很明显，子空间直线 L 或平面 P 上，任取两个向量相加，得到的向量仍在该 子空间中。而且将其上的向量做数乘伸长或缩短一定倍数，其结果也还在该子空 间中。所以它们都对线性运算封闭。
## **2.2 子空间的“交”与“并”**
上面我们都是分别研究的两个子空间，那么接下来我们对两个空间之间联系 部分展开讨论
## **2.2.1 P∪L 空间**
还是讨论上面
