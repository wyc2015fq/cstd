# 基于开源项目SharpMap的热力图(HeatLayer)实现。 - 毛小亮 - 博客园
# [基于开源项目SharpMap的热力图(HeatLayer)实现。](https://www.cnblogs.com/xianerwonder/p/6078492.html)
　　当前公司需要一个用时较少的热力图呈现方案，在避免较底层的GDI开发和比较了多家GIS产品的实际效果之后，团队决定用sharpMap的API来实现，由于之前框架采用的是另外一个开源项目GMap.net，两个项目的交互必然存在一个过渡，而这个过渡就是Image类。
　　为了方便大家理解，以及之后我回头再看不至于看不懂，加入了较多的注释。
**先放一张最终效果图和数据点的对比：**
![](https://images2015.cnblogs.com/blog/688944/201611/688944-20161118172954607-1985882310.png)
**实现过程：**
1、首先添加几个NuGet程序包，SharpMap和SharpMap.Layers.HeatLayer，另外，个人建议把SharpMap.UI也加入到项目里，因为地图控件在这个程序集之中。
2、初始化一个地图对象
```
1 private static SharpMap.Map GetDefaultMap(Size sharpMapSize)
 2         {
 3             //必须先初始化一个地图对象，否则数据源会报没有初始化对象(这tm肯定是个bug)。
 4             SharpMap.Map mySharpMap = new SharpMap.Map(new Size(sharpMapSize.Width, sharpMapSize.Height));
 5             //这里可以设置最小的放大级别，如果不够小，地图无法缩放，会影响最终image边界。
 6             mySharpMap.MinimumZoom = 0.01; //Minimum zoom allowed 
 7             //背景色透明
 8             mySharpMap.BackColor = Color.Transparent;
 9 
10             return mySharpMap;
11         }
地图对象构建
```
3、通过一个ShapeFile文件，构造热图层对象。
```
1 private static SharpMap.Layers.HeatLayer GetHeatLayerByVector(string vectorPath, string valueColumn, float scale)
 2         {
 3             //初始化一个矢量图层，作为数据源。
 4             SharpMap.Layers.VectorLayer myVectorLayer = new SharpMap.Layers.VectorLayer("My layer");
 5             //这个数据源非常重要。
 6             myVectorLayer.DataSource = new SharpMap.Data.Providers.ShapeFile(vectorPath);
 7             myVectorLayer.Style.PointColor = Brushes.Transparent;
 8 
 9             //新建热层对象。第三个参数非常重要，与value相乘后的百分数决定了symbol显示的颜色，大于1的是靛蓝色
10             SharpMap.Layers.HeatLayer heatLayer = new SharpMap.Layers.HeatLayer(myVectorLayer, valueColumn, scale);
11 
12             return heatLayer;
13         }
构造图层
```
4、图层相关设置
```
1             //这两个变量跟比例尺有关，在区间内可变，区间外symbol不再变
2             heatLayer.ZoomMin = 0f;
3             heatLayer.ZoomMax = 1000000f;//myMap.GetExtents().Width;
4             //控制透明度
5             heatLayer.OpacityMax = 0.2f;
6             heatLayer.OpacityMin = 0.6f;
7             //设置地图颜色条带，官方就2种，一种彩虹，一种火焰。
8             heatLayer.HeatColorBlend = SharpMap.Layers.HeatLayer.Classic;    
设置图层参数
```
5、根据图层和地图对象，得到image
```
1 private static System.Drawing.Image GetHeatLayerImage(SharpMap.Layers.HeatLayer heatLayer, SharpMap.Map mySharpMap, Envelope Extent)
 2         {
 3 
 4             //在地图对象中加入构造的热图对象。
 5             mySharpMap.Layers.Add(heatLayer);
 6             //转到当前四至范围
 7             mySharpMap.ZoomToBox(Extent);
 8             //通过地图对象，拿到img对象。-----------------------------------------------Image-----------------------------
 9             Image imgMap = mySharpMap.GetMap();
10 
11             return imgMap;
12         }
得到image
```
6、之后根据这个image，加载到相关地图控件就可以了，由于地图控件代码并不属于SharpMap项目，暂时不给出了。
ps:如果有人需要**源码**，**请移步**[传送门](http://download.csdn.net/detail/mm190835773/9702687)。如有任何问题，请回复交流。

