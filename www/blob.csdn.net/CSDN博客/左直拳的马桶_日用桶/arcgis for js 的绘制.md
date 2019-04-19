# arcgis for js 的绘制 - 左直拳的马桶_日用桶 - CSDN博客
2018年01月05日 20:03:47[左直拳](https://me.csdn.net/leftfist)阅读数：462
我尝试捋一下arcgis for js中有关画图的各种对象关系。
1、要想绘制，最好在GraphicsLayer或FeatureLayer上进行。FeatureLayer继承自GraphicsLayer。
2、symbol是画笔。有好几种： 
![这里写图片描述](https://img-blog.csdn.net/20180105194335301?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```
//设置填充symbol
    var bColor = arColor; bColor.push(0.5);//透明度
    var sfl = new esri.symbol.SimpleFillSymbol(
        esri.symbol.SimpleFillSymbol.STYLE_SOLID,
        new esri.symbol.SimpleLineSymbol(
            esri.symbol.SimpleLineSymbol.STYLE_SOLID,
            new dojo.Color(arColor),
            2
        ),
        new dojo.Color(bColor)
    );
```
3、但画笔要绘制成图形才算
```
//将点连成多边形
    //注意这个多边形并未画在“纸”上，只形成于画家的“脑海”
    //或者说，仅是打草稿阶段，用铅笔在纸上简单地勾画了几下
    var polygon = new esri.geometry.Polygon(points);
    //直到这时才用画笔symbol画出来，还附带上各种自定义数据
    var gr = new esri.Graphic(polygon, sfl,
        {
            "level": c.level, "wn": c.WN, "en": c.EN, "es": c.ES, "ws": c.WS,
            "attrtype": "airRing", "index": index,
        },
        new esri.InfoTemplate("风圈:${level}", tplAirRing));
```
4、但是，地图上还不会出现这幅画，直到明确地将画加进图层
`layer.add(gr)`
> 
layer：图层  
  graphic：图像  
  geometry：几何对象  
  symbol：画笔
其中graphic可以承载我们加进去的信息：
```
//直到这时才用画笔symbol画出来，还附带上各种风圈数据
    var gr = new esri.Graphic(polygon, sfl,
        {
            "level": c.level, "wn": c.WN, "en": c.EN, "es": c.ES, "ws": c.WS,
            "attrtype": "airRing", "index": index,
        },
        new esri.InfoTemplate("风圈:${level}", tplAirRing));
```
```xml
var tplAirRing = "<style>.tyconring{padding: 3px;}"
    + ".tyconring div{line-height:25px;height:25px;}"
    + ".e{width: 49%;text-align: center;float: left;}"
    + ".d{border-right:solid 1px #000;}"
    + "hr{width:100%;}"
    + "</style>"
    + "<div class='tyconring'>"
    + "<div>${level}级风圈半径：</div>"
    + "<div><div class='e d'>西北${wn}公里</div><div class='e'>东北${en}公里</div></div>"
    + "<hr />"
    + "<div><div class='e d'>西南${ws}公里</div><div class='e'>东南${es}公里</div></div>"
    + "</div>";
```
这些信息，在需要的时候可以派上用场： 
![这里写图片描述](https://img-blog.csdn.net/20180105200332101?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
