# Matlab atan2 - wishchinYang的专栏 - CSDN博客
2015年12月21日 12:03:07[wishchin](https://me.csdn.net/wishchin)阅读数：5370
         对Matlab不是很熟悉，在这个Matlab atan2 函数上出现了问题。
         百度知道上的解释是这样的：
atan2() 区别于 atan() 函数，返回 -pi~+pi 范围的角度；
使用过程中发现在 MATLAB 和 Excel 中该函数的表达形式不一致，故记录如下。
- Excel
定义atan2(x,y) 返回点 (x,y) 与 x 轴正向的夹角；
即在第一象限中** atan2(x,y) = atan(y/x)**.
- Matlab 与 CFX CEL
定义 atan2(y,x) 返回点(x,y)与x轴正向的夹角；
即在第一象限中 atan2(y,x) = atan(y/x).
-------------------------------------------
故点(3,4)与x轴的夹角分别表达如下：
```
Excel ：atan2(3,4)=0.927295218
Matlab：atan2(4,3)=0.9273
```
