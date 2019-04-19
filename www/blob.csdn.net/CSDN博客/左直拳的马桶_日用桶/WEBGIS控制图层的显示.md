# WEBGIS控制图层的显示 - 左直拳的马桶_日用桶 - CSDN博客
2017年01月19日 11:02:04[左直拳](https://me.csdn.net/leftfist)阅读数：1669
看别人搞的GIS应用，常常有勾选、取消某个图层的操作，然后地图上就有相应的图形显隐，觉得好酷。
咋实现的？
其实说起来好像也不是特别复杂。在WEB端，实现该功能的原理和步骤是：
1、WEB端加载地图服务的时候，可以拿到图层信息，包括名称，ID，父ID，子图层数组，默认可见性，等等。 
![这里写图片描述](https://img-blog.csdn.net/20170119104446273?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2、如此，即可以在WEB端列出图层信息矣
3、在列出图层的同时，可根据图层的默认可见性来控制图层的显隐，这是初始化阶段
4、之后，手动勾选每个图层前面的复选框，每勾选一次，则遍历图层列表，凡checked=true的，都将该图层ID放入某个数组，然后该数组作为参数，调用设置函数，如此，可相应控制图层显隐矣。
由4可知，控制任一图层的显示或隐藏，策略都是重绘地图。
代码如下：
```xml
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html>
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title>zt</title>
    <link rel="stylesheet" type="text/css" href="http://localhost/arcgis_js_api/library/3.15/3.15/dijit/themes/tundra/tundra.css"/>
    <link rel="stylesheet" type="text/css" href="http://localhost/arcgis_js_api/library/3.15/3.15/esri/css/esri.css" />
    <style type="text/css">
      .block{
        border:1px solid #999;
        height:600px;
      }
      .list_item{
      }
    </style>
  </head>
  <body class="tundra">
    <div id="mapDiv" style="float:right;width:85%;" class="block"></div>
    <div id="layer_list" style="float:right;width:13%;margin-right:10px;" class="block"></div>
  </body>
</html>
<script type="text/javascript" src="http://localhost/arcgis_js_api/library/3.15/3.15/init.js"></script>
<script type="text/javascript">
  dojo.require("esri.map");
  var map,visible = [];  
  var layer;  
  function init() {
    map = new esri.Map("mapDiv", {logo:false});
    layer = new esri.layers.ArcGISDynamicMapServiceLayer("http://WIN-NH9JATPNTFA:6080/arcgis/rest/services/ZT/MapServer");
    if (layer.loaded) {  
        buildLayerList(layer);  
    }  
    else {  
        dojo.connect(layer, "onLoad", buildLayerList);  
    } 
  }
  function buildLayerList(layer) {  
    var items = dojo.map(   // 适用于回调到给定数组中的每个元素，并返回一个数组的结果。  
        layer.layerInfos,   // 在服务返回可见的图层的数组，默认是可见的  
        function(info,index){  
            if(info.parentLayerId != -1){
              if (info.defaultVisibility) {  
                  visible.push(info.id);  // 将id编号放入数组中  
              }  
              return "<div><input type='checkbox' class='list_item'" + (info.defaultVisibility ? "checked=checked" : "") + "' id='" + info.id + "' onclick='updateLayerVisibility();' /><label for='" + info.id + "'>" + info.name + "(" + info.parentLayerId + ")</label></div>";  
            }
        }  
    );  
    dojo.byId("layer_list").innerHTML = items.join(' ');  
    layer.setVisibleLayers(visible,true);   // setVisibleLayers(ids, doNotRefresh?)  默认为false（及时刷新）  
    map.addLayer(layer);    // 将图层加入到地图中  
  }  
  function updateLayerVisibility() {  
    var inputs = dojo.query(".list_item"), input;   // 使用dojo.query()方法查询  
    visible = [];  
    // 遍历每一个文本框  
    dojo.forEach(inputs,function(input){  
      if (input.checked) {  
          visible.push(input.id);   // 填充数组值  
      }
    });  
    // 如果它们中有任何一个不显示的图层将 设置数组为 -1  
    if(visible.length === 0){  
      visible.push(-1);  
    }  
    layer.setVisibleLayers(visible); 
  }  
  dojo.addOnLoad(init);
</script>
```
![这里写图片描述](https://img-blog.csdn.net/20170119110120999?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
有个东西忘了讲，如果父层默认显示，并且选中，则子层的勾选没有意义，任你怎么点，都没啥反应，全部都是可见。所以我在代码中，并没有将根层列出来。
