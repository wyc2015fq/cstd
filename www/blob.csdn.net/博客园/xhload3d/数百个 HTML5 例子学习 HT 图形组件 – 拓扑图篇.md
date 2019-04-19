# 数百个 HTML5 例子学习 HT 图形组件 – 拓扑图篇 - xhload3d - 博客园
[HT](http://www.hightopo.com/) 是啥：***Everything you need to create cutting-edge 2D and 3D visualization.***
这口号是当年心目中的产品方向，接着就朝这个方向慢慢打磨，如今 [HT](http://www.hightopo.com/) 算是达到了这样的效果，谈不上用尽洪荒之力，但我们对产品结果很满意，特别是 HT 的用户手册，将例子和文档无缝融合一体，小小 10 来兆开发包居然包含了四十五份手册，数百个活生生的 HTML5 例子，还没体验过的同学可以点击 [http://www.hightopo.com/guide/readme.html](http://www.hightopo.com/guide/readme.html )HT 手册入口玩一玩。
这样综合并可搜索的[手册入口](http://www.hightopo.com/guide/readme.html)居然还常被问及 HT 的 Demo 在哪里？只能怪这些年深入人心的极致用户体验理念，把人惯得包括很多程序员都如此之“懒”，当然也怪我们没把用户体验的最后一公里做到位，于是最近 wangyinlong 同学人工例子抓图，将数百个手册例子进行了归类整理，最终形成了方便大家直观查找所有 HT 例子的页面：[http://www.hightopo.com/demos/index.html](http://www.hightopo.com/demos/index.html)
![Screen Shot 2016-08-09 at 4.46.48 PM](http://www.hightopo.com/blog/wp-content/uploads/2016/08/Screen-Shot-2016-08-09-at-4.46.48-PM.png)
很明显 [http://www.hightopo.com/demos/index.html](http://www.hightopo.com/demos/index.html )相比 [http://www.hightopo.com/guide/readme.html](http://www.hightopo.com/guide/readme.html) 页面，对于 HT 老用户包括我们自己做技术支持，都感觉查找例子更直观方便，但对于 HT 初学者，面对这一堆数百个涵括通用组件、网络拓扑图组件、3D 组件、矢量图形、各种编辑器等等五法八门的 HTML5 例子盛宴，往往无从下手，为此我打算写个《数百个HTML5例子学习HT图形组件》的系列文章，引导各种电力、电信、工控 SCADA 等不同行业领域用户，学习使用 HT 这全套一站式 HTML5 图形组件。
拓扑图组件作为最典型的 HT 客户需求应用，自然是首篇登场。
HT 的拓扑图组件主要基于 HTML5 的 Canvas 技术，拓扑图是电力和电信的叫法，但该组件远不仅仅用于呈现节点和连线，在 HT 里面我们没把该组件狭义的称为 Topology、Network 或 Diagram，我们将该组件类名称为更通用的 GraphView，只要你有想象力，你可以用拓扑图组件做出任何其他组件，对于一些需求你想不出来用啥组件实现时，往往就是拓扑图组件需要站出来发力了：
![1](http://www.hightopo.com/blog/wp-content/uploads/2016/08/11.png)
![2](http://www.hightopo.com/blog/wp-content/uploads/2016/08/21.png)
可能大部分不在游戏领域混的同学会好奇上面两张图是什么鬼？这两张是开源的 HTML5 游戏引擎工具 QICI Engine （[https://github.com/qiciengine/qiciengine](https://github.com/qiciengine/qiciengine)）游戏工程抓图，有兴趣的同学可到 Github 上下载试玩。
QICI Engine：A free JavaScript game engine library with a web-based comprehensive suite of toolset for making HTML5 games.
整个 QICI Engine 的 HTML5 游戏引擎编辑工具就是基于 [HT](http://www.hightopo.com/) 组件定制开发出来的，上图刀塔场景的资源管理器就是图标可缩放，图集可展开合并，可缩小变成树层次的 GraphView 拓扑图，而第二张的动作编辑器、曲线编辑器等等部分也都是 GraphView 的身影。
现在你应该能理解为什么我说拓扑图组件几乎可作出任何其他组件，当然定制其他组件是需要时间精力成本的，否则也不需要有其他组件的存在意义了，开发 QICI Engine 产品是另一个故事，今天就先不展开了。HTML5 游戏已经不是啥新鲜玩意儿了，但游戏开发工具完全基于 HTML5 可运行在浏览器还是有点意思的，下图附上阿明同学的得意之作《[城市消消乐](http://engine.zuoyouxi.com/game/Subara/index.php)》抓图，有兴趣的可玩之：[http://engine.zuoyouxi.com/game/Subara/index.php](http://engine.zuoyouxi.com/game/Subara/index.php)
![IMG_1006](http://www.hightopo.com/blog/wp-content/uploads/2016/08/IMG_1006.jpg)
回到今天的主题，学习使用拓扑图，第一步就是: Get Your Hands Dirty，哪怕你不熟悉 HTML，直接照葫芦画瓢按 [http://www.hightopo.com/guide/guide/core/beginners/examples/example_edge.html](http://www.hightopo.com/guide/guide/core/beginners/examples/example_edge.html) 的代码构建出两个节点三条连线：
```
// 创建数据模型容器
var dataModel = new ht.DataModel();
// 创建拓扑图组件
var graphView = new ht.graph.GraphView(dataModel);
// 创建一个起始节点，设置名字和位置属性，添加到数据模型容器
var source = new ht.Node();
source.setName('source');
source.setPosition(100, 70);                
dataModel.add(source);
// 创建一个结束节点，设置名字和位置属性，添加到数据模型容器
var target = new ht.Node();
target.setName('target');  
target.setPosition(260, 70);
dataModel.add(target);
// 创建一条连线，设置起始和结束节点，添加到数据模型容器           
var edge = new ht.Edge();
edge.setSource(source);
edge.setTarget(target);
dataModel.add(edge);
```
相信上面的代码很直观就能理解拓扑图构建的原理，很多人会问 HT 如何通讯获取数据，对后端有限制吗？HT 只是客户端组件，不介入到后台通讯，因此客户可以采用任何后台系统，Java、C++、PHP、Node.js 等等，可采用任何通讯方式 HTTP/AJAX、WebSocket 等，采用任何数据格式 XML、JSON 或 TXT 等，只需要最后用户根据自己格式的数据内容，采用 HT 图形库提供的标准 API 接口创建相应的节点、连线等图元，设置相关的属性信息，即可实现图形呈现，所以 HT 组件对后台和通讯方式没有任何要求。
以上的例子对于名称和位置的设置都是 demo 中随意设置的值，真正的系统用户一般会通过后台数据库查询，然后根据业务数据内容构建图元并设置相应的属性。你会发现虽然在构建拓扑图，但你80%的代码都在和 Data 和 DataModel 打交道，这两个又是什么鬼？其实绝大部分 HT 的客户开发完整个系统都没必要认真学习《[HT for Web 数据模型手册](http://www.hightopo.com/guide/guide/core/datamodel/ht-datamodel-guide.html)》，也不用管 HT 采用的是啥 MVC/MVP/MVVM 框架，需要深入了解的可阅读 [http://www.hightopo.com/guide/guide/core/datamodel/ht-datamodel-guide.html](http://www.hightopo.com/guide/guide/core/datamodel/ht-datamodel-guide.html )手册
![X5AAAAAElFTkSuQmCC](http://www.hightopo.com/blog/wp-content/uploads/2016/08/X5AAAAAElFTkSuQmCC.png)
简单说 Data 就是 HT 的最小数据单元，对于 GraphView 组件可以说是一个图元，一个节点或一条连线；对于 Tree 组件可以说是一个树节点；对于 Table 组件可以说是一行记录，这些 Data 图元需要添加到 DataModel 的数据容器中，而 HT 的所有视图组件 GraphView、List、Tree、Table 甚至是 3D 组件 Graph3dView 都会绑定一个数据容器，这些视图都会监听数据容器的事件变化，进行自身组件的刷新显示工作，这些事件包括图元的属性变化、图元的增删，还有很重要的一个关系，就是图元的父子关系变化。
提到父子关系让我们来看看一个比较综合性的小例子：[http://www.hightopo.com/guide/guide/core/beginners/examples/example_overview.html](http://www.hightopo.com/guide/guide/core/beginners/examples/example_overview.html)
![Screen Shot 2016-08-09 at 8.15.41 PM](http://www.hightopo.com/blog/wp-content/uploads/2016/08/Screen-Shot-2016-08-09-at-8.15.41-PM.png)
```
// 创建数据容器
dataModel = new ht.DataModel();
// 创建拓扑图、属性页、列表、树、表格、树表组件
graphView = new ht.graph.GraphView(dataModel);
propertyView = new ht.widget.PropertyView(dataModel);
listView = new ht.widget.ListView(dataModel);
treeView = new ht.widget.TreeView(dataModel);
tablePane = new ht.widget.TablePane(dataModel);
treeTablePane = new ht.widget.TreeTablePane(dataModel);
// 创建组图元，添加孩子节点，加入数据容器
group = new ht.Group();
group.setName('HT for Web ' + ht.Default.getVersion());
group.addChild(hello);
group.addChild(world);
group.addChild(edge);
dataModel.add(group);
```
这个例子增加了设置父子关系的代码，同时有更多的组件（拓扑图、属性页、列表、树、表格、树表组件）绑定了同一个数据模型 DataModel，但用户的代码依然主要在 Data 和 DataModel 上的操作，这也是 HT 架构设计的优势，用户使用更多组件并没有增加新的学习成本，只要掌握的 DataModel 的基本操作，更多的组件也是如此。
虽然图元属性设置、增加变化甚至是选中操作都可以通过 DataModel 搞定，但并不意味着 DataModel 是万能的，有些需求还是需要通过 View 视图进行。例如很多基本需求是双击图元要做些业务处理，如何监听呢？怎么从 Node 上没找到啥 onClick 之类的接口？这可以从《[HT for Web 入门手册](http://www.hightopo.com/guide/guide/core/beginners/ht-beginners-guide.html#ref_graphviewinteraction)》的交互章节找到答案：[http://www.hightopo.com/guide/guide/core/beginners/ht-beginners-guide.html#ref_graphviewinteraction](http://www.hightopo.com/guide/guide/core/beginners/ht-beginners-guide.html#ref_graphviewinteraction)
```
graphView.addInteractorListener(function (e) {
    if(e.kind === 'clickData'){
        console.log(e.data + '被单击');
    }
    else if(e.kind === 'doubleClickData'){
        console.log(e.data + '被双击');
    }                          
});
```
如果你还想打破砂锅问到底为什么不直接在 Data 或 DataModel 上提供交互处理，Data 数据是可以给很多 View 视图共享用的，DataModel 甚至都不知道 View 视图的存在，他们只会派发模型变化事件，而 View 通过监听模型的变化进行相应的更新处理，HT 的模型架构上极其类似 Facebook React[https://facebook.github.io/react/]( https://facebook.github.io/react/) 的 Flux 单向流设计理念：[http://facebook.github.io/flux/docs/overview.html](http://facebook.github.io/flux/docs/overview.html)
![flux-simple-f8-diagram-explained-1300w](http://www.hightopo.com/blog/wp-content/uploads/2016/08/flux-simple-f8-diagram-explained-1300w.png)
HT 的 DataModel 相当于 Flux 中的 Store 模块，拓扑图、树和表格这些自然对应 View 模块，一般发起 Action 动作就是后台数据变化，或者用户手工输入表格属性值之类，但最终都不是直接修改 View，都是从 Data/DataModel/Store 发起修改，然后数据模型派发事件给所有 View，最后 View 根据不同事件做相应的处理。
有兴趣的同学可以打开 [http://www.hightopo.com/guide/guide/core/beginners/examples/example_overview.html](http://www.hightopo.com/guide/guide/core/beginners/examples/example_overview.html )控制台，输入如图的代码体验下实时修改数据模型的效果：
![Screen Shot 2016-08-09 at 9.05.56 PM](http://www.hightopo.com/blog/wp-content/uploads/2016/08/Screen-Shot-2016-08-09-at-9.05.56-PM.png)
刚才的例子我们提到了一个 Group 类型，这个类型图元节点在 GraphView 上显示成了组合效果，可双击展开合并，可跟随着孩子节点的位置大小变化和自适应变动，除了 Node、Edge、Group 外，HT 还提供了 Shape、Polyline、Grid、SubGraph 等多种图元类型，这些图元类型都具是有针对性的展示效果，以满足各种行业的基本图元需求：[http://www.hightopo.com/guide/guide/core/beginners/ht-beginners-guide.html#ref_datatype](http://www.hightopo.com/guide/guide/core/beginners/ht-beginners-guide.html#ref_datatype)
![Screen Shot 2016-08-09 at 9.14.16 PM](http://www.hightopo.com/blog/wp-content/uploads/2016/08/Screen-Shot-2016-08-09-at-9.14.16-PM.png)
对于连线 Edge，HT 还提供了自定义走向的扩展机制，并提供了相应的预制扩展连线类型插件，参见《[HT for Web 连线类型手册](http://www.hightopo.com/guide/guide/plugin/edgetype/ht-edgetype-guide.html)》[http://www.hightopo.com/guide/guide/plugin/edgetype/ht-edgetype-guide.html](http://www.hightopo.com/guide/guide/plugin/edgetype/ht-edgetype-guide.html)
![Screen Shot 2016-08-10 at 12.33.18 AM](http://www.hightopo.com/blog/wp-content/uploads/2016/08/Screen-Shot-2016-08-10-at-12.33.18-AM.png)
当然预定义的类型再多也不可能满足千奇百怪的行业图元需求，这是你该阅读下《[HT for Web 矢量手册](http://www.hightopo.com/guide/guide/core/vector/ht-vector-guide.html)》[http://www.hightopo.com/guide/guide/core/vector/ht-vector-guide.html](http://www.hightopo.com/guide/guide/core/vector/ht-vector-guide.html) 的时候了，关于矢量的介绍以前写过两篇老文供大家参考：《[HT全矢量化的图形组件设计](http://www.hightopo.com/blog/175.html)》和《[HT图形组件设计之道（二）](http://www.hightopo.com/blog/194.html)》
![Screen Shot 2014-08-12 at 8.57.11 PM](http://www.hightopo.com/blog/wp-content/uploads/2014/08/Screen-Shot-2014-08-12-at-8.57.11-PM.png)
http://player.youku.com/player.php/sid/XNzU0ODAzNzU2/v.swf
以上这个例子你可以在这里玩：[http://www.hightopo.com/demo/2deditor_20151010/HT-2D-Editor.html](http://www.hightopo.com/demo/2deditor_20151010/HT-2D-Editor.html) 。
另外 HT 的矢量内置了 Chart 类型，矢量内置的 Chart 不同于传统独立的 Chart 组件，传统的 Chart 是独立的视图 DOM 组件，但 HT 的拓扑是 Cavans 方式绘制，因此没法实现 Chart 组件和普通图元的不同层次融合显示，但矢量的 Chart 类型倒是完美的实现了这样的融合，很多工控应用的客户会将矢量 Chart 与拓扑图元素融合一体，可参见《[基于HT for Web的Web SCADA工控移动应用](http://www.hightopo.com/blog/402.html)》[http://www.hightopo.com/blog/402.html](http://www.hightopo.com/blog/402.html) 一文中 HT 的 Web SCADA 应用案例
![07896D8D-6878-4D63-8200-E330E2F9684F](http://www.hightopo.com/blog/wp-content/uploads/2016/08/07896D8D-6878-4D63-8200-E330E2F9684F.png)
当然你也可以把 HT 与 百度 ECharts 进行整合，只要是 HTML 图形组件就可以整合到 HT 的布局容器中，可参考《[ECharts整合HT for Web的网络拓扑图应用](http://www.cnblogs.com/xhload3d/p/4340901.html )》，此例子将 ECharts 整合 HT 拓扑图做了告警统计的图表呈现效果：
![160046212452792](http://www.hightopo.com/blog/wp-content/uploads/2016/08/160046212452792.png)
除了混合第三方图形组件库外，HT 图扑还能嵌入 SVG 的图片进行矢量呈现，参见《[绘制SVG内容到Canvas的HTML5应用](http://www.hightopo.com/blog/394.html)》
![Screen Shot 2015-02-01 at 2.21.39 PM](http://www.hightopo.com/blog/wp-content/uploads/2015/02/Screen-Shot-2015-02-01-at-2.21.39-PM.png)
在某些特殊的情况下用户还可以将 HTML 元素嵌入作为普通的 Node 节点，并支持拓扑图的缩放、改变大小等等操作效果，可参考《[HT for Web HtmlNode 手册](http://www.hightopo.com/guide/guide/plugin/htmlnode/ht-htmlnode-guide.html)》[http://www.hightopo.com/guide/guide/plugin/htmlnode/ht-htmlnode-guide.html](http://www.hightopo.com/guide/guide/plugin/htmlnode/ht-htmlnode-guide.html) ，这种结合有一定的局限性，不到万不得已不建议用户使用该插件。
HT 提供了各种丰富的通用组件，以及复杂的拓扑图编辑交互插件 [http://www.hightopo.com/guide/guide/plugin/xeditinteractor/ht-xeditinteractor-guide.html](http://www.hightopo.com/guide/guide/plugin/xeditinteractor/ht-xeditinteractor-guide.html)，用户分分钟可以快速开发出各种各样的编辑器工具。入门手册中也有个 [http://www.hightopo.com/guide/guide/core/beginners/examples/example_editor.html](http://www.hightopo.com/guide/guide/core/beginners/examples/example_editor.html) 简单的编辑器例子，教用户如何自定义创建节点、连线和多边形的示例教材，学习自定义拓扑图交互可从此例子入手。
![Screen Shot 2016-08-09 at 10.11.20 PM](http://www.hightopo.com/blog/wp-content/uploads/2016/08/Screen-Shot-2016-08-09-at-10.11.20-PM.png)
甚至可以像[http://www.hightopo.com/guide/guide/core/beginners/examples/example_animation.html]( http://www.hightopo.com/guide/guide/core/beginners/examples/example_animation.html) 这个例子那样，关闭掉拓扑图的所有默认交互，直接通过添加原生的 HTML DOM 的事件监听进行自定义交互逻辑处理。
![Screen Shot 2016-08-09 at 9.36.24 PM](http://www.hightopo.com/blog/wp-content/uploads/2016/08/Screen-Shot-2016-08-09-at-9.36.24-PM.png)
构建出拓扑图后很多人关系的是如何实现动画，动画从本质上来说就是在一定的时间点上驱动图元的参数变化，例如大小、颜色、粗细包括可见不可见等来实现各种动画效果，而 HT 本来所有图元都是数据驱动，用户随时可以修改 Data 上的任何属性，因此随便起个定时器 windwo.setInterval 不断改变图元属性值即可实现动画效果，但为了方便客户 HT 还提供了诸多方便函数和插件。
例如 [http://www.hightopo.com/guide/guide/core/beginners/ht-beginners-guide.html#ref_animation](http://www.hightopo.com/guide/guide/core/beginners/ht-beginners-guide.html#ref_animation) 中的 ht.Default.startAnim 函数，该函数支持 Frame-Based 和 Time-Based 两种方式的动画，为了理解其中的 Easing 参数可参见 《[透过WebGL 3D看动画Easing函数本质](http://www.hightopo.com/blog/153.html)》文章和 [http://www.hightopo.com/guide/guide/plugin/form/examples/example_easing.html](http://www.hightopo.com/guide/guide/plugin/form/examples/example_easing.html )这个例子
![Screen Shot 2014-05-29 at 3.07.23 PM](http://www.hightopo.com/blog/wp-content/uploads/2014/05/Screen-Shot-2014-05-29-at-3.07.23-PM.png)
如果是持续周期的变化可采用《[HT for Web 调度手册](http://www.hightopo.com/guide/guide/core/schedule/ht-schedule-guide.html)》[http://www.hightopo.com/guide/guide/core/schedule/ht-schedule-guide.html](http://www.hightopo.com/guide/guide/core/schedule/ht-schedule-guide.html) 的调度插件：
![Screen Shot 2016-08-09 at 10.37.04 PM](http://www.hightopo.com/blog/wp-content/uploads/2016/08/Screen-Shot-2016-08-09-at-10.37.04-PM.png)
如果你喜欢 [https://github.com/tweenjs/tween.js](https://github.com/tweenjs/tween.js) 的 chaining 函数方式串联多种动画，可采用《[HT for Web 动画手册](http://www.hightopo.com/guide/guide/plugin/animation/ht-animation-guide.html)》[http://www.hightopo.com/guide/guide/plugin/animation/ht-animation-guide.html](http://www.hightopo.com/guide/guide/plugin/animation/ht-animation-guide.html) 的插件。对于连线流动、虚线流动等常见连线动画需求 HT 已经封装了相应的插件《[HT for Web 流动手册](http://www.hightopo.com/guide/guide/plugin/flow/ht-flow-guide.html)》和《[HT for Web 虚线流动手册](http://www.hightopo.com/guide/guide/plugin/dashflow/ht-dashflow-guide.html)》
![Screen Shot 2016-08-09 at 11.00.45 PM](http://www.hightopo.com/blog/wp-content/uploads/2016/08/Screen-Shot-2016-08-09-at-11.00.45-PM.png)
HT 的拓扑图的节点位置都是逻辑坐标，并非 GIS 的地理信息经纬度坐标，但这并不妨碍 HT 的拓扑图可以和百度地图、GoogleMap、OpenLayers 等第三方 GIS 地图引擎进行整合呈现，参见《[百度地图、ECharts整合HT for Web网络拓扑图应用](http://www.hightopo.com/blog/445.html)》，该文将 HT 拓扑图、百度地图、ECharts、HT 连线流动和面板等插件做了个综合的呈现展示效果：[http://www.hightopo.com/demo/blog_baidu_20150928/ht-baidu.html](http://www.hightopo.com/demo/blog_baidu_20150928/ht-baidu.html)
![230146593649858](http://www.hightopo.com/blog/wp-content/uploads/2016/08/230146593649858.gif)
![hightopo_5](http://www.hightopo.com/blog/wp-content/uploads/2016/08/hightopo_5.png)
拓扑图做完呈现需求效果之后，常会有拓扑图纸数据需要保存的需求，这个很简单，DataModel 内置将所有数据转换成 JSON 的功能，非常方便用户将拓扑图内容进行导入导出工作，一般用户会将 JSON 数据字符串化并进行压缩，然后存到后台数据库或文件系统，运行时再进行加载导入，这方面请参见《[HT  for Web 序列化手册](http://www.hightopo.com/guide/guide/core/serialization/ht-serialization-guide.html)》，其实 HT 的可序列化功能并非为拓扑图组件特殊设计，本质是整个 DataModel 数据层就可以序列化，所以意味着你可以存储表格内容、树层次关系包括 3D 场景。
关于客户端存储数据方面还可参考这篇文章《[HTML5 Web 客户端五种离线存储方式汇总](http://www.hightopo.com/blog/344.html)》
[http://www.hightopo.com/guide/guide/core/serialization/examples/example_exportimport.html](http://www.hightopo.com/guide/guide/core/serialization/examples/example_exportimport.html)
![Screen Shot 2014-12-22 at 1.39.12 AM](http://www.hightopo.com/blog/wp-content/uploads/2014/12/Screen-Shot-2014-12-22-at-1.39.12-AM.png)
另外 HT 的数据绑定功能是非常赞的特点，往往让你意想不到的寥寥几行代码即可实现很有趣的功能，例如下面这个例子，实现一个 Chart 图元在拓扑图中，用户看看拖拽图元实现饼图旋转，双击切换是否中空，只有寥寥十来行代码即可实现，我是想不到如何还能更简单实现这样的功能：[http://www.hightopo.com/guide/guide/core/databinding/examples/example_piebinding.html](http://www.hightopo.com/guide/guide/core/databinding/examples/example_piebinding.html)
![Screen Shot 2016-08-09 at 11.46.32 PM](http://www.hightopo.com/blog/wp-content/uploads/2016/08/Screen-Shot-2016-08-09-at-11.46.32-PM.png)
对于电信网管客户常常有告警的特殊行业呈现需求，这方面 HT 也提供了针对电信 OSS/BSS 网管拓扑图应用的《[HT for Web 电信扩展手册](http://www.hightopo.com/guide/guide/plugin/telecom/ht-telecom-guide.html)》[http://www.hightopo.com/guide/guide/plugin/telecom/ht-telecom-guide.html](http://www.hightopo.com/guide/guide/plugin/telecom/ht-telecom-guide.html) 插件，针对图元告警渲染、告警传播等提供了特殊定制化的展示效果：[http://www.hightopo.com/guide/guide/core/beginners/examples/example_bodycolor.html](http://www.hightopo.com/guide/guide/core/beginners/examples/example_bodycolor.html)
![Screen Shot 2016-08-10 at 12.24.25 AM](http://www.hightopo.com/blog/wp-content/uploads/2016/08/Screen-Shot-2016-08-10-at-12.24.25-AM.png)
虽然 HT 的拓扑图组件可以承载万以上的拓扑图元量，但如果这些拓扑图元都通过手工布局那是不可思议的工作量，因此 HT 为拓扑图组件提供的多种自动布局引擎算法，例如弹力布局、圆形布局、星型布局和层次布局等多种样式效果
《[HT for Web 自动布局手册](http://www.hightopo.com/guide/guide/plugin/autolayout/ht-autolayout-guide.html)》[http://www.hightopo.com/guide/guide/plugin/autolayout/ht-autolayout-guide.html](http://www.hightopo.com/guide/guide/plugin/autolayout/ht-autolayout-guide.html)
![Screen Shot 2016-08-10 at 12.10.13 AM](http://www.hightopo.com/blog/wp-content/uploads/2016/08/Screen-Shot-2016-08-10-at-12.10.13-AM.png)
《[HT for Web 弹力布局手册](http://www.hightopo.com/guide/guide/plugin/forcelayout/ht-forcelayout-guide.html)》[http://www.hightopo.com/guide/guide/plugin/forcelayout/ht-forcelayout-guide.html](http://www.hightopo.com/guide/guide/plugin/forcelayout/ht-forcelayout-guide.html)
![Screen Shot 2016-08-10 at 12.11.46 AM](http://www.hightopo.com/blog/wp-content/uploads/2016/08/Screen-Shot-2016-08-10-at-12.11.46-AM.png)
HT 还为拓扑图组件提供了贴心的鹰眼 Overview 插件（[http://www.hightopo.com/guide/guide/plugin/overview/ht-overview-guide.html](http://www.hightopo.com/guide/guide/plugin/overview/ht-overview-guide.html)），整合上 HT 的 Panel 面板插件（[http://www.hightopo.com/guide/guide/plugin/panel/ht-panel-guide.html](http://www.hightopo.com/guide/guide/plugin/panel/ht-panel-guide.html)）是用户常见的融合拓扑和鹰眼的展示方式，鹰眼具有实时同步拓扑内容，可滚轮缩放，点击定位，拖拽平移等操作方式，非常方便用户浏览导航大数据量拓扑图的应用场景。
![Screen Shot 2016-08-10 at 12.15.25 AM](http://www.hightopo.com/blog/wp-content/uploads/2016/08/Screen-Shot-2016-08-10-at-12.15.25-AM.png)
拓扑图篇就写这些吧，HT 的拓扑图组件还有非常多的功能，无法在一篇博客中说透，我只能蜻蜓点水的提到大部分拓扑图应用需要关注的功能点，HT 更多的 HTML5 拓扑图功能留给大家去探索吧。
[http://www.hightopo.com/guide/guide/plugin/flow/examples/example_demo3.html](http://www.hightopo.com/guide/guide/plugin/flow/examples/example_demo3.html)
![Screen Shot 2016-08-10 at 1.09.36 AM](http://www.hightopo.com/blog/wp-content/uploads/2016/08/Screen-Shot-2016-08-10-at-1.09.36-AM.png)
## Everything you need to create cutting-edge 2D and 3D visualization
