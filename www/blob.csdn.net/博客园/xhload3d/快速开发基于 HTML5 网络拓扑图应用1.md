# 快速开发基于 HTML5 网络拓扑图应用1 - xhload3d - 博客园
今天开始我们就从最基础解析如何构建 HTML5 Canvas 拓扑图应用，HT 内部封装了一个拓扑图形组件 ht.graph.GraphView（以下简称 GraphView）是 HT 框架中 2D 功能最丰富的组件，其相关类库都在 ht.graph 包下。GraphView 具有基本图形的呈现和编辑功能，拓扑节点连线及自动布局功能，电力和电信等行业预定义对象，具有动画渲染等特效，因此其应用面很广泛，可作为监控领域的绘图工具和人机界面，可作为一般性的图形化编辑工具，可扩展成工作流和组织图等企业应用。简单说来就是：拓扑图是泛化的说法，电信网管的网络拓扑图、电力的电网拓扑图、工业控制的监控图、工作流程图、思维脑图等等，简单说就是节点连线构成的这些都是这里指的拓扑图。
用 HT 开发一个网络拓扑图是非常容易的一件事，只需要短短几行代码就能完成一个简单的服务器与客户端的拓扑图：
![](https://images2017.cnblogs.com/blog/591709/201801/591709-20180107203649721-1058745736.gif)
这个例子非常基础，几乎完成了服务器与客户端在拓扑上的所有功能。话不多说，猜猜看这个例子包括 HTML 标签的所有部分总共花了多少行代码？减去空行也就 50 行，我还做了很多样式部分的设计，毕竟给大家看的例子不能太丑嘛~
大家可以在 [http://download.csdn.net/download/u013161495/10193716](http://download.csdn.net/download/u013161495/10193716) 自行下载代码，注意因为有 json 文件，会存在图片跨域问题，需要用 Firefox 或者本地服务器跑起来。
我们在最开始就说明一下，HT 是基于 HTML5 标准的企业应用图形界面一站式解决方案， 其包含通用组件、拓扑组件和 3D 渲染引擎等丰富的图形界面开发类库，用户只需要引入 ht.js 即可，而且跟别的任何东西完全不冲突，因为 HT 只是声明了一个全局变量 ht，仅此而已。
接下来解析代码部分，首先，搭建拓扑图场景：
```
dm = new ht.DataModel();//数据容器
gv = new ht.graph.GraphView(dm);//拓扑组件 参数为dm 绑定的数据模型
gv.addToDOM();//将拓扑图添加进body体中
```
HT 的所有组件的根部都是一个 div，通过 getView() 方法获取，我们在 addToDOM 方法中就用到了这个方法：
```
addToDOM = function(){   
    var self = this,
        view = self.getView(),  //获取组件底层 div
        style = view.style; 
    document.body.appendChild(view);//将底层 div 添加进 body 体中           
    style.left = '0';//HT 一般将组件都设置为 absolute 的绝对定位
    style.right = '0';
    style.top = '0';
    style.bottom = '0';      
    window.addEventListener('resize', function () { self.iv(); }, false); //事件监听窗口大小变化，iv 为延时刷新组件         
}
```
然后向拓扑场景中添加“服务器”以及“客户端”节点：
```
var server = new ht.Node();
server.setName('server');//设置节点名称，显示在节点下方
server.setImage('serverImage');//设置节点图片
server.setSize(20, 60);//设置节点大小
dm.add(server);//将节点添加进数据容器dm中
server.setPosition(100, 100);//设置节点坐标(x, y)
var group = new ht.Group();//组，组中可以有多个节点
group.setImage('groupImage');//设置图片
dm.add(group);
var client = new ht.Node();//这个节点是添加进组中的
client.setName('client');
client.setImage('clientImage');
dm.add(client);
group.addChild(client);//组添加孩子
group.setExpanded(true);//设置组为展开模式
client.setPosition(200, 100);//设置节点位置 如果组中只有一个节点，那么这个节点的位置可以为组的位置
```
服务端与客户端的连线？2 行代码搞定！其实 HT 中添加节点的方法非常简单，一般就 2 行代码能结束：先声明实例变量，然后将这个实例变量添加进数据容器中。
```
var edge = new ht.Edge(server, client);
dm.add(edge);
```
我们很好奇虚线是怎么做出来的？虚线的形成是搭建在连线之上的，步骤有 3 个：
- 引入 ht-dashflow.js 文件 ;
- 将连线的样式属性 edge.dash.flow 设置为 true；
- 在场景组件中打开虚线流动的开关，这里就是 gv.enableDashFlow(true);
是不是非常简单！接下来我们看看怎么设置：
```
edge.s({//节点设置样式属性
    'edge.dash': true,//显示虚线
    'edge.dash.flow': true,//开启虚线流动
    'edge.dash.color': 'yellow',//虚线颜色
    'edge.dash.pattern': [8, 8],//虚线样式
    'label': 'flow',//节点注释
    'label.background': 'pink',//节点注释背景颜色        
});
```
这样所有的显示部分就介绍完毕啦~等等，好像还少点什么？对了，我忘了介绍 HT 中的 ht.Group 类了，顾名思义，就是“组”的意思，组中可以包含很多节点，双击可显示或隐藏组内的所有节点，上面代码有写到，但是我还做了一点小动作，就是组右上角的显示部分，其实就是一个标注，用来提示说明的：
```
group.s({
    'group.background': 'rgba(255, 255, 0, 0.1)',//设置组的背景颜色
    'note': "Double click me!",//标注 显示的内容
    'note.position': 13,//标注位置
    'note.offset.y': 10,//标注位置y轴偏移
});
```
我们可以通过 note.position 来改变标注的位置（具体位置信息请参考 [HT for Web 位置手册](http://www.hightopo.com/guide/guide/core/position/ht-position-guide.html)），也可以使用 note.offset.x 和 note.offset.y 来改变标注的位置。
全部代码解析完毕！我会尽快更新，如果大家觉得进度慢的话，可以自己去我们官网（[HT for Web](http://www.hightopo.com/)）上学习，希望大家能有更多的收获，学习是自己的事情，快动手实践将这片文章的内容变成你自己的知识吧！
