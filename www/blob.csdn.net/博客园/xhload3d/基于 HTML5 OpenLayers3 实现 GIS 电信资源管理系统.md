# 基于 HTML5 OpenLayers3 实现 GIS 电信资源管理系统 - xhload3d - 博客园
# 前言
通过结合 HTML5 和 OpenLayers 可以组合成非常棒的一个电信地图网络拓扑图的应用，形成的效果可以用来作为电信资源管理系统，美食定位分享软件，片区找房，绘制铁轨线路等等，各个领域都能够涉及的一款应用。虽然这个 Demo 是结合 OpenLayers3 的，其实还可推广到与 ArcGIS、百度地图以及 GoogleMap 等众多 GIS 地图引擎融合。
![](https://images2018.cnblogs.com/blog/591709/201804/591709-20180419194115489-1818325570.gif)
[http://www.hightopo.com/demo/openlayers/](http://www.hightopo.com/demo/openlayers/)
# 代码生成
## 创建地图
OpenLayers 是一个用于开发 WebGIS 客户端的 JavaScript 包。OpenLayers 支持的地图来源包括 Google Maps、Yahoo、 Map、微软 Virtual Earth 等多种离线在线地图，这里用到的是比较大众化的谷歌地图 Google Map 的在线地图，使用 OpenLayers 前只需要引入相关的类库以及 css 文件：
```
<link rel="stylesheet" href="css/ol.css" type="text/css">
<script src="lib/ol.js"></script>
```
初始化地图的操作则是将 Map 放进一个 div 元素中，初始化一个 ol.Map 地图类，这在整个电信资源管理系统中必不可少，然后设置这个类中的各个参数：
```
var mapDiv = document.getElementById('mapDiv');
map = new ol.Map({
    target: 'mapDiv',// 地图容器
    controls: ol.control.defaults().extend([
        graphViewControl,// 自定义拓扑控件
        new ol.control.OverviewMap(),// 地图全局视图控件
        new ol.control.ScaleLine(),// 比例尺控件
        new ol.control.ZoomSlider(),// 缩放刻度控件
        new ol.control.ZoomToExtent()// 缩放到全局控件
    ]),
    layers: [// 图层
        new ol.layer.Tile({
            source: new ol.source.XYZ({// 谷歌地图 
                url:'http://www.google.cn/maps/vt/pb=!1m4!1m3!1i{z}!2i{x}!3i{y}!2m3!1e0!2sm!3i345013117!3m8!2szh-CN!3scn!5e1105!12m4!1e68!2m2!1sset!2sRoadmap!4e0'  
            })  
        })
    ],
    view: new ol.View({// 地图视图
        projection: 'EPSG:3857',// 投影
        center: ol.proj.fromLonLat([106, 35]),// 视图的初始中心 中心的坐标系由projection选项指定    
        zoom: 4// 缩放级别 用于计算视图的初始分辨率
    })
});
```
上面的代码根据每行的代码注释加上官方 API 解释应该没有什么难度。细心的朋友可能注意到了一个非官方的控件：graphViewControl 控件，这个控件是我自定义出来，用来在这个控件上绘制拓扑图形的，声明和定义部分在 GraphViewControl.js 文件中。
## 自定义控件
![](https://images2018.cnblogs.com/blog/591709/201804/591709-20180420171255158-707372897.png)
自定义 OpenLayers 的控件，无非就是将某个类继承于 ol.control.Control 类，然后针对不同的需求重写父类方法或者增加方法。
我在声明类的时候传了一个 options 参数，通过在定义类的时候设置控件的容器元素并且将控件渲染到 GIS 地图的 viewport 之外：
```
var view = graphView.getView();// 获取拓扑组件的 div
ol.control.Control.call(this, {
element: view,// 控件的容器元素
target: options.target// 将控件渲染到地图的视口之外
});
```
上面的 graphView 是通过 GraphViewControl 在父类方法上新添加的一个方法并且初始化值为 ht.graph.GraphView，HT 的拓扑图形组件：
```
// 获取GraphView对象
GraphViewControl.prototype.getGraphView = function() { return this._graphView; };
var graphView = this._graphView = new ht.graph.GraphView();// 拓扑图组件
```
我在控件中还给 graphView 拓扑组件添加了一些事件的监听，由于 OpenLayers 和 HT 是两款不同的 js 库，有着各自的交互系统和坐标系，首先我们将某些我们需要获取在 HT 上做的交互事件并停止事件传播到 OpenLayers 上：
```
// 拖拽 node 时不移动地图
var stopGraphPropagation = function(e) {
    var data = graphView.getDataAt(e);// 获取 graphView 事件下的节点
    var interaction = graphView.getEditInteractor();// 获取编辑交互器
    if (data || e.metaKey || e.ctrlKey || interaction && interaction.gvEditing) {
        e.stopPropagation();// 不再派发事件 该方法将停止事件的传播，阻止它被分派到其他 Document 节点
    }
}
/** pointerdown 当指针变为活动事件
*    对于鼠标，当设备从按下的按钮转换到至少一个按钮被按下时，它会被触发。
*    对于触摸，当与数字化仪进行物理接触时会被触发。
*    对于笔，当触笔与数字化仪进行物理接触时会被触发。
**/
view.addEventListener('pointerdown', stopGraphPropagation, false);
view.addEventListener('touchstart', stopGraphPropagation, false);// 当触摸点被放置在触控面板上事件
view.addEventListener('mousedown', stopGraphPropagation, false);// 鼠标点下事件
```
GraphViewControl 类定义部分还添加了一些关于移动和编辑节点的交互事件，主要是将节点的像素坐标转为 OpenLayers 的 ol.Cordinate 地图视图投影中的坐标并存储到节点的业务属性（HT 的一个可以存储任意值的对象）中，这样我们只需要通过获取或设置节点的业务属性 coord 就可以自由获取和设置节点在 map 上的像素坐标。
```
var position = data.getPosition(),// 获取选中节点的坐标
    x = position.x + graphView.tx(),// 节点横坐标+graphView水平平移值
    y = position.y + graphView.ty();// 节点纵坐标+graphView垂直平移值
var coord = map.getCoordinateFromPixel([x, y]);// 根据坐标的像素获取地图视图投影中的坐标
data.a('coord', coord);
```
这里我就提一些基础的功能，其他的就不作解释了，只是一些扩展。
值得注意的一点是，我们在上面对节点在电信 GIS 地图视图投影中的坐标进行了数据存储，但是这个方法对于 Shape 类型的节点来说不太合适，因为地图上一般都是用点围成区域面，勾勒出某个国家或者某个城市的轮廓，缩放的时候并不实时保持大小，而是根据地图的缩放来缩放，实时保持在电信 GIS 地图的某个位置，所以我对 Shape 类型的节点中所有的点遍历了一遍，都设置了业务属性 pointCoord，获取地图视图投影中的坐标：
```
// 给 shape 类型的节点的每个点位置都设置为经纬度
if (e.kind === 'endEditPoint' || e.kind === 'endEditPoints' || e.kind === 'endEditResize' || e.kind === 'endMove') {
    if (data instanceof ht.Shape) {// Shape 类型的节点
        data.getPoints().forEach(function(point, index) {
            var pointCoord = map.getCoordinateFromPixel([point.x, point.y]);// 获取给定像素的坐标
            data.a('pointCoord['+index+']', pointCoord);
        });
    }
}
```
## 图层叠加
![](https://images2018.cnblogs.com/blog/591709/201804/591709-20180420171517364-1984141436.png)
OpenLayers 的结构比较复杂，而 HT 相对来说简单很多，所以我将 HT 叠加到 OpenLayers Map 的 viewport 中。这里我在子类 GraphViewControl 中重载了父类 ol.control.Control 的 setMap 方法，在此方法中将 HT 的拓扑组件 graphView 添加到 OpenLayers 的视图 viewport 中，我们知道，HT 的组件一般都是绝对定位的，所以我们要设置 css 中的位置和宽高属性：
```
var graphView = self._graphView;// = GraphViewControl.getGraphView()
var view = graphView.getView();// 获取 graphView 组件的 div
var dataModel = graphView.getDataModel();// 获取 graphView 的数据容器
view.style.top = '0';
view.style.left = '0';
view.style.width = '100%';
view.style.height = '100%';
map.getViewport().insertBefore(view, map.getViewport().firstChild);// getViewPort 获取用作地图视口的元素 insertBefore 在指定的已有子节点(参数二)之前插入新的子节点(参数一)
```
并对数据容器增删变化事件进行监听，通过监听当前加入数据容器的节点类型，将当前节点的像素坐标转为地图视图投影中的坐标存储在节点的业务属性 coord 上：
```
dataModel.addDataModelChangeListener(function(e) {// 数据容器增删改查变化监听
    if (e.kind === 'add' && !(e.data instanceof ht.Edge)) {// 添加事件&&事件对象不是 ht.Edge 类型
        if (e.data instanceof ht.Node) {
            var position = e.data.getPosition();
            var coordPosition = map.getCoordinateFromPixel([position.x, position.y]);// 获取给定像素的坐标
            e.data.a('coord', coordPosition);
        }
        if (e.data instanceof ht.Shape) {// 给 shape 类型的节点上的每个点都设置经纬度
            e.data.getPoints().forEach(function(point, index) {// 对 shape 类型的节点则将所有点的坐标都转为经纬度
                var pointCoord = map.getCoordinateFromPixel([point.x, point.y]);// 获取给定像素的坐标
                e.data.a('pointCoord['+index+']', pointCoord);
            });
        }
    }
});
```
最后监听地图更新事件，重设拓扑：
```
map.on('postrender', function() { self.resetGraphView(); });
```
## 坐标转换
重设拓扑在这边的意思就是将拓扑图中节点坐标从我们一开始设置在 HT 中的像素坐标重新通过地图的缩放或者移动将地图视图投影中的坐标转为像素坐标设置到节点上，这时候前面存储的业务属性 coord 就派上用场了，记住，Shape 类型的节点是例外的，还是要对其中的每个点都重新设置坐标：
```
GraphViewControl.prototype.resetGraphView = function() {// 重置 graphView 组件的状态
    var graphView = this._graphView;
    graphView.tx(0);// grpahView 水平平移值
    graphView.ty(0);// graphView 垂直平移值
    graphView.dm().each(function(data) {// 遍历 graphView 中的数据容器 
        var coord = data.a('coord');// 获取节点的业务属性 coord
        if (coord) {
            var position = map.getPixelFromCoordinate(coord);// 获取给定坐标的像素
            data.setPosition(position[0], position[1]);// 重新给节点设置像素坐标
        }
        if (data instanceof ht.Shape) {
            var points = data.toPoints();// 构建一个新的Shape点集合并返回
            data.getPoints().clear();// 清空点集合
            data._points = new ht.List();
            points.forEach(function(point, index) {// 给 shape 重新设置每一个点的像素坐标
                point.x = map.getPixelFromCoordinate(data.a('pointCoord['+ index +']'))[0];
                point.y = map.getPixelFromCoordinate(data.a('pointCoord['+ index +']'))[1];
                data._points.add(point);
            });
            data.setPoints(data._points);
        }
    });
    graphView.validate();//刷新拓扑组件
}
```
## 场景搭建
![](https://images2018.cnblogs.com/blog/591709/201804/591709-20180420181345443-1540992369.png)
OpenLayers 的 Map 部分做好了，接下来就是将它放进场景中了~但是从上面的截图中能看到，除了地图，顶部有工具条（但是我是用 formPane 表单组件做的），左侧有一个可供拖拽的 Palette 面板组件，通过 HT 的 borderPane 边框面板组件将整个场景布局好：
```
raphViewControl = new GraphViewControl();// 自定义控件，作为 openlayers 地图上自定义控件
graphView = graphViewControl.getGraphView();// 获取拓扑图组件
dm = graphView.getDataModel();// 获取拓扑图中的数据容器
palette = new ht.widget.Palette();// 创建一个组件面板
formPane = createFormPane();// 工具条的 form 表单
borderPane = new ht.widget.BorderPane();// 边框面板组件
borderPane.setTopView(formPane);// 设置顶部组件为 formPane
borderPane.setLeftView(palette, 260);// 设置左边组件为 palette 参数二为设置 该view的宽度
borderPane.setCenterView(mapDiv);// 设置中间组件为 mapDiv
borderPane.addToDOM();// 将面板组件添加到 body 中
```
这样整个场景的布局和显示就完成了，非常轻松~
## 工具条
![](https://images2018.cnblogs.com/blog/591709/201804/591709-20180420170841754-1086593688.png)
本身 HT 有自带的工具条，但是因为 form 表单在排布以及样式上面可以更灵活，所以采用这个。
```
var fp = new ht.widget.FormPane();
fp.setVGap(0);// 设置表单组件水平间距 默认值为6
fp.setHGap(0);// 设置表单的行垂直间距 默认值为6
fp.setHPadding(4);// 设置表单左边和右边与组件内容的间距，默认值为8
fp.setVPadding(4);// 设置表单顶部和顶部与组件内容的间距，默认值为8
fp.setHeight(40);// 设置表单高度
var btBgColor = '#fff',
    btnIconColor = 'rgb(159, 159, 159)',
    btnSelectColor = 'rgb(231, 231, 231)';
fp.addRow([// 添加行 首尾各加了一个''，并且占的宽度均为相对值0.1，就会将中间部分居中
    '', {
        id: 'select',// id 唯一标示属性，可通过 formPane.getItemById(id) 获取添加到对应的 item 对象
        button: {// ht.widget.Button 为按钮类
            background: btBgColor,// 设置背景颜色
            icon: './symbols/icon/select.json',// 设置图标
            iconColor: btnIconColor,// 设置图标颜色
            selectBackground: btnSelectColor,// 设置选中背景颜色
            togglable: true,// 设置按钮是否处于开关状态
            groupId: 't',// 设置组编号，属于同组的togglable按钮具有互斥功能
            toolTip: '编辑',// 设置文字提示，可通过 enableToolTip() 和 disableToolTip() 启动和关闭文字提示
            onClicked: function() {// 按钮点击触发函数
                editableFunc();
            }
        }
    }, {
        id: 'pointLine',
        button: {
            background: btBgColor,
            icon: './symbols/icon/line.json',
            iconColor: btnIconColor,
            selectBackground: btnSelectColor,
            togglable: true,
            groupId: 't',
            toolTip: '连线',
            onClicked: function () {
                /** 通过 setInteractors 组合交互器
                * DefaultInteractor实现Group、Edge和SubGraph图元的默认双击响应，手抓图平移，滚轮缩放，键盘响应等功能
                * TouchInteractor实现移动设备上的Touch交互功能
                * CreateEdgeInteractor 为 CreateEdgeInteractor.js 文件中自定义的连线交互器
                * CreateShapeInteractor 为 CreateShapeInteractor.js 文件中自定义的多边形交互器
                **/
                graphView.setInteractors([new ht.graph.DefaultInteractor(graphView), new ht.graph.TouchInteractor(graphView, {
                    selectable: false
                }), new CreateEdgeInteractor(graphView)]);
            }
        }
    },''
], [0.1, 36, 36, 0.1]);
```
上面的 form 表单中添加行我只列出了两个功能，一个编辑的功能，另一个绘制连线的功能。formPane.addRow 为添加一行元素，参数一为元素数组，元素可为字符串、json 格式描述的组件参数信息、html 元素或者为 null 的空，参数二为为每个元素宽度信息数组，宽度值大于1代表固定绝对值，小于等于1代表相对值，也可为 80+0.3 的组合。
为了让我想显示的部分显示在工具栏的正中央，所以我在第一项和最后一项都设置了一个空，占 0.1 的相对宽度，并且比例相同，所以中间的部分才会显示在正中央。
上面代码通过 setInteractors 组合我们所需要的交互器。DefaultInteractor 实现 Group、Edge 和 SubGraph 图元的默认双击响应，手抓图平移，滚轮缩放，键盘响应等功能；TouchInteractor 实现移动设备上的 Touch 交互功能。至于最后面的 CreateEdgeInteractor 则是继承于 ht.graph.Interactor 交互器的创建连线的交互器。这里细细地分析一下这个部分，以后就可以修改或者自定义新的交互器。
### 自定义交互器
![](https://images2018.cnblogs.com/blog/591709/201804/591709-20180420171812941-1606157397.png)
 我们通过 ht.Default.def(className, superClass, methods) 定义类，并在 methods 对象中对方法和变量进行声明。
setUp 方法在对象被创建的时候被调用，根据需求在这里设置一些功能，我设置的是清除所有的选中的节点：
```
setUp: function () {// CreateEdgeInteractor 对象被创建的时候调用的函数
    CreateEdgeInteractor.superClass.setUp.call(this);this._graphView.sm().cs();// 清除所有选中
}
```
 tearDown 方法在对象结束调用的时候被调用，绘制连线的时候，如果未结束绘制怎么办？下一次绘制不可能连着上一次继续绘制，所以我们得在结束调用这个类的时候将之前的绘制的点都清除：
```
tearDown: function () {// CreateEdgeInteractor 对象结束调用的时候调用的函数
    CreateEdgeInteractor.superClass.tearDown.call(this);
    // 清除连线起点、终点以及连线中间的各个点  
    this._source = null;
    this._target = null;
    this._logicalPoint = null;
}
```
关于鼠标事件以及 touch 事件，我希望这两者在操作上相同，所以直接在鼠标事件中调用的 touch 事件的方法。
绘制连线需要鼠标左键先选中一个节点，然后拖动鼠标左键不放，移动鼠标到连线的终点节点上，此时一条连线创建完毕。
首先是 touchstart 选中一个节点：
```
handle_mousedown: function (e) {// 鼠标点下事件
    this.handle_touchstart(e);
},
handle_touchstart: function (e) {// 开始 touch
    this._sourceNode = this.getNodeAt(e);// 获取事件下的节点
    if (this._sourceNode) {
        this._targetNode = null;// 初始化 targetNode
        this.startDragging(e);
        this._graphView.addTopPainter(this);// 增加顶层Painter 使用Canvas的画笔对象自由绘制任意形状，顶层Painter绘制在拓扑最上面
        this._graphView.sm().ss(this._sourceNode);// 设置选中
    }
},
getNodeAt: function(e){// 获取事件下的节点
    if (ht.Default.isLeftButton(e) && ht.Default.getTouchCount(e) === 1) {// 鼠标左键被按下 && 当前Touch手指个数为1
        var data = this._graphView.getDataAt(e);// 获取事件下的节点
        if(data instanceof ht.Node) return data;// 为 ht.Node 类型的节点
    } 
    return null;
}
```
![](https://images2018.cnblogs.com/blog/591709/201804/591709-20180420171957411-754066058.png)
然后手指滑动 touchmove ：
```
handleWindowMouseMove: function (e) {
    this.handleWindowTouchMove(e);
},
handleWindowTouchMove: function (e) {// 手指滑动
    var graphView = this._graphView;// 拓扑组件
    this.redraw();// 如果不重新绘制矩形区域，那么容易造成脏矩形
    this._logicalPoint = graphView.getLogicalPoint(e);// 获取事件下的逻辑坐标
    this._targetNode = this.getNodeAt(e);// 获取事件下的 edge 的终点
    if (this._targetNode) graphView.sm().ss([this._sourceNode, this._targetNode]);// 设置起始和终止节点都被选中
    else graphView.sm().ss([this._sourceNode]);// 只选中起始节点
},
redraw: function () {
    var p1 = this._sourceNode.getPosition(),// 获取连线起始端的节点的坐标
        p2 = this._logicalPoint;
    if (p1 && p2) {
        var rect = ht.Default.unionPoint(p1, p2);// 将点组合成矩形
        ht.Default.grow(rect, 1);// 改变rect大小，上下左右分别扩展 extend 的大小
        this._graphView.redraw(rect);// 重绘拓扑，rect参数为空时重绘拓扑中的所有图元，否则重绘矩形范围内的图元
    }
}
```
最后 touchend 创建连线：
```
handleWindowMouseUp: function (e) {
    this.handleWindowTouchEnd(e);
},      
handleWindowTouchEnd: function (e) { 
    if (this._targetNode) {
        var edge = new ht.Edge(this._sourceNode, this._targetNode);// 创建新的连线节点
        if (this._edgeType) edge.s('edge.type', this._edgeType);// 设置连线的类型
        this._graphView.dm().add(edge);// 将节点添加进数据容器
        this._graphView.sm().ss(edge);// 设置选中您当前连线
    }
    editableFunc();// 绘制结束后 工具条选中“编辑”项
    this._graphView.removeTopPainter(this);// 移除顶层画笔
}
```
至于还未创建连线之前（也就是说为选中终止节点），鼠标在拖动的过程中会创建一条连线，这里是直接用 canvas 绘制的：
```
draw: function (g) {// 绘制起点与鼠标移动位置的连线
    var p1 = this._sourceNode.getPosition(),
        p2 = this._logicalPoint;    
    if(p1 && p2){
        g.lineWidth = 1;
        g.strokeStyle = '#1ABC9C';
        g.beginPath();
        g.moveTo(p1.x, p1.y);
        g.lineTo(p2.x, p2.y);
        g.stroke();              
    }
}
```
这样，自定义连线类结束！
## 面板组件
![](https://images2018.cnblogs.com/blog/591709/201804/591709-20180420170737809-375205704.png)
左侧面板组件 ht.widget.Palette 支持自定义样式及单选、拖拽操作，由 ht.DataModel 驱动，用 ht.Group 展示分组，ht.Node 展示按钮元素。
展示分组，首先得创建分组和组中的按钮元素：
```
function initPalette(palette) {// 加载palette面板组件中的图元
    var nodeArray = ['city', 'equipment'];
    var nameArray = ['城市', '大型'];// arrNode中的index与nameArr中的一一对应
    for (var i = 0; i < nodeArray.length; i++) {
        var name = nameArray[i];
        nodeArray[i] = new ht.Group();// palette面板是将图元都分在“组”里面，然后向“组”中添加图元即可
        palette.dm().add(nodeArray[i]);// 向palette面板组件中添加group图元
        nodeArray[i].setExpanded(true);// 设置分组为打开的状态
        nodeArray[i].setName(name);// 设置组的名字
        var imageArray = [];
        switch(i){
            case 0:
                imageArray = ['symbols/5.json', 'symbols/6.json', 'symbols/叉车.json', 'symbols/公交车.json', 'symbols/人1.json', 'symbols/人2.json', 'symbols/人3.json', 'symbols/树.json', 'symbols/树2.json'];
                break;
            case 1: 
                imageArray = ['symbols/飞机.json', 'symbols/吊机.json', 'symbols/卡车.json', 'symbols/货轮.json', 'symbols/龙门吊.json', 'symbols/公园.json'];
                break;
            default: 
                break;
        }
        setPaletteNode(imageArray, nodeArray[i], palette);
    }
}
function setPaletteNode(imageArray, array, palette) {// 创建 palette 上 节点及设置名称、显示图片、父子关系
    for (var i = 0; i < imageArray.length; i++) {
        var imageName = imageArray[i],
            name = imageName.slice(imageName.lastIndexOf('/')+1, imageName.lastIndexOf('.'));// 获取最后一个 / 和最后一个.中间的文本，作为节点的 name
        
        createNode(imageName, name, array, palette);// 创建节点，显示在 palette 面板上
    }
}
function createNode(image, name, parent, palette) {// 创建palette面板组件上的节点
    var node = new ht.Node();
    palette.dm().add(node);// 将节点添加进 palette 的数据容器中
    node.setImage(image);// 设置节点的图片
    node.setName(name);// 设置节点名称
    node.setParent(parent);// 设置节点的父亲
    node.s({// 设置节点的属性
        'draggable': true,// 如果Node的draggable设为true，Palette可以自动处理dragstart，但是dragover和drop事件需要我们处理
        'image.stretch': 'centerUniform',// 图片的绘制方式为非失真方式
    });
    return node;
}
```
![](https://images2018.cnblogs.com/blog/591709/201804/591709-20180420172148963-990503726.png)
创建完后，我们就要启用模拟的拖拽事件 handleDragAndDrop(e, state)： 
```
palette = new ht.widget.Palette();// 创建一个组件面板
var data;
palette.handleDragAndDrop = function(e, state) {// 左侧面板组件拖拽功能
    if ( state === 'prepare' ) data = palette.getDataAt(e);
    else if( state === 'begin' || state === 'between' ) {}
    else {
        if (!ht.Default.containedInView(e, graphView)) return; // 判断交互事件所处位置是否在graphView组件之上
        var node = new ht.Node();// 拖拽到graphView中就创建一个新的节点显示在graphView上
        node.setImage(data.getImage());// 设置节点上贴图
        node.setName(data.getName());// 设置名称（为了显示在属性栏中）
        node.s('label', '');// 在graphView中节点下方不会出现setName中的值，label优先级高于name
        node.p(graphView.lp(e));// 将节点的位置设置为graphView事件下的拓扑图中的逻辑坐标，即设置鼠标点下的位置为节点坐标
        graphView.dm().add(node);// 将节点添加进graphView中
        graphView.sm().ss(node);// 默认选中节点
        graphView.setFocus(node);// 设置将焦点聚集在该节点上
        
        editableFunc();// 设置节点为可编辑状态并且选中导航栏中的“编辑”
    }
}
```
好了，先在你就可以直接从左侧的 palette 面板组件上直接拖拽节点到右侧的地图上的 graphView 拓扑图。
我们可以在 graphView 上进行绘制节点的编辑、绘制连线、绘制直角连线以及绘制多边形。
# 最后
在上面基于 GIS 的电信资源管理系统的基础上我尝试了增加切换地图的功能，同时还在导航栏上添加了“地铁线路图”，这个地铁线路图实现起来也是非常厉害的，下次我会再针对这个地铁线路图进行一次详解，这里就不多做解释，来看看我添加后的最终结果：
![](https://images2018.cnblogs.com/blog/591709/201805/591709-20180523073906794-853083308.gif)
[http://www.hightopo.com/demo/openlayers/](http://www.hightopo.com/demo/openlayers/)
如果有什么建议或者意见，欢迎留言或者私信我，也可以直接去 HT for Web（[https://hightopo.com/](https://hightopo.com/)） 官网查阅相关资料。
