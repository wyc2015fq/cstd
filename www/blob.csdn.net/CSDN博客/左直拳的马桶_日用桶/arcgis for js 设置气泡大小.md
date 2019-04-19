# arcgis for js 设置气泡大小 - 左直拳的马桶_日用桶 - CSDN博客
2017年12月04日 18:59:20[左直拳](https://me.csdn.net/leftfist)阅读数：680
气泡大小，也就是`map.infoWindow`，是有函数可以进行设置的：`map.infoWindow.resize(width,height)`。
问题是，什么时候进行设置？
在创世纪，即map初始化之时可以进行设置；
在图层加载时也可以进行设置；
然后呢？
map.infoWindow只有一个，是所有图层公用的，你在map创建之初设置，根本不能适应各个图层的特有情况；在加载图层时设置好一点，但同时加载多个图层又该如何？
最自然的想法，就是在点击图块的时候，再根据点击的是哪个图层来进行设置。可是这个点击事件是什么呢？map.onclick,还是graphic.onclick?即使搞得很清楚，也要传不少参数吧。
**方案一、在气泡内容中带上气泡样式**
最好是由气泡内容自己驱动，根据气泡内容的不同，自动调整气泡的大小。气泡的内容是在加载图层的时候，作为模板传给infoWindow的：
```
var featureLayer = new FeatureLayer("http://sampleserver1.arcgisonline.com/rest/services/MapServer/0",{
    mode: FeatureLayer.MODE_ONDEMAND,
    infoTemplate: infoTemplate,//模板
    outFields: ["*"]
  });
```
所谓的模板，其实是一些包含显示字段的html，类似：
```
<div>优势种名:<b>${YSZMC}</b></div>
<div>经度:<b>${JD}</b></div>
<div>纬度:<b>${WD}</b></div>
<div>范围:<b>${FW}</b>平方公里</div>
```
可以将设置气泡的样式的CSS代码写在里面：
```xml
<style>
.sizer{width:500px!important;}/*气泡宽度*/
.sizer.content{overflow:hidden;}
.contentPane{max-height:345px!important;}/*气泡内容高度*/
</style>
<div>优势种名:<b>${YSZMC}</b></div>
<div>经度:<b>${JD}</b></div>
<div>纬度:<b>${WD}</b></div>
<div>范围:<b>${FW}</b>平方公里</div>
```
我今天运行了下，此方案可行。并且该样式只对当前气泡起作用，并不会影响其他。
**方案二、气泡加载后，根据气泡内容改变气泡样式**
气泡有一个打开之后的事件：
```
map.infoWindow.on("show", function (targ) {
    var dnodes = targ.target.domNode.childNodes[0].childNodes;//构成气泡的标题、内容、页脚、及连接三角符。
    var content = targ.target.domNode.innerHTML;//气泡内容，html
});
```
![这里写图片描述](https://img-blog.csdn.net/20171204185520395?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
我意，在此事件中，分析气泡内容，设置dnodes的相关节点。仅在chrome的debugger下试了一下，可行，但没有真正写在代码里。
两个方案，一个是内容提供方设置，一个是内容受理方设置，我喜欢第一种。
