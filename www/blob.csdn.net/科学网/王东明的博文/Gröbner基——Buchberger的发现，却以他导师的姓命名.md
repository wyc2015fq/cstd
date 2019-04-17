# 科学网—Gröbner基——Buchberger的发现，却以他导师的姓命名 - 王东明的博文




# Gröbner基——Buchberger的发现，却以他导师的姓命名                                                   ![](./static/image/blog/recommendico.gif)精选

已有 7240 次阅读2018-4-18 11:35|个人分类:[阿狗数学](http://blog.sciencenet.cn/home.php?mod=space&uid=1362128&do=blog&classid=173744&view=me)|系统分类:[科普集锦](http://blog.sciencenet.cn/home.php?mod=space&do=blog&view=all&uid=1362128&catid=7)|[多项式](misc.php?mod=tag&id=43855), [理想](misc.php?mod=tag&id=4362), [Groebner基](misc.php?mod=tag&id=262851), [符号计算](misc.php?mod=tag&id=36461)



任给*n*维线性空间中的向量*v*1*,...,v**k*和*v*，如何判断*v*是否属于*v*1*,...,v**k*所张成的线性子空间*V*？这就是线性空间中的成员判定问题，也是最基本的数学问题，如同判定苹果是不是水果、自行车是不是交通工具是最基本的认知问题一样。解决该问题的方法之一是将*v*1*,...,v**k*和*v*作为列向量构造一个矩阵，然后用高斯消去法将矩阵化为阶梯形。那么*v*属于*V*当且仅当不存在仅有最后一个元素非零的行。

现在考虑上述问题的升级版：任给环*K*[*x*1*,...,xn*]中的多元多项式*f*1*,...,f**k*和*f*，如何判断*f*是否属于*f*1*,...,f**k*生成的理想？与线性空间中的成员判定问题简单易解截然不同，对于多项式理想的成员判定问题，其完整解决则需要用到强大的计算代数工具——Gröbner基。
|![](home.php?mod=attachment&filename=1.png&id=189860)|
|----|

**欧洲科学院院士、奥地利科学家B. Buchberger (左)与他的博士导师W. Gröbner (右)**

Gröbner基的概念由B. Buchberger于1965年在他的博士论文《Ein Algorithmus zum Auffinden der Basiselemente des Restklassenrings nach einem nulldimensionalen Polynomideal》（计算零维多项式理想的剩余类环的基的算法）中引入，论文的导师是著名的代数几何学家W. Gröbner。Buchberger在论文中设计了计算多元多项式理想的Gröbner基算法（称为Buchberger算法），并提出了优化该算法的若干准则（称为Buchberger第一、第二准则）。作为代数学的基础，Gröbner基的理论和方法已被写入很多通用代数教材。譬如，J. J. Rotman所著的《Advanced Modern Algebra》（高等近世代数）一书就有专门的章节介绍Gröbner基与Buchberger算法。Gröbner的名字随着Buchberger的Gröbner基方法的深远影响而广为人知。
|![](home.php?mod=attachment&filename=2.png&id=189861)|
|----|

**包含Gröbner基方法的通用代数教材**

Gröbner基是由多元多项式理想的特殊生成元构成的集合，它具有非常良好的性质。通过计算Gröbner基很多有关多项式理想的基本问题都可以算法化求解。前面提到的理想成员判定问题也可以通过使用Gröbner基的性质获得解决：一个多元多项式属于给定生成元的多项式理想当且仅当它对该理想的Gröbner基的范式为0。又譬如，字典序Gröbner基关于其中出现的变元具有一定的层级结构，即所谓的消元性质。基于这种性质，Gröbner基方法又可以用来研究各种消元问题，如多项式方程组求解、参数曲线与曲面的隐式化等。
|![](home.php?mod=attachment&filename=3.png&id=189859)|
|----|

**字典序Gröbner基的消元性质**
|![](home.php?mod=attachment&filename=4.png&id=189862)|
|----|

**破解密码体制到求解多项式方程组的转化(图片取自J.-C. Faugère的演讲幻灯片)**

1965年春天，23岁的Buchberger在导师Gröbner的指导下在奥地利因斯布鲁克顺利地完成了博士论文。当时Gröbner已经是享誉盛名的大数学家，有很多助理教授紧随其后。年轻的Buchberger几乎没有机会见到Gröbner本人，他有关博士学习和论文答辩的行政手续大多通过助理教授来完成。为了能在高校找到一份工作，Buchberger读博期间就已经在全职编程。在提交了博士论文之后，他怀着忐忑不安的心情等待了一段时间便获知论文通过了。

Gröbner当时因为工作繁忙，实际上并没有为Buchberger读博期间的研究工作给予多少具体指导。他可能也没有时间去审阅Buchberger的论文。Gröbner对Buchberger博士论文的实际贡献只是提出了论文研究的问题。可是Gröbner基为什么会冠上他的名字呢？

Gröbner基方法在提出之后没有多少人问津，Buchberger发表的论文沉寂多年，少有引用。Buchberger的研究兴趣也转向了逻辑和算法理论。可10年之后，也就是1976年，随着符号计算方法和软件的快速发展，Gröbner基方法开始引起较为广泛的关注，人们开始认识到对于非线性多项式系统求解Buchberger所解决的问题是非常基本的。与此同时，Buchberger也深刻地意识到导师Gröbner当时给他提出的关于剩余类环的有限基及其构造问题的意义和重要性。Gröbner凭借其深厚的学识和敏锐的洞察力，实际上他至少在25年之前就已经意识到此问题的重要性，但未能给出问题的解答。1964年，他将这个问题作为博士论文研究课题交给了Buchberger。Buchberger不负导师的厚望，一年之后就满意地解决了导师提出的问题，进而创建了后来称之为Gröbner基的理论和方法，并在非常原始的计算机上具体实现了其主要算法。在他1976年系统给出了所有证明的论文中，Buchberger用自己最引以为傲的学术发现回报师恩，首次将他自己提出的概念、理论和方法命名为Gröbner基、Gröbner基理论和Gröbner基方法，从而留下了一段有关学术传承和学生感恩导师的美谈佳话。

Buchberger的Gröbner基方法已被写进20余部教材，在3000多论著中有后续研究，被引用数千次。它与吴文俊所发展的特征列方法、G. E. Collins提出的柱形代数分解方法并列，成为符号计算领域的三大核心方法。Buchberger不仅是开拓符号计算领域的先锋，也是推动这个领域繁荣发展的学术活动家和教育家。他是《符号计算杂志》(Journal of Symbolic Computation)的创刊主编；在创刊号的编辑前言中他精辟地论述了符号计算与《符号计算杂志》的内涵，并确定了符号计算领域未来发展的基调。《符号计算杂志》早已成为符号计算领域的权威期刊, 在计算机数学的交叉研究中扮演着举足轻重的角色。
|![](home.php?mod=attachment&filename=5.png&id=189865)|
|----|

**《符号计算杂志》创刊号**

Buchberger还于1987年在奥地利林茨创立了符号计算研究所（Research Institute for Symbolic Computation）。该研究所为符号计算领域培育了大量优秀科研人员，许多国际著名学者都曾到此深造、访问、交流。
|![](home.php?mod=attachment&filename=6.png&id=189864)|
|----|

**Buchberger与吴文俊在国际会议ISSAC 2005上合影**

Buchberger对Gröbner基理论和方法的建立与发展作出了卓越贡献。有关Gröbner基的命名，他与恩师之间的佳话有着不同的版本，在学术圈内流传。那么Gröbner基是否应该称为Gröbner基呢？王东明在近期有关Gröbner基和Ritt特征列内在关联的论文标题中将Gröbner基更名为Buchberger-Gröbner基, 受到了国内外同行的认同。读者以为如何？
|![](home.php?mod=attachment&filename=7.png&id=189863)|
|----|

**B. Buchberger，M. F. Singer，D. Y. Grigor’ev和王东明（从左到右，1988年摄于奥地利林茨）**

（王东明教授审阅了本文并为文中有关Buchberger与Gröbner的佳话提供了背景材料）

（牟晨琪）

来源：[阿狗数学AlgoMath](http://mp.weixin.qq.com/s/vcP4gj1i3LZ6KRsJ5CaZOg)



转载本文请联系原作者获取授权，同时请注明本文来自王东明科学网博客。
链接地址：[http://blog.sciencenet.cn/blog-1362128-1109587.html](http://blog.sciencenet.cn/blog-1362128-1109587.html)

上一篇：[阿狗传道——方程术（下）](blog-1362128-1107972.html)
下一篇：[转轴拨弦三两声——浅谈弦图](blog-1362128-1111919.html)


