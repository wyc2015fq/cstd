# arcgis for js如何画台风风圈 - 左直拳的马桶_日用桶 - CSDN博客
2018年01月05日 19:29:53[左直拳](https://me.csdn.net/leftfist)阅读数：1571
arcgis for js里面，画一个圆不是什么难事。小的可以用`SimpleMarkerSymbol`，`"style": "esriSMSCircle"`； 
大的话，直接就来一个`new Circle`
但是，如果是类似台风风圈这类的呢？ 
![这里写图片描述](https://img-blog.csdn.net/20180105190913660?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
似圆非圆，该咋画？ 
答案是画多边形。如图所示的风圈，正是3个多边形。其实所谓的圆，不就是多边形无限逼近的结果吗？好像微积分里有说到这个？
画台风风圈的原理如下：以圆心为原点，以正北方向为0度，顺时针画点，1度为1个点，并连成线，并最终回到正北方向，形成封闭的多边形；然后再加以填充。由于台风半径分为东北、东南、西南、西北四个方向，每个方向距离圆心可能长度不一，于是就形成了有四个锯齿状的风圈。
代码如下：
```java
function getAirRing(c, center, index) {//风圈;level风力级数，center原点
    var arColor;
    switch (c.level) {
        case 7: arColor = [83, 168, 113]; break;//7级风圈
        case 10: arColor = [175, 192, 130]; break;
        case 12: arColor = [185, 121, 96]; break;
        default: arColor = [83, 168, 113]; break;
    }
    //算出所有的点
    var points = new Array();
    getPoints(center, c.EN, 0);//东北方向
    getPoints(center, c.ES, 90);
    getPoints(center, c.WS, 180);
    getPoints(center, c.WN, 270);
    points[points.length] = points[0];//首尾连接
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
    //将点连成多边形
    var polygon = new esri.geometry.Polygon(points);
    //并填充，还附带上各种风圈数据
    var gr = new esri.Graphic(polygon, sfl,
        {
            "level": c.level, "wn": c.WN, "en": c.EN, "es": c.ES, "ws": c.WS,
            "attrtype": "airRing", "index": index,
        },
        new esri.InfoTemplate("风圈:${level}", tplAirRing));
    return gr;
    //此方法照抄来的，1度1个点
    function getPoints(center, cradius, startAngle) {
        var radius = cradius / 100; 
        var pointNum = 90;
        var endAngle = startAngle + 90;
        var sin;
        var cos;
        var x;
        var y;
        var angle;
        for (var i = 0; i <= pointNum; i++) {
            angle = startAngle + (endAngle - startAngle) * i
                / pointNum;
            sin = Math.sin(angle * Math.PI / 180);
            cos = Math.cos(angle * Math.PI / 180);
            x = center[0] + radius * sin;
            y = center[1] + radius * cos;
            points.push([x, y]);
        }
    }
}
```
