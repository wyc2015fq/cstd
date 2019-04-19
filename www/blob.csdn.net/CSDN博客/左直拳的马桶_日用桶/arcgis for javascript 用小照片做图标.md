# arcgis for javascript 用小照片做图标 - 左直拳的马桶_日用桶 - CSDN博客
2017年05月08日 19:05:08[左直拳](https://me.csdn.net/leftfist)阅读数：1817
![这里写图片描述](https://img-blog.csdn.net/20170508181554922?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如图所示，勾选相关选项，在地图上就显示这两小张图片，点击则出现明细大图。
![这里写图片描述](https://img-blog.csdn.net/20170508184601939?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
咋整？
首先，这两张小图片不是地图的一部分，也不是从地图服务器里请求回来的，是客户端自己画的，所以，它们位于FeatureLayer。
其次，这两张小图不一样，它们处于不同的FeatureLayer。也就是说，一张图层只能显示一种图标。
具体代码该咋写？
![这里写图片描述](https://img-blog.csdn.net/20170508182334175?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
图一  
图一的代码结构很简单，如果选项”采集照片“点击选中，则检查是否已生成相应FeatureLayer，没有则创建之。
如何创建呢？ 
首先，FleatureLayer里面有许多选项和字段，要先定义。因为FeatureLayer是在客户端描绘生成的。在我们平常使用的FeatureLayer中，有许多是由地图服务提供的，但地图服务提供的也是定义和数据，并非像切片图层和动态图层那样，传给客户端的是图片。
其次是绑定一些事件。比如图层成功添加的后续处理。我的思路是图标的样式（也就是那两张小图片）并非预先设定，而是根据后台获得的数据来动态生成。所以，图层可以先生成并添加，添加成功后再向后台请求数据，并绘制图标。
上代码。这是前端代码：
```java
//采集照片
(function (map, iconUrlHead, requestUrl) {
    var fls = [];
    $("#chePhoto").click(function () {
        var bv = document.getElementById("chePhoto").checked;
        if (bv == true && fls.length == 0) {
            /* 动态构造要素图层（定义部分） begin */
            var featureCollection = {
                "layerDefinition": null,
                "featureSet": {
                    "features": [],
                    "geometryType": "esriGeometryPoint"
                }
            };
            featureCollection.layerDefinition = {
                "geometryType": "esriGeometryPoint",
                "objectIdField": "ObjectID",
                "drawingInfo": {
                    "renderer": {
                        "type": "simple",
                        "symbol": {
                            "type": "esriPMS",
                            "url": null,
                            "contentType": "image/png",
                            "width": 52.5,//点，这里 1P = 0.75 * 1PX。arcgis的图标不是按像素来画的，而是点。
                            "height": 37.5
                        }
                    }
                },
                "fields": [{
                    "name": "ObjectID",
                    "alias": "ObjectID",
                    "type": "esriFieldTypeOID"
                }, {
                    "name": "description",
                    "alias": "Description",
                    "type": "esriFieldTypeString"
                }, {
                    "name": "title",
                    "alias": "Title",
                    "type": "esriFieldTypeString"
                }]
            };
            /* 动态构造要素图层（定义部分） end */
            //弹窗
            var popupTemplate = new PopupTemplate({
                title: "{title}",
                description: "{description}"
            });
            function getFL(typeId) {
                var iconUrl = iconUrlHead + "map_icon" + typeId + ".png";//根据参数来构造图标
                featureCollection.layerDefinition.drawingInfo.renderer.symbol.url = iconUrl;
                //创建FeatureLayer
                var featureLayer = new FeatureLayer(featureCollection, {
                    id: "photolayer" + typeId,
                    infoTemplate: popupTemplate,
                    visible: false
                });
                featureLayer.on("click", function (evt) {
                    map.infoWindow.setFeatures([evt.graphic]);
                    map.infoWindow.resize(400, 300);
                });
                return featureLayer;
            }
            fls.push(getFL("01"));//创建id=photolayer01的图层
            fls.push(getFL("02"));//创建id=photolayer02的图层
            //绑定 “图层添加成功” 事件
            map.on("layers-add-result", function (results) {
                function requestSucceeded(response, io) {//获取后台数据成功。获取过程是异步的，见下面：requestHandle.then(requestSucceeded, requestFailed);
                    var features = {};
                    array.forEach(response.items, function (item) {
                        var attr = {};
                        attr["description"] = item.description;
                        attr["title"] = item.title ? item.title : "采集照片";
                        var geometry = new Point(item);//new Point(json);json object内容自定义，但必须有坐标值，名称为x、y或longitude、latitude皆可。
                        var graphic = new Graphic(geometry);
                        graphic.setAttributes(attr);
                        if (typeof features[item.type] == 'undefined') features[item.type] = [];
                        features[item.type].push(graphic);
                    });
                    results.layers.forEach(function (resultitem) {
                        var id = resultitem.layer.id.substring("photolayer".length);//图层id=photolayer***
                        resultitem.layer.applyEdits(features[id], null, null);
                    });
                }
                function requestFailed(error) {
                }
                var requestHandle = esriRequest({
                    url: requestUrl,
                    callbackParamName: "jsoncallback"
                });
                //向后台（不是地图服务）请求数据是一个异步的过程
                requestHandle.then(requestSucceeded, requestFailed);
            });
            map.addLayers(fls);
        }
        fls.forEach(function (fl) {
            fl.setVisibility(bv);
        });
    });
})(map,
    "@Url.StaticFile("~/Content/images/map/")",
    "@Url.StaticFile("~/Map/Home/GetPhotos")");
```
featureCollection.layerDefinition.drawingInfo.renderer.symbol.url =“图标地址”; 
featureCollection.layerDefinition是图层定义。图层定义可以用在FeatureLayer的构造函数里，也可以如上面这句，图层生成后再修改。
后台代码：
```
public ContentResult GetPhotos()
        {
            return new ContentResult
            {
                ContentEncoding = Encoding.GetEncoding("utf-8"),
                ContentType = "text/plain;charset=UTF-8",
                //一个item代表一种图标；图标数据自定义，但必须有坐标值，名称为x、y或longitude、latitude皆可
                Content = $@"{{
""title"": ""近期采集的照片"",
""items"": [
    {{
        ""title"": ""海岛一"",
        ""description"": ""<p><img src=\""{System.Web.HttpContext.Current.Request.ApplicationPath.Replace("/",@"\/")}\/Content\/Images\/Map\/hd1.jpg\"" alt=\""海岛2\"" \/><\/p>"",
        ""latitude"" : ""28.163280"",
        ""longitude"" : ""121.598709"",
        ""type"":""01""
    }},
    {{
        ""title"": ""海岛二"",
        ""description"": ""<p><img src=\""{System.Web.HttpContext.Current.Request.ApplicationPath.Replace("/", @"\/")}\/Content\/Images\/Map\/hd1.jpg\"" alt=\""海岛2\"" \/><\/p>"",
        ""latitude"" : ""29.163280"",
        ""longitude"" : ""121.598709"",
        ""type"":""01""
    }},
    {{
        ""title"": ""珊瑚礁"",
        ""description"": ""<p><img src=\""{System.Web.HttpContext.Current.Request.ApplicationPath.Replace("/", @"\/")}\/Content\/Images\/Map\/hd2.jpg\"" alt=\""海岛2\"" \/><\/p>"",
        ""latitude"" : ""29.609811"",
        ""longitude"" : ""122.341567"",
        ""type"":""02""
    }}]
}}"
            };
        }
```
