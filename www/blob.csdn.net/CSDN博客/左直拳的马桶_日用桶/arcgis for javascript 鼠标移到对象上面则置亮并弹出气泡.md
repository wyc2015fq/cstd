# arcgis for javascript 鼠标移到对象上面则置亮并弹出气泡 - 左直拳的马桶_日用桶 - CSDN博客
2017年05月08日 19:46:23[左直拳](https://me.csdn.net/leftfist)阅读数：2493
![这里写图片描述](https://img-blog.csdn.net/20170508193726386?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
地图中，为啥鼠标移到某块地形上，该地形就能置亮，并弹出气泡呢？
”是事件“，西蒙说。
```java
var fl = new FeatureLayer(...);
map.addLayer(fl);
fl.on("mouse-over", showtip(param.template));
function showtip(template) {
    return function (evt) {
        var content = esriLang.substitute(evt.graphic.attributes, template);
        var highlightGraphic = new Graphic(evt.graphic.geometry, highlightSymbol);
        map.graphics.add(highlightGraphic);//添加一个高亮对象到地图
        dialog.setContent(content);
        domStyle.set(dialog.domNode, "opacity", 0.85);
        dijitPopup.open({
            popup: dialog,
            x: evt.pageX,
            y: evt.pageY
        });
    }
}
//对话框
var dialog = new TooltipDialog({
    id: "tooltipDialog",
    style: "position: absolute; width: 250px; font: normal normal normal 10pt Helvetica;z-index:100"
});
dialog.startup();
//置亮样式
var highlightSymbol = new SimpleFillSymbol(
    SimpleFillSymbol.STYLE_SOLID,
    new SimpleLineSymbol(
        SimpleLineSymbol.STYLE_SOLID,
        new Color([255, 0, 0]), 3
    ),
    new Color([125, 125, 125, 0.35])
);
```
