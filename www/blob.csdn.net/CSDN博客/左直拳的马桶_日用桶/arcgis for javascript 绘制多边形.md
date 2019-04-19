# arcgis for javascript 绘制多边形 - 左直拳的马桶_日用桶 - CSDN博客
2017年05月23日 20:27:50[左直拳](https://me.csdn.net/leftfist)阅读数：2869
arcgis for javascript 绘制多边形。怎样绘制呢？
> 
1、读取数据库中的多边形数据 
  2、在地图上绘制多边形 
  3、地图放大，并定位到指定的坐标
地图放大很重要。因为一般我们画的多边形，可能放在一幅全国地图里，会非常的小，如果不放大地图比例，根本看不到，还以为我们啥都没画出来。
**1、读取数据库中的多边形数据**
不提。我读的是一个oracle数据库中的，geometry类型的字段内容。要把它转成
`""polygon"":[[121.55535194, 28.56218222],[121.55510556, 28.56213333],[121.55667889, 28.55941528],[121.55660250, 28.55925389],[121.55492417, 28.55878417],[121.55445528, 28.55865306],[121.55254972, 28.55811944],[121.55234250, 28.55820278],[121.55084500, 28.56132222],[121.55058722, 28.56127278],[121.55220694, 28.55790889],[121.55452083, 28.55855889],[121.55457000, 28.55845778],[121.55671556, 28.55905833],[121.55693139, 28.55897861],[121.55940111, 28.55471250],[121.55684139, 28.55379361],[121.55708361, 28.55329611],[121.55738778, 28.55267444],[121.55988917, 28.55357222],[121.56039806, 28.55376333],[121.56187444, 28.55433389],[121.56232306, 28.55451222],[121.56401778, 28.55520639],[121.56327111, 28.55624194],[121.56155694, 28.55554528],[121.55962750, 28.55479722],[121.55716861, 28.55904500],[121.55724500, 28.55920639],[121.55912667, 28.55973306],[121.56048028, 28.56011194],[121.56035333, 28.56030361],[121.55713167, 28.55940194],[121.55691583, 28.55948167],[121.55535194, 28.56218222]],`
这种格式。
**2、在地图上绘制多边形**
如何绘制呢？ 
**1）动态构造一个要素图层** FeatureLayer（属于图形图层），并添加到地图；
**2）在地图添加图层事件中，绘制几何图形**
一个图形图层，可以绘制多个几何图形，不同颜色，不同边线。
```java
//图层成功添加后，向服务器请求数据，并绘制到该图层
//参数results应该就是成功添加的图层
    map.on("layers-add-result", function (results) {
        function requestSucceeded(response, io) {
            var features = [];
            var point;
            array.forEach(response.items, function (item) {
                var attr = {};
                attr["description"] = item.description;
                attr["title"] = item.title ? item.title : "移动采集";
                //定义“画笔”
                var symbol = new SimpleFillSymbol({
                    "type": "esriSFS",
                    "style": "esriSFSSolid",
                    "color": item.bg,//背景色在服务器数据中定义
                    "outline": {//轮廓
                        "type": "esriSLS",
                        "style": "esriSLSSolid",
                        "color": item.outline,
                        "width": 1
                    }
                });
                //绘制一个多边形
                var polygon = new Polygon(new SpatialReference({ wkid: 4326 }));
                polygon.addRing(item.polygon);
                var graphic = new Graphic(polygon, symbol);
                graphic.setAttributes(attr);
                //定位坐标点
                point = new Point([item.longitude, item.latitude], new SpatialReference({ wkid: 4326 }));
                features.push(graphic);//画好的多边形加到要素集
            });
            results.layers.forEach(function (resultitem) {//要素集加到图层
                resultitem.layer.applyEdits(features, null, null);
            });
            if (point != null) {
                map.centerAndZoom(point, 15);//缩放并定位
            }
        }
        function requestFailed(error) {
        }
        var requestHandle = esriRequest({
            url: requestUrl,//数据请求地址
            callbackParamName: "jsoncallback"
        });
        requestHandle.then(requestSucceeded, requestFailed);
    });
```
有关这个symbol，可以参考如下资料： 
[http://help.arcgis.com/en/arcgisserver/10.0/apis/rest/symbol.html#sfs](http://help.arcgis.com/en/arcgisserver/10.0/apis/rest/symbol.html#sfs)。十几天前，我对这个symbol颇有感悟，但现在已经忘得差不多了。
**3、地图放大，并定位到这个多边形的第一个点。**
`map.centerAndZoom(point, 15);//缩放并定位`
