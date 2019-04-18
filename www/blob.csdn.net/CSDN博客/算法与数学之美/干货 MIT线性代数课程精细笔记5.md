# 干货|MIT线性代数课程精细笔记5 - 算法与数学之美 - CSDN博客
2017年10月02日 00:00:00[算法与数学之美](https://me.csdn.net/FnqTyr45)阅读数：1458
#### MIT线性代数课程精细笔记[第五课]
**前言**
MIT线性代数课程精细笔记[第一课]笔记见[MIT线性代数课程精细笔记](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554640&idx=3&sn=f475c7e76b9dc736be25f97cb0a82dc0&chksm=8b7e34cbbc09bddd2f2d95666941f7b3e93a959903aae44d717d6c32d0329716177b23cb1e7b&scene=21#wechat_redirect)1。
MIT线性代数课程精细笔记[第二课]笔记见[MIT线性代数课程精细笔记2](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554716&idx=2&sn=1e575fdf8a0f2fd03026a0e9c087d9c1&chksm=8b7e3487bc09bd91d320e55a4cb9965da30ef61734dc7ae102484481c83dec08239cde4cf3c3&scene=21#wechat_redirect)。
MIT线性代数课程精细笔记[第三课]笔记见[干货|MIT线性代数课程精细笔记3](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652555019&idx=3&sn=db8bffcca94617c5e7af8ee62283f753&chksm=8b7e3650bc09bf46bd08e0c481f9275ee0cfa1c9a47007bda99badb62a0808d9df6d17e95ee4&scene=21#wechat_redirect)。
MIT线性代数课程精细笔记[第四课]笔记见[干货|MIT线性代数课程精细笔记4](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652555368&idx=2&sn=291dc8786b3498bae295c3473dbf4043&chksm=8b7e3933bc09b025682180833166f1874841d465f80ecd3649c9791a21e9881f74ea0f398d41&scene=21#wechat_redirect)
该笔记是连载笔记，希望对大家有帮助。
1知识概要
本节我们再谈置换矩阵与转置矩阵，并介绍对称阵。之后便进入学习线代的关键所在：向量空间与子空间。
2 置换矩阵
置换矩阵回顾
所谓的置换矩阵 P，就是用来完成行交换的矩阵，更具体来讲，是行重新排列 了的单位矩阵。例如 I 就是一个置换矩阵，只不过 I 对矩阵没影响。
那么对于 n 阶矩阵来说，有多少个置换矩阵呢？答案是：n!种，也就是将单 位矩阵 I 各行重新排列后所有可能的情况数量。
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/nJZZib3qIQW6DYbc7uEqBOED5GSYP6H7kGJiaBQUicEX8SicCbicsqwGGAUrvt3PEeeuuJUEIeaGYo6apzIp31ibuFPQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/nJZZib3qIQW6DYbc7uEqBOED5GSYP6H7k39c6ibnDq7ADFedb2wtrtE9vVuUZw6G6aFnXqYzucmw6ibrAosHXyUtw/640?wx_fmt=png)
置换矩阵的使用
在讲消元法的时候，主元位置为 0 是一件很让人头疼的事情，这时就需要置 换矩阵 P 来完成行交换，确保消元过程顺利进行。上节课学习 A = LU 分解时， 我们没有考虑要交换行的过程，如果我们想写出更普适的 LU 分解式的话，必须把行交换情况考虑进去，即：
PA = LU
先用行交换使得主元位置不为 0，行顺序正确。其后再用 LU 分解。
3 转置矩阵
转置矩阵回顾
之前简单介绍过转置矩阵，即
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/nJZZib3qIQW6DYbc7uEqBOED5GSYP6H7kOSsia37IBI9akHIXrTm7n5JQVKs6ylVOFZxQZJphDTwHPtYF1g1SohQ/640?wx_fmt=png)
对称阵
对称矩阵，顾名思义，就是主对角线两侧元素对应相等的矩阵。或者说，对 矩阵 A，如果有：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/nJZZib3qIQW6DYbc7uEqBOED5GSYP6H7kSuglK0icPcymqmZDBZW9fxvBicWQgAoVSrqTSb0T4WfbeicrYu0qx2iang/640?wx_fmt=png)
4向量空间与子空间
向量空间
首先明确“向量空间”的概念，它表示一整个空间的向量，但是要注意，不是任意向量的集合都能被称为向量空间。所谓的向量空间，必须满足一定规则， 就是：该空间对线性运算（相加，数乘）封闭。类似：v → 3v 或 v，w → v+w 运算，若得到的 3v 或者 v+w 都仍然在此空间中，那么这个空间可称为向量空间。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/nJZZib3qIQW6DYbc7uEqBOED5GSYP6H7k3kZmKx4CmeIL0DGXLcyQmmpPtApBMzeco37Bb5fP0bHgyp2uOBManQ/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/nJZZib3qIQW6DYbc7uEqBOED5GSYP6H7kx17l1mLObZFdjqP0EbfIpzuwajd9rAA1GUTIhzKoxNtOZ8cxRcUf6w/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/nJZZib3qIQW6DYbc7uEqBOED5GSYP6H7k7fMLzooykPJU9Ah8X50bxuJY66fLZeelKiaRtEKyZp4Z8sHKJSPxicHw/640?wx_fmt=png)
很明显，这部分空间无法满足“线性组合仍在空间中”的要求，比如数乘运算时，随便取个负数，向量就跑到第三象限去，脱离 D 空间范围内了。
子空间
上面的反例已经证明了。在向量空间里随便取其一部分，很可能得到的不是 向量空间。那如果我们取向量空间的一部分，将其打乱，构成的有没有可能是向 量空间呢？
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/nJZZib3qIQW6DYbc7uEqBOED5GSYP6H7klazHId8nUAUynu0NThaicsxEiaxPurm14ITpia87Rnvk1MmG8ghVHicdFw/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/nJZZib3qIQW6DYbc7uEqBOED5GSYP6H7kga7QB7FXb1ictWMY6TFEbWYoSF0pJOGVfyks27icE2h0xtlqbzms1KLg/640?wx_fmt=png)
列空间简要介绍
上面介绍的子空间都是基于已知的图像来寻找的，接下来我们来通过具体的 矩阵来构造出一个子空间，比如：列向量构造出的列空间。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/nJZZib3qIQW6DYbc7uEqBOED5GSYP6H7kdwedRiaaWnWd2RP8svWtRg6Ep6zAujZCYia2vjND35Y4VqnjEwicP6mbw/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/nJZZib3qIQW6DYbc7uEqBOED5GSYP6H7kCCUsichdUkl9WQh28icPhY6vUEqZ7I7TiaYeazuHUEkiahXz4A37icfeKiaQ/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/nJZZib3qIQW6DYbc7uEqBOED5GSYP6H7kmTmQlPdHU1UgvVJXBV1RwiccB26XdDR5DxuBmUqibTkJGacOPSnQZUow/640?wx_fmt=png)
这里还要注意列向量之间的性质，如果列向量之间就是共线的，那么其列空间就是一条过原点的直线。
5 学习感悟
这节算是结束了之前部分对基本运算和基本概念的介绍。介绍了向量空间和子空间，并由子空间引出了通过具体的列向量构成的空间—列空间。如何理解空间十分重要，本节中对低维的空间做了图，目的主要是便于我们理解“空间”这 一概念。
** 欢迎关注公众号学习交流~ **
![640.png?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/nJZZib3qIQW5QjWVUmJtUIVEfDlp22bJQcMFEX2BKansfE4RoQUJl3PlIibZ1fOO2weDwb1zATYWuCIctOHRWI4w/640.png?)
**----------------****----**
黎曼，欧拉，傅里叶竟每每被提及！神秘的群里竟然反复出现应数、建模、数统！里面究竟隐藏着什么秘密？来来来——算法与数学之美数学粉丝群成立啦！
下方高能！！！
**数学粉丝qq群号：602146986 。**
**微信群请扫描下方二维码！加我好友拉你入群！**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabky4RsTeautZqoacu4TUZhqwUCk6lzvibJm9V0ibSf6MFBXHJJzubicibowX7Rd5LjFBhKd9zNwXibicFBoQ/640?wx_fmt=jpeg)
**算法数学之美微信公众号欢迎赐稿**
**稿件涉及数学、物理、算法、计算机、编程等相关领域。**
**稿件一经采用，我们将奉上稿酬。**
**投稿邮箱：math_alg@163.com**
