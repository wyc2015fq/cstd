# arcgis for js 通用气泡 - 左直拳的马桶_日用桶 - CSDN博客
2018年11月18日 12:33:42[左直拳](https://me.csdn.net/leftfist)阅读数：213
```
var infoTemplate = new InfoTemplate("Attributes", "${*}");
```
arcgis for js中，气泡（InfoTemplate）里的内容一般是定制的，里面要显示哪些字段，格式如何，都可以定制。
但如果事先不知道每个图层有哪些字段呢，怎么展示？
办法就是全部展示，气泡中展示所有字段内容。
如何展示所有字段内容？
方法就是文章开头第一句。
什么场景试用这种耍泼的做法？
图层太多的情况。众所周知，arcgis for js中，动态图层（ArcGISDynamicMapServiceLayer）是一个图层集合，加载的时候所有子图层一股脑地全部加载进来。如果一个系统，又有好多个这种动态图层，那么图层数量就非常大了。子子孙孙，无穷匮也。如果每个子图层，都定义一下，工作量挺大。
所以，简单的一刀切办法就是有啥字段我照单全收，全部显示。完整代码如下：
```
var layer = new ArcGISDynamicMapServiceLayer("图层地址");
var sublayerId = 子图层ID;
var templates = {};
var t = new InfoTemplate("子图层名称", "${*}");
templates[sublayerId + ""] = { infoTemplate: t };
layer.setInfoTemplates(templates);//
```
