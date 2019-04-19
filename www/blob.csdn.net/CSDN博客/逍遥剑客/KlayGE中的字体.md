# KlayGE中的字体 - 逍遥剑客 - CSDN博客
2008年12月06日 20:47:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2984
官方文档参见:http://klayge.sourceforge.net/html/font_in_klayge.html
这里只是一些测试
看了龚敏敏写的文档, 觉得这种方式很意思, 就拿来玩玩.
因为中文字体的渲染在3D程序中确实是个问题, 尤其是字体大小不固定时, 放缩会出现走样.
KlayGE的方法是一种用点阵图模拟矢量图的方案, 把计算过程放在字体生成时, 而运行时的绘制相对常规方法来说消耗很小. 
KFontGen目前好像有问题, 生成过程中会由于超出32位所能寻址的最大内存而报错退出. 原先我以为2G内存不够用, 换自己的4G内存机器也不行. 难道生成时要用64位机? 看来这个生成工具需要修正一下内存的占用率. 
在DEBUG模式下, 字数很多时, 帧率会急剧下降, 用PerfHUD看了一下曲线图, 是CPU瓶颈, 可能是因为KlayGE中大量使用的boost在DEBUG模式下的速度变成牛一样吧?
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20081206/KFontDebug.JPG)
Release下就好多了:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20081206/KFontReleaseNoPerfHUD.JPG)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20081206/KFontRelease.JPG)
字体的绘制只有一次DP:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20081206/KFontFrameDebugger.JPG)
字体绘制时使用的纹理buffer, 从这里我们可以看到distance field的一个比较直观的储存格式:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20081206/KFontDistanceField.JPG)
对比一下ID3DXFont的效率(Nebula3中debug text的绘制方式):
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20081206/KFontReleaseNoScale.JPG)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20081206/Nebula3DebugText.JPG)
