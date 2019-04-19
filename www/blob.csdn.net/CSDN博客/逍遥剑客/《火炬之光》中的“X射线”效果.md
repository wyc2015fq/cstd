# 《火炬之光》中的“X射线”效果 - 逍遥剑客 - CSDN博客
2009年11月09日 21:34:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：7206
玩了下《火炬之光》, 没想到能用OGRE做到这么个程度, 很让人惊叹!
刚开始玩就注意到人被遮挡时, 并不是像其它游戏那把遮挡物半透明化, 而是把被遮挡的部分处理成一种透视效果:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20091109/Torchlight.jpg)
想了想, 其实也挺简单的, 把模型画两遍就成了:
1. 先画场景
2. 画被遮挡的部分, ZWrite=False; ZFunc=Greater; 开启Alpha混合, 光照亮度做为alpha值输出, 颜色由参数传入.
3. 还原正常的渲染状态, 绘制原模型(为了显示未被遮挡的部分)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20091109/XRay1.jpg)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20091109/XRay2.jpg)
可能混合公式还要调节一下, 不过效果已经差不多了, 呵呵.
