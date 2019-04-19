# 电力 Web SCADA 工控组态编辑器 - xhload3d - 博客园
# 前言
SVG 并非仅仅是一种图像格式， 由于它是一种基于 XML 的语言，也就意味着它继承了 XML 的跨平台性和可扩展性，从而在图形可重用性上迈出了一大步。如 SVG 可以内嵌于其他的 XML 文档中，而 SVG 文档中也可以嵌入其他的 XML 内容，各个不同的 SVG 图形可以方便地组合， 构成新的 SVG 图形。这个 Demo 运用的技术基于 HTML5 的技术适应了只能电网调度、配电网运行监控与配电网运维管控，通过移动终端实现 Web SCADA 账上运维的时代需求。由于传统电力行业 CS 桌面监控系统一直到新一代 Web 和移动终端进化中，HT 是实施成本最低，开发和运行效率最高的前端图形技术解决方案。SVG 矢量图形大家都不会陌生了，尤其是在工控电信等领域，但是这篇文章并不是要制作一个新的绘制 SVG 图的编辑器，而是一个可绘制矢量图形并且对这个图形进行数据绑定的更高阶。
# 效果图
![](https://images2018.cnblogs.com/blog/591709/201804/591709-20180417070126248-541087473.gif)
# 代码实现
## 整体框架
根据上图看得出来，整个界面被分为五个部分，分别为 palette 组件面板，toolbar 工具条，graphView 拓扑组件，propertyPane 属性面板以及 treeView 树组件，这五个部分中的组件需要先创建出来，然后才放到对应的位置上去：
```
palette = new ht.widget.Palette();// 组件面板
toolbar = new ht.widget.Toolbar(toolbar_config);// 工具条
g2d = new ht.graph.GraphView(dataModel);// 拓扑组件  
treeView = new ht.widget.TreeView(dataModel);// 树组件
propertyPane = new ht.widget.PropertyPane(dataModel);// 属性面板
propertyView = propertyPane.getPropertyView();// 属性组件
rulerFrame = new ht.widget.RulerFrame(g2d);// 刻度尺
```
![](https://images2018.cnblogs.com/blog/591709/201804/591709-20180408191242375-1043688896.png)
这些布局，只需要结合 splitView 和 borderPane 进行布局即可轻松完成~其中 splitView 为 HT 中的 分割组件，参数1为放置在前面的 view 组件（可为左边的，或者上面的）；参数2为放置在后面的 view 组件（可为右边的，或者下面的）；参数3为可选值，默认为 h，表示左右分割，若设置为 v 则为上下分割；参数4即为分割的比例。borderPane 跟 splitView 的作用有些相似，但是在这个 Demo 中布局，结合这两种组件，代码看起来会更加清爽。
```
borderPane = new ht.widget.BorderPane();// 边框面板
leftSplit = new ht.widget.SplitView(palette, borderPane, 'h', 260);// 分割组件，h表示左右分割，v表示上下分割
rightSplit = new ht.widget.SplitView(propertyPane, treeView, 'v', 0.4);
mainSplit = new ht.widget.SplitView(leftSplit, rightSplit, 'h', -260);                                              
borderPane.setTopView(toolbar);// 设置边框面板的顶部组件为 toolbar
borderPane.setTopHeight(30);
borderPane.setCenterView(rulerFrame);// 设置边框面板的中间组件为 rulerframe
mainSplit.addToDOM();// 将 mainSplit 的底层 div 添加进 body 体中
dataModel.deserialize(datamodel_config);// 反序列化 datamodel_config 的内容，将json内容转为拓扑图场景内容
g2d.fitContent(true);
```
布局结束后，就要考虑每一个容器中应该放置哪些内容，我将这些内容分别封装到不同的函数中，通过调用这些函数来进行数据的显示。
## Palette 组件面板
左侧的 Palette 组件面板需要向其内部添加 group 作为分组，然后再向组内添加节点。但是我们使用这个组件的最重要的一个原因是它能够拖拽节点，但是因为我们拖拽后需要在 graphView 拓扑组件中生成一个新的节点显示在拓扑图上，所以我将拖拽部分的逻辑写在了 graphView 拓扑组件的初始化函数中，这一小节就不做解释。
虽然说最重要的因素是拖拽，但是不可否认，这个组件在分类上也是非常直观：
![](https://images2018.cnblogs.com/blog/591709/201804/591709-20180409105005776-840759334.png)
如上图，我在 Palette 中做了三个分组：电力、食品加工厂以及污水处理。并在这些分组下面填充了很多属于该组类型的节点。我将这些分组的信息存储在 palette_config.js 文件中，由于三组中的信息量太大，这里只将一小部分的信息展示出来，看看是如何通过 json 对象来对分组进行数据显示的：
```
palette_config = {
    scene: {
        name: '电力',
        items: [
            { name: '文字', image: '__text__', type: ht.Text },
            { name: '箭头', image: 'symbols/arrow.json' },
            { name: '地线', image: 'symbols/earthwire.json' }
        ]
    },
    food: {
        name: '食品加工厂',
        items: [
            { name: '间歇式流化床处理器', image: 'symbols/food/Batch fluid bed processor.json'},
            { name: '啤酒瓶', image: 'symbols/food/Beer bottle.json'},
            { name: '台式均质机', image: 'symbols/food/Batch fluid bed processor.json'}
        ]
    },
    pumps: {
        name: '污水处理',
        items: [
            { name: '3维泵', image: 'symbols/pumps/3-D Pump.json'},
            { name: '18-惠勒卡车', image: 'symbols/pumps/18-wheeler truck 1.json'}
        ]
    }     
};
```
通过遍历这个对象获取内部数据，显示不同的数据信息。当然，在获取对象的信息的时候，我们需要创建 ht.Group 类的对象，以及分组内部的 ht.Node 类的元素（这些元素都为组的孩子），然后将这些获取来的数据赋值到这两种类型的节点上，并且将这些节点添加到 Palette 的数据容器中：
```
function initPalette(){// 初始化组件面板中的内容
    for(var name in palette_config){// 从 palette_config.js 文件中获取数据
        var info = palette_config[name];
        var group = new ht.Group();// 组件面板用ht.Group展示分组，ht.Node展示按钮元素
        group.setName(info.name);
        group.setExpanded(false);// 设置group默认关闭
        palette.dm().add(group);// 将节点添加到 palette 的数据容器中
        
        info.items.forEach(function(item){
            var node = new ht.Node();// 新建 ht.Node 类型节点
            node.setName(item.name);// 设置名称 用于显示在 palette 面板中节点下方说明文字
            node.setImage(item.image);// 设置节点在 palette 面板中的显示图片
            // 文本类型
            if (item.type === ht.Text) {// 通过 json 对象中设置的 type 信息来获取当前信息为何种类型的节点，不同类型的节点有些属性设置不同
                node.s({
                    'text': 'Text',// 文本类型的节点需要设置这个属性显示文本的内容
                    'text.align': 'center',// 文本对齐方式
                    'text.vAlign': 'middle',// 文本垂直对齐方式
                    'text.font': '32px Arial'// 文本字体
                });
            }
            node.item = item;
            node.s({
                'image.stretch': item.stretch || 'centerUniform',// 设置节点显示图片为填充的方式，这样不同比例的图片也不会因为拉伸而导致变形
                'draggable': item.draggable === undefined ? true : item.draggable,// 设置节点是否可被拖拽
            });                          
            group.addChild(node);// 将节点设置为 group 组的孩子
            palette.dm().add(node);// 节点同样也得添加到 palette 的数据容器中进行存储
        });
    }             
}
```
## graphView 拓扑组件
![](https://images2018.cnblogs.com/blog/591709/201804/591709-20180409124531515-2020865665.gif)
[http://www.hightopo.com/demo/2deditor/HT-2D-Editor.html](http://www.hightopo.com/demo/2deditor/HT-2D-Editor.html)
前面说到了 Palette 组件中节点拖拽到 graphView 拓扑图形中，来看看这个部分是如何实现的。如果 Palette 中的 Node 的 draggable 属性设置为  true ，那么 Palette 可以自动处理 dragstart ，但是 dragover 和 dragdrop 事件需要我们处理，我们知道 IOS 和 Android 设备上并不支持 dragover 和 dragdrop 这类事件，所以 Palette 插件还提供了模拟的拖拽事件 handleDragAndDrop，可以完美兼容 PC 和手持终端。
```
function initGraphView(){       
    if(ht.Default.isTouchable){// 判断是否为触屏可Touch方式交互
        palette.handleDragAndDrop = function(e, state) {// 重写此方法可以禁用HTML5原生的Drag和Drop事件并启用模拟的拖拽事件
            if(ht.Default.containedInView(e, g2d)){// 判断交互事件所处位置是否在View组件之上
                if(state === 'between'){
                    e.preventDefault();// 取消事件的默认动作。
                }
                else if(state === 'end'){// 当state为end时，判断e是否在graphView的范围内，如果是，则创建Node
                    handleDrop(e);
                }
            }
        };
    }
    else{
        g2d.getView().addEventListener("dragover", function(e) {
            e.dataTransfer.dropEffect = "copy";
            e.preventDefault();
        });
        g2d.getView().addEventListener("drop", function(e) {
            handleDrop(e);
        });
    }
}
function handleDrop(e){// 被拖拽的元素在目标元素上同时鼠标放开触发的事件
    e.preventDefault();
    var paletteNode = palette.dm().sm().ld();// 获取 palette 面板上最后选中的节点                 
    if (paletteNode) {   
        var item = paletteNode.item,
            image = item.image;
            data = g2d.getDataAt(e, null, 5);// 获取事件下的节点
        var node = new (item.type || ht.Node)();
        node.setImage(image);// 设置节点图片
        node.setName(item.name);// 设置节点名称
        node.p(g2d.lp(e));// 设置节点的坐标为拓扑中的逻辑坐标 lp函数为将事件坐标转换为拓扑中的逻辑坐标
        node.s('label', '');// 设置节点在 graphView 中底部不显示 setName 中的说明。因为 label 的优先级大于 name 
        if(data instanceof ht.Group){// 如果拖拽到“组类型”的节点上，那么直接设置父亲孩子关系
            node.setParent(data);// 设置节点的父亲
            data.setExpanded(true);// 展开分组
        }else{
            node.setParent(g2d.getCurrentSubGraph());
        }       
        g2d.dm().add(node);
        g2d.sm().ss(node);                                                     
    }                    
}
```
我在 graphView 拓扑图的场景中央添加了一个 json 场景，通过 dm.deserialize(datamodel_config) 反序列化 json 场景内容导出的一个电信行业的图纸。HT 独特的矢量引擎功能满足电力行业设备种类繁多、设备图元和线路网络需无极缩放、绑定量测数据实时刷新等需求；三维呈现技术使得电力厂站和变压器等设备 3D 可视化监控成为可能。
## treeView 树组件 
![](https://images2018.cnblogs.com/blog/591709/201804/591709-20180410150808298-1753912310.png)
至于树组件，树组件和 graphView  拓扑组件共用同一个 dataModl 数据容器，本来只需要创建出一个树组件对象，然后将其添加进布局容器中即可显示当前拓扑图形中的所有的数据节点，一般 HT 会将树组件上的节点分为几种类型进行显示，ht.Edge、ht.Group、ht.Node、ht.SubGraph、ht.Shape 等类型进行显示，但是这样做有一个问题，如果创建的节点非常多的话，那么无法分辨出那个节点是哪一个，也就无法快速地定位和修改该节点，会给绘图人员带来很大的困扰，所以我在 treeView 的 label 和 icon 的显示上做了一些处理：
```
// 初始化树组件
function initTreeView() {
    // 重载树组件上的文本显示
    treeView.getLabel = function (data) {
        if (data instanceof ht.Text) {
            return data.s('text');
        }
        else if (data instanceof ht.Shape) {
            return data.getName() || '不规则图形'
        }
        return data.getName() || '节点'
    };
    // 重载树组件上的图标显示
    var oldGetIconFunc = treeView.getIcon;
    treeView.getIcon = function (data) {
        if (data instanceof ht.Text) {
            return 'symbols/text.json';
        }
        var img = data.getImage();
        return img ? img : oldGetIconFunc.apply(this, arguments);
    }
}
```
##  propertyPane 属性面板
![](https://images2018.cnblogs.com/blog/591709/201804/591709-20180410164620695-1217052817.png)
属性面板，即为显示属性的一个容器，不同的类型的节点可能在属性的显示上有所不同，所以我在 properties_config.js 文件中将几个比较常见的类型的属性存储到数组中，主要有几种属性： text_properties 用于显示文本类型的节点的属性、data_properties 所有的 data 节点均显示的属性、node_properties 用于显示 ht.Node 类型的节点的属性、group_properties 用于显示 ht.Group 类型的节点的属性以及 edge_properties 用于显示 ht.Edge 类型的节点的属性。通过将这些属性分类，我们可以对在 graphView 中选中的不同的节点类型来对属性进行过滤：
```
function initPropertyView(){// 初始化属性组件               
    dataModel.sm().ms(function(e){// 监听选中变化事件
        propertyView.setProperties(null);
        var data = dataModel.sm().ld();
        
        // 针对不同类型的节点设置不同的属性内容
        if (data instanceof ht.Text) {// 文本类型
            propertyView.addProperties(text_properties);
            return;
        }
        if(data instanceof ht.Data){// [data](http://hightopo.com) 类型，所有的节点都基于这个类型
            propertyView.addProperties(data_properties);
        }                                        
        if(data instanceof ht.Node){// node 类型
            propertyView.addProperties(node_properties);
        }
        if(data instanceof ht.Group){// 组类型
            propertyView.addProperties(group_properties);
        }
        if(data instanceof ht.Edge){// 连线类型
            propertyView.addProperties(edge_properties);
        }     
    });                
}
```
数据绑定在属性栏中也有体现，拿 data_properties 中的“标签”和“可编辑”作为演示：
```
{
    name: 'name',// 设置了 name 属性，如果没有设置 accessType 则默认通过 get/setName 来获取和设置 name 值
    displayName: '名称',// 用于存取属性名的显示文本值，若为空则显示name属性值
    editable: true// 设置该属性是否可编辑                       
}, 
{
    name: '2d.editable',// 结合 accessType，则通过 node.s('2d.editable') 获取和设置该属性
    accessType: 'style',// 操作存取属性类型
    displayName: '可编辑',// 用于存取属性名的显示文本值，若为空则显示name属性值
    valueType: 'boolean',// 布尔类型，显示为勾选框
    editable: true// 设置该属性是否可编辑  
}
```
这两个属性比较有代表性，一个是直接通过 get/set 来设置 name 属性值，一个是通过结合属性的类型来控制 name 的属性值。只要在属性栏中操作“名称”和“可编辑”两个属性，就可以直接在拓扑图中看到对应的节点的显示情况，这就是数据绑定。当然，还可以对矢量图形进行局部的数据绑定，但不是本文的重点，有兴趣的可以参考我的这篇文章 [WebGL 3D 电信机架实战之数据绑定](http://www.cnblogs.com/xhload3d/p/8620358.html)。
## toolbar 工具栏
![](https://images2018.cnblogs.com/blog/591709/201804/591709-20180410154352600-946308204.png)
差点忘记说这个部分了，toolbar 上总共有 8 种功能，分别是选中编辑、连线、直角连线、不规则图形、刻度尺显示、场景放大、场景缩小以及场景内容导出 json。这 8 种功能都是存储在 toolbar_config.js 文件中的，通过绘制 toolbar 中的元素给每一个元素都添加上了对应的点击触发的内容，主要讲讲 CreateEdgeInteractor.js 创建连线的内容。
我们通过 ht.Default.def 自定义了 CreateEdgeInteractor 类，然后通过 graphView.setInteractors([ new CreateEdgeInteractor(graphView, 'points')]) 这种方式来添加 graphView 拓扑图中的交互器，可以实现创建连线的交互功能。
在 CreateEdgeInteractor 类中通过监听 touchend 放手后事件向 graphView 拓扑图中添加一个 edge 连线，可以通过在 CreateEdgeInteractor 函数中传参来绘制不同的连线类型，比如 “ortho” 则为折线类型：
```
var CreateEdgeInteractor = function (graphView, type) {
    CreateEdgeInteractor.superClass.constructor.call(this, graphView);   
    this._type = type;
};
ht.Default.def(CreateEdgeInteractor, DNDInteractor, {// 自定义类，继承 DNDInteractor，此交互器有一些基本的交互功能
    handleWindowTouchEnd: function (e) {
        this.redraw();
        var isPoints = false;
        if(this._target){
            var edge = new ht.Edge(this._source, this._target);// 创建一条连线，传入起始点和终点
            edge.s({
                'edge.type': this._type// 设置连线类型 为传入的参数 [type](http://hightopo.com/guide/guide/plugin/edgetype/ht-edgetype-guide.html#ref_edgetype) 类型
            });
            isPoints = this._type === 'points';// 如果没有设置则默认为 points 连线方式
            if(isPoints){
                edge.s({
                    'edge.points': [{// 设置连线的点
                         x: (this._source.p().x + this._target.p().x)/2,
                         y: (this._source.p().y + this._target.p().y)/2
                    }]
                });                
            }
            edge.setParent(this._graphView.getCurrentSubGraph());// 设置连线的父亲节点为当前子网
            this._graphView.getDataModel().add(edge);// 将连线添加到拓扑图的数据容器中
            this._graphView.getSelectionModel().setSelection(edge);// 设置选中该节点                        
        }
        this._graphView.removeTopPainter(this);// 删除顶层Painter
        if(isPoints){
            resetDefault();// 重置toolbar导航栏的状态
        }        
    }            
});
```
# 总结
 一开始想说要做这个编辑器还有点怕怕的，感觉任务重，但是不上不行，所以总是在拖，但是后来整体分析下来，发现其实一步一步来就好，不要把步骤想得太复杂，什么事情都是从小堆到大的，以前我们用 svg 绘制的图形都可以在这上面绘制，当然，如果有需要拓展也完全 ok，毕竟别人写的编辑器不一定能够完全满足你的要求。这个编辑器虽说在画图上面跟别家无异，但是最重要的是它能够绘制出矢量图形，结合 HT 的数据绑定和动画，我们就可以对这些矢量图形中的每一个部分进行操作，比如灯的闪烁啊，比如人眨眼睛等等操作，至于这些都是后话了。有了这个编辑器我也能够更加快速地进行开发了~
