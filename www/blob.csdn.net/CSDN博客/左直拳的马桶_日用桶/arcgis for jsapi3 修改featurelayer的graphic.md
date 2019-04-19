# arcgis for jsapi3 修改featurelayer的graphic - 左直拳的马桶_日用桶 - CSDN博客
2019年03月04日 19:05:15[左直拳](https://me.csdn.net/leftfist)阅读数：80
featurelayer，要素图层/属性图层，里面充满了一个个的graphic对象。咱们可以在客户端构建一个featurelayer（featurelayer是graphicslayer的子类），然后在上面任意作画。
画的图形一多，就会有性能的问题。据说这是arcgis for jsapi3的专利，最多只能支持几千个graphic，而且渲染的速度还很慢；4的话就好很多了，用webgl绘制，贼快；据说还能直接将数据赋给featurelayer，让它自己渲染。也就是说，假设数据变了，直接丢给featurelayer，无须指导它一个个怎么重画。
但是！4.10有好多坑；所以我们又改回了3.27。呵呵，一切从头再来，收拾旧河山，朝天阙。雄鸡一声撅一撅；雄鸡二声撅二撅；三声唤出夏桑菊，扫退火气与眼屎。
总结一下：
**一、批量更新graphic对象**
在featurelayer上添加、修改、删除graphic对象，如果数量很多，最好是调用它这个方法：
```
applyEdits(adds?, updates?, deletes?, callback?, errback?)
```
已经测试过了，我们构造一个graphic对象，这只是在内存上操作而已，只要还没`featurelayer.add(graphic)`，地图上就还没生效。但是，你add吧，地图就会更新；如果成千上万个地add，就意味着刷新成千上万次，就会很慢了。
但用这个applyEdits的话，它应该是只刷新一次。
有关这个applyEdits，它可以用在手动构建的featurelayer上。
**二、如何更换graphic对象**
想当然地，将原来的graphic对象remove，然后再画一个。
拿衣服！愚不可及！
可以酱紫：
```
//g,newg都是graphic对象
//gemoetry是几何对象，即几何形状、坐标等等
//symbol，外形
newg.attributes.OBJECTID = g.attributes.OBJECTID;//假设OBJECTID是主键，要保留
g = g.setGeometry(newg.geometry).setSymbol(newg.symbol).setAttributes(newg.attributes);
```
