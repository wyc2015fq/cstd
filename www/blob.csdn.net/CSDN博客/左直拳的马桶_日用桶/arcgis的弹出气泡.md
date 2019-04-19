# arcgis的弹出气泡 - 左直拳的马桶_日用桶 - CSDN博客
2017年10月18日 18:52:56[左直拳](https://me.csdn.net/leftfist)阅读数：1285
arcgis里面，弹出气泡是很基本的操作。我目前知道的气泡方式，有2种。
**一、点击地理对象，出气泡**
这个是arcgis的固有功能，并不需要我们写什么代码。只须在加载图层时，指定一个模板参数：infoTemplate。
```
fl = new FeatureLayer(param.nodePath, {
    id: param.id + "",
    mode: FeatureLayer.MODE_ONDEMAND,
    definitionExpression: param.expression,
    infoTemplate: new InfoTemplate(param.title,param.template),//<-----
    outFields: fields,
    visible: false
});
```
这个所谓的模板参数，无非就是一些hmtl代码，里面指定了动态字段，如：
```xml
<b>${名称}</b><hr>类型:<b>${类型}</b><br />面积:<b>${面积}</b>（公顷）<br />所在市县：<b>${所在市} - ${所在县}</b><br />
```
仅此而已。图层加载以后，点击该图层上的各个地理块，即弹出infoWindow，内容按照模板设置输出。
**二、提示框TooltipDialog**
具体可写成当鼠标 mouseover 图层地理对象上面时，显示提示框；mouseout时提示框消失。 
![这里写图片描述](https://img-blog.csdn.net/20170508193726386?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
为加强效果，mouseover时，可将边线置亮。但是，这样一来，点击地理对象，就没有infoWindow可以弹出来了。原因是，为了置亮边线，其实是在地图上加了一个Grapthic对象，覆盖在原来的地理对象上面。这时，你点击这个地理对象时就被屏蔽了，实际上点击的是该Grapthic对象，所以默认的出来infoWindow功能就没有了。
代码如下：
```
//定义提示框
var dialog = new TooltipDialog({
    id: "tooltipDialog",
    style: "position: absolute; width: 250px; font: normal normal normal 10pt Helvetica;z-index:100"
});
dialog.startup();
var highlightSymbol = new SimpleFillSymbol(//红边画笔
    SimpleFillSymbol.STYLE_SOLID,
    new SimpleLineSymbol(
        SimpleLineSymbol.STYLE_SOLID,
        new Color([255, 0, 0]), 3
    ),
    new Color([125, 125, 125, 0.35])
);
//打开提示框
function showtip(template) {
    return function (evt) {
        var content = esriLang.substitute(evt.graphic.attributes, template);
        var highlightGraphic = new Graphic(evt.graphic.geometry, highlightSymbol);
        map.graphics.add(highlightGraphic);//红色描边
        dialog.setContent(content);
        domStyle.set(dialog.domNode, "opacity", 0.85);
        dijitPopup.open({
            popup: dialog,
            x: evt.pageX,
            y: evt.pageY
        });
    }
}
//关闭提示框
function closeDialog() {
    map.graphics.clear();//擦去红色描边
    dijitPopup.close(dialog);
}
function buildFeatureLayer(param) {
    var fl = new FeatureLayer(param.url, {
        id: param.id,
        mode: FeatureLayer.MODE_ONDEMAND,
        definitionExpression: param.expression,
        infoTemplate: param.template,
        outFields: param.fields,
        visible: false
    });
    map.addLayer(fl);
    if (param.template != null) {
        fl.on("mouse-over", showtip(param.template));//mouseover时，打开提示框
    }
    return fl;
}
map.on("load", function () {
    map.graphics.enableMouseEvents();
    map.graphics.on("mouse-out", closeDialog);//mouseout时，关闭提示框
    map.on("mouse-down", closeDialog);
});
```
其实，我想实现的效果是：mouseover时，出提示框；click时，出infoWindow。但昨天捣鼓了半天，没搞出来。留待空闲时再看看。
