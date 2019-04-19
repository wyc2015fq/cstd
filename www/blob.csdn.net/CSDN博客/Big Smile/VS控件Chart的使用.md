# VS控件Chart的使用 - Big Smile - CSDN博客
2017年05月25日 23:21:12[王啸tr1912](https://me.csdn.net/tr1912)阅读数：6381
   这两天忙着做一个图表的分析系统，正好用到了VS的chart的这个控件，感觉比以前的直观的数据统计不知道要好多少倍，在此分享给大家。
# 一、控件介绍
  说道chart控件呢，是一个VS带的比较有意思的控件，因为之前没有接触过直接用应用程序来展示数据的需求，所以也就没有用过这个类型的控件，这次正好遇到了，其实和之前在.NET中用到的带有数据绑定的控件非常一样，只不过他以一种形式把数据以图表的形式展现了出来。
如图：
![](https://img-blog.csdn.net/20170525224739152?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
  由于这个图表是二维的，所以在数据区一次只能绑定两个数据，也就是在X和Y轴上绑定数据。但是可以无限添加数据组，从而有了上图所示的样子，温度、湿度、PM2.5都是一个一个的二维数据组成的，通过控件内部就可以实现图形的显示了，话不多说，下面开始介绍如何绑定数据。
# 二、数据绑定
  其实整个控件的数据绑定非常简单，就是直接dataBind 和  dataSource 就行了，代码如下：
```
chart1.DataSource = dt;
chart1.DataBind();
```
  这里要注意的是，我们用的chart控件数据源只是接收datatable类型的数据，不能接收泛型等其他类型的数据。博主亲自试验过了，dataTable是最好的数据源。绑定完了数据源，我们要怎么显示呢？就是要添加数据组项。
  在这个控件的属性中有这样一个属性，是Series，这个属性就是整个数据显示对象的配置集合，里面包含了数据显示的形式（折线，柱状，饼状，条状，等等）,还有对于数据成员的添加，字段的绑定，以及数据显示的方面的设定，如下：
首先是数据成员（几种数据）：
![](https://img-blog.csdn.net/20170525230512238?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
  我们可以看到，上面就是如果想要有三种类型的数据对比显示的话，就要有三个成员在成员项里面，温度、湿度、PM2.5都是数据成员的一种，他们的表现形式都可以自由设置。
  接下来就是成员的设定了，首先是图表类型：
![](https://img-blog.csdn.net/20170525230715384?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
  里面包含了我们日常需要的所有图表类型，我们一定要先选好，然后数据就可以按照我们选定的类型显示了，然后就是X、Y轴的数据绑定：
![](https://img-blog.csdn.net/20170525230923166?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
  这里需要跟想要绑定的数据字段的名称一样即可，然后数据就会自动绑定到X/Y轴上，就可以按照要求显示了，这里不同的成员绑定的数据相同才有对比性，虽然可以不同，但是数据的显示区间如果想要保持一致的话，数据的X周最好绑定成一样的数据。
  如果我们想要在代码中切换X/Y轴的数据怎么办呢？
```
chart1.Series[0].XValueMember = "dateTime";
chart1.Series[1].XValueMember = "dateTime";
chart1.Series[2].XValueMember = "dateTime";
```
  series后面的序号是指的具体成员，然后后面的是具体属性，任何属性都可以这样改变。
  这个里面还有一个属性就是数据点和数据标签，在属性中和在实际图表中的表示如图所示：
![](https://img-blog.csdn.net/20170525231644171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
  我们通过调整属性就可以直接控制数据点的颜色，形状和数据标签的显示样式了。当然图例和空白标签的设定也在这里，我们也可以通过代码来实现控制，这里不再赘述。
# 三、总结
  我们在以后的开发中，数据的展示可以不仅仅只是通过表格的方式，也可以尝试一下这种图表的统计方式，他会让数据变得更加的生动和形象，可以方便人们更加直观的了解数据走向。
