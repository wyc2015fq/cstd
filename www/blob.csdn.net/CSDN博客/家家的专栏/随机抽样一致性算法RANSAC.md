# 随机抽样一致性算法RANSAC - 家家的专栏 - CSDN博客





2010年10月29日 11:28:00[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：1414









**转自：[http://www.shenlejun.cn/my/article/show.asp?id=43](http://www.shenlejun.cn/my/article/show.asp?id=43)**



**什么是RANSAC?**

RANSAC是**RAN**dom** SA**mple **C**onsensus（随机抽样一致性）的缩写。它是从一个观察数据集合中，估计模型参数（模型拟合）的迭代方法。它是一种随机的不确定算法，每次运算求出的结果可能不相同，但总能给出一个合理的结果，为了提高概率必须提高迭代次数。

RANSAC很强大。如图1所示，**RANSAC用于在强干扰环境中寻找以某种模型（如直线）出现的数据**。图中黄色点代表坐标原点，蓝色点代表输入的数据点集合（其中包含20个真实点和40个干扰点），红色点代表RANSAC算法在强干扰环境中，经过500次左右的抽样迭代，能找出的和直线拟合最好的13点（之所以不等于20，是因为真实直线也受到轻微干扰，所以并不笔直）。本文来自:ｓｈｅｎｌｅｊｕｎ．ｃｎ.

![](http://www.shenlejun.cn/my/article/UploadPic/2009-6/20096121354538039.jpg)

图1，随机抽样一致性算法示例图本文来自:http://shenlejun.cn.

RANSAC常常用于计算机视觉，例如求解图像对应点和估计立体视觉的基本矩阵。作者曾将本算法用于视觉测量（[点击查看该文](http://www.shenlejun.cn/my/article/show.asp?id=19)）。更多的应用参见文献[1]（RANSAC25年）。

**那里能找到源程序？**

该算法简单实用，Ziv Yaniv曾经写了一个不错的C++程序。推荐理由1：没有使用任何其他的函数库，省去您更多负担。如解线性方程、随机数产生器。理由2：唯一觉得可惜的是他的算法中过多的使用了C++的特性，如vector，虚函数，设计模式。如果您刚好是C++高手，正是您阅读的好程序。理由3：作者的注解详细，每2行就有1行注释，容易看懂。偶自愧不如（汗~~~）。

可惜源程序在原作者的主页上已经无法下载[3]，本站未做任何修改，将Ziv Yaniv的源程序提供给各位同行。
[点击下载此文件](http://www.shenlejun.cn/my/article/show.asp?id=43)（17KB）

**入门教程呢？**本文来自:http://shenlejun.cn.

本来想把文献[2]翻译一下，贴到这里。请让我偷个懒吧。能找到这里的读者，应该都是E文好手，我翻译得不清不楚，反而给大家增添烦恼了。 ^_^
推荐文献[2]的理由1：文字少且简洁易懂；理由2：伪代码只有10多行，却清楚明白，比市面上某些教材好多了。


**参考文献**本文来自:ｓｈｅｎｌｅｊｕｎ．ｃｎ.

[1]RANSAC25年,http://cmp.felk.cvut.cz/ransac-cvpr2006/
[2]RANSAC名词解释,http://en.wikipedia.org/wiki/RANSAC
[3]RANSAC源代码下载,www.cs.huji.ac.il/~zivy/software/ransac.tar.gz




