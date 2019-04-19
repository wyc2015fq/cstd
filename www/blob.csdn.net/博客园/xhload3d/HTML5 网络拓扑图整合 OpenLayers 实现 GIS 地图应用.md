# HTML5 网络拓扑图整合 OpenLayers 实现 GIS 地图应用 - xhload3d - 博客园
在前面《[百度地图、ECharts整合HT for Web网络拓扑图应用](http://www.cnblogs.com/xhload3d/p/4358804.html)》我们有介绍百度地图和 [HT for Web](http://www.hightopo.com/) 的整合，我们今天来谈谈 [OpenLayers](http://openlayers.org/) 和 [HT for Web](http://www.hightopo.com/) 的整合。
[HT for Web](http://www.hightopo.com/)作为逻辑拓扑图形组件自身没有GIS功能，但可以与各种GIS引擎即其客户端组件进行融合，各取所长实现逻辑拓扑和物理拓扑的无缝融合，本章将具体介绍[HT for Web](http://www.hightopo.com/)与开发免费的[OpenLayers](http://openlayers.org/)地图结合应用的关键技术点，该文介绍的结合的原理，其实还可推广到与ArcGIS、百度地图以及GoogleMap等众多GIS地图引擎融合的解决方案。
![Screen Shot 2014-12-02 at 1.15.33 AM](http://www.hightopo.com/blog/wp-content/uploads/2014/12/Screen-Shot-2014-12-02-at-1.15.33-AM1.png)
以上抓图为本文介绍的例子最终运行效果，接下来我们一步步来实现，首选显示地图信息需要有城市经纬度数据，搜索了下[感谢此篇博客提供的数据](http://www.cnblogs.com/zhangqs008/archive/2011/05/09/2341138.html)。这么大量的数据我采用的是《[HT图形组件设计之道（四）](http://www.hightopo.com/blog/229.html)》中介绍的getRawText函数方式，有了数据之后剩下就是呈现的问题了，我们需要将[HT](http://www.hightopo.com/)的GraphView组件与OpenLayers的map地图组件叠加在一起，也就是[OpenLayers](http://openlayers.org/)的tile地图图片在下方，GraphView的组件在上方，由于GraphView默认是透明的，因此非图元部分用户可穿透看到地图内容。找到合适的组件插入位置是头疼的事情，ArcGIS、百度地图包括GoogleMap几乎每个不同的GIS组件都需要尝试一番才能找到合适的插入位置，其他GIS引擎组件的整合以后章节再介绍，本文我们关注的OpenLayers的插入方式为map.viewPortDiv.appendChild(graphView.getView())。
[HT](http://www.hightopo.com/)和OpenLayers组件叠加在一起之后，剩下就是拓扑里面图元的摆放位置与经纬度结合的问题，常规网络拓扑图中存储在ht.Node图元的position是逻辑位置，和经纬度没有任何关系，因此在GIS应用中我们需要根据图元的经纬度信息换算出position的屏幕逻辑坐标信息，如果你知道投影算法也可以自己提供函数处理，但所有GIS组件都提供了类似的API函数供调用，当然这部分也没有标准化，不同的GIS组件需要调用的API都有差异，但基本原理是一致的，对于OpenLayers我们通过map.getPixelFromLonLat(data.lonLat)可以将经纬度信息转换成屏幕像素逻辑坐标，也就是ht.Node需要的position坐标信息。
细心的同学会想到转换是双向的，有可能用户需要拖动图元节点改变其经纬度信息，这时候我们就需要另外一个方向函数，即根据屏幕逻辑坐标转换成当前坐标对应的经纬度，在OpenLayers中我们通过map.getLonLatFromPixel(new OpenLayers.Pixel(x, y));可以搞定。
显示搞定后剩下就是交互的问题了，[HT](http://www.hightopo.com/)自己有套交互体系，OpenLayers也需要地图漫游和缩放的交互，两者如何结合呢？如果能保留住两者的功能那就最好了，答案时肯定的，我们只需要添加mousedown或touchstart事件监听，如果graphView.getDataAt(e)选中了图元我们就通过e.stopPropagation();停止事件的传播，这样map地图就不会响应，这时候[HT](http://www.hightopo.com/)接管了交互，如果没有选中图元则map接管地图操作的交互。
以上交互设计似乎很完美了，结果运行时发现了几处折腾了我很久才找到解决方案的坑：
- 设置map.events.fallThrough = true;否则map不会将事件透传到[HT](http://www.hightopo.com/)的GraphView组件
- graphView.getView().style.zIndex = 999; 需要指定一定的zIndex否则会被遮挡
- graphView.getView().className = ‘olScrollable’; 否则滚轮不会响应地图缩放
- 设置ht.Default.baseZIndex: 1000 否则ToolTip会被遮挡
为了让这个例子用户体验更友好，我还用心折腾了些技术点供参考：
- 采用开源免费的[http://llllll.li/randomColor/](http://llllll.li/randomColor/)随机颜色类库，该类库还有很多非常棒的颜色获取函数，我只是简单的为每个省份显示不一样的颜色
- 重载了isVisible、isNoteVisible和isLabelVisible仅在缩放达到一定级别才显示更详细的内容，否则缩小时所有城市信息都显示完全无法查看，多少也能提高显示性能
以下为最终效果的抓图、视频和源代码：[http://v.youku.com/v_show/id_XODM5Njk0NTU2.html](http://v.youku.com/v_show/id_XODM5Njk0NTU2.html)
http://player.youku.com/player.php/sid/XODM5Njk0NTU2/v.swf
![Screen Shot 2014-12-02 at 1.15.33 AM](http://www.hightopo.com/blog/wp-content/uploads/2014/12/Screen-Shot-2014-12-02-at-1.15.33-AM2.png)![Screen Shot 2014-12-02 at 1.16.18 AM](http://www.hightopo.com/blog/wp-content/uploads/2014/12/Screen-Shot-2014-12-02-at-1.16.18-AM.png)![Screen Shot 2014-12-02 at 1.16.47 AM](http://www.hightopo.com/blog/wp-content/uploads/2014/12/Screen-Shot-2014-12-02-at-1.16.47-AM.png)![Screen Shot 2014-12-02 at 1.17.30 AM](http://www.hightopo.com/blog/wp-content/uploads/2014/12/Screen-Shot-2014-12-02-at-1.17.30-AM.png)
```
function init(){                
    graphView = new ht.graph.GraphView();
    var view = graphView.getView();                 
    map = new OpenLayers.Map("map");
    var ol_wms = new OpenLayers.Layer.WMS(
        "OpenLayers WMS",
        "http://vmap0.tiles.osgeo.org/wms/vmap0",
        {layers: "basic"}
    );
    map.addLayers([ol_wms]);
    map.addControl(new OpenLayers.Control.LayerSwitcher());
    map.zoomToMaxExtent();                
    map.events.fallThrough = true;
    map.zoomToProxy = map.zoomTo;
    map.zoomTo =  function (zoom,xy){
        view.style.opacity = 0;
        map.zoomToProxy(zoom, xy);    
        console.log(zoom);
    };                
    map.events.register("movestart", this, function() {
    });
    map.events.register("move", this, function() {                   
    });
    map.events.register("moveend", this, function() {
        view.style.opacity = 1;
        reset();
    });                
    graphView.getView().className = 'olScrollable';
    graphView.setScrollBarVisible(false);
    graphView.setAutoScrollZone(-1);
    graphView.handleScroll = function(){};
    graphView.handlePinch = function(){};     
    graphView.mi(function(e){
        if(e.kind === 'endMove'){
            graphView.sm().each(function(data){
                if(data instanceof ht.Node){
                   var position = data.getPosition(),
                       x = position.x + graphView.tx(),
                       y = position.y + graphView.ty();  
                   data.lonLat = map.getLonLatFromPixel(new OpenLayers.Pixel(x, y));                                                                     
                }                            
            });
        }
    });
    graphView.enableToolTip();
    graphView.getToolTip = function(event){
        var data = this.getDataAt(event);
        if(data){
            return '城市：' + data.s('note') + '
经度：' + data.lonLat.lon + '
维度：' + data.lonLat.lat;
        }
        return null;
    };
    graphView.isVisible = function(data){
        return map.zoom > 1 || this.isSelected(data);
    };
    graphView.isNoteVisible = function(data){
        return map.zoom > 6 || this.isSelected(data);
    }; 
    graphView.getLabel = function(data){
        return '经度：' + data.lonLat.lon + '\n维度：' + data.lonLat.lat;
    };
    graphView.isLabelVisible = function(data){
        return map.zoom > 7 || this.isSelected(data);
    };                 
    view.addEventListener("ontouchend" in document ? 'touchstart' : 'mousedown', function(e){
        var data = graphView.getDataAt(e);
        if(data || e.metaKey || e.ctrlKey){
            e.stopPropagation();
        }                      
    }, false); 
    view.style.position = 'absolute';
    view.style.top = '0';
    view.style.left = '0';
    view.style.right = '0';
    view.style.bottom = '0';                
    view.style.zIndex = 999;
    map.viewPortDiv.appendChild(view);
    var color = randomColor();
    lines = china.split('\n');
    for(var i=0; i<lines.length; i++) {
        line = lines[i].trim();
        if(line.indexOf('【') === 0){
            //province = line.substring(1, line.length-1);                
            color = randomColor();
        }else{
            var ss = line.split(' ');
            if(ss.length === 3){
                createNode(parseFloat(ss[1].substr(3)), parseFloat(ss[2].substr(3)), ss[0].substr(3), color);                                                      
            }
        }
    }                                
}
function reset(){
    graphView.tx(0);
    graphView.ty(0);
    graphView.dm().each(function(data){                    
        if(data.lonLat){                            
            data.setPosition(map.getPixelFromLonLat(data.lonLat));                           
        }
    });
    graphView.validate();
}
function createNode(lon, lat, name, color){
    var node = new ht.Node();
    node.s({
        'shape': 'circle',
        'shape.background': color,
        'note': name,                    
        'label.background': 'rgba(255, 255, 0, 0.5)',                    
        'select.type': 'circle'
    });
    node.setSize(10, 10);
    var lonLat = new OpenLayers.LonLat(lon, lat);
    lonLat.transform('EPSG:4326', map.getProjectionObject());
    node.setPosition(map.getPixelFromLonLat(lonLat));
    node.lonLat = lonLat;
    graphView.dm().add(node);
    return node;
}
```
