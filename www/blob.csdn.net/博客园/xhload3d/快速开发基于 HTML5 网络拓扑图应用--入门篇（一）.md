# 快速开发基于 HTML5 网络拓扑图应用--入门篇（一） - xhload3d - 博客园
计算机网络的拓扑结构是引用拓扑学中研究与大小，形状无关的点、线关系的方法。把网络中的计算机和通信设备抽象为一个点，把传输介质抽象为一条线，由点和线组成的几何图形就是计算机网络的拓扑结构。网络的拓扑结构反映出网中各实体的结构关系，是建设计算机网络的第一步，是实现各种网络协议的基础，它对网络的性能，系统的可靠性与通信费用都有重大影响。拓扑在计算机网络中即是指连接各结点的形式与方法；在网络中的工作站和服务器等网络单元抽象为“点”；网络中的电缆等抽象为“线”。影响网络性能、系统可靠性、通信费用。
拓扑还分为：总线拓扑、星型拓扑、环形拓扑树形拓扑以及网状拓扑。这篇文章写的案例就是总线拓扑中的一个分支，我们一步一步来搭建这个拓扑图形。
示例图片：
![](https://images2017.cnblogs.com/blog/591709/201801/591709-20180108183406879-741123802.png)
拓扑图形组件 ht.graph.GraphView (以下简称 GarphView)是 HT 框架中 2D功能最丰富的组件，其相关类库都在 ht.graph包下。 GarphView 具有基本图形的呈现和编辑功能，拓扑节点连线及自动布局功能，电力和电信等行业预定义对象，具有动画渲染等特效， 因此其应用面很广泛，可作为监控领域的绘图工具和人机界面，可作为一般性的图形化编辑工具，可扩展成工作流和组织图等企业应用。
HT 框架的组件指的是可视化可交互的视图控件，HT 框架基于 HTML5 技术，因此`HT`组件的可视化部分本质就是 HTML 的元素， 大部分 HT 组件与 DataModel 数据模型绑定，用户通过操作纯 js 的语言即可驱动可视化组件，这样屏蔽了 HTML 底层图形技术复杂性。 HT 对 HTML5 技术封装的目的在于提高开发效率和可维护性，但并不意味着不允许用户直接操作 HTML 原生元素， 有 HTML5 开发经验的程序员，在了解 HT 系统机制的前提下，大可运用各种 HTML5 的技术对 HT 组件做自定义扩展。
我们就是用 ht.graph.GraphView 组件作为场景的，具体代码只有两行：
```
graphView = new ht.graph.GraphView();
graphView.addToDOM();
```
首先声明变量，然后将这个组件实例变量添加进 body 体中。所有 HT 组件最根层都为一个 div 组件，可通过组件的 getView() 函数获得， 默认和自定义交互事件监听一般添加在该 div 上(getView().addEventListener(type, func, false))， 渲染层一般由 canvas 提供。 用户可直接对根 div 和 canvas 层设置css背景等样式， 也可以添加新的 HTML 组件到根 div 层上，作为 canvas 的兄弟组件一起呈现。 HT 组件一般都以设置 position 为 absolute 的绝对定位方式。HT 的组件一般都会嵌入BorderPane、SplitView 和 TabView等容器中使用，而最外层的 HT 组件则需要用户手工将 getView()返回的底层 div 元素添加到页面的 DOM元素中，这里需要注意的是，当父容器大小变化时，如果父容器是 BorderPane和 SplitView 等这些 HT 预定义的容器组件，则 HT 的容器会自动递归调用孩子组件 invalidate函数通知更新。但如果父容器是原生的 html元素， 则 HT 组件无法获知需要更新，因此最外层的 HT 组件一般需要监听 window的窗口大小变化事件，调用最外层组件 invalidate函数进行更新。
为了最外层组件加载填充满窗口的方便性，HT 的所有组件都有 addToDOM 函数，其实现逻辑如下，其中 iv 是 invalidate的简写：
```
addToDOM = function(){   
    var self = this,
        view = self.getView(),  //获取底层 div 
        style = view.style;
    document.body.appendChild(view); //将组件底层 div 添加进 body 体中           
    style.left = '0';//ht 的组件默认将 position 设置为 absolute 绝对定位
    style.right = '0';
    style.top = '0';
    style.bottom = '0';      
    window.addEventListener('resize', function () { self.iv(); }, false);            
}
```
接下来我们要引入“数据容器”的概念，顾名思义，就是装载数据的容器，数据容器 ht.DataModel(以下简称 DataModel)作为承载 Data 数据的模型，管理着 Data 数据的增删以及变化事件派发， HT 框架所有组件都是通过绑定 DataModel，以不同的形式呈现到用户界面，可以通过 view.getDataModel() （简写为 view.dm()）方式获取数据容器；同时组件也会监听 DataModel 模型的变化事件（view.getSelectionModel()，简写为 view.sm()）， 实时同步更新界面数据信息，掌握了 DataModel 的操作就掌握了所有组件的模型驱动方式。
我们要创建两个节点 ht.Node（继承于 ht.Data 类），一个服务器，一个客户端，中间连线两条以及一条自环的连线，这些都是 Data 数据，都需要添加进 DataModel 数据容器中。但是我们得先创建这个数据节点，用 HT 只需要两句话就能将节点添加进数据容器中了，方法跟上面创建拓扑组件的方法很像，拓扑组件是添加进 body 体中，而节点是添加进数据容器中：
```
var server = new ht.Node();//创建“服务器”数据节点
graphView.dm().add(server);//将节点添加进数据容器 graphView.dm() 中
```
客户端也是一样的创建方式，节点名为 client。为了美观，我给服务器和客户端的节点都设置了图片，设置图片有两种方式：
- 直接将图片相对或绝对路径，设置到数据模型的相应属性上，server.setImage('images/server.png')
```
```
- 先通过 ht.Default.setImage('server', 'images/server.png') 进行注册，再将注册名称设置到模型上 server.setImage('server')
```
```
直接设置路径方式开发方便，无需提前注册图片，但数据模型[序列化](http://www.hightopo.com/guide/guide/core/serialization/ht-serialization-guide.html)时图片路径占用内存较多，将来图片路径变化后不利于管理维护，两种方式都是正确的使用方式，可根据项目情况选中不同方式或混合使用。 如果采用 url 的路径方式 HT 内部会自动加载图片，并在 onload 之后自动更新相应的视图组件。
HT 的框架下图片被赋予了更广泛的含义，`HT`提供了自定义 JSON 格式的矢量描述形式，以 HT 标准定义的 JSON 矢量格式， 也可以作为图片进行注册和使用，HT 的矢量方式比传统图片格式更节省空间，缩放不失真，最强大之处在于矢量的所有图形参数， 皆可与 Data 模型上的数据动态绑定，具体说明参见[矢量手册](http://www.hightopo.com/guide/guide/core/vector/ht-vector-guide.html)，现在我这边的讲解还没到这个步骤，感兴趣的可以自行了解。
接着是创建两个节点之间的连线，ht.Edge 类型用于连接起始和目标两个 Node 节点，两个节点间可以有多条 Edge 存在，也允许起始和目标为同一节点。为同一节点的情况就是形成自环的情况啦~这里有三条连线，由于有两条类似，我就只将直线连线和自环的连线代码贴出来进行解释：
```
var edge = new ht.Edge(server, client);//创建连线节点，参数为起始节点和目标节点
graphView.dm().add(edge);//将连线节点添加进数据容器中
var cirEdge = new ht.Edge(server, server);//这个连线的起始节点和目标节点都为同一个节点，形成自环
graphView.dm().add(cirEdge);
```
连线节点也继承于 Data 类，所以添加进数据容器的方式也是一样的。至于连线上面的字，则是通过 setStyle 方法来实现的。
HT 的数据 Data 可分为三种属性类型：
- get/set 或 is/set类型，例如 getName(0/setName('ht') 和 isExpanded()，用于常用属性操作
- attr 类型，通过 getAttr(name)和 setAttr(key, value) 存取，该类型是 HT 预留给用户存储业务数据
- style 类型，通过 getStyle(name)和 setStyle(key, value) 进行操作，GraphView 上图元样式由该类型属性控制
这里我是通过 style 类型来添加的文字说明，通过 ‘label’ 属性设置显示的文字：
```
edge.s({//setStyle 的简写
    'label': '请求',//设置文字
    'label.position': 3//设置文字位置
});
```
最终显示文字由 GraphView.getLabel 函数决定：
```
getLabel: function (data) {
    var label = data.getStyle('label');//获取 style 样式中的 label 属性的 value
    return label === undefined ? data.getName() : label; 
}
```
上面还有一个文字位置，是 HT 封装的位置，具体显示位置如下：
![](https://images2017.cnblogs.com/blog/591709/201801/591709-20180108231434722-1340505416.png)
其中 17 为中心位置，大家可根据这个位置自行调整看看效果，位置的具体说明请参考 [HT for Web 位置手册](http://www.hightopo.com/guide/guide/core/position/ht-position-guide.html)。
至此，全部代码解析完毕，下一节我会给大家介绍如何添加连线中的流动哦，记得围观哦~
