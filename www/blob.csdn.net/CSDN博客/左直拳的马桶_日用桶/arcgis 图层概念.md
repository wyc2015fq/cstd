# arcgis 图层概念 - 左直拳的马桶_日用桶 - CSDN博客
2017年05月08日 17:57:00[左直拳](https://me.csdn.net/leftfist)阅读数：1856
arcgis有图层的概念。
图层好好几种，什么切片图层、动态图层、图形图层、影像图层；还有 for google的KML图层。 
![这里写图片描述](https://img-blog.csdn.net/20170508161628504?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 
ArcGISTiledMapServiceLayer 切片图层 
  ArcGISDynamicMapServiceLayer 动态图层 
  GraphicsLayer 图形图层 
  FeatureLayer 要素图层
切片图层就是静态图层，服务器可以先切好，或者利用缓存预先切好。目的就是快速显示。
动态图层是服务器即时生成图片返回，而没有预先缓存。根据用户请求，计算，即时生成。数据更新及时，但速度较慢。
底图既可以用切片图层，也可以用动态图层。
GraphicsLayer（图形图层），充满了各种几何图形的图层。这是我们用的最多的图层。这个图层，是由客户端绘制而成的，当然，数据可能来自于服务器。但由客户端绘制。这个跟切片图层和动态图层不同。切片图层、动态图层都在服务器端绘制，以图片方式传回来；而GraphicsLayer传回来的是一堆数据。FeatureLayer（要素图层）属于图形图层。
影像图层（MapImageLayer），不是很清楚。
有关图层，凡用过photoshop，就很容易理解。一幅地图，可能就是由基础底图 + 各种图层叠加上去的。虽然图层是个逻辑概念，但表现控制起来，还真的就像一张张透明的塑料，各种图形都画在上面。
——————————————-2018.1.6 
图层中，比较容易搞混的是动态图层（ArcGISTiledMapServiceLayer） 和 要素图层（FeatureLayer）。我目前知道的图层加载方式有两种： 
1、使用动态图层加载整个地图服务，然后控制其中图层的显隐，从而达到貌似加载某一张图层的效果。
2、使用要素图层，单独加载指定的图层。
有什么区别呢？目前来看，作为对图层的忠实展示的话，应该用第一种方式。按照前面的理解，动态图层是从服务器返回图片，前端展示；而要素图层，从服务器返回数据，前端绘制。所以，用要素图层加载，可能会漏掉一些啥东西，因为服务器只返回该图层特有的要素或特征（Feature）。
那什么时候使用FeatureLayer呢？绘制，在图层作画的时候。
