# Python史上最牛逼的可视化神器 - zhusongziye的博客 - CSDN博客





2019年01月13日 20:46:41[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：433








数据分析离不开数据可视化。比如我们前面介绍过pandas画图，matplotlib画图，pyecharts画图当然还有Tableau，今天推荐一款更牛的可视化神器，**主要是这款神器的画出来的图很漂亮，色彩太酷了**。



**神器Plotly**



Plotly 是一款用来做数据分析和可视化的在线平台，功能非常强大,可以在线绘制很多图形比如条形图、散点图、饼图、直方图等等。而且还是支持在线编辑，以及多种语言python、javascript、matlab、R等许多API，当然我们这里主要介绍Python语言，可以直接用pip install plotly即可。

plotly可以画出很多媲美Tableau的高质量图，比如：



![](https://img-blog.csdnimg.cn/20190113204432683.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



![](https://img-blog.csdnimg.cn/20190113204445377.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

下面我们介绍几种最常用的图的用法，反正我学完之后觉得美观感要秒杀pyecharts的配图颜色。



**1.直方图**

**plotly有在线和离线两种模式**，在线需要有账号可以云编辑！而我这里选择用离线模式，然后用jupyter notebook来玩数据可视化(为了好看我用sublime text 显示代码)

![](https://img-blog.csdnimg.cn/20190113204505255.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

上面几行代码主要是引用一些库，最重要的一点是要把plotly设置为offline模式，然后直接在notebook里面显示。(后面的每一种图都是类似的设置).



![](https://img-blog.csdnimg.cn/20190113204515774.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

随机4过参数，x轴的数字和y轴，其中y轴随机3组数据。

然后画三种类型的图，trace0是markers，trace1是折线图和markers,trace3是存折线图。然后把三种图放在data这个列表里面，调用py.iplot(data)即可。

看一下效果:

![](https://img-blog.csdnimg.cn/20190113204528860.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

是不是默认的配色非常漂亮。



**2.散点图**

散点图也是我们非常常用的一种图形，这里的散点图有点可以匹配大名鼎鼎的ggplot里面的散点图.

![](https://img-blog.csdnimg.cn/20190113204539202.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

这个是mode设置为markers就是散点图，然后marker里面设置一组参数，比如颜色的随机范围，散点的大小，还有图例等等。最后一行里面的filename是在当前目录下生成html文件。

看一下效果：

![](https://img-blog.csdnimg.cn/20190113204550348.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



**3.气泡图**

气泡图是一种比较酷的图形，Tableau里面也有这样的图形，玩好了非常酷，plotly里面使用也很简单。

![](https://img-blog.csdnimg.cn/20190113204559398.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

data的参数也是跟上面类型设置x,y轴的数据，然后设置一下气泡的颜色和大小，图例等等。plotly的好处就是对图的细节的设置比较细腻！

看一下效果:

![](https://img-blog.csdnimg.cn/20190113204608583.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



**4.柱状图**

柱状图是非常常用的一种图形，也是用的比较多的。我们来看看plotly里面怎么设置的，这里我们用一个对比的柱状图来显示:

![](https://img-blog.csdnimg.cn/20190113204619123.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

是不是看着眼熟，跟我们的pandas里面设置有点类似，跟pyecharts里面也很相似，其实很多库都是相互借鉴的，设计的思路和使用的参数都类似的。

看一下效果：

![](https://img-blog.csdnimg.cn/20190113204628162.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



好上面的举例都plotly的冰上一角都是一些最基本的用法，它还有很多很酷的用法和图形，尤其是跟pandas结合画的图非常漂亮，比如一些股票的K线图，大家有兴趣可以去看一下！



链接:https://plot.ly/python/



