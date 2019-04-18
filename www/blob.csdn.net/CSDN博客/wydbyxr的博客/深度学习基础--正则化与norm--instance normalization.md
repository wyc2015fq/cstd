# 深度学习基础--正则化与norm--instance normalization - wydbyxr的博客 - CSDN博客
2018年11月20日 10:24:59[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：142
# instance normalization
  instance normalization的方法，IN可以理解成为每个图片自己做BN，这样比BN能保留更多scale信息。更新的研究表明如果训练收敛不是问题的话，进一步去掉IN的效果也会更好。
## 分析
  scale和mean包含了图片style的信息，当用一个网络transfer一种style的时候，IN可以把原图的style丢掉换上新的style（由gamma和beta encode），所以结果有提升。
  当然，如果要一个网络transfer任意style，IN反而会hurt performance （因为IN把所有图都转成了同样的scale和mean）。
  故我们提出了一种新的normalization来解决这个问题。（？忘记是哪篇论文了）
