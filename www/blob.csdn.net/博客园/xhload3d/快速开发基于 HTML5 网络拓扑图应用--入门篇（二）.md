# 快速开发基于 HTML5 网络拓扑图应用--入门篇（二） - xhload3d - 博客园
上一篇我们绘制了一个 graphView 场景，在场景之上通过 graphView.dm() 获取数据容器，并通过 graphView.dm().add() 函数添加了两个 Node 节点，并通过 setPosition 设置节点位置以及 setImage 给节点添加图片；接着在两个节点之间通过 ht.Edge(sourceNode, targetNode) 添加了两条连线（实际上还可以更多），并给连线添加了 label 标签显示文字，同时也通过 label.position 设置了文字在连线上的位置。
这一篇我的想法是在左侧添加一个显示所有节点的“树”节点列表，就是将场景中所有的节点都添加到这个“树”列表中。这个部分在我看来简直是简单到极点啊，当然，HT 很多组件都跟这个类似，非常方便。
首先我们得创建这个“树”，HT 中的 [ht.widget.treeView](http://www.hightopo.com/guide/guide/core/treeview/ht-treeview-guide.html) 组件就是用来创建“树列表”的。我们在上一篇中有介绍过 addToDOM 函数，用来将组件添加进 body 体中，我们要实现左边是“数列表”右边是拓扑场景，该如何做呢？这里再引入一个组件 [ht.widget.SplitView](http://www.hightopo.com/guide/guide/core/splitview/ht-splitview-guide.html)，顾名思义“分割”组件。分割组件用于左右或上下分割两个子组件， 子组件可为`HT`框架提供的组件，也可为 HTML 的原生组件，子组件以 position 为 absolute 方式进行绝对定位。
通过 splitView = new ht.widget.SplitView( leftView, rightView, orientation, position )初始化构建一个分割组件对象。leftView 左组件或上组件  rightView 右组件或下组件 orientation 字符串类型，默认为 horizontal 或 h 代表左右分割，可设为 vertical 或 v 代表上下分割 position 分割位置，默认值为 0.5，如果设置值为 0~1 则按百分比分割，大于 1 代表左组件或上组件的绝对宽或高，小于 1 代表右组件或下组件的绝对宽或高。
通过分割组件我们树组件和拓扑组件同时添加进 body 体中，需要的操作只有如下几行：
```
graphView = new ht.graph.GraphView();//拓扑组件
var treeView = new ht.widget.TreeView(graphView.dm());//树组件，参数为树组件绑定的数据模型，这里绑定的是 graphView 的数据模型，就可以进行数据共享
var splitView = new ht.widget.SplitView(treeView, graphView, 'h', 0.2);//分割组件 （leftView, rightView, orientation, position）
splitView.addToDOM();//将 splitView 添加进 body 中
```
记住，原代码中的 graphView.addToDOM 语句得注释掉，否则后面添加到 body 体中的组件的根层 div 会覆盖住上一个添加进 body 体中的组件的底层 div，大家还记得吧？ addToDOM 函数的定义，这里我再次将函数的定义给大家展示一下，大家可以巩固一下：
```
addToDOM = function(){   
    var self = this,
        view = self.getView(),   //获取组件的底层 div
        style = view.style;
    document.body.appendChild(view); //将组件的底层 div 添加进 body 中           
    style.left = '0';// HT 的绝大部分组件都是绝对定位，所以需要设置位置
    style.right = '0';
    style.top = '0';
    style.bottom = '0';      
    window.addEventListener('resize', function () { self.iv(); }, false); //添加 resize 事件，iv 为延时刷新           
}
```
其他部分我们都没有动，效果如下：
![](https://images2017.cnblogs.com/blog/591709/201801/591709-20180112155901519-1175657991.png)
 这样看着好像不太明确到底拓扑图上的哪部分是树列表上的元素？不急，我们继续，给我们的所有节点都设置上“名称”，为了不混淆视听，我将之前的代码也粘出来，也顺便复习一下（这里只显示其中一个节点的代码）：
```
var server = new ht.Node();//服务器节点
graphView.dm().add(server);// 将服务器节点添加进拓扑图组件的数据容器中
server.setName('server');//设置名称 只添加了这一句
server.setPosition(300, 200);//设置服务器节点的位置
server.setImage('images/server.png');//设置服务器节点的显示图片
```
结果如下：
![](https://images2017.cnblogs.com/blog/591709/201801/591709-20180112160348707-1386646973.png)
 我们会发现，拓扑图中的服务器节点以及客户端节点底部也多了 setName 设置的名称！如果不想要在节点下面显示字的话，可以直接设置节点 style 属性：
```
server.s({//s 为 setStyle 的缩写，设置样式
    'label': ''// 将 label 属性设置为空
});
```
这样就不显示啦！但是为什么这样设置可以呢？我们来看看 label 属性的定义，label 属性即为 GraphView.getLabel 函数的封装：
```
getLabel: function (data) {
    var label = data.getStyle('label');//获取样式属性 label 的值
    return label === undefined ? data.getName() : label;        
}
```
通过以上代码可知，style 上的 label 属性优先级高于 name 属性，可重载 GraphView.getLabel 函数改变文字获取逻辑。所以就算我们设置了 name 属性，但是再设置 label 属性还是能将 name 属性设置的值给覆盖掉的，而树组件上获取的只是节点的 name，所以两者并不冲突。
突然感觉树上的节点显示图标为什么都一样。。。改图标！
treeView 树组件通过 getIcon(data) 返回 data 对象对应的 icon 图标，可重载自定义，默认返回的都是图元原始的图标，这里我们要自定义这个函数，针对不同的节点返回不同的 icon，前面我们设置的 name 属性派上用场了：
```
treeView.getIcon = function(data){
    var self = this,
          edge_icon = data.getIcon();//获取对象的 icon
    if(data.getName() === 'server'){//如果是 name 为 server 的节点时
        return 'images/server.png';
    }else if(data.getName() === 'client'){//如果是 name 为 client 的节点时
        return 'images/node.png';
    }else if(data instanceof ht.Edge){//如果是 ht.Edge 类型的节点时
        return edge_icon
    }
}
```
选中树上的节点，拓扑图中的对应节点也会被选中，反过来，选中拓扑图中的节点，树上的节点也会被选中。
最终结果如下:
![](https://images2017.cnblogs.com/blog/591709/201801/591709-20180112172911488-2072426482.png)
次回，我们将在场景中添加工具条！请持续跟踪哦~
