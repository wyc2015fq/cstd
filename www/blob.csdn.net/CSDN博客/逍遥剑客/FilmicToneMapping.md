# FilmicToneMapping - 逍遥剑客 - CSDN博客
2012年04月15日 23:26:49[xoyojank](https://me.csdn.net/xoyojank)阅读数：2633
个人分类：[GPU编程](https://blog.csdn.net/xoyojank/article/category/337623)
照葫芦画瓢弄了一个, 正好可以解决一下色彩不够鲜艳的问题
见 [http://blog.csdn.net/ccanan/article/details/6745207](http://blog.csdn.net/ccanan/article/details/6745207)
![](https://img-my.csdn.net/uploads/201204/15/1334503619_1969.png)
- 曝光度弄成16铁定过了, 如果是非常暗的场景, 并且是线性颜色空间才有意义(GammaCorrection)
- LDR其实也可以用, 我上面的没有使用HDR颜色空间
