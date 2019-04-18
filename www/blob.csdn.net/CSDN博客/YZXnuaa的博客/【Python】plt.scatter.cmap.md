# 【Python】plt.scatter.cmap - YZXnuaa的博客 - CSDN博客
2018年03月14日 10:51:34[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：3429
# python设置scatter颜色渐变
参考代码如下：
**[python]**[view plain](http://blog.csdn.net/yefengzhichen/article/details/52757722#)[copy](http://blog.csdn.net/yefengzhichen/article/details/52757722#)
- <span style="font-size:18px;">import matplotlib.pyplot as plt  
- cm = plt.cm.get_cmap('RdYlBu')  
- xy = range(20)  
- z = xy  
- sc = plt.scatter(xy, xy, c=z, vmin=0, vmax=20, s=35, cmap=cm)  
- plt.colorbar(sc)  
- plt.show()</span>  
其中get_cmap中取值可为：Possible values are: Accent, Accent_r, Blues, Blues_r, BrBG, BrBG_r, BuGn, BuGn_r, BuPu, BuPu_r, CMRmap, CMRmap_r, Dark2, Dark2_r, GnBu, GnBu_r, Greens, Greens_r, Greys, Greys_r, OrRd, OrRd_r, Oranges, Oranges_r, PRGn, PRGn_r, Paired, Paired_r, Pastel1, Pastel1_r, Pastel2, Pastel2_r, PiYG, PiYG_r, PuBu, PuBuGn, PuBuGn_r, PuBu_r, PuOr, PuOr_r, PuRd, PuRd_r, Purples, Purples_r, RdBu, RdBu_r, RdGy, RdGy_r, RdPu, RdPu_r, RdYlBu, RdYlBu_r, RdYlGn, RdYlGn_r, Reds, Reds_r, Set1, Set1_r, Set2, Set2_r, Set3, Set3_r, Spectral, Spectral_r, Wistia, Wistia_r, YlGn, YlGnBu, YlGnBu_r, YlGn_r, YlOrBr, YlOrBr_r, YlOrRd, YlOrRd_r...其中末尾加r是颜色取反。
![](https://img-blog.csdn.net/20161009142258945?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
在自己的程序中，是要绘制出一个网络，使得网络节点的颜色变化来表示度的变化，初步绘制的结果如下：
![](https://img-blog.csdn.net/20161009142428229?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
但是到现在，很不足的一点是，在matplotlib中plt.plot，plt.scatter这两个不能设置显示的上下层关系，不管两句话的顺序如何，都是plot在上，scatter在下，想让scatter在上层。导致绘制图中的圆点总是被直线覆盖，目前处理的方式是，将直线透明度设置为alpha = 0.2，可以稍微好看一点。
